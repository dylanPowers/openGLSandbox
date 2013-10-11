#include "renderer.h"
#include <jni.h>

extern "C" {
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_construct(JNIEnv*, jclass);
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_onDrawFrame(JNIEnv*, jclass);
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceCreated(JNIEnv*,
                                                                     jclass,
                                                                     jstring fragment_shader,
                                                                     jstring vertex_shader);
  JNIEXPORT void JNICALL
  Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceChanged(JNIEnv*,
                                                                     jclass,
                                                                     jint width,
                                                                     jint height);
};

static Renderer renderer;
JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_construct(JNIEnv* env, jclass cls) {
  renderer = Renderer();
}

JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_onDrawFrame(JNIEnv* env, jclass obj) {
  renderer.onDrawFrame();
}

JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceCreated(JNIEnv* env, jclass obj,
                                                                   jstring fragment_shader,
                                                                   jstring vertex_shader) {
  char const* s = env->GetStringUTFChars(fragment_shader, NULL);
  string frag_s = s;
  env->ReleaseStringUTFChars(fragment_shader, s);

  s = env->GetStringUTFChars(vertex_shader, NULL);
  string vert_s = s;
  env->ReleaseStringUTFChars(vertex_shader, s);

  renderer.onSurfaceCreated(frag_s, vert_s);
}

JNIEXPORT void JNICALL
Java_me_digitain_openglsandbox_RendererJniWrapper_onSurfaceChanged(JNIEnv* env, jclass obj,
                                                                   jint width, jint height) {
  renderer.onSurfaceChanged(width, height);
}
