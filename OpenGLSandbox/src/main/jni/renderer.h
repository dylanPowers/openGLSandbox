#ifndef RENDERER_H
#define RENDERER_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>
using std::string;

class Renderer {
  public:
    Renderer();

    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated(const string& fragment_shader,
                          const string& vertex_shader);

  private:
    float m_projection_matrix[4][4];

    static const float m_triangle1_vertices[];
    static const float m_triangle2_vertices[];
    static const float m_triangle3_vertices[];
    int compileProgram(int fragment_shader_handle, int vertex_shader_handle);
    int createShader(const string&, int shader_type);
};


#endif