#define	I2CaddrW 0xEE
#define I2CaddrR 0xEF

#define AC1addr 0XAA
#define AC2addr 0XAC
#define AC3addr 0XAE
#define AC4addr 0XB0
#define AC5addr 0XB2
#define AC6addr 0XB4
#define B1addr 0XB6
#define B2addr 0XB8
#define MBaddr 0XBa
#define MCaddr 0XBC
#define MDaddr 0XBE

int BMP180StartCal(unsigned int);
void BMP180StartRT(void);
void BMP180StartRT(void);
int  BMP180readT(void);
int  BMP180readP(void);
