#include "sogepch.hpp"

#include "SOGE/Math/Projection.hpp"


namespace soge
{
    PerspectiveProjection::PerspectiveProjection(const float aFovY) : m_fovY{aFovY}
    {
    }

    glm::mat4x4 PerspectiveProjection::ToMatrix(const float aWidth, const float aHeight, const float aNearPlane,
                                                const float aFarPlane) const
    {
        const float aspect = (aHeight != 0.0f) ? aWidth / aHeight : 0.0f;
        return glm::perspectiveLH_ZO(m_fovY, aspect, aNearPlane, aFarPlane);
    }

    OrthographicProjection::OrthographicProjection(const glm::vec2 aUnits) : m_units{aUnits}
    {
    }

    glm::mat4x4 OrthographicProjection::ToMatrix(const float aWidth, const float aHeight, const float aNearPlane,
                                                 const float aFarPlane) const
    {
        const float invAspect = (aWidth != 0.0f) ? aHeight / aWidth : 0.0f;
        const float halfX = m_units.x / 2.0f;
        const float halfY = m_units.y / 2.0f;
        return glm::orthoLH_ZO(-halfX, halfX, halfY * invAspect, -halfY * invAspect, aNearPlane, aFarPlane);
    }
}
