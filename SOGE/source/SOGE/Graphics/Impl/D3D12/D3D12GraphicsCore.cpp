#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"

#include "SOGE/Core/Timestep.hpp"
#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsSwapchain.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"
#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include <Extensions/NRIWrapperD3D12.h>

#ifdef SOGE_DEBUG
#include <nvrhi/validation.h>
#endif


namespace
{
    // Taken from NRI source code (function `GetResultFromHRESULT`)
    nri::Result ConvertResult(const HRESULT aResult)
    {
        if (SUCCEEDED(aResult))
            return nri::Result::SUCCESS;

        if (aResult == E_INVALIDARG || aResult == E_POINTER || aResult == E_HANDLE)
            return nri::Result::INVALID_ARGUMENT;

        if (aResult == DXGI_ERROR_UNSUPPORTED)
            return nri::Result::UNSUPPORTED;

        if (aResult == DXGI_ERROR_DEVICE_REMOVED || aResult == DXGI_ERROR_DEVICE_RESET)
            return nri::Result::DEVICE_LOST;

        if (aResult == E_OUTOFMEMORY)
            return nri::Result::OUT_OF_MEMORY;

        return nri::Result::FAILURE;
    }
}

namespace soge
{
    D3D12GraphicsCore::D3D12GraphicsCore() : m_nriCallbackInterface{}, m_nriAllocationCallbacks{}, m_totalFrameCount{}
    {
        SOGE_INFO_LOG("Creating D3D12 render backend...");
        m_nriCallbackInterface.MessageCallback = NriMessageCallback;

        SOGE_INFO_LOG("Choosing best render device...");
        nri::AdapterDesc bestAdapterDesc{};
        std::uint32_t adapterDescNum = 1;
        NRIThrowIfFailed(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescNum), "choosing best render device");
        SOGE_INFO_LOG(R"(Rendering device "{}" was chosen...)", bestAdapterDesc.name);

