#ifndef RENDERER_H
#define RENDERER_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>

class Renderer {


  public:
    static void onDrawFrame() {
      glClear(GL_COLOR_BUFFER_BIT);
    }

    static void onSurfaceCreated() {
      glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    }

    static void onSurfaceChanged(int width, int height) {
      glViewport(0, 0, width, height);

      float left = -1.0;
      float right = 1.0;
      float bottom = -1.0;
      float top = 1.0;
      const float near = 1.0;
      const float far = 10.0;

      float ratio = (float) width / height;
      if (ratio > 1.0) {
        left = -ratio;
        right = ratio;
      } else {
        bottom = - (1 / ratio);
        top = 1 / ratio;
      }

      mat4::frustum(m_projection_matrix, 0, left, right, bottom, top, near, far);
    }

  private:
    static mat4 m_projection_matrix = mat4();
};


#endif