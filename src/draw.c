#include "draw.h"

#include <stdlib.h>
#include <string.h>

const int RES_X = 160;
const int RES_Y = 44;
const int BUF_SIZE = RES_X * RES_Y;

void draw_char(char buf[BUF_SIZE], int x, int y, char c) {
    int o = x + RES_X * y;
    buf[o] = c;
}

void draw_point(char buf[BUF_SIZE], int x, int y, float b) {
    int o = x + RES_X * y;
    int i = 11 * b;
    buf[o] = i[".,-~:;=!*#$@"];
}

void draw_point_uv(char buf[BUF_SIZE], float u, float v, float b) {
    draw_point(buf, u * RES_X, v * RES_Y, b);
}

void draw_line_low(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
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
            strcpy(line_chars, "/");
            break;
        case 2:
            strcpy(line_chars, "_^");
            break;
        case 3:
            strcpy(line_chars, "_-^");
            break;
        case 4:
            strcpy(line_chars, "_--^");
            break;
        case 5:
            strcpy(line_chars, "__-^^");
            break;
        case 6:
            strcpy(line_chars, "__--^^");
            break;
        case 7:
            strcpy(line_chars, "__---^^");
            break;
        case 8:
            strcpy(line_chars, "___--^^^");
            break;
        case 9:
            strcpy(line_chars, "___---^^^");
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
            strcpy(line_chars, "\\");
            break;
        case 2:
            strcpy(line_chars, "^_");
            break;
        case 3:
            strcpy(line_chars, "^-_");
            break;
        case 4:
            strcpy(line_chars, "^--_");
            break;
        case 5:
            strcpy(line_chars, "^^-__");
            break;
        case 6:
            strcpy(line_chars, "^^--__");
            break;
        case 7:
            strcpy(line_chars, "^^---__");
            break;
        case 8:
            strcpy(line_chars, "^^^--___");
            break;
        case 9:
            strcpy(line_chars, "^^^---___");
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
        draw_char(buf, x, y, c);
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

void draw_line_high(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
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
        draw_char(buf, x, y, '|');
        if (d > 0) {
            x += xi;
            d += 2 * (dx - dy);
        } else {
            d += 2 * dx;
        }
    }
}

// Bresenham's line algorithm
void draw_line(char buf[BUF_SIZE], int x0, int y0, int x1, int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            draw_line_low(buf, x1, y1, x0, y0);
        } else {
            draw_line_low(buf, x0, y0, x1, y1);
        }
    } else {
        if (y0 > y1) {
            draw_line_high(buf, x1, y1, x0, y0);
        } else {
            draw_line_high(buf, x0, y0, x1, y1);
        }
    }
}

void draw_line_uv(char buf[BUF_SIZE], float u0, float v0, float u1, float v1) {
    draw_line(buf, u0 * RES_X, v0 * RES_Y, u1 * RES_X, v1 * RES_Y);
}
