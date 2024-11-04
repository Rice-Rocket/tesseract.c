#include "matrix.h"

#include <math.h>
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

    Mat3 tesseract_rotation = R3x(0, -1); // theta = -pi/2
    float angle = 0;
    Vec2 cube_position = {.m = {0.5, 0.5}};
    float cube_scale = 3.0;

    for (;;) {
        memset(buf, 32, BUF_SIZE);

        float cos_angle = cos(angle);
        float sin_angle = sin(angle);

        Mat3 rx = R3x(cos_angle, sin_angle);
        Mat3 ry = R3y(cos_angle, sin_angle);
        Mat3 rz = R3z(cos_angle, sin_angle);

        Mat4 rxy = R4xy(cos_angle, sin_angle);
        Mat4 rxz = R4xz(cos_angle, sin_angle);
        Mat4 rxw = R4xw(cos_angle, sin_angle);
        Mat4 ryz = R4yz(cos_angle, sin_angle);
        Mat4 ryw = R4yw(cos_angle, sin_angle);
        Mat4 rzw = R4zw(cos_angle, sin_angle);

        // Project points
        for (int i = 0; i < 16; i++) {
            Vec4 point = points[i];

            Vec4 r3 = matmul4(&rxy, &point);
            r3 = matmul4(&rzw, &r3);

            float distance = 5;
            float w = 1.0 / (distance - r3.m[3]);

            Mat4 proj_m4 = {.m = {
                                {w, 0, 0, 0},
                                {0, w, 0, 0},
                                {0, 0, w, 0},
                                {0, 0, 0, 1},
                            }};

            Vec4 proj_3 = matmul4(&proj_m4, &r3);
            Vec3 r2 = {.m = {proj_3.m[0], proj_3.m[1], proj_3.m[2]}};
            // r2 = matmul3(&tesseract_rotation, &r2);
            r2 = matmul3(&rx, &r2);

            float z = 1.0 / (distance - (r2.m[2] + r3.m[3]));
            Mat3 proj_m3 = {.m = {{z, 0, 0}, {0, z, 0}, {0, 0, 1}}};

            Vec3 proj_2 = matmul3(&proj_m3, &r2);

            projected_points[i] =
                (Vec2){.m = {
                           proj_2.m[0] * cube_scale + cube_position.m[0],
                           proj_2.m[1] * cube_scale + cube_position.m[1],
                       }};

            plot_point_uv(buf, projected_points[i].m[0],
                          projected_points[i].m[1], 1.0);
        }

        angle += 0.01;

        for (int k = 0; 1761 > k; k++)
            putchar(k % 80 ? buf[k] : 10);
        usleep(15000);
        printf("\x1b[23A");
    }

    return 0;
}
