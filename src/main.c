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

// Bresenham's line algorithm
// TODO: Xiaolin Wu's line algorithm
void plot_line(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        plot_point(buf, x, y, 1.0);
        if (d > 0) {
            y++;
            d -= 2 * dx;
        }
        d += 2 * dy;
    }
}

void plot_line_uv(char buf[BUF_SIZE], float u0, float v0, float u1, float v1) {
    plot_line(buf, u0 * RES_X, v0 * RES_Y, u1 * RES_X, v1 * RES_Y);
}

int main() {
    char buf[BUF_SIZE];

    for (;;) {
        memset(buf, 32, BUF_SIZE);
        plot_line_uv(buf, 0.1, 0.2, 0.8, 0.7);
        for (int k = 0; 1761 > k; k++)
            putchar(k % 80 ? buf[k] : 10);
        usleep(15000);
        printf("\x1b[23A");
    }

    return 0;
}
