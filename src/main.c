#include "draw.h"
#include "matrix.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void connect_point(char buf[BUF_SIZE], Vec2 points[16], int i, int j,
                   int offset) {
    Vec2 a = points[i + offset];
    Vec2 b = points[j + offset];
    draw_line_uv(buf, a.m[0], a.m[1], b.m[0], b.m[1]);
}

const Vec4 points[16] = {
    {-1, -1, 1, 1},   {1, -1, 1, 1},   {1, 1, 1, 1},   {-1, 1, 1, 1},
    {-1, -1, -1, 1},  {1, -1, -1, 1},  {1, 1, -1, 1},  {-1, 1, -1, 1},
    {-1, -1, 1, -1},  {1, -1, 1, -1},  {1, 1, 1, -1},  {-1, 1, 1, -1},
    {-1, -1, -1, -1}, {1, -1, -1, -1}, {1, 1, -1, -1}, {-1, 1, -1, -1},
};

int main() {
    char buf[BUF_SIZE];

    Vec2 projected_points[16] = {
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
    };

    Mat3 tesseract_rotation = R3x(0, -1); // theta = -pi/2
    float angle = 0;
    Vec2 cube_position = {.m = {0.5, 0.5}};
    float cube_scale = 4.0;

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
            Vec4 r3 = points[i];

            r3 = matmul4(&rxy, &r3);
            r3 = matmul4(&rzw, &r3);

            // Stereographic projection
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

            draw_point_uv(buf, projected_points[i].m[0],
                          projected_points[i].m[1], 1.0);
        }

        for (int m = 0; m < 4; m++) {
            connect_point(buf, projected_points, m, (m + 1) % 4, 8);
            connect_point(buf, projected_points, m + 4, ((m + 1) % 4) + 4, 8);
            connect_point(buf, projected_points, m, m + 4, 8);
        }

        for (int m = 0; m < 4; m++) {
            connect_point(buf, projected_points, m, (m + 1) % 4, 0);
            connect_point(buf, projected_points, m + 4, ((m + 1) % 4) + 4, 0);
            connect_point(buf, projected_points, m, m + 4, 0);
        }

        for (int m = 0; m < 8; m++) {
            connect_point(buf, projected_points, m, m + 8, 0);
        }

        angle += 0.01;

        for (int k = 0; BUF_SIZE + 1 > k; k++)
            putchar(k % RES_X ? buf[k] : 10);
        usleep(15000);
        printf("\x1b[%dA", RES_Y + 1);
    }

    return 0;
}
