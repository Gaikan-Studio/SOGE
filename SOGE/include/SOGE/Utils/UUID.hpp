#ifndef SOGE_UTILS_UUID_HPP
#define SOGE_UTILS_UUID_HPP

#include <uuid_v4/uuid_v4.hpp>


namespace soge
{
    class UUID final
    {
    private:
        static UUIDv4::UUIDGenerator<std::mt19937_64> s_uuidGenerator;

    public:
        explicit UUID() = delete;

        static UUIDv4::UUID Generate();
    };
}

template <>
struct eastl::hash<UUIDv4::UUID>
{
    std::size_t operator()(const UUIDv4::UUID& aUuid) const
    {
        return aUuid.hash();
    }
};

#endif // SOGE_UTILS_UUID_HPP
