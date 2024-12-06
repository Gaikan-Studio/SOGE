#include "sogepch.hpp"
#include "SOGE/Core/LayerStack.hpp"


namespace soge
{
    LayerStack::~LayerStack()
    {
        for (Layer* iLayer : m_layerBuffer)
        {
            delete iLayer;
        }
    }

    void LayerStack::PushLayer(Layer* aLayer)
    {
        m_layerBuffer.emplace(m_layerBuffer.begin() + m_layerInsertIndex, aLayer);
        m_layerInsertIndex += 1;
    }

    void LayerStack::PushOverlay(Layer* aOverlayLayer)
    {
        m_layerBuffer.emplace_back(aOverlayLayer);
    }

    void LayerStack::PopLayer(Layer* aLayer)
    {
        auto it = eastl::find(m_layerBuffer.begin(), m_layerBuffer.end(), aLayer);
        if (it != m_layerBuffer.end())
        {
            m_layerBuffer.erase(it);
            m_layerInsertIndex -= 1;
        }
    }

    void LayerStack::PopOverlay(Layer* aOverlayLayer)
    {
        auto it = eastl::find(m_layerBuffer.begin(), m_layerBuffer.end(), aOverlayLayer);
        if (it != m_layerBuffer.end())
        {
            m_layerBuffer.erase(it);
        }
    }

    eastl::vector<Layer*>::iterator LayerStack::begin()
    {
        return m_layerBuffer.begin();
    }

    eastl::vector<Layer*>::iterator LayerStack::end()
    {
        return m_layerBuffer.end();
    }
}
