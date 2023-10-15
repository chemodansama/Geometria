//
//  utils.h
//
//  Created by Grigory Shabaganov on 10.08.2013.
//

#pragma once

#include <random>

#ifdef __ANDROID__
#include <android/log.h>
#elif defined(_WIN32)
#include <iostream>
#endif

#include <cstdlib>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <fmt/format.h>

#ifdef __ANDROID__
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "gmt", __VA_ARGS__)
#elif defined(_WIN32) || defined(__APPLE__)
#define LOGI(...) { printf(__VA_ARGS__); printf("\n"); }
#else
#define LOGI(...) CCLOG(__VA_ARGS__)
#endif

template<typename T, size_t N>
static T* begin(T (&arr)[N])
{
    return &arr[0];
}

template<typename T, size_t N>
static T* end(T (&arr)[N])
{
    return &arr[0] + N;
}

template<typename T, size_t N>
constexpr size_t length(T (&arr)[N])
{
    return N;
}

namespace details
{

inline void log(fmt::memory_buffer &ss)
{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

    time_t rawtime;
    time(&rawtime);
#if defined(__ANDROID__) || defined(__APPLE__)
    const struct tm* timeinfo = std::localtime(&rawtime);
#else
    std::tm localtime;
    localtime_s(&localtime, &rawtime);
    auto timeinfo = &localtime;
#endif

    std::ostringstream os;
    os << std::put_time(timeinfo, "%H:%M:%S");

    LOGI("%s: %s", os.str().c_str(), std::string{ ss.data(), ss.size() }.c_str());
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}

template <typename H, typename... T>
void log(fmt::memory_buffer &ss, H h, T... t)
{
    fmt::format_to(std::back_inserter(ss), "{}", h);
    log(ss, t...);
}

inline std::string concat(fmt::memory_buffer &ss)
{
    return std::string{ ss.data(), ss.size() };
}

template <typename H, typename... T>
std::string concat(fmt::memory_buffer &ss, const H &h, const T & ... t) {
    fmt::format_to(std::back_inserter(ss), "{}", h);
    return concat(ss, t...);
}

std::default_random_engine &randomEngine();

}

namespace gmt
{

namespace utils
{

template <typename... T>
void log(T... t);

template <typename F, typename... T>
void logf(F&& format, T... t);

inline void log(const std::string &str);
inline void log(const char *str);

template <typename... T>
std::string concat(const T & ... t);

template <typename Out>
void split(const std::string &s, char delim, Out destination);

long getTime(void);

template <typename T>
class Distribution
{
public:
    Distribution(T dist, std::default_random_engine &engine)
        : dist_{ std::move(dist) }
        , engine_{ engine }
    {
    }

    auto operator()() { return dist_(engine_); }

private:
    T dist_;
    std::default_random_engine &engine_;
};

template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
auto randomDistribution(T min, T max)
{
    using D = std::uniform_real_distribution<T>;
    D actualDistribution{ min, max };
    return Distribution<D>(actualDistribution, ::details::randomEngine());
}

template <typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
auto randomDistribution(T min, T max)
{
    using D = std::uniform_int_distribution<T>;
    D actualDistribution{ min, max };
    return Distribution<D>(actualDistribution, ::details::randomEngine());
}

template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
T random(T min, T max)
{
    std::uniform_real_distribution<T> dist(min, max);
    return dist(::details::randomEngine());
}

template <typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
T random(T min, T max)
{
    std::uniform_int_distribution<T> dist(min, max);
    return dist(::details::randomEngine());
}

template <typename T>
struct Iterable
{
    Iterable(std::pair<T, T> range)
        : range_{ range }
    {
    }

    auto begin() const { return range_.first; }
    auto end() const { return range_.second; }

    std::pair<T, T> range_;
};

template <typename T>
Iterable<T> iterable(std::pair<T, T> range) { return Iterable<T>{ std::move(range) }; }

#ifdef NDEBUG
template <typename T, typename... Args>
void assume(T&& condition, Args&&... args)
{
    (void)condition;
}
template <typename T>
void assume(T&& condition)
{
    (void)condition;
}
#else
template <typename T, typename... Args>
void assume(T&& condition, Args&&... args)
{
    if (!condition) {
        utils::logf(std::forward<Args>(args)...);
        std::abort();
    }
}
template <typename T>
void assume(T&& condition)
{
    if (!condition) {
        std::abort();
    }
}
#endif

}

}

// Implementation

namespace gmt
{

namespace utils
{

template <typename... T>
void log(T... t)
{
    fmt::memory_buffer ss;
    ::details::log(ss, t...);
}

template <typename F, typename... T>
void logf(F&& format, T... t)
{
    log(fmt::format(std::forward<F>(format), std::forward<T>(t)...));
}

void log(const std::string &str)
{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
   LOGI(str.c_str());
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}

void log(const char *str)
{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
    LOGI(str);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}

template <typename Out>
void split(const std::string &s, char delim, Out destination)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        destination = item;
        destination++;
    }
}

template <typename... T>
std::string concat(const T & ... t)
{
    fmt::memory_buffer ss;
    return ::details::concat(ss, t...);
}

}

}