        SOGE_INFO_LOG("Creating NRI render device...");
        nri::DeviceCreationDesc deviceCreationDesc{};
        deviceCreationDesc.graphicsAPI = nri::GraphicsAPI::D3D12;
#ifdef SOGE_DEBUG
        deviceCreationDesc.enableGraphicsAPIValidation = true;
        deviceCreationDesc.enableNRIValidation = true;
#endif
        deviceCreationDesc.adapterDesc = &bestAdapterDesc;
        deviceCreationDesc.callbackInterface = m_nriCallbackInterface;
        deviceCreationDesc.allocationCallbacks = m_nriAllocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDevice(deviceCreationDesc, m_nriInitDevice), "creating render device");

        SOGE_INFO_LOG("Retrieving NRI interfaces...");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::CoreInterface),
                                              static_cast<nri::CoreInterface*>(&m_nriInterface)),
                         "retrieving NRI core interface");
        m_nriInterface.SetDeviceDebugName(*m_nriInitDevice, "SOGE device");

        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::HelperInterface),
                                              static_cast<nri::HelperInterface*>(&m_nriInterface)),
                         "retrieving NRI helper interface");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::StreamerInterface),
                                              static_cast<nri::StreamerInterface*>(&m_nriInterface)),
                         "retrieving NRI streamer interface");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriInitDevice, NRI_INTERFACE(nri::SwapChainInterface),
                                              static_cast<nri::SwapChainInterface*>(&m_nriInterface)),
                         "retrieving NRI swap chain interface");

        SOGE_INFO_LOG("Creating graphics command queue...");
        D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc{};
        graphicsCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        graphicsCommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        graphicsCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        nvrhi::RefCountPtr<ID3D12CommandQueue> graphicsCommandQueue;
        const auto d3d12Device = static_cast<ID3D12Device*>(m_nriInterface.GetDeviceNativeObject(*m_nriInitDevice));
        NRIThrowIfFailed(ConvertResult(d3d12Device->CreateCommandQueue(&graphicsCommandQueueDesc,
                                                                       IID_PPV_ARGS(&graphicsCommandQueue))),
                         "creating graphics command queue");

        SOGE_INFO_LOG("Creating NRI device wrapper from D3D12 device...");
        nri::DeviceCreationD3D12Desc deviceCreationD3D12Desc{};
        deviceCreationD3D12Desc.d3d12Device = d3d12Device;
        deviceCreationD3D12Desc.d3d12GraphicsQueue = graphicsCommandQueue.Get();
        deviceCreationD3D12Desc.callbackInterface = m_nriCallbackInterface;
        deviceCreationD3D12Desc.allocationCallbacks = m_nriAllocationCallbacks;
        NRIThrowIfFailed(nri::nriCreateDeviceFromD3D12Device(deviceCreationD3D12Desc, m_nriDevice),
                         "creating NRI device wrapper from D3D12 device");

        SOGE_INFO_LOG("Retrieving NRI interfaces from device wrapper...");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::CoreInterface),
                                              static_cast<nri::CoreInterface*>(&m_nriInterface)),
                         "retrieving NRI core interface from device wrapper");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::HelperInterface),
                                              static_cast<nri::HelperInterface*>(&m_nriInterface)),
                         "retrieving NRI helper interface from device wrapper");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::StreamerInterface),
                                              static_cast<nri::StreamerInterface*>(&m_nriInterface)),
                         "retrieving NRI streamer interface from device wrapper");
        NRIThrowIfFailed(nri::nriGetInterface(*m_nriDevice, NRI_INTERFACE(nri::SwapChainInterface),
                                              static_cast<nri::SwapChainInterface*>(&m_nriInterface)),
                         "retrieving NRI swap chain interface from device wrapper");

        NRIThrowIfFailed(
            m_nriInterface.GetCommandQueue(*m_nriDevice, nri::CommandQueueType::GRAPHICS, m_nriGraphicsCommandQueue),
            "retrieving NRI graphics command queue");
        m_nriInterface.SetCommandQueueDebugName(*m_nriGraphicsCommandQueue, "SOGE graphics command queue");

        SOGE_INFO_LOG("Creating NVRHI render device...");
        const nvrhi::d3d12::DeviceDesc deviceDesc{
            .errorCB = &m_nvrhiMessageCallback,
            .pDevice = d3d12Device,
            .pGraphicsCommandQueue = graphicsCommandQueue.Detach(),
        };
        m_nvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);
#ifdef SOGE_DEBUG
        m_nvrhiDevice = nvrhi::validation::createValidationLayer(m_nvrhiDevice);
