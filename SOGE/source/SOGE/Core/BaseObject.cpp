#include "sogepch.hpp"
#include "SOGE/Core/BaseObject.hpp"


namespace soge
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<BaseObject>("BaseObject")
            .constructor<>()
            .method("ClassName", &BaseObject::ClassName)
            .method("SizeOf", &BaseObject::SizeOf)
            .method("Methods", &BaseObject::Methods);
    }

    rttr::type soge::BaseObject::GetRttrType()
    {
        return rttr::type::get(*this);
    }

    BaseObject::BaseObject()
    {

    }

    eastl::string BaseObject::ClassName()
    {
        rttr::type type = GetRttrType();
        return eastl::string(std::move(type.get_name().to_string().c_str()));
    }

    BaseObject::sizeType soge::BaseObject::SizeOf()
    {
        rttr::type type = GetRttrType();
        return static_cast<sizeType>(type.get_sizeof());
    }

    BaseObject::methodArray BaseObject::Methods()
    {
        rttr::type type = GetRttrType();
        return type.get_methods();
    }

    BaseObject::propArray soge::BaseObject::Fields()
    {
        rttr::type type = GetRttrType();
        return type.get_properties();
    }
}
