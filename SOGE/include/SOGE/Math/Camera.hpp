#ifndef SOGE_MATH_CAMERA_HPP
#define SOGE_MATH_CAMERA_HPP

#include "SOGE/Math/Projection.hpp"
#include "SOGE/Math/Transform.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    struct Camera
    {
        float m_width{};
        float m_height{};
        float m_nearPlane{};
        float m_farPlane{};
        Transform m_transform{};
        UniquePtr<Projection> m_projection{};

        [[nodiscard]]
        constexpr float AspectRatio() const noexcept;
        [[nodiscard]]
        constexpr float InverseAspectRatio() const noexcept;

        [[nodiscard]]
        glm::mat4x4 GetProjectionMatrix() const;
    };

    constexpr float Camera::AspectRatio() const noexcept
    {
        return (m_height != 0.0f) ? m_width / m_height : 0.0f;
    }

    constexpr float Camera::InverseAspectRatio() const noexcept
    {
        return (m_width != 0.0f) ? m_height / m_width : 0.0f;
    }

    inline glm::mat4x4 Camera::GetProjectionMatrix() const
    {
        if (m_projection == nullptr)
        {
            return glm::mat4x4{1.0f};
        }
        return m_projection->ToMatrix(m_width, m_height, m_nearPlane, m_farPlane);
    }
}

#endif // SOGE_MATH_CAMERA_HPP
