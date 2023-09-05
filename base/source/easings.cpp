//
//  easings.cpp
//
//  Created by Grigory Shabaganov on 24.02.2015.
//

#include "gmt/easings.h"

#include <cmath>
#include <glm/gtc/constants.hpp>

namespace gmt::easings
{

float cubicInOut(float t, float b, float c, float d)
{
    t /= d / 2;
    auto t2 = t - 2;
    return t < 1
           ? c / 2 * t * t * t + b
           : c / 2 * (t2 * t2 * t2 + 2) + b;
}

float cubicOut(float t, float b, float c, float d)
{
    t = t / d - 1;
    return c * (t * t * t + 1) + b;
}

float circInOut(float t, float b, float c, float d)
{
    t /= d / 2;
    return t < 1
           ? -c / 2 * (sqrt(1 - t * t) - 1) + b
           : c / 2 * (sqrt(1 - (t - 2) * (t - 2)) + 1) + b;
}

float circOut(float t, float b, float c, float d)
{
    t = t / d - 1;
    return c * sqrt(1 - t * t) + b;
}

float elasticOut(float t, float b, float c, float d)
{
    if (t == 0) {
        return b;
    }

    if ((t /= d) == 1) {
        return b + c;
    }

    float p = d * .3f;
    float a = c;
    float s = p / 4;
    return (a * pow(2.0f, -10 * t) * sin((t * d - s) * (2 * glm::pi<float>()) / p) + c + b);
}

float quartIn(float t, float b, float c, float d)
{
    t /= d;
    return c * t * t * t * t + b;
}

float quartOut(float t, float b, float c, float d)
{
    t = t / d - 1;
    return -c * (t * t * t * t - 1) + b;
}

float quartInOut(float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1)
        return c / 2 * t * t * t * t + b;
    t -= 2;
    return -c / 2 * (t * t * t * t - 2) + b;
}

float circIn(float t, float b, float c, float d)
{
    t /= d;
    return -c * (sqrt(1 - t * t) - 1) + b;
}

float bounceOut(float t, float b, float c, float d)
{
    if ((t /= d) < (1 / 2.75f)) {
        return c * (7.5625f * t * t) + b;
    } else if (t < (2 / 2.75f)) {
        float postFix = t -= (1.5f / 2.75f);
        return c * (7.5625f * (postFix) * t + .75f) + b;
    } else if (t < (2.5 / 2.75)) {
        float postFix = t -= (2.25f / 2.75f);
        return c * (7.5625f * (postFix) * t + .9375f) + b;
    } else {
        float postFix = t -= (2.625f / 2.75f);
        return c * (7.5625f * (postFix) * t + .984375f) + b;
    }
}

float quadIn (float t,float b , float c, float d)
{
    t /= d;
    return c * t * t + b;
}

float quadOut(float t,float b , float c, float d)
{
    t /= d;
    return -c * t * (t - 2.0f) + b;
}

float quadInOut(float t,float b , float c, float d)
{
    t /= d / 2.0f;
    if (t < 1.0f) {
        return c / 2.0f * t * t + b;
    }
    return -c / 2.0f * (((t - 2.0f) * (t - 1.0f)) - 1.0f) + b;
}

float compute(Easings easing, float t, float d)
{
    return easing == Easings::LINEAR ? t / d :
           easing == Easings::ELASTIC_OUT ? elasticOut(t, 0, 1, d) :
           easing == Easings::QUAD_IN ? quadIn(t, 0, 1, d) :
           easing == Easings::QUAD_OUT ? quadOut(t, 0, 1, d) :
           easing == Easings::QUAD_INOUT ? quadInOut(t, 0, 1, d) :
           easing == Easings::CUBIC_OUT ? cubicOut(t, 0, 1, d) :
           easing == Easings::CUBIC_INOUT ? cubicInOut(t, 0, 1, d) :
           easing == Easings::CIRC_OUT ? circOut(t, 0, 1, d) :
           easing == Easings::CIRC_INOUT ? circInOut(t, 0, 1, d) :
           easing == Easings::QUART_IN ? quartIn(t, 0, 1, d) :
           easing == Easings::QUART_OUT ? quartOut(t, 0, 1, d) :
           easing == Easings::QUART_INOUT ? quartInOut(t, 0, 1, d) :
           easing == Easings::BOUNCE_OUT ? bounceOut(t, 0, 1, d)
                                          : t / d;
}

} /* namespace spinny */
