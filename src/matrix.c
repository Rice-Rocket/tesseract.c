#include "matrix.h"

Mat3 matmul33(Mat3 *a, Mat3 *b) {
    Mat3 res;

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            float sum = 0;

            for (int k = 0; k < 3; k++) {
                sum += a->m[x][k] * b->m[k][y];
            }

            res.m[x][y] = sum;
        }
    }

    return res;
}

Mat4 matmul44(Mat4 *a, Mat4 *b) {
    Mat4 res;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            float sum = 0;

            for (int k = 0; k < 4; k++) {
                sum += a->m[x][k] * b->m[k][y];
            }

            res.m[x][y] = sum;
        }
    }

    return res;
}

Vec3 matmul3(Mat3 *m, Vec3 *v) {
    Vec3 res;

    for (int x = 0; x < 3; x++) {
        float sum = 0;

        for (int k = 0; k < 3; k++) {
            sum += m->m[x][k] * v->m[k];
        }

        res.m[x] = sum;
    }

    return res;
}

Vec4 matmul4(Mat4 *m, Vec4 *v) {
    Vec4 res;

    for (int x = 0; x < 4; x++) {
        float sum = 0;

        for (int k = 0; k < 4; k++) {
            sum += m->m[x][k] * v->m[k];
        }

        res.m[x] = sum;
    }

    return res;
}
