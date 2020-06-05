#include "font.h"
#include "ssd1306.h"

void drawChar(unsigned char x, unsigned char y, unsigned char l ){
    
    int i,j;
    i = 0;
    unsigned char v;
    
    for (i;i<5;i++)
    {
        j = 0;
        for (j;j<8;j++)
        {
            v = (ASCII[ l - 0x20 ][i] >> j) & 1;
            ssd1306_drawPixel(x+i, y+j, v);
            
        }
    }
}

void drawString(unsigned char x, unsigned char y, unsigned char *m){
    
    int s = 0;
    unsigned char px = x ;
    unsigned char py = y ;
    while (m[s] != 0)
    {
        drawChar(px, py, m[s]);
        px = px + 5;
        s = s+1;
    }
    ssd1306_update();
    
}