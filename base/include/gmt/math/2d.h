//
//  2d.h
//
//  Created by Grigory Shabaganov on 27.08.2023.
//

#pragma once

#include <array>
#include <cmath>
#include <cassert>
#include <iterator>
#include <span>
#include <type_traits>
#include <vector>

#include <glm/glm.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/norm.hpp>

namespace gmt
{

struct Rotation
{
    static Rotation identity() { return Rotation{ 1.0f, 0.0f }; }

    Rotation(float c, float s) : c{ c }, s{ s } {}
    explicit Rotation(float angle) : c{ std::cos(angle) }, s{ std::sin(angle) } {}

    template <typename T>
    [[nodiscard]] T apply(const T &v) const
    {
        T result;
        result.x = v.x * c - v.y * s;
        result.y = v.x * s + v.y * c;
        return result;
    }

    float c;
    float s;
};

template <typename T = float>
struct alignas(T) Vector
{
    [[nodiscard]] static Vector<T> zero()
    {
        return Vector<T>(static_cast<T>(0), static_cast<T>(0));
    }

    T x{}, y{};

    Vector() = default;
    Vector(T x, T y);

    Vector(const glm::vec2 &v): x{ v.x }, y{ v.y } {}
    [[nodiscard]] operator glm::vec2() const { return glm::vec2{ x, y }; }

    [[nodiscard]] T *data();
    [[nodiscard]] bool operator==(const Vector<T> &v) const;
    [[nodiscard]] Vector<T> operator+(const Vector<T> &v) const;
    Vector<T>& operator+=(const Vector<T> &v);
    [[nodiscard]] Vector<T> operator-(const Vector<T> &v) const;
    [[nodiscard]] Vector<T> operator*(const Vector<T> &v) const;
    [[nodiscard]] Vector<T> operator*(T v) const;
    [[nodiscard]] Vector<T> operator/(T v) const;
    Vector<T>& operator*=(T v);
    Vector<T>& operator/=(T v);

    template <typename U>
    Vector<T>& operator-=(const Vector<U> &v);

    [[nodiscard]] Vector<T> operator-() const;
    void set(T x, T y);
    [[nodiscard]] T dot(const Vector<T> &v) const;
    [[nodiscard]] T cross(const Vector<T> &v) const;

