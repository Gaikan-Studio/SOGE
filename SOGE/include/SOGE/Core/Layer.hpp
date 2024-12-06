#ifndef SOGE_CORE_LAYER_HPP
#define SOGE_CORE_LAYER_HPP


namespace soge
{
    class Layer
    {
    protected:
        eastl::string m_layerName;

    public:
        Layer(const eastl::string& aLayerName);
        virtual ~Layer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnFixedUpdate(float aDeltaTime) = 0;

        const eastl::string& GetName() const;

    };
}

#endif // !SOGE_CORE_LAYER_HPP
