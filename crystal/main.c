#include "ee308.h"
#include <xc.h>

#pragma config JTAGEN=OFF, FWDTEN = 0

const unsigned char LookUp[16] = "0123456789ABCDEF";
unsigned char msgbuff[3] = "  0";
unsigned char count = 0, tempcount, tintflag = 0;

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _T2Interrupt ( void )
{
    LATAbits.LATA10 = ~LATAbits.LATA10;
    IFS0bits.T2IF = 0 ;
    tintflag = 1;
}

int main() { 
    init_IO();
    init_timer();
    init_I2C();
    init_LCD();
    
    while(1)
    {
        if(tintflag)
        {
            tintflag = 0;
            count++;
            msgbuff[2] = count%10 + '0';
            tempcount = (unsigned char) count/10;
            if (tempcount == 0) msgbuff[1] = ' '; else msgbuff[1] = tempcount%10 + '0';
            tempcount = (unsigned char) tempcount/10;
            if (tempcount == 0) msgbuff[0] = ' '; else msgbuff[0] = tempcount%10 + '0';
            printXY(1,13,3,msgbuff);
        }
    }
    return 0;
}
