#ifndef __draw_h__
#define __draw_h__

extern const int RES_X;
extern const int RES_Y;
extern const int BUF_SIZE;

void draw_char(char buf[BUF_SIZE], int x, int y, char c);
void draw_point(char buf[BUF_SIZE], int x, int y, float b);
void draw_point_uv(char buf[BUF_SIZE], float u, float v, float b);

void draw_line_low(char buf[BUF_SIZE], int x0, int y0, int x1, int y1);
void draw_line_high(char buf[BUF_SIZE], int x0, int y0, int x1, int y1);
void draw_line(char buf[BUF_SIZE], int x0, int y0, int x1, int y1);
void draw_line_uv(char buf[BUF_SIZE], float u0, float v0, float u1, float v1);

#endif // __draw_h__
