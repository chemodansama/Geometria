#include "gmt/math/Plane.h"

#include <glm/gtx/norm.hpp>

namespace gmt
{

Plane::Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
    : u_(p1 - p0)
    , v_(p2 - p0)
    , a_(p0)
{
    n_ = glm::cross(u_, v_);
    const auto length2 = glm::length2(n_);
    if (length2 > 1.0E-06) {
        n_ /= sqrt(length2);
        valid_ = true;
    }
}

glm::vec3 Plane::normal() const
{
    return n_;
}

float Plane::calculateSignedDistance(const glm::vec3& p) const
{
    return glm::dot(p - a_, n_);
}

glm::vec3 Plane::project(const glm::vec3& p) const
{
    return p - n_ * calculateSignedDistance(p);
}

glm::vec3 Plane::intersect(const glm::vec3& s, const glm::vec3& t) const
{
    const auto sp = project(s);
    const auto ss = sp - s;
    const auto st = t - s;
    return s + st * glm::length2(ss) / glm::dot(ss, st);
}

}