    template <typename V>
    [[nodiscard]] T distanceSq(const V &v) const;
    [[nodiscard]] T distance(const Vector<T> &v) const;
    [[nodiscard]] T lengthSq() const;
    [[nodiscard]] T length() const;
    [[nodiscard]] T atan2f() const;
    [[nodiscard]] Vector<T> rotate(float angle) const;
    [[nodiscard]] Vector<T> rotate(const Rotation &rotation) const;
    [[nodiscard]] Vector<T> normalize() const;
};
static_assert(sizeof(Vector<double>) == 2 * sizeof(double), "");
static_assert(sizeof(Vector<float>) == 2 * sizeof(float), "");

using vec2 = Vector<float>;

template <typename T>
Vector<T> mix(const Vector<T> &x, const Vector<T> &y, const T a)
{
    return x * (T(1) - a) + y * a;
}

template <typename T>
Vector<T> vector(const T *v)
{
    return Vector<T>{ v[0], v[1] };
}

template <typename K>
auto vector(const K &source) -> Vector<decltype(source.x)>
{
    return Vector<decltype(source.x)>(source.x, source.y);
}

template <typename T>
auto vector(const T &x, const T &y) -> Vector<T>
{
    return Vector<>(x, y);
}

template <typename T>
T sqrt(T value);

template <typename T>
void rotate(T &v, decltype(T::x) angle)
{
    v = Rotation{ angle }.apply(v);
}

template <typename T>
T remainder(const T &a, const T &b)
{
    return T{ std::remainder(a.x, b.x), std::remainder(a.y, b.y) };
}

template <typename T>
auto cross(const T &left, const T &right)
{
    return left.x * right.y - left.y * right.x;
}

template <typename T>
auto dot(const T& left, const T& right)
{
    return left.dot(right);
}

template <>
inline auto dot<glm::vec2>(const glm::vec2& left, const glm::vec2& right)
{
    return glm::dot(left, right);
}

template <typename T>
auto length(const T &p)
{
    return p.length();
}

template <>
inline auto length<glm::vec2>(const glm::vec2& p)
{
    return glm::length(p);
}

template <typename T>
auto lengthSq(const T& p)
{
    return p.lengthSq();
}

template <>
inline auto lengthSq<glm::vec2>(const glm::vec2& p)
{
    return glm::length2(p);
}

template <typename T>
int mergePoints(std::span<T> points, const float margin = 0.001f)
{
    if (points.empty()) {
        return 0;
    }

    int k = 0;
    for (int i = 1; i < static_cast<int>(points.size()); i++) {
        if ((points[k] - points[i]).length() > margin) {
            k += 1;
            points[k] = points[i];
        }
    }
    return k + 1;
}

template <typename It>
auto make_span(It from, It to)
{
    return std::span<std::remove_reference_t<decltype(*from)>>(from, to);
}

// Implementation

template <>
double inline sqrt<double>(double value)
{
    return ::sqrt(value);
}

template <>
float inline sqrt<float>(float value)
{
    return sqrtf(value);
}

namespace details
{

template <typename T>
int sgn(T value)
{
    return value < 0 ? -1 : 1;
}

}

namespace line
{

template <typename T>
T sign(const Vector<T> &a, const Vector<T> &ab, const Vector<T> &p)
{
    const auto nonNormalizeDistance = (p - a).cross(ab);
    const auto zero = static_cast<T>(0.0);
    const auto result =
            nonNormalizeDistance > zero ? 1 :
            nonNormalizeDistance < zero ? -1
                                        : 0;
    return static_cast<T>(result);
}

template <typename T>
T distance(const Vector<T> &a, const Vector<T> &ab, const Vector<T> &p)
{
    return (p - a).cross(ab) / ab.length();
}

template <typename T>
T projectBasic(const Vector<T> &a, const Vector<T> &ab, const Vector<T> &p)
{
    return (p - a).dot(ab) / ab.dot(ab);
}

template <typename T>
Vector<T> project(const Vector<T> &a, const Vector<T> &ab, const Vector<T> &p)
{
    auto k = projectBasic(a, ab, p);
    return a + ab * k;
}

struct LineIntersectionData
{
    float t;
};

template <typename T>
bool intersectsLine(const Vector<T> &p, const Vector<T> &r, const Vector<T> &q, const Vector<T> &s,
        Vector<T> *out, LineIntersectionData *data = nullptr)
{
    const auto v = r.cross(s);
    if (v == 0) {
        return false;
    }

    const auto t = (q - p).cross(s) / v;
    *out = p + r * t;

    if (data) {
        data->t = t;
    }

    return true;
}

template <typename T, typename V, typename Out>
bool intersectsCircle(T r, const V &s, const V &t, Out out, int *count)
{
    const auto dx = t.x - s.x;
    const auto dy = t.y - s.y;
    const auto dr2 = dx * dx + dy * dy;
    const auto d = s.x * t.y - s.y * t.x;

    const auto discriminant = r * r * dr2 - d * d;
    if (discriminant > 0) {
        const auto rootOfDicriminant = sqrt(discriminant);
        *out = V{ (d * dy + details::sgn(dy) * dx * rootOfDicriminant) / dr2,
                  (-d * dx + std::abs(dy) * rootOfDicriminant) / dr2 };
        ++out;

        *out = V{ (d * dy - details::sgn(dy) * dx * rootOfDicriminant) / dr2,
                  (-d * dx - std::abs(dy) * rootOfDicriminant) / dr2 };
        ++out;

        *count = 2;
    } else if (discriminant == 0) {
        *out = V{ (d * dy) / dr2, (-d * dx) / dr2 };
        ++out;

        *count = 1;
    } else {
        *count = 0;
        return false;
    }

    return true;
}

}

namespace segment
{

template <typename T>
bool intersectsSegment(const Vector<T> &p, const Vector<T> &r, const Vector<T> &q, const Vector<T> &s,
        Vector<T> *out)
{
    auto pq = q - p;
    auto u = pq.cross(r);
    auto t = pq.cross(s);
    auto v = r.cross(s);

    if (v == 0) {
        return false;
    }

    u /= v;
    if (u < 0 || u > 1) {
        return false;
    }

    t /= v;
    if (t < 0 || t > 1) {
        return false;
    }

    *out = p + r * t;
    return true;
}

template <typename T>
bool intersectsRay(const Vector<T> &p, const Vector<T> &r, const Vector<T> &q, const Vector<T> &s,
        Vector<T> *out)
{
    auto pq = q - p;
    auto u = pq.cross(r);
    auto t = pq.cross(s);
    auto v = r.cross(s);

    if (v == 0) {
        return false;
    }

    u /= v;
    if (u < 0) {
        return false;
    }

    t /= v;
    if (t < 0 || t > 1) {
        return false;
    }

    *out = p + r * t;
    return true;
}

struct LineIntersectionData
{
    float t;
};

template <typename T>
bool intersectsLine(const Vector<T> &p, const Vector<T> &r, const Vector<T> &q, const Vector<T> &s,
        Vector<T> *out, LineIntersectionData *data = nullptr)
{
    auto t = (q - p).cross(s);
    auto v = r.cross(s);

    if (v == 0) {
        return false;
    }

    t /= v;
    if (t < 0 || t > 1) {
        return false;
    }

    *out = p + r * t;

    if (data) {
        data->t = t;
    }

    return true;
}

template <typename T, typename V, typename Out>
bool intersectsCircle(T r, const V &s, const V &t, Out out, int *count)
{
    if (!line::intersectsCircle(r, s, t, out, count)) {
        return false;
    }

    bool inside[2];
    auto st = t - s;
    for (int i = 0; i < *count; i++) {
        const auto &v = out[i];
        inside[i] = dot(v - s, st) >= 0 && dot(v - t, st) <= 0;
    }

    if (*count == 2) {
        if (!inside[0] && !inside[1]) {
            return false;
        } else if (!inside[0]) {
            out[0] = out[1];
            *count = 1;
        } else if (!inside[1]) {
            *count = 1;
        }
    } else {
        assert(*count == 1);
        if (!inside[0]) {
            return false;
        }
    }

    return true;
}

}

namespace circle
{
    template <typename It>
    bool containsPolygon(float r, It begin, It end)
    {
        const auto radiusSq = r * r;
        for (auto it = begin; it != end; ++it) {
            if (lengthSq(*it) > radiusSq) {
                return false;
            }
        }
        return true;
    }

