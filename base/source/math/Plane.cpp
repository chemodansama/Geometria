#include "gmt/math/Plane.h"

#include <glm/gtx/norm.hpp>

namespace gmt
{

Plane::Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
    : u_(p1 - p0)
    , v_(p2 - p0)
    , a_(p0)
{
}

glm::vec3 Plane::normal() const
{
    return glm::cross(u_, v_);
}

bool Plane::calculateSignedDistance(const glm::vec3& p, float *result) const
{
    auto n = normal();
    const auto length2 = glm::length2(n);
    if (length2 < 1.0E-06) {
        return false;
    }

    n /= sqrt(length2);
    *result = glm::dot(p - a_, n);
    return true;
}

}
