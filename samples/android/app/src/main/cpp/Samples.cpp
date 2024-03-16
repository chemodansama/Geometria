#include <jni.h>

#include <gmt/gmt.h>
#include <glm/gtc/constants.hpp>

extern "C"
JNIEXPORT jint JNICALL
Java_com_geometria_samples_MainActivity_callNative(JNIEnv *env, jobject thiz) {
    (void)env;
    (void)thiz;

    gmt::Rotation rot{ glm::pi<float>() * 0.25f };
    const auto x = gmt::vec2{ 1.0f, 0.0f }.rotate(rot);
    return static_cast<int>(x.x * 1000.0f);
}