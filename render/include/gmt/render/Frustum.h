#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gmt/Observable.h"
#include "gmt/math/Plane.h"

namespace gmt
{

class FrustumListener
{
public:
    virtual ~FrustumListener() = default;
    
    virtual void onViewProjChanged() = 0;
};

class Frustum : public Observable<FrustumListener>
{
public:
    Frustum()
        : mPlanesDirty(true), mViewProjDirty(true)
        , mInvViewProjDirty(true), mCornersDirty(true) {}

    void projSetPerspective(const float &fovy, const float &aspect,
        const float &zNear, const float &zFar);
    
    void projSetOrtho(float left, float right, float bottom, float top, float near, float far);

    void viewSetIdentity();
    void viewRotate(float angle, glm::vec3 axis);
    void viewTranslate(const glm::vec3 &t);
    void viewTranslate(float x, float y, float z);

    glm::vec3 getX() const;
    glm::vec3 getY() const;
    glm::vec3 getZ() const;

    const glm::mat4 &getView() const;
    const glm::mat4 &getProj() const;
    const glm::mat4 &getViewProj() const;
    const glm::mat4 &getInvViewProj() const;
    const glm::mat3 &getNormal() const;

    float getNear() const { return mNear; }
    float getFar() const { return mFar; }

    const float *getViewPtr() const { return glm::value_ptr(getView()); }
    const float *getProjPtr() const { return glm::value_ptr(getProj()); }
    const float *getViewProjPtr() const { return glm::value_ptr(getViewProj()); }
    const float *getInvViewProjPtr() const { return glm::value_ptr(getInvViewProj()); }
    const float *getNormalPtr() const { return glm::value_ptr(getView()); }

    /*  0: (-1, -1, -1)
        1: ( 1, -1, -1)
        2: ( 1,  1, -1)
        3: (-1,  1, -1)
        4: (-1, -1,  1)
        5: ( 1, -1,  1) 
        6: ( 1,  1,  1) 
        7: (-1,  1,  1)
    */
    const glm::vec4 &getCorner(int index) const;

    const Plane &getPlane(int index) const;
    
    void update();
    
private:
    mutable Plane mPlanes[6];
    mutable bool mPlanesDirty{ true };
    
    glm::mat4 mView;
    glm::mat4 mProj;
    
    float mNear{ 1.0f };
    float mFar{ 10.0f };
    
    mutable glm::mat4 mViewProj;
    mutable bool mViewProjDirty;
    
    mutable glm::mat4 mInvViewProj;
    mutable bool mInvViewProjDirty;
    
    mutable glm::vec4 mCorners[8];
    mutable bool mCornersDirty;
    
    mutable glm::mat3 mNormal;
    mutable bool mNormalDirty{ true };
    
    void changed();
    void computePlanes() const;
};

}
