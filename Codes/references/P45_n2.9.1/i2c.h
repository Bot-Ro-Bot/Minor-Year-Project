//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//I2C start bit
void I2CStart(void)
{
    _I2C_BitClk = 1;
    _I2C_BitDat = 1;
    delay_us(10);
    _I2C_BitDat = 0;
    delay_us(10);
    _I2C_BitClk = 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//I2C stop bit
void I2CStop(void)
{
    _I2C_BitDat = 0;
    _I2C_BitClk = 1;
    delay_us(10);
    _I2C_BitDat = 1;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//такт
void I2Cclk(void)
{
    delay_us(10);
    _I2C_BitClk = 1;
    delay_us(10);
    _I2C_BitClk = 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//передать байт по I2C
void I2CsendByte(UCHAR i2cByte)
{
    UCHAR jj;
    for(jj = 0; jj < 8; jj++)
    {
        if(i2cByte & 0x80) _I2C_BitDat = 1;
        else _I2C_BitDat = 0;
	    I2Cclk();
	    i2cByte <<= 1;
    }
    I2Cclk(); //ASK
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CONST__GAINoff 0B10001000
#define CONST__GAINon  0B10001001 // Weak Signal Booster ON
#define CONST__LoBAND  0B00000001
#define CONST__MiBAND  0B00000010
#define CONST__HiBAND  0B00001100
//передача данных в селектор
void KSH14xPLL(void)
{
    UINT  fPCH1;
    UCHAR Band;

    Band = CONST__LoBAND;

    if(radio.FreqCurrent > 155000L) Band = CONST__MiBAND;
    if(radio.FreqCurrent > 440000L) Band = CONST__HiBAND;

    fPCH1 = (UINT)((radio.FreqCurrent + (ULONG)_RCVPCH1) / (ULONG)50);

    //запретить прерывания
	_INT_OFF;

    I2CStart();
    I2CsendByte(0b11000000);   // Tuner Address
    I2CsendByte((UCHAR)(fPCH1 >> 8));
    I2CsendByte((UCHAR)fPCH1);
    if (radioadd.Gain) I2CsendByte(CONST__GAINon);
    else I2CsendByte(CONST__GAINoff);
    I2CsendByte(Band);
    I2CStop();

	//разрешить прерывания
 	_INT_ON;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
