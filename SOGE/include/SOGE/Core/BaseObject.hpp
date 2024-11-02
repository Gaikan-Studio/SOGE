#ifndef SOGE_CORE_BASEOBJECT_HPP
#define SOGE_CORE_BASEOBJECT_HPP

#include <rttr/registration.h>


namespace soge
{
    class BaseObject
    {
        RTTR_ENABLE()

    public:
        BaseObject()            = default;
        virtual ~BaseObject()   = default;

        virtual rttr::string_view ClassName();

    };
}

#endif // SOGE_CORE_BASEOBJECT_HPP
