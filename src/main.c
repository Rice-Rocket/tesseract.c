#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int RES_X = 80;
const int RES_Y = 22;
const int BUF_SIZE = RES_X * RES_Y;

void plot(char buf[BUF_SIZE], float u, float v, float b) {
    int x = u * RES_X;
    int y = v * RES_Y;
    int o = x + RES_X * y;
    int i = 11 * b;
    buf[o] = i[".,-~:;=!*#$@"];
}

int main() {
    char buf[BUF_SIZE];

    for (;;) {
        memset(buf, 32, BUF_SIZE);
        plot(buf, 0.5, 0.5, 1.0);
        for (int k = 0; 1761 > k; k++)
            putchar(k % 80 ? buf[k] : 10);
        usleep(15000);
        printf("\x1b[23A");
    }

    return 0;
}
