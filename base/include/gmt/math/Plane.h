#pragma once

#include <glm/glm.hpp>

namespace gmt
{

class Plane
{
public:
    Plane() = default;
    Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);

    operator bool() const { return valid_; }

    const auto &origin() const { return a_; }
    glm::vec3 normal() const;

    const auto &u() const { return u_; }
    const auto &v() const { return v_; }

    float calculateSignedDistance(const glm::vec3 &p) const;

    glm::vec3 project(const glm::vec3 &p) const;
    glm::vec3 intersect(const glm::vec3 &s, const glm::vec3 &t) const;

private:
    glm::vec3 u_{ 1.0f, 0.0f, 0.0f };
    glm::vec3 v_{ 0.0f, 1.0f, 0.0f };
    glm::vec3 a_;
    glm::vec3 n_;

    bool valid_{ false };
};

}
