#include "renderer.h"

const float Renderer::m_triangle1_vertices[] = {
  // X, Y, Z,
  // R, G, B, A
  -0.5f, -0.25f, 0.0f,
  1.0f, 0.0f, 0.0f, 1.0f,

  0.5f, -0.25f, 0.0f,
  0.0f, 0.0f, 1.0f, 1.0f,

  0.0f, 0.559016994f, 0.0f,
  0.0f, 1.0f, 0.0f, 1.0f
};

const float Renderer::m_triangle2_vertices[] = {
  // X, Y, Z,
  // R, G, B, A
  -0.5f, -0.25f, 0.0f,
  1.0f, 1.0f, 0.0f, 1.0f,

  0.5f, -0.25f, 0.0f,
  0.0f, 1.0f, 1.0f, 1.0f,

  0.0f, 0.559016994f, 0.0f,
  1.0f, 0.0f, 1.0f, 1.0f
};

const float Renderer::m_triangle3_vertices[] = {
  // X, Y, Z,
  // R, G, B, A
  -0.5f, -0.25f, 0.0f,
  1.0f, 1.0f, 1.0f, 1.0f,

  0.5f, -0.25f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f,

  0.0f, 0.559016994f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};

Renderer::Renderer() {
  m_color_handle = 0;
  m_model_matrix = Mat4();
  m_MVP_matrix_handle = 0;
  m_MVP_matrix = Mat4();
  m_position_handle = 0;
  m_projection_matrix = Mat4();
  m_view_matrix = Mat4();
}

Renderer::~Renderer() {

}

Renderer::Renderer(const Renderer& other) {
  m_color_handle = other.m_color_handle;
  m_model_matrix = other.m_model_matrix;
  m_MVP_matrix_handle = other.m_MVP_matrix_handle;
  m_MVP_matrix = other.m_MVP_matrix;
  m_position_handle = other.m_position_handle;
  m_projection_matrix = other.m_projection_matrix;
  m_view_matrix = other.m_view_matrix;
}

int Renderer::compileShader(const string& shader_source, int shader_type) {
  int shader_handle = glCreateShader(shader_type);
  string shader_error;

  if (shader_handle != 0) {
    const char* source_str = shader_source.c_str();
    const int str_length = shader_source.length();
    glShaderSource(shader_handle, 1,
                   &source_str, &str_length);
    glCompileShader(shader_handle);

    int compile_status;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == 0) {
      int log_length;
      glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);
      char* error_msg = new char[log_length];
      glGetShaderInfoLog(shader_handle, log_length, NULL, error_msg);
      shader_error = error_msg;
      glDeleteShader(shader_handle);
      shader_handle = 0;
    }
  }
  if (shader_handle == 0) {
    string error = "Error creating the ";
    switch (shader_type) {
      case GL_FRAGMENT_SHADER:
        error += "fragment";
        break;
      case GL_VERTEX_SHADER:
        error += "vertex";
        break;
      default:
        error += "unknown";
        break;
    }
    error += " shader. Shader Log: " + shader_error;
    LOGE("Renderer", "%s", error.c_str());
  }

  return shader_handle;
}

