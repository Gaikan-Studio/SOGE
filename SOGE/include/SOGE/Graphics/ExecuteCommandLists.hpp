#ifndef SOGE_GRAPHICS_EXECUTECOMMANDLISTS_HPP
#define SOGE_GRAPHICS_EXECUTECOMMANDLISTS_HPP

#include <nvrhi/nvrhi.h>

#include <EASTL/vector.h>

#include <ranges>


namespace soge
{
    template <typename T>
    concept CommandListRange =
        std::ranges::range<T> && std::convertible_to<std::ranges::range_value_t<T>, nvrhi::CommandListHandle>;

    template <CommandListRange T>
    void ExecuteCommandLists(nvrhi::IDevice& aDevice, T&& aCommandLists,
                             const nvrhi::CommandQueue aExecutionQueue = nvrhi::CommandQueue::Graphics)
    {
        thread_local eastl::vector<nvrhi::CommandListHandle> commandLists;
        thread_local eastl::vector<nvrhi::ICommandList*> commandListPtrs;

        if constexpr (std::ranges::sized_range<T>)
        {
            commandLists.reserve(std::ranges::size(aCommandLists));
        }

        for (auto&& commandList : std::forward<T>(aCommandLists))
        {
            auto commandListHandle = static_cast<nvrhi::CommandListHandle>(commandList);
            if (commandListHandle == nullptr)
            {
                continue;
            }
            commandLists.push_back(commandListHandle);
        }

        commandListPtrs.reserve(commandLists.size());
        for (auto&& commandList : commandLists)
        {
            commandListPtrs.push_back(commandList);
        }

        aDevice.executeCommandLists(commandListPtrs.data(), commandListPtrs.size(), aExecutionQueue);

        commandListPtrs.clear();
        commandLists.clear();
    }
}

#endif // SOGE_GRAPHICS_EXECUTECOMMANDLISTS_HPP
