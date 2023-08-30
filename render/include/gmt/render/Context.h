//
//  Context.h
//
//  Created by Grigory Shabaganov on 26.07.2017.
//

#pragma once

#include "gmt/Observable.h"

namespace gmt
{

class ContextListener
{
public:
    virtual ~ContextListener() = default;

    virtual void onResize(int width, int height) = 0;
};

class Context : public Observable<ContextListener>
{
public:
    int width() const { return width_; }
    int height() const { return height_; }

protected:
    Context() = default;

    void setWidth(int value) { width_ = value; }
    void setHeight(int value) { height_ = value; }

    void onResize(int width, int height);

private:
    int width_{ 0 };
    int height_{ 0 };
};

}
