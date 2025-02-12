#include "bmp180.h"
#include <xc.h>
#include <libpic30.h>

int BMP180StartCal(unsigned int c)
{
    unsigned char msb, lsb;
    int i;
    I2Cstart();
    I2Cwrite(I2CaddrW);
    I2Cwrite(c);
    I2Crestart();
    I2Cwrite(I2CaddrR);
    msb = I2Cread();
    I2Cack();
    lsb = I2Cread();
    I2Cack();
    I2Cstop();
    i = (msb<<8) + lsb;
    return(i);
}
void BMP180StartRT(void)
{
    I2Cstart();
    I2Cwrite(I2CaddrW);
    I2Cwrite(0XF4);
    I2Cwrite(0X2E);
    I2Cstop();
}

void BMP180StartRP(void)
{
    I2Cstart();
    I2Cwrite(I2CaddrW);
    I2Cwrite(0XF4);
    I2Cwrite(0X34);
    I2Cstop();
}

int BMP180readT(void)
{
    unsigned char msb, lsb;
    int i;
    I2Cstart();
    I2Cwrite(I2CaddrW);
    I2Cwrite(0xF6);
    I2Crestart();
    I2Cwrite(I2CaddrR);
    msb = I2Cread();
    I2Cack();
    lsb = I2Cread();
    I2Cnack();
    I2Cstop();
    i = (msb<<8) + lsb;
    return(i);
    
}

int BMP180readP(void)
{
    unsigned char msb, lsb, xlsb;
    int i;
    I2Cstart();
    I2Cwrite(I2CaddrW);
    I2Cwrite(0xF6);
    I2Crestart();
    I2Cwrite(I2CaddrR);
    msb = I2Cread();
    I2Cack();
    lsb = I2Cread();
    I2Cack();
    xlsb = I2Cread();
    I2Cnack();
    I2Cstop();
    i = msb;
    i = i<<8;
    i = i+ lsb;
    return(i);
    
}
