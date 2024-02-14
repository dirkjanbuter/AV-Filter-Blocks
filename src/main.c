#include "main.h"

static int _w = 0;
static int _h = 0;
static int _framecount = 0;

static int _s = 0;
static int _sizebytes = 0;
static unsigned char *_oi;

void clearcolor(unsigned int color) {
    int i;
    unsigned char ina = (color >> 24) & 0xff;
    unsigned char inr = (color >> 16) & 0xff;
    unsigned char ing = (color >> 8) & 0xff;
    unsigned char inb = color & 0xff;

    for(i = 0; i < _s; i++) {
       _oi[i*4] = inr;
       _oi[i*4] = ing;
       _oi[i*4] = inb;
       _oi[i*4] = ina;
    }
}

void blockblend(int x, int y, int w, int h, unsigned int color) {
    int i, j;
    int sx = x;
    int sy = y;
    int ex = x+w;
    int ey = y+h;
    unsigned char ina = (color >> 24) & 0xff;
    unsigned char ininva = 0xff - ina;
    unsigned char inr = (color >> 16) & 0xff;
    unsigned char ing = (color >> 8) & 0xff;
    unsigned char inb = color & 0xff;

    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;

    for(j = sy; j < ey && j < _h; j++) for(i = sx; i < ex && i < _w; i++) {
       unsigned char orgr = _oi[(j*_w+i)*4];
       unsigned char orgg = _oi[(j*_w+i)*4+1];
       unsigned char orgb = _oi[(j*_w+i)*4+2];
       unsigned char outr = (orgr * ininva + inr * ina) >> 8;
       unsigned char outg = (orgg * ininva + ing * ina) >> 8;
       unsigned char outb = (orgb * ininva + inb * ina) >> 8;
       unsigned char outa = 0xff;
       _oi[(j*_w+i)*4] = outr;
       _oi[(j*_w+i)*4+1] = outg;
       _oi[(j*_w+i)*4+2] = outb;
       _oi[(j*_w+i)*4+3] = outa;
    }
}

int filterstep(unsigned char *buffer, int w, int h, unsigned int color, char *text, int64_t framecount) {
    int i;

    int bnum = (w*h*30)>>11;
    float bx[bnum];
    float by[bnum];
    float bs[bnum];
    float bd[bnum];

    _w = w;
    _h = h;
    _framecount = framecount;

    _s = _w * _h;
    _sizebytes = _s * 4;

    _oi = buffer;

    srand(0);
    for(i = 0; i < bnum; i++) {
        bx[i] = -50.0f + (float)(rand()%(_w+100));
        by[i] = -((float)_h+50.0f) + (float)(rand()%(_h*30+100));
        bs[i] = 10.0f+(float)(rand()%100);
        bd[i] = 0.01+(float)(rand()%1000)*0.01;

        by[i] -= bd[i]*(float)_framecount;
    }

    for(i = 0; i < bnum; i++) {
        blockblend((int)(bx[i]+0.5), (int)(by[i]+0.5), (int)(bs[i]+0.5), (int)(bs[i]+0.5), color);
    }

    return 1;
}