void Renderer::drawTriangle(const float* triangle) {
  const float* triangle_pos = triangle + POSITION_OFFSET * sizeof(float);
  glVertexAttribPointer(m_position_handle, POSITION_DATA_SIZE,
                        GL_FLOAT, false, STRIDE_BYTES, triangle_pos);
  glEnableVertexAttribArray(m_position_handle);

  const float* triangle_color = triangle + COLOR_OFFSET * sizeof(float);
  glVertexAttribPointer(m_color_handle, COLOR_DATA_SIZE,
                        GL_FLOAT, false, STRIDE_BYTES, triangle_color);
  glEnableVertexAttribArray(m_color_handle);

  LOGD("Mult", "View * Model");
  m_view_matrix.debugOut();
  m_model_matrix.debugOut();
  m_MVP_matrix = m_view_matrix * m_model_matrix;
  m_MVP_matrix.debugOut();
  LOGD("Mult", "Result * Projection");
  m_projection_matrix.debugOut();
  m_MVP_matrix *= m_projection_matrix;
  m_MVP_matrix.debugOut();

  glUniformMatrix4fv(m_MVP_matrix_handle, 1, false,
                     m_MVP_matrix.getBuffer());
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int Renderer::linkProgram(int fragment_shader_handle,
                          int vertex_shader_handle) {
  int program_handle = glCreateProgram();
  string program_error;

  if (program_handle != 0) {
    glAttachShader(program_handle, vertex_shader_handle);
    glAttachShader(program_handle, fragment_shader_handle);

    glBindAttribLocation(program_handle, 0, "a_Position");
    glBindAttribLocation(program_handle, 1, "a_Color");

    glLinkProgram(program_handle);

    int compile_status;
    glGetProgramiv(program_handle, GL_LINK_STATUS, &compile_status);

    if (compile_status == 0) {
      int log_length;
      glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &log_length);
      char* error_msg = new char[log_length];
      glGetProgramInfoLog(program_handle, log_length, NULL, error_msg);
      program_error = error_msg;
      glDeleteProgram(program_handle);
      program_handle = 0;
    }
  }
  if (program_handle == 0) {
//    throw new runtime_error("Error compiling the GLES 2.0 shader program: " +
//                            program_error);
    LOGE("Renderer", "%s",
         ("Error compiling the GLES 2.0 shader program: " + program_error).c_str());
  }

  return program_handle;
}

void Renderer::onDrawFrame() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  clock_t ticks = clock();
  long time_mod = ticks % (5 * CLOCKS_PER_SEC); // Rotate every 5s
  float angle_rad = (TAU / 5 * CLOCKS_PER_SEC) * (time_mod);

  m_model_matrix.setIdentity();
  m_model_matrix.rotate(angle_rad, 0.0, 0.0, 1.0);
  drawTriangle(m_triangle1_vertices);

  m_model_matrix.setIdentity();
  m_model_matrix.translate(0.0, -1.0, 0.0);
  m_model_matrix.rotate(TAU / 4.0, 1.0, 0.0, 0.0);
  m_model_matrix.rotate(angle_rad, 0.0, 0.0, 1.0);
  drawTriangle(m_triangle2_vertices);

  m_model_matrix.setIdentity();
  m_model_matrix.translate(1.0, 0.0, 0.0);
  m_model_matrix.rotate(TAU / 4.0, 0.0, 1.0, 0.0);
  m_model_matrix.rotate(angle_rad, 0.0, 0.0, 1.0);
  drawTriangle(m_triangle3_vertices);
}

void Renderer::onSurfaceCreated(const string& fragment_shader,
                                const string& vertex_shader) {
  glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

  m_view_matrix.setLookAt(0.0, 0.0, 1.5,
                          0.0, 0.0, -5.0,
                          0.0, 1.0, 0.0);


  const int vertex_shader_handle = compileShader(vertex_shader, GL_VERTEX_SHADER);
  const int fragment_shader_handle = compileShader(fragment_shader, GL_FRAGMENT_SHADER);
  if (vertex_shader_handle != 0 && fragment_shader_handle != 0) {
    int program_handle = linkProgram(fragment_shader_handle,
                                     vertex_shader_handle);
    m_MVP_matrix_handle = glGetUniformLocation(program_handle, "u_MVPMatrix");
    m_position_handle = glGetAttribLocation(program_handle, "a_Position");
    m_color_handle = glGetAttribLocation(program_handle, "a_Color");
    glUseProgram(program_handle);
  }
}

void Renderer::onSurfaceChanged(int width, int height) {
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
    width *= 1 / ratio;
  } else {
    bottom = - (1 / ratio);
    top = 1 / ratio;
    height *= ratio;
  }

  m_projection_matrix.frustum(left, right, bottom, top, near, far);
}