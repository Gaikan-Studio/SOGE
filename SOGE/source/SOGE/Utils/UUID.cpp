#include "sogepch.hpp"

#include "SOGE/Utils/UUID.hpp"


namespace soge
{
    UUIDv4::UUIDGenerator<std::mt19937_64> UUID::s_uuidGenerator;

    UUIDv4::UUID UUID::Generate()
    {
        return s_uuidGenerator.getUUID();
    }
}
