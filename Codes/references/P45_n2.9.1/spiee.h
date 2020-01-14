//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//передача по SPI аппаратно (с максимальной скоростью)
void SPIsend(UCHAR byte, UCHAR dord)
 {
    SPCR = (1 << SPE) | (1 << MSTR)|(1<<SPR1)  | (1<<SPR0);
    if (dord == 1) SPCR |= (1 << DORD);//LSB first
    SPSR = (0 << SPI2X);// 1=> speed = 4MHz

    SPDR = byte;
    while(!(SPSR & (1 << SPIF)));
    SPCR = 0x00; //off SPI module
    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//чтение EEPROM
void ReadEEPROMbuff(UCHAR *buff, UINT addr, UINT len)
 {
    pEE = 0;
    pEE += addr;
    while(len--) *buff++ = *pEE++;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//запись EEPROM
void WriteEEPROMbuff(UCHAR *buff, UINT addr, UINT len)
 {
    pEE = 0;
    pEE += addr;
    while(len--) *pEE++ = *buff++;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//восстанавливаю настройки из EEPROM
void RestoreSetupFromEEPROM(void)
 {
    radio.FreqCurrent = FreqCurrent;
    radioadd.Modulation = Modulation;
    radioadd.AGC = AGC;             //
    radioadd.LevAGC = LevAGC;       //
    radioadd.Gain = Gain;           //
    //
    radioadd.Volume = Volume;
    radioadd.FreqScanMin = FreqScanMin;
    radioadd.FreqScanMax = FreqScanMax;
    radioadd.FreqStep = FreqStep;
    radioadd.SQL = SQL;
    radioadd.Pause = Pause;
    radioadd.Speed = Speed;
    radioadd.bank = bank;
    radioadd.scan = scan;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

