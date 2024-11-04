#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int RES_X = 80;
const int RES_Y = 22;
const int BUF_SIZE = RES_X * RES_Y;

typedef struct {
    float v[4][4];
} Mat4;

Mat4 matmul(Mat4 *a, Mat4 *b) {
    Mat4 res;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            float sum = 0;

            for (int k = 0; k < 4; k++) {
                sum += a->v[x][k] * b->v[k][y];
            }

            res.v[x][y] = sum;
        }
    }

    return res;
}

void plot_point(char buf[BUF_SIZE], int x, int y, float b) {
    int o = x + RES_X * y;
    int i = 11 * b;
    buf[o] = i[".,-~:;=!*#$@"];
}

void plot_point_uv(char buf[BUF_SIZE], float u, float v, float b) {
    plot_point(buf, u * RES_X, v * RES_Y, b);
}

int main() {
    char buf[BUF_SIZE];

    for (;;) {
        memset(buf, 32, BUF_SIZE);
        plot_point_uv(buf, 0.5, 0.5, 1.0);
        for (int k = 0; 1761 > k; k++)
            putchar(k % 80 ? buf[k] : 10);
        usleep(15000);
        printf("\x1b[23A");
    }

    return 0;
}
