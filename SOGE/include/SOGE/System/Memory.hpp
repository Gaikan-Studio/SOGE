#ifndef SOGE_SYSTEM_MEMORY_HPP
#define SOGE_SYSTEM_MEMORY_HPP

#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>


namespace soge
{
    template <typename T>
    using SharedPtr = eastl::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr SharedPtr<T> CreateShared(Args&&... args)
    {
        return eastl::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using UniquePtr = eastl::unique_ptr<T>;

    template <typename T, typename... Args>
    constexpr UniquePtr<T> CreateUnique(Args&&... args)
    {
        return eastl::make_unique<T>(std::forward<Args>(args)...);
    }
}

#endif // SOGE_SYSTEM_MEMORY_HPP
