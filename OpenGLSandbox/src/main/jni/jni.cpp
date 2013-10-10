#include "renderer.h"
#include <jni.h>

extern "C" {
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_onDrawFrame(JNIEnv*, jclass);
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceCreated(JNIEnv*, jclass);
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceChanged(JNIEnv*, jclass,
                                                                       jint width, jint height);
};

JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_onDrawFrame(JNIEnv* env, jclass obj) {
  Renderer::onDrawFrame();
}

JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceCreated(JNIEnv* env, jclass obj) {
  Renderer::onSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceChanged(JNIEnv* env, jclass obj,
                                                                   jint width, jint height) {
  Renderer::onSurfaceChanged(width, height);
}
