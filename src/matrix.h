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

#define R3x(cos, sin)                                                          \
    Mat3 {                                                                     \
        .m = { {1, 0, 0}, {0, cos, -sin}, {0, sin, cos} }                      \
    }

#define R3y(cos, sin)                                                          \
    Mat3 {                                                                     \
        .m = { {cos, 0, -sin}, {0, 1, 0}, {sin, 0, cos} }                      \
    }

#define R3z(cos, sin)                                                          \
    Mat3 {                                                                     \
        .m = { {cos, -sin, 0}, {sin, cos, 0}, {0, 0, 1} }                      \
    }

#define R4xy(cos, sin)                                                         \
    Mat4 {                                                                     \
        .m = {                                                                 \
            {cos, -sin, 0, 0},                                                 \
            {sin, cos, 0, 0},                                                  \
            {0, 0, 1, 0},                                                      \
            {0, 0, 0, 1}                                                       \
        }                                                                      \
    }

#define R4xz(cos, sin)                                                         \
    Mat4 {                                                                     \
        .m = {                                                                 \
            {cos, 0, -sin, 0},                                                 \
            {0, 1, 0, 0},                                                      \
            {sin, 0, cos, 0},                                                  \
            {0, 0, 0, 1}                                                       \
        }                                                                      \
    }

#define R4xw(cos, sin)                                                         \
    Mat4 {                                                                     \
        .m = {                                                                 \
            {cos, 0, 0, -sin},                                                 \
            {0, 1, 0, 0},                                                      \
            {0, 0, 1, 0},                                                      \
            {sin, 0, 0, cos}                                                   \
        }                                                                      \
    }

#define R4yz(cos, sin)                                                         \
    Mat4 {                                                                     \
        .m = {                                                                 \
            {1, 0, 0, 0},                                                      \
            {0, cos, -sin, 0},                                                 \
            {0, sin, cos, 0},                                                  \
            {0, 0, 0, 1}                                                       \
        }                                                                      \
    }

#define R4yw(cos, sin)                                                         \
    Mat4 {                                                                     \
        .m = {                                                                 \
            {1, 0, 0, 0},                                                      \
            {0, cos, 0, -sin},                                                 \
            {0, 0, 1, 0},                                                      \
            {0, sin, 0, cos}                                                   \
        }                                                                      \
    }

#define R4zw(cos, sin)                                                         \
    Mat4 {                                                                     \
        .m = {                                                                 \
            {1, 0, 0, 0},                                                      \
            {0, 1, 0, 0},                                                      \
            {0, 0, cos, -sin},                                                 \
            {0, 0, sin, cos}                                                   \
        }                                                                      \
    }

Mat3 matmul33(Mat3 *a, Mat3 *b);
Mat4 matmul44(Mat4 *a, Mat4 *b);

Vec3 matmul3(Mat3 *m, Vec3 *v);
Vec4 matmul4(Mat4 *m, Vec4 *v);

#endif // __matrix_h__
