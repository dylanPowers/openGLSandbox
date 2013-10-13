#ifndef MATRIX_H
#define MATRIX_H

#define BUFFER_SIZE 16
class Mat4 {
  public:
    Mat4(void);
    ~Mat4(void);
    Mat4(const Mat4&);
    Mat4& operator=(const Mat4&);
    Mat4 operator*(const Mat4&) const;
    Mat4& operator*=(const Mat4&);

    void frustum(float left, float right,
                 float bottom, float top,
                 float near, float far);
    float getAt(int col, int row) const;
    float* getBuffer(void);
    void rotate(float radians, float x, float y, float z);
    void setAt(int col, int row, float value);
    void setIdentity(void);
    void translate(float x, float y, float z);

  private:
    float m_matrix_buffer[BUFFER_SIZE];
};

#endif