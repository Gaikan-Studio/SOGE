#include "sogepch.hpp"

#include "SOGE/Math/Transform.hpp"


namespace soge
{
    glm::vec3 Transform::Right() const
    {
        constexpr glm::vec3 right{1.0f, 0.0f, 0.0f};

        return glm::rotate(m_rotation, right);
    }

    glm::vec3 Transform::Up() const
    {
        constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};

        return glm::rotate(m_rotation, up);
    }

    glm::vec3 Transform::Forward() const
    {
        constexpr glm::vec3 forward{0.0f, 0.0f, 1.0f};

        return glm::rotate(m_rotation, forward);
    }

    glm::mat4x4 Transform::WorldMatrix() const
    {
        constexpr auto identity = glm::identity<glm::mat4x4>();

        const auto translation = glm::translate(identity, m_position);
        const auto rotation = glm::toMat4(m_rotation);
        const auto scale = glm::scale(identity, m_scale);
        return translation * rotation * scale;
    }

    glm::mat4x4 Transform::ViewMatrix() const
    {
        return glm::lookAtLH(m_position, m_position + Forward(), Up());
    }
}