#endif
    }

    D3D12GraphicsCore::D3D12GraphicsCore(D3D12GraphicsCore&& aOther) noexcept
        : m_nriInitDevice{}, m_nriDevice{}, m_nriInterface{}, m_nriGraphicsCommandQueue{}, m_nriCallbackInterface{},
          m_nriAllocationCallbacks{}, m_nvrhiMessageCallback{}, m_nvrhiDevice{}, m_swapChain{}, m_totalFrameCount{}
    {
        swap(aOther);
    }

    D3D12GraphicsCore& D3D12GraphicsCore::operator=(D3D12GraphicsCore&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    void D3D12GraphicsCore::swap(D3D12GraphicsCore& aOther) noexcept
    {
        using std::swap;

        swap(m_nriInitDevice, aOther.m_nriInitDevice);
        swap(m_nriDevice, aOther.m_nriDevice);
        swap(m_nriInterface, aOther.m_nriInterface);
        swap(m_nriGraphicsCommandQueue, aOther.m_nriGraphicsCommandQueue);
        swap(m_nriCallbackInterface, aOther.m_nriCallbackInterface);
        swap(m_nriAllocationCallbacks, aOther.m_nriAllocationCallbacks);

        swap(m_nvrhiMessageCallback, aOther.m_nvrhiMessageCallback);
        swap(m_nvrhiDevice, aOther.m_nvrhiDevice);

        swap(m_swapChain, aOther.m_swapChain);

        swap(m_totalFrameCount, aOther.m_totalFrameCount);
    }

    D3D12GraphicsCore::~D3D12GraphicsCore()
    {
        SOGE_INFO_LOG("Destroying D3D12 render backend...");

        if (m_nriGraphicsCommandQueue != nullptr)
        {
            m_nriInterface.WaitForIdle(*m_nriGraphicsCommandQueue);
        }
        m_nvrhiDevice->waitForIdle();
        m_nvrhiDevice->runGarbageCollection();

        DestroySwapChain();
        DestroyDevice();
    }

    void D3D12GraphicsCore::DestroySwapChain()
    {
        m_swapChain = nullptr;
    }

    void D3D12GraphicsCore::DestroyDevice()
    {
        if (m_nvrhiDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NVRHI render device...");
            m_nvrhiDevice = nullptr;
        }

        if (m_nriDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NRI render device wrapper...");
            nri::nriDestroyDevice(*m_nriDevice);
            m_nriDevice = nullptr;
            m_nriGraphicsCommandQueue = nullptr;
        }

        if (m_nriInitDevice != nullptr)
        {
            SOGE_INFO_LOG("Destroying NRI render device...");
            nri::nriDestroyDevice(*m_nriInitDevice);
            m_nriInitDevice = nullptr;
        }
    }

    void D3D12GraphicsCore::SetRenderTarget(const Window& aWindow)
    {
        DestroySwapChain();

        m_swapChain = CreateUnique<D3D12GraphicsSwapchain>(*this, aWindow);
    }

    GraphicsSwapchain* D3D12GraphicsCore::GetSwapchain()
    {
        return m_swapChain.get();
    }

    void D3D12GraphicsCore::Update(RenderGraph& aRenderGraph, const Entities aEntities)
    {
        SOGE_INFO_LOG("Rendering {} frame with input delta time of {}...", m_totalFrameCount, Timestep::DeltaTime());

        m_swapChain->WaitForPresent();
        m_nvrhiDevice->runGarbageCollection();

        aRenderGraph.Execute(aEntities);

        m_swapChain->Present();
        m_totalFrameCount++;
    }

    nvrhi::IDevice& D3D12GraphicsCore::GetRawDevice()
    {
        return *m_nvrhiDevice;
    }

    eastl::string_view D3D12GraphicsCore::GetCompiledShaderExtension() const
    {
        return SOGE_STRINGIZE(SOGE_GRAPHICS_COMPILED_SHADER_EXTENSION_D3D12);
    }

    D3D12GraphicsCore::NvrhiMessageCallback::NvrhiMessageCallback(NvrhiMessageCallback&&) noexcept
    {
    }

    auto D3D12GraphicsCore::NvrhiMessageCallback::operator=(NvrhiMessageCallback&&) noexcept -> NvrhiMessageCallback&
    {
        return *this;
    }

    void D3D12GraphicsCore::NvrhiMessageCallback::message(const nvrhi::MessageSeverity aSeverity,
                                                          const char* aMessageText)
    {
        Logger::Level level{Logger::Level::trace};
        switch (aSeverity)
        {
        case nvrhi::MessageSeverity::Info:
            level = Logger::Level::info;
            break;
        case nvrhi::MessageSeverity::Warning:
            level = Logger::Level::warn;
            break;
        case nvrhi::MessageSeverity::Error:
            level = Logger::Level::err;
            break;
        case nvrhi::MessageSeverity::Fatal:
            level = Logger::Level::critical;
            break;
        }
        Logger::EngineLog(level, "[NVRHI] {}", aMessageText);
    }

    void D3D12GraphicsCore::NriMessageCallback(const nri::Message aMessageType, const char* aFile, std::uint32_t aLine,
                                               const char* aMessage, [[maybe_unused]] void* aUserArg)
    {
        Logger::Level level{Logger::Level::trace};
        switch (aMessageType)
        {
        case nri::Message::INFO:
            level = Logger::Level::info;
            break;
        case nri::Message::WARNING:
            level = Logger::Level::warn;
            break;
        case nri::Message::ERROR:
            level = Logger::Level::err;
            break;
        case nri::Message::MAX_NUM:
            break;
        }
        Logger::EngineLog(level, "[NRI] ({}:{}) {}", aFile, aLine, aMessage);
    }
}
