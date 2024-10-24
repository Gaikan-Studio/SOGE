#include "sogepch.hpp"
#include "SOGE/Core/Layer.hpp"


namespace soge
{
    Layer::Layer(const eastl::string& aLayerName) : m_layerName(aLayerName)
    {
    }

    const eastl::string& Layer::GetName() const
    {
        return m_layerName;
    }


}
