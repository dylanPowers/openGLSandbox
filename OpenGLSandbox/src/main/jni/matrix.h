#ifndef MATRIX_H
#define MATRIX_H

#include "log.h"

#include <cmath>
#include <sstream>
#include <string>
using std::string;
using std::ostringstream;

#define BUFFER_SIZE 16
#define INDEX(_col, _row) (_col * 4 + _row)

class Mat4 {
  public:
    Mat4(void);
    ~Mat4(void);
    Mat4(const Mat4&);
    Mat4& operator=(const Mat4&);
    Mat4 operator*(const Mat4&) const;
    Mat4& operator*=(const Mat4&);

    void debugOut(void) const;
    void frustum(float left, float right,
                 float bottom, float top,
                 float near, float far);
    float getAt(int col, int row) const;
    float* getBuffer(void);
    void rotate(float radians, float x, float y, float z);
    void setAt(int col, int row, float value);
    void setIdentity(void);
    void setLookAt(float eye_x, float eye_y, float eye_z,
                   float look_x, float look_y, float look_z,
                   float up_x, float up_y, float up_z);
    void translate(float x, float y, float z);

  private:
    float m_matrix_buffer[BUFFER_SIZE];
};

#endif