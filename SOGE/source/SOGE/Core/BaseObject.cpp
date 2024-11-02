#include "sogepch.hpp"
#include "SOGE/Core/BaseObject.hpp"


namespace soge
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<BaseObject>("BaseObject")
            .constructor<>()
            .method("ClassName", &BaseObject::ClassName);
    }

    rttr::string_view BaseObject::ClassName()
    {
        return rttr::type::get(*this).get_name();
    }
}
