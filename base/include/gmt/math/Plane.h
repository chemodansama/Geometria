#pragma once

#include <glm/glm.hpp>

namespace gmt
{

class Plane
{
public:
    Plane() = default;
    Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);

    const auto &origin() const { return a_; }
    glm::vec3 normal() const;

    const auto &u() const { return u_; }
    const auto &v() const { return v_; }

    bool calculateSignedDistance(const glm::vec3 &p, float *result) const;

private:
    glm::vec3 u_{ 1.0f, 0.0f, 0.0f };
    glm::vec3 v_{ 0.0f, 1.0f, 0.0f };
    glm::vec3 a_;
};

}
