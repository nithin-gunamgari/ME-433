#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"font.h"
#include"ssd1306.h"
#include<stdio.h>
#include<string.h>
#include"ws2812b.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0xffff // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0;
    TRISBbits.TRISB4 = 1;
    
    adc_setup();
    ctmu_setup();
    i2c_master_setup();
    ssd1306_setup();
    ws2812b_setup();

    __builtin_enable_interrupts();
    
    wsColor c[4];
    
    c[0].b = 0;
    c[0].g = 0;
    c[0].r = 0;
    c[3].b = 0;
    c[3].g = 0;
    c[3].r = 0;
    c[1].b = 0;
    c[1].g = 0;
    c[1].r = 0;
    c[2].b = 0;
    c[2].g = 0;
    c[2].r = 0;
    
    ws2812b_setColor(c,4);
    
    
     
    int left,right,i,dl,dr,sum,l,r;
    float lp,rp,pos;
    char message[50];

    while (1) {
        
        i = 0;
        left = 0;
        right = 0;
        for (i;i<10;i++)
        {
            left = left + ctmu_read(4,15);
            right = right + ctmu_read(5,15);
        }
        
        sprintf(message, "%d", left); 
        drawString(5, 0, message);
        sprintf(message, "%d", right); 
        drawString(5, 8, message);
        
        dl = 8500-left ;
        dr = 8900-right ;
        sum = dl+dr;
        lp = (dl*100)/(dl+dr);
        rp = ((1-dr)*100)/(dl+dr);
        
        pos = (lp+rp)/2;
        sprintf(message, "%f", pos); 
        drawString(5, 16, message);
        
        if (sum > 2500)
        {
            if (pos >= 0 & pos <= 30)
            {
                l = (pos)* 255/100;
                c[1].g = l;
            }
            else if (pos < 0 & pos >= -35)
            {
               l = -(pos)* 255/100;
               c[1].g = l; 
            }
            else if (pos > 30)
            {
                c[0].r = 255;
            }
            else
            {
                c[3].b = 255;
            }
        }
        else
        {
           c[1].g = 0;
           c[0].r = 0;
           c[3].b = 0;
        }
        ws2812b_setColor(c,4);
    }
}