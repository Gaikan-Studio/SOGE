#ifndef SOGE_CORE_LAYERSTACK_HPP
#define SOGE_CORE_LAYERSTACK_HPP

#include "SOGE/Core/Layer.hpp"


namespace soge
{
    class LayerStack
    {
    private:
        eastl::vector<Layer*> m_layerBuffer;
        unsigned m_layerInsertIndex;

    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* aLayer);
        void PushOverlay(Layer* aOverlayLayer);

        void PopLayer(Layer* aLayer);
        void PopOverlay(Layer* aOverlayLayer);

        eastl::vector<Layer*>::iterator begin();
        eastl::vector<Layer*>::iterator end();

    };
}

#endif // !SOGE_CORE_LAYERSTACK_HPP
