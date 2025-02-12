#include "ee308.h"
#include <xc.h>

#pragma config JTAGEN=OFF, FWDTEN = 0

const unsigned char LookUp[16] = "0123456789ABCDEF";
unsigned char msgbuff[2] = "  0";
unsigned char count = 0, tempcount, tintflag = 0; 

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _T1Interrupt ( void )
{
    LATAbits.LATA10 = ~LATAbits.LATA10;
    IFS0bits.T1IF = 0 ;
    tintflag = 1;
}

int main() { 
    init_IO();
    init_timer();
    init_I2C();
    init_LCD();
    
    //asm volatile("push w7");
    //asm volatile("push w8");
    //asm volatile("disi #5");
    //asm volatile("mov #0x55, w7");
    //asm volatile("mov w7, _NVMKEY");
    //asm volatile("mov #0xAA, w8");
    //asm volatile("mov w8, _NVMKEY");
    //asm volatile("bset _RCFGCAL, #13");
    //asm volatile("pop w8");
    //asm volatile("pop w7");
    
    //RTCPWCbits.RTCLK1 = 0;
    //RTCPWCbits.RTCLK0 = 1;
    
    //RCFGCALbits.RTCEN = 1;
    
    //RCFGCALbits.RTCPTR0 = 0;
    //RCFGCALbits.RTCPTR1 = 0;
    
    while(1)
    {
        if(tintflag)
        {
            tintflag = 0;
        //count = (RTCVAL&0xFF);
        count++;
        msgbuff[1] = count%10 + '0';
        tempcount = (unsigned char) count/10;
        if (tempcount == 0) msgbuff[0] = ' '; else msgbuff[0] = tempcount%10 + '0';
        printXY(1,14,2,msgbuff);
        }
        
    }
    return 0;
}
