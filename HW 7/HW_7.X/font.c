#include "font.h"
#include "ssd1306.h"
#include<math.h>

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

void barx(signed short x){
    
    double so;
    int o,i;
    so = (x/17000.0)*63.0 ;
    o = (int)(so);
    if (o >= 0)
    {
        for (i=0;i<=o;i++)
        {
            ssd1306_drawPixel(64+i,16,1);
        }
    }
    else 
    {
        o = o* -1;
        for (i=0;i<=o;i++)
        {
            ssd1306_drawPixel(64-i,16,1);
        }
    }
    ssd1306_update();
}
void bary(signed short y){
    
    double so;
    int o,i;
    so = (y/17000.0) * 15.0;
    o = (int)(so);
    if (o >= 0)
    {
        for (i=0;i<=o;i++)
        {
            ssd1306_drawPixel(64,16+i,1);
        }
    }
    else 
    {
        o = o* -1;
        for (i=0;i<=o;i++)
        {
            ssd1306_drawPixel(64,16-i,1);
        }
    }
    ssd1306_update();
}

