#include "matrix.h"

Mat4::Mat4() {
  setIdentity();
}

Mat4::~Mat4() {

}

Mat4::Mat4(const Mat4& other) {
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    m_matrix_buffer[i] = other.m_matrix_buffer[i];
  }
}

Mat4& Mat4::operator=(const Mat4& other) {
  if (&other != this) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
      m_matrix_buffer[i] = other.m_matrix_buffer[i];
    }
  }

  return *this;
}

Mat4 Mat4::operator*(const Mat4& rhs) const {
  Mat4 result = *this;
  result *= rhs;
  return result;
}

Mat4& Mat4::operator*=(const Mat4& rhs) {
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    m_matrix_buffer[i] *= rhs.m_matrix_buffer[i];
  }
  return *this;
}

void Mat4::frustum(float left, float right,
             float bottom, float top,
             float near, float far) {

}

float Mat4::getAt(int col, int row) const {
  // m_matrix_buffer[col][row] (opposite to normal C conventions)
  return m_matrix_buffer[col * 4 + row];
}

float* Mat4::getBuffer() {
  return m_matrix_buffer;
}

void Mat4::rotate(float radians, float x, float y, float z) {

}

void Mat4::setAt(int col, int row, float value) {
  m_matrix_buffer[col * 4 + row] = value;
}

void Mat4::setIdentity() {
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    if (i % 5 == 0) {
      m_matrix_buffer[i] = 1.0;
    } else {
      m_matrix_buffer[i] = 0.0;
    }
  }
}

void Mat4::translate(float x, float y, float z) {

}