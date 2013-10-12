#ifndef MATRIX_H
#define MATRIX_H

#define BUFFER_SIZE 16
class Mat4 {
  public:
    Mat4();
    ~Mat4();
    Mat4(const Mat4&);
    Mat4& operator=(const Mat4&);



  private:
    float m_matrix_buffer[BUFFER_SIZE];
};

#endif