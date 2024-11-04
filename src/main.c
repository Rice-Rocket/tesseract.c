#include "matrix.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int RES_X = 80;
const int RES_Y = 22;
const int BUF_SIZE = RES_X * RES_Y;

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

    Vec4 points[16] = {
        {-1, -1, 1, 1},   {1, -1, 1, 1},   {1, 1, 1, 1},   {-1, 1, 1, 1},
        {-1, -1, -1, 1},  {1, -1, -1, 1},  {1, 1, -1, 1},  {-1, 1, -1, 1},
        {-1, -1, 1, -1},  {1, -1, 1, -1},  {1, 1, 1, -1},  {-1, 1, 1, -1},
        {-1, -1, -1, -1}, {1, -1, -1, -1}, {1, 1, -1, -1}, {-1, 1, -1, -1},
    };

    Vec2 projected_points[16] = {
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    };

    for (;;) {
        memset(buf, 32, BUF_SIZE);

        for (int k = 0; 1761 > k; k++)
            putchar(k % 80 ? buf[k] : 10);
        usleep(15000);
        printf("\x1b[23A");
    }

    return 0;
}
