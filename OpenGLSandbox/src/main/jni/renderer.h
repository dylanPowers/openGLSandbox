#ifndef RENDERER_H
#define RENDERER_H

#include <android/log.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <ctime>
#include <string>
using std::string;

#define LOGE(...) ((void) __android_log_print(ANDROID_LOG_ERROR, "GL Renderer", __VA_ARGS__))

class Renderer {
  public:
    Renderer();

    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated(const string& fragment_shader,
                          const string& vertex_shader);

  private:
    int m_color_handle;
    int m_MVP_matrix_handle;
    int m_position_handle;


    float m_projection_matrix[4][4];

    static const float m_triangle1_vertices[];
    static const float m_triangle2_vertices[];
    static const float m_triangle3_vertices[];
    int compileShader(const string& shader_source, int shader_type);
    int linkProgram(int fragment_shader_handle, int vertex_shader_handle);
};


#endif