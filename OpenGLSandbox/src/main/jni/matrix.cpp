#include "matrix.h"

Mat4::Mat4() {

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