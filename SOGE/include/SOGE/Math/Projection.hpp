#ifndef SOGE_MATH_PROJECTION_HPP
#define SOGE_MATH_PROJECTION_HPP

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>


namespace soge
{
    class Projection
    {
    public:
        constexpr explicit Projection() = default;

        constexpr Projection(const Projection&) noexcept = default;
        constexpr Projection& operator=(const Projection&) noexcept = default;

        constexpr Projection(Projection&&) noexcept = default;
        constexpr Projection& operator=(Projection&&) noexcept = default;

        constexpr virtual ~Projection() = default;

        [[nodiscard]]
        constexpr virtual glm::mat4x4 ToMatrix(float aWidth, float aHeight, float aNearPlane,
                                               float aFarPlane) const = 0;
    };

    class PerspectiveProjection : public Projection
    {
    public:
        float m_fovY;

        explicit PerspectiveProjection(float aFovY);

        [[nodiscard]]
        glm::mat4x4 ToMatrix(float aWidth, float aHeight, float aNearPlane, float aFarPlane) const override;
    };

    class OrthographicProjection : public Projection
    {
    public:
        glm::vec2 m_units;

        explicit OrthographicProjection(glm::vec2 aUnits);

        [[nodiscard]]
        glm::mat4x4 ToMatrix(float aWidth, float aHeight, float aNearPlane, float aFarPlane) const override;
    };
}

#endif // SOGE_MATH_PROJECTION_HPP
