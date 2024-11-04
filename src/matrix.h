#ifndef __matrix_h__
#define __matrix_h__

typedef struct {
    float m[3][3];
} Mat3;

typedef struct {
    float m[4][4];
} Mat4;

typedef struct {
    float m[4];
} Vec4;

typedef struct {
    float m[3];
} Vec3;

typedef struct {
    float m[2];
} Vec2;

Mat3 matmul33(Mat3 *a, Mat3 *b);
Mat4 matmul44(Mat4 *a, Mat4 *b);

Vec3 matmul3(Mat3 *m, Vec3 *v);
Vec4 matmul4(Mat4 *m, Vec4 *v);

#endif // __matrix_h__
