#include "ee308.h"
#include <xc.h>
#include "bmp180.h"
#include <libpic30.h>

#pragma config JTAGEN=OFF, FWDTEN = 0

const unsigned char LookUp[16] = "0123456789ABCDEF";
unsigned char msgbuff[3] = "  0";
unsigned char count = 0, tempcount, tintflag = 0;

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _T1Interrupt ( void )
{
    LATAbits.LATA10 = ~LATAbits.LATA10;
    IFS0bits.T1IF = 0 ;
    tintflag = 1;
}

int main() { 

    int UT, T, CT;
    int X1, X2, X3;
    int B3, B4, B5, B6, B7;
    int UP, P;
    int AC1,AC2,AC3,AC4,AC5,AC6,B1,B2,MB,MC,MD;
    int ALT;
    
    init_IO();
    init_timer();
    init_I2C();
    init_LCD();
    
    AC1 = BMP180StartCal(AC1addr);
    AC2 = BMP180StartCal(AC2addr);
    AC3 = BMP180StartCal(AC3addr);
    AC4 = BMP180StartCal(AC4addr);
    AC5 = BMP180StartCal(AC5addr);
    AC6 = BMP180StartCal(AC6addr);
    B1 = BMP180StartCal(B1addr);
    B2 = BMP180StartCal(B2addr);
    MB = BMP180StartCal(MBaddr);
    MC = BMP180StartCal(MCaddr);
    MD = BMP180StartCal(MDaddr);
    
    while(1)
    {
		
		__delay_ms(500);
        PORTCbits.RC0 = 1;
        
        BMP180StartRT();
        __delay_ms(4.5);
        
        UT = BMP180readT();
        X1 = (UT-AC6)*AC5/32768;
        X2 = MC*2048/(X1+MD);
        B5 = X1 + X2;
        T = (B5 + 8)/16;
	
		BMP180StartRP();	
		__delay_ms(4.5);
        
        UP = BMP180readP();
        B6 = B5-4000;
        X1 =(B2*(B6*B6/4096))/2048;
        X2 = AC2*B6/2048;
        X3 = X1 + X2;
        B3 = ((AC1*4+X3)+2)/4;
        X1 = AC3*B6/8192;
        X2 = (B1*(B6*B6/4096))/65536;
        X3 = ((X1+X2)+2)/4;
        B4 = AC4*(unsigned int)(X3+32768)/32768;
        B7 = ((unsigned int)UP-B3);
        if(B7<0x80000000)
        {
            P = (B7*2)/B4;
        }
        else
        {
            P = (B7/B4)*2;
        }
        
        X1 = (P/256)*(P/256);
        X1 = (X1*3038)/65536;
        X2 = (-7357*P)/65536;
        P = P + (X1+X2+3791)/16;
        
        //ALT = 44330*(1-(P/1013.25)^(1/5.255));
        
        if(tintflag)
        {
            tintflag = 0;
            //count++;
            msgbuff[2] = T%10 + '0';
            tempcount = (unsigned char) T/10;
            if (tempcount == 0) msgbuff[1] = ' '; else msgbuff[1] = tempcount%10 + '0';
            tempcount = (unsigned char) tempcount/10;
            if (tempcount == 0) msgbuff[0] = ' '; else msgbuff[0] = tempcount%10 + '0';
            printXY(0,12,3,msgbuff);
            
            msgbuff[2] = P%10 + '0';
            tempcount = (unsigned char) P/10;
            if (tempcount == 0) msgbuff[1] = ' '; else msgbuff[1] = tempcount%10 + '0';
            tempcount = (unsigned char) tempcount/10;
            if (tempcount == 0) msgbuff[0] = ' '; else msgbuff[0] = tempcount%10 + '0';
            printXY(1,13,3,msgbuff);
        }
        
		
		__delay_ms(500);
		PORTCbits.RC0 = 0;
        
    }
    return 0;
}
