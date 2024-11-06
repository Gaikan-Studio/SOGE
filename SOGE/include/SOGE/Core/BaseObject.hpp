#ifndef SOGE_CORE_BASEOBJECT_HPP
#define SOGE_CORE_BASEOBJECT_HPP

#include "SOGE/Core/ReflectionDefs.hpp"
#include <rttr/registration.h>


namespace soge
{
    class BaseObject
    {
        using sizeType      = eastl_size_t;
        using methodArray   = rttr::array_range<rttr::method>;
        using propArray     = rttr::array_range<rttr::property>;

        RTTR_ENABLE()

    private:
        rttr::type GetRttrType();

    public:
        BaseObject();
        virtual ~BaseObject()   = default;

        virtual eastl::string ClassName();
        virtual sizeType SizeOf();
        virtual methodArray Methods();
        virtual propArray Fields();

    };
}

#endif // SOGE_CORE_BASEOBJECT_HPP
