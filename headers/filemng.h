#pragma once
#include <stdio.h>
int read_file(int x, int y, char *s, int len, int map[len][len], int fx, int fy) {
    FILE *fp = fopen(s,"r");
    char c;
    int sx = 0;
    int sy = 0;
    while(fp != NULL && (c = getc(fp)) != EOF) {
        if(c == '0') {
            map[(x+sx*fx+len)%len][(y+sy*fy+len)%len] = 0;
            sx++;
        }
        if(c == 'n') {
            sx++;
        }
        if(c == '1') {
            map[(x+sx*fx+len)%len][(y+sy*fy+len)%len] = 1;
            sx++;
        }
        if(c == '\n') {
            sy++;
            sx = 0;
        }
    }
    fclose(fp);
    if(fp == NULL)
        return 0;
    return 1;
}
void write_file(char *s, int len, int map[len][len], int null) {
    FILE *fp = fopen(s, "w");
    for(int x = 0; x < len; x++) {
        for(int y = 0; y < len; y++) {
            putc(map[y][x] ? '1' : ((null) ? 'n' : '0'),fp);
        }
        putc('\n',fp);
    }
    fclose(fp);
}