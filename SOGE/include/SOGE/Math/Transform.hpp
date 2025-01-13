#ifndef SOGE_MATH_TRANSFORM_HPP
#define SOGE_MATH_TRANSFORM_HPP

#include <glm/gtx/quaternion.hpp>


namespace soge
{
    struct Transform
    {
        alignas(16) glm::vec3 m_position{};
        alignas(16) glm::quat m_rotation{1.0f, 0.0f, 0.0f, 0.0f};
        alignas(16) glm::vec3 m_scale{1.0f};

        [[nodiscard]]
        glm::vec3 Right() const;
        [[nodiscard]]
        glm::vec3 Up() const;
        [[nodiscard]]
        glm::vec3 Forward() const;

        [[nodiscard]]
        glm::mat4x4 WorldMatrix() const;
        [[nodiscard]]
        glm::mat4x4 ViewMatrix() const;
    };
}

#endif // SOGE_MATH_TRANSFORM_HPP
