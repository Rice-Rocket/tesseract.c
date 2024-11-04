#include "matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int RES_X = 160;
const int RES_Y = 44;
const int BUF_SIZE = RES_X * RES_Y;

void plot_char(char buf[BUF_SIZE], int x, int y, char c) {
    int o = x + RES_X * y;
    buf[o] = c;
}

void plot_point(char buf[BUF_SIZE], int x, int y, float b) {
    int o = x + RES_X * y;
    int i = 11 * b;
    buf[o] = i[".,-~:;=!*#$@"];
}

void plot_point_uv(char buf[BUF_SIZE], float u, float v, float b) {
    plot_point(buf, u * RES_X, v * RES_Y, b);
}

void plot_line_low(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    char line_chars[11];

    int m;
    if (dy == 0) {
        m = 11;
    } else {
        int md = abs(dx / dy);
        m = md < 11 ? md : 11;
    }

    if (dy < 0) {
        yi = -1;
        dy = -dy;

        switch (m) {
        case 1:
            strcpy(line_chars, "/         ");
            break;
        case 2:
            strcpy(line_chars, "_^        ");
            break;
        case 3:
            strcpy(line_chars, "_-^       ");
            break;
        case 4:
            strcpy(line_chars, "_--^      ");
            break;
        case 5:
            strcpy(line_chars, "__-^^     ");
            break;
        case 6:
            strcpy(line_chars, "__--^^    ");
            break;
        case 7:
            strcpy(line_chars, "__---^^   ");
            break;
        case 8:
            strcpy(line_chars, "___--^^^  ");
            break;
        case 9:
            strcpy(line_chars, "___---^^^ ");
            break;
        case 10:
            strcpy(line_chars, "____--^^^^");
            break;
        default:
            strcpy(line_chars, "----------");
            break;
        }
    } else {
        switch (m) {
        case 1:
            strcpy(line_chars, "\\         ");
            break;
        case 2:
            strcpy(line_chars, "^_        ");
            break;
        case 3:
            strcpy(line_chars, "^-_       ");
            break;
        case 4:
            strcpy(line_chars, "^--_      ");
            break;
        case 5:
            strcpy(line_chars, "^^-__     ");
            break;
        case 6:
            strcpy(line_chars, "^^--__    ");
            break;
        case 7:
            strcpy(line_chars, "^^---__   ");
            break;
        case 8:
            strcpy(line_chars, "^^^--___  ");
            break;
        case 9:
            strcpy(line_chars, "^^^---___ ");
            break;
        case 10:
            strcpy(line_chars, "^^^^--____");
            break;
        default:
            strcpy(line_chars, "----------");
            break;
        }
    }

    int d = 2 * dy - dx;
    int y = y0;
    int run = 0;
    m = m < 10 ? m : 10;
    m--;

    for (int x = x0, i = 0; x <= x1; x++, i = (i + 1) % m) {
        char c = line_chars[run < m ? run : m];
        plot_char(buf, x, y, c);
        if (d > 0) {
            y += yi;
            d += 2 * (dy - dx);
            run = 0;
        } else {
            d += 2 * dy;
            run += 1;
        }
    }
}

void plot_line_high(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int d = 2 * dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        plot_char(buf, x, y, '|');
        if (d > 0) {
            x += xi;
            d += 2 * (dx - dy);
        } else {
            d += 2 * dx;
        }
    }
}

// Bresenham's line algorithm
void plot_line(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            plot_line_low(buf, x1, y1, x0, y0);
        } else {
            plot_line_low(buf, x0, y0, x1, y1);
        }
    } else {
        if (y0 > y1) {
            plot_line_high(buf, x1, y1, x0, y0);
        } else {
            plot_line_high(buf, x0, y0, x1, y1);
        }
    }
}

void plot_line_uv(char buf[BUF_SIZE], float u0, float v0, float u1, float v1) {
    plot_line(buf, u0 * RES_X, v0 * RES_Y, u1 * RES_X, v1 * RES_Y);
}

void connect_point(char buf[BUF_SIZE], Vec2 points[16], int i, int j,
                   int offset) {
    Vec2 a = points[i + offset];
    Vec2 b = points[j + offset];
    plot_line_uv(buf, a.m[0], a.m[1], b.m[0], b.m[1]);
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

            plot_point_uv(buf, projected_points[i].m[0],
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
