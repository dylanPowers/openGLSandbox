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

}

int Renderer::compileProgram(int fragment_shader_handle,
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
    throw new runtime_error("Error compiling the GLES 2.0 shader program: " +
                            program_error);
  }

  return program_handle;
}

int Renderer::createShader(const string& shader_source, int shader_type) {
  int shader_handle = glCreateShader(shader_type);
  string shader_error;

  if (shader_handle != 0) {
    glShaderSource(shader_handle, 1,
                   &(shader_source.c_str()), shader_source.length());
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
    throw new runtime_error(error);
  }

  return shader_handle;
}

void Renderer::onDrawFrame() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::onSurfaceCreated(const string& fragment_shader,
                                const string& vertex_shader) {
  glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

  int vertex_shader_handle = 0;
  int fragment_shader_handle = 0;
  try {
    vertex_shader_handle = createShader(vertex_shader, GL_VERTEX_SHADER);
    fragment_shader_handle = createShader(fragment_shader, GL_FRAGMENT_SHADER);
  } catch (runtime_error error) {

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
  } else {
    bottom = - (1 / ratio);
    top = 1 / ratio;
  }
}