    template <typename T>
    bool containsPolygon(float r, const std::initializer_list<T> &points)
    {
        return containsPolygon(r, points.begin(), points.end());
    }

    template <typename T, typename K>
    bool intersectsRectangle(float r, const T& center, const T& extents, K out)
    {
        const auto a = center - extents;
        const auto b = center + T{ extents.x, -extents.y };
        const auto c = center + extents;
        const auto d = center + T{ -extents.x, extents.y };

        std::array<T, 8> intersections;
        int total{ 0 };
        int count{ 0 };
        if (segment::intersectsCircle(r, a, b, intersections.data(), &count)) {
            total += count;
        }

        if (segment::intersectsCircle(r, b, c, intersections.data() + total, &count)) {
            total += count;
        }

        if (segment::intersectsCircle(r, c, d, intersections.data() + total, &count)) {
            total += count;
        }

        if (segment::intersectsCircle(r, d, a, intersections.data() + total, &count)) {
            total += count;
        }

        total = mergePoints(make_span(intersections.data(), intersections.data() + total));
        for (int i = 0; i < total; i++) {
            *out = intersections[i];
            ++out;
        }

        if (total == 0) {
            if (containsPolygon(r, { a, b, c, d })) {
                // Rectangle is fully contained, return true and return no intersection points.
                return true;
            }
        }

        return total > 0;
    }

    template <typename T, typename K>
    bool intersectsRectangle(float r, const T& center, const T& extents, float angle, K out)
    {
        const auto c = gmt::Rotation{ -angle }.apply(center);
        std::vector<T> v;
        const auto result = intersectsRectangle(r, c, extents, std::back_inserter(v));
        if (!v.empty()) {
            
            gmt::Rotation r{ angle };
            for (auto& i : v) {
                *out = r.apply(i);
                ++out;
            }
        }
        return result;
    }
}

//

template <typename T>
Vector<T>::Vector(T x, T y): x{ x }, y{ y }
{
}

template <typename T>
T *Vector<T>::data() { return &x; }

template <typename T>
bool Vector<T>::operator==(const Vector<T> &v) const
{
    return x == v.x && y == v.y;
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T> &v) const
{
    return Vector<T>{ x + v.x, y + v.y };
}

template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T> &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T> &v) const
{
    return Vector<T>{ x - v.x, y - v.y };
}

template <typename T>
Vector<T> Vector<T>::operator*(const Vector<T> &v) const
{
    return Vector<T>{ x * v.x, y * v.y };
}

template <typename T>
Vector<T> Vector<T>::operator*(T v) const
{
    return Vector<T>{ x * v, y * v };
}

template <typename T>
Vector<T> Vector<T>::operator/(T v) const
{
    return Vector<T>{ x / v, y / v };
}

template <typename T>
Vector<T>& Vector<T>::operator*=(T v)
{
    x *= v;
    y *= v;
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator/=(T v)
{
    x /= v;
    y /= v;
    return *this;
}

template <typename T>
template <typename U>
Vector<T>& Vector<T>::operator-=(const Vector<U> &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename T>
Vector<T> Vector<T>::operator-() const
{
    return Vector<T>(-x, -y);
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#endif
template <typename T>
void Vector<T>::set(T x, T y)
{
    this->x = x;
    this->y = y;
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

template <typename T>
T Vector<T>::dot(const Vector<T> &v) const
{
    return x * v.x + y * v.y;
}

template <typename T>
T Vector<T>::cross(const Vector<T> &v) const
{
    return gmt::cross(*this, v);
}

template <typename T>
template <typename V>
T Vector<T>::distanceSq(const V &v) const
{
    auto dx = x - v.x;
    auto dy = y - v.y;
    return dx * dx + dy * dy;
}

template <typename T>
T Vector<T>::distance(const Vector<T> &v) const
{
    return sqrt(distanceSq(v));
}

template <typename T>
T Vector<T>::lengthSq() const
{
    return x * x + y * y;
}

template <typename T>
T Vector<T>::length() const
{
    return sqrt(lengthSq());
}

template <typename T>
T Vector<T>::atan2f() const
{
    return ::atan2f(y, x);
}

template <typename T>
Vector<T> Vector<T>::rotate(float angle) const
{
    auto result = *this;
    gmt::rotate(result, angle);
    return result;
}

template <typename T>
Vector<T> Vector<T>::rotate(const Rotation &rotation) const
{
    Vector<T> result;
    result.x = x * rotation.c - y * rotation.s;
    result.y = x * rotation.s + y * rotation.c;
    return result;
}

template <typename T>
Vector<T> Vector<T>::normalize() const
{
    const auto len = length();
    return { x / len, y / len };
}

}

