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
    //m_matrix_buffer[i] *= rhs.m_matrix_buffer[i];
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

}

float Mat4::getAt(int col, int row) const {
  // m_matrix_buffer[col][row] (opposite to normal C conventions)
  return m_matrix_buffer[INDEX(col, row)];
}

//static int Mat4::getIndexAt(int col, int row) {
//  return col * 4 + row;
//}

float* Mat4::getBuffer() {
  return m_matrix_buffer;
}

void Mat4::rotate(float radians, float x, float y, float z) {

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
  float z1 = look_x - eye_x;
  float z2 = look_y - eye_y;
  float z3 = look_z - eye_z;
  float mag = sqrt(z1*z1 + z2*z2 + z3*z3);
  z1 = z1 / mag;
  z2 = z2 / mag;
  z3 = z3 / mag;

//  mag = sqrt(up_x*up_x + up_y*up_y + up_z*up_z);
//  float y1 = up_x / mag;
//  float y2 = up_y / mag;
//  float y3 = up_z / mag;
//
//  float x1 = y2*z3 - y3*z2;
//  float x2 = -y1*z3 + y3*z1;
//  float x3 = y1*z2 - y2*z1;

  float x1 = -up_y*z3 + up_z*z2;
  float x2 = up_x*z3 - up_z*z1;
  float x3 = -up_x*z2 + up_y*z1;
  mag = sqrt(x1*x1 + x2*x2 + z3*z3);
  x1 /= mag;
  x2 /= mag;
  x3 /= mag;

  float y1 = -z2*x3 + z3*x2;
  float y2 = z1*x3 - z3*x1;
  float y3 = -z1*x2 + z2*x1;
//  mag = sqrt(y1*y1 + y2*y2 + y3*y3);
//  y1 /= mag;
//  y2 /= mag;
//  y3 /= mag;

  LOGD("Matrix", "\n[%f %f %f]\n[%f %f %f]\n[%f %f %f]",
       x1, y1, z1,
       x2, y2, z2,
       x3, y3, z3);

  setAt(0, 0, x1);
  setAt(0, 1, x2);
  setAt(0, 2, x3);

  setAt(1, 0, y1);
  setAt(1, 1, y2);
  setAt(1, 2, y3);

  setAt(2, 0, z1);
  setAt(2, 1, z2);
  setAt(2, 2, z3);

//  setAt(3, 0, eye_x);
//  setAt(3, 1, eye_y);
//  setAt(3, 2, eye_z);

  translate(-eye_x, -eye_y, -eye_z);
  //debugOut();
}

void Mat4::translate(float x, float y, float z) {
  for (int i = 0; i < 4; ++i) {
    m_matrix_buffer[INDEX(3, i)] += m_matrix_buffer[INDEX(0, i)] * x +
                                    m_matrix_buffer[INDEX(1, i)] * y +
                                    m_matrix_buffer[INDEX(2, i)] * z;
  }
}