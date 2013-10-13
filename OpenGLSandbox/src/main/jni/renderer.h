#ifndef RENDERER_H
#define RENDERER_H

#include "matrix.h"

#include <android/log.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <ctime>
#include <string>
using std::string;

#define LOGE(...) ((void) __android_log_print(ANDROID_LOG_ERROR, "GL Renderer", __VA_ARGS__))

#define POSITION_OFFSET 0
#define POSITION_DATA_SIZE 3
#define COLOR_OFFSET 3
#define COLOR_DATA_SIZE 4

#define TAU 6.283185307179586476925286766559 // I'm pretty exact

class Renderer {
  public:
    Renderer();
    ~Renderer();
    Renderer(const Renderer&);

    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated(const string& fragment_shader,
                          const string& vertex_shader);

  private:
    int m_color_handle;
    int m_MVP_matrix_handle;
    int m_position_handle;

    Mat4 m_model_matrix;
    Mat4 m_MVP_matrix;
    Mat4 m_projection_matrix;
    Mat4 m_view_matrix;

    static const float m_triangle1_vertices[];
    static const float m_triangle2_vertices[];
    static const float m_triangle3_vertices[];
    static const float STRIDE_BYTES = 7 * sizeof(float);

    int compileShader(const string& shader_source, int shader_type);
    void drawTriangle(const float*);
    int linkProgram(int fragment_shader_handle, int vertex_shader_handle);
};


#endif