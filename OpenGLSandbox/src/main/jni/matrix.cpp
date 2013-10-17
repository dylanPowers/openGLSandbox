#include "matrix.h"

Mat4::Mat4() {
  setIdentity();
}

Mat4::~Mat4() {

}

Mat4::Mat4(const Mat4& other) {
//  for (int i = 0; i < BUFFER_SIZE; ++i) {
//    m_matrix_buffer[i] = other.m_matrix_buffer[i];
//  }
  *this = other;
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
  Mat4 copy = *this;
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      float cell_total = 0.0;
      for (int n = 0; n < 4; ++n) {
        cell_total += copy.getAt(n, row) * rhs.getAt(col, n);
      }

      m_matrix_buffer[INDEX(col, row)] = cell_total;
    }
  }
  return *this;
}

void Mat4::debugOut() const {
  LOGD("Matrix", "\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]",
       m_matrix_buffer[INDEX(0, 0)], m_matrix_buffer[INDEX(1, 0)],
          m_matrix_buffer[INDEX(2, 0)], m_matrix_buffer[INDEX(3, 0)],
       m_matrix_buffer[INDEX(0, 1)], m_matrix_buffer[INDEX(1, 1)],
          m_matrix_buffer[INDEX(2, 1)], m_matrix_buffer[INDEX(3, 1)],
       m_matrix_buffer[INDEX(0, 2)], m_matrix_buffer[INDEX(1, 2)],
          m_matrix_buffer[INDEX(2, 2)], m_matrix_buffer[INDEX(3, 2)],
       m_matrix_buffer[INDEX(0, 3)], m_matrix_buffer[INDEX(1, 3)],
          m_matrix_buffer[INDEX(2, 3)], m_matrix_buffer[INDEX(3, 3)]);

}

void Mat4::frustum(float left, float right,
                   float bottom, float top,
                   float near, float far) {
  setIdentity();
  SET_AT(0, 0, (2.0f * near) / (right - left));
  SET_AT(1, 1, (2.0f * near) / (top - bottom));

  SET_AT(2, 0, (right + left) / (right - left));
  SET_AT(2, 1, (top + bottom) / (top - bottom));
  SET_AT(2, 2, (far + near) / (far - near));
  SET_AT(2, 3, -1.0f);

  SET_AT(3, 2, (2.0f * far * near) / (far - near));
  SET_AT(3, 3, 0);
}

float Mat4::getAt(int col, int row) const {
  // m_matrix_buffer[col][row] (opposite to normal C conventions)
  return m_matrix_buffer[INDEX(col, row)];
}

const float* Mat4::getBuffer() {
  return m_matrix_buffer;
}

void Mat4::rotate(float radians, float x, float y, float z) {
  float mag = sqrt(x*x + y*y + z*z);
  x /= mag;
  y /= mag;
  z /= mag;

  float c = cos(radians);
  float s = sin(radians);
  float nc = 1 - c;

  Mat4 rotation = Mat4();
  rotation.setAt(0, 0, x*x*nc + c);
  rotation.setAt(0, 1, y*x*nc + z*s);
  rotation.setAt(0, 2, z*x*nc - y*s);

  rotation.setAt(1, 0, x*y*nc - z*s);
  rotation.setAt(1, 1, y*y*nc + c);
  rotation.setAt(1, 2, z*y*nc + x*s);

  rotation.setAt(2, 0, x*z*nc + y*s);
  rotation.setAt(2, 1, y*z*nc - x*s);
  rotation.setAt(2, 2, z*z*nc + c);
  *this *= rotation;
}

void Mat4::setAt(int col, int row, float value) {
  m_matrix_buffer[INDEX(col, row)] = value;
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

void Mat4::setLookAt(float eye_x, float eye_y, float eye_z,
                     float look_x, float look_y, float look_z,
                     float up_x, float up_y, float up_z) {

  // -(gaze direction)
  float z1 = -(look_x - eye_x);
  float z2 = -(look_y - eye_y);
  float z3 = -(look_z - eye_z);
  float mag = sqrt(z1*z1 + z2*z2 + z3*z3);
  z1 = z1 / mag;
  z2 = z2 / mag;
  z3 = z3 / mag;

  // up X z
  float x1 = up_y*z3 - up_z*z2;
  float x2 = -up_x*z3 + up_z*z1;
  float x3 = up_x*z2 - up_y*z1;
  mag = sqrt(x1*x1 + x2*x2 + x3*x3);
  x1 = x1 / mag;
  x2 /= mag;
  x3 /= mag;

  // z X x
  float y1 = z2*x3 - z3*x2;
  float y2 = -z1*x3 + z3*x1;
  float y3 = z1*x2 - z2*x1;

  SET_AT(0, 0, x1);
  SET_AT(0, 1, x2);
  SET_AT(0, 2, x3);
  SET_AT(0, 3, 0.0);

  SET_AT(1, 0, y1);
  SET_AT(1, 1, y2);
  SET_AT(1, 2, y3);
  SET_AT(1, 3, 0.0);

  SET_AT(2, 0, z1);
  SET_AT(2, 1, z2);
  SET_AT(2, 2, z3);
  SET_AT(2, 3, 0.0);

  SET_AT(3, 0, 0.0);
  SET_AT(3, 1, 0.0);
  SET_AT(3, 2, 0.0);
  SET_AT(3, 3, 1.0);

  translate(-eye_x, -eye_y, -eye_z);
}

void Mat4::translate(float x, float y, float z) {
  for (int i = 0; i < 4; ++i) {
    m_matrix_buffer[INDEX(3, i)] += m_matrix_buffer[INDEX(0, i)] * x +
                                    m_matrix_buffer[INDEX(1, i)] * y +
                                    m_matrix_buffer[INDEX(2, i)] * z;
  }
}