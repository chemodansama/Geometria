//
//  easings.h
//
//  Created by Grigory Shabaganov on 24.02.2015.
//

#pragma once

namespace gmt
{

namespace easings
{
    enum Easings
    {
        LINEAR,
        ELASTIC_OUT,
        QUAD_IN,
        QUAD_OUT,
        QUAD_INOUT,
        QUART_IN,
        CUBIC_OUT,
        CUBIC_INOUT,
        QUART_OUT,
        QUART_INOUT,
        CIRC_OUT,
        CIRC_INOUT,
        BOUNCE_OUT
    };

    float cubicOut(float t, float b, float c, float d);
    float cubicInOut(float t, float b, float c, float d);
    float quartIn(float t, float b, float c, float d);
    float quartOut(float t, float b, float c, float d);
    float quartInOut(float t, float b, float c, float d);
    float circOut(float t, float b, float c, float d);
    float circIn(float t, float b, float c, float d);
    float circInOut(float t, float b, float c, float d);
    float elasticOut(float t, float b, float c, float d);
    float bounceOut(float t, float b, float c, float d);
    float compute(Easings easing, float t, float d);
}

}
