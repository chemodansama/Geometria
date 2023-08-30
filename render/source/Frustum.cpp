#include "gmt/render/Frustum.h"

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_integer.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gmt
{

namespace
{

Plane createNormalizedPlane(const glm::vec4 &equation)
{
    const auto length = glm::length(glm::vec3(equation));
    assert(length > 1.0e-06);
    const auto a = equation.x / length;
    const auto b = equation.y / length;
    const auto c = equation.z / length;
    const auto d = equation.w;

    const auto lowestComponent = 1.0f / sqrt(3.0f) * 0.75f;

    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 origin;

    if (a > lowestComponent) {
        u = { -b / a, 1.0f, 0.0f };
        v = { -c / a, 0.0f, 1.0f };
        origin = { -d / a, 0.0f, 0.0f };
    } else if (b > lowestComponent) {
        u = { 1.0f, -a / b, 0.0f };
        v = { 0.0f, -c / b, 1.0f };
        origin = { 0.0f, -d / b, 0.0f };
    } else {
        assert(c > lowestComponent);
        u = { 1.0f, 0.0f, -a / c };
        v = { 0.0f, 1.0f, -b / c };
        origin = { 0.0f, 0.0f, -d / c };
    }

    return Plane(origin, origin + u, origin + v);
}

}

void Frustum::computePlanes() const
{
    if (!mPlanesDirty) {
        return;
    }

    const glm::mat4 &viewProj = getViewProj();
    
    glm::vec4 rows[4];
    for (int i = 0; i < 4; i++) {
        rows[i] = glm::row(viewProj, i);
    }

    mPlanes[0] = createNormalizedPlane(rows[3] + rows[0]);
    mPlanes[1] = createNormalizedPlane(rows[3] - rows[0]);

    mPlanes[2] = createNormalizedPlane(rows[3] + rows[1]);
    mPlanes[3] = createNormalizedPlane(rows[3] - rows[1]);
    
    mPlanes[4] = createNormalizedPlane(rows[3] + rows[2]);
    mPlanes[5] = createNormalizedPlane(rows[3] - rows[2]);

    mPlanesDirty = false;
}

const Plane &Frustum::getPlane(int index) const
{
    computePlanes();
    return mPlanes[index];
}

const glm::mat4 &Frustum::getViewProj() const 
{
    if (mViewProjDirty) {
        mViewProj = mProj * mView;
        mViewProjDirty = false;
        
        notify([](auto *x){
            x->onViewProjChanged();
        });
    }

    return mViewProj;
}

const glm::mat4 &Frustum::getInvViewProj() const
{
    if (mInvViewProjDirty) {
        mInvViewProj = glm::inverse(getViewProj());
        mInvViewProjDirty = false;
    }

    return mInvViewProj;
}

const glm::mat4 &Frustum::getView() const
{
    return mView;
}

const glm::mat3 &Frustum::getNormal() const 
{
    if (mNormalDirty) {
        mNormal = glm::transpose(glm::inverse(glm::mat3(mView)));
        mNormalDirty = false;
    }

    return mNormal;
}

const glm::mat4 &Frustum::getProj() const
{
    return mProj;
}

void Frustum::changed() 
{
    mPlanesDirty = true;
    mViewProjDirty = true;
    mInvViewProjDirty = true;
    mCornersDirty = true;
    mNormalDirty = true;
}

static const glm::vec4 CUBE[] = {
    glm::vec4(-1, -1, -1, 1), glm::vec4(1, -1, -1, 1), 
    glm::vec4(1, 1, -1, 1), glm::vec4(-1, 1, -1, 1), 
    glm::vec4(-1, -1, 1, 1), glm::vec4(1, -1, 1, 1), 
    glm::vec4(1, 1, 1, 1), glm::vec4(-1, 1, 1, 1)
};

const glm::vec4 &Frustum::getCorner(int index) const
{
    if (mCornersDirty) {
        const glm::mat4 &invViewProj = getInvViewProj();
        for (int i = 0; i < 8; i++) {
            mCorners[i] = invViewProj * CUBE[i];
            mCorners[i] /= mCorners[i].w;
        }
        mCornersDirty = false;
    }

    return (index >= 0 && index < 8) ? mCorners[index] : mCorners[0];
}

void Frustum::projSetOrtho(float left, float right, float bottom, float top, float near, float far)
{
    mNear = near;
    mFar = far;
    mProj = glm::ortho(left, right, bottom, top, near, far);
    changed();
}

void Frustum::projSetPerspective(const float &fovy, const float &aspect,
    const float &zNear, const float &zFar)
{
    mNear = zNear;
    mFar = zFar;
    mProj = glm::perspective(fovy, aspect, zNear, zFar);
    changed();
}

void Frustum::viewSetIdentity()
{
    mView = glm::mat4(1);
    changed();
}

void Frustum::viewRotate(float angle, glm::vec3 axis)
{
    mView = glm::rotate(mView, angle, axis);
    changed();
}

void Frustum::viewTranslate(const glm::vec3 &t)
{
    mView = glm::translate(mView, t);
    changed();
}

void Frustum::viewTranslate(float x, float y, float z)
{
    mView = glm::translate(mView, glm::vec3(x, y, z));
    changed();
}

glm::vec3 Frustum::getX() const
{
    return glm::vec3(glm::row(getView(), 0));
}

glm::vec3 Frustum::getY() const
{
    return glm::vec3(glm::row(getView(), 1));
}

glm::vec3 Frustum::getZ() const
{
    return glm::vec3(glm::row(getView(), 2));
}

void Frustum::update()
{
    getViewProj();
    getInvViewProj();
    getNormal();
    getCorner(0);
}

}
