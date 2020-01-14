//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
//передать S-meter
void SmeterTransmitt(void)
{
    UCHAR sm;
    sm = ADCmeasure();
    putchar(' ');
    putchar('0');
    putchar(0x30 + (UCHAR)(sm/100));
    putchar(0x30 + (UCHAR)((sm%100)/10));
    putchar(0x30 + (UCHAR)(sm%10));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//передать частоту и S-meter 3 раза
void FreqAndSmeterTransmitt(void)
{
    UCHAR index, jj;
    LCDfreqCalculate(radio.FreqCurrent);//"пересчитать в разряды"
    //передаю "F "
    putchar(0x0D);
    putchar('F');
    putchar(' ');
    //передаю частоту XXX.XXX
    for (index = 0; index < 7; index++) putchar(menu.Fdig[index]);
    //передаю 3 раза S-meter
    jj = 3;
    while(jj--) SmeterTransmitt();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void FreqCurrIncr(void);
void FreqCurrDecr(void);
//RS232
void RS232Control(void)
{
    UCHAR val, index;
    //сообщение не закончено - выйти...
    if (usart.endRX == 0) return;
    //-----------------------------------------------------------
    val = usart.buffRX[2] - 0x30;
    if (val > 9) return;//ошибка - выйти...
    switch(usart.buffRX[0])
    {
        //Установка частоты настройки приемника XXX.XXX
        case 'F': {
            menu.Fval[5] = usart.buffRX[usart.numbRX - 1] - 0x30;
            menu.Fval[4] = usart.buffRX[usart.numbRX - 2] - 0x30;
            menu.Fval[3] = usart.buffRX[usart.numbRX - 3] - 0x30;
            //здесь точка!!! [4]
            menu.Fval[2] = usart.buffRX[usart.numbRX - 5] - 0x30;
            menu.Fval[1] = usart.buffRX[usart.numbRX - 6] - 0x30;
            menu.Fval[0] = usart.buffRX[usart.numbRX - 7] - 0x30;
            if (menu.Fval[0] > 9) menu.Fval[0] = 0;
            radio.FreqCurrent = FreqNewValue();
            SetRADIOfreq();
            //передать частоту и S-meter 3 раза
            FreqAndSmeterTransmitt();
        } break;
        //Увеличение частоты настройки приемника на один шаг
        case '+': {
            FreqCurrIncr();//увеличить частоту
            //передать частоту и S-meter 3 раза
            FreqAndSmeterTransmitt();
        } break;
        //Уменьшение частоты настройки приемника на один шаг
        case '-': {
            FreqCurrDecr();//уменьшить частоту
            //передать частоту и S-meter 3 раза
            FreqAndSmeterTransmitt();
        } break;
        //Запрос текущей частоты настройки приемника
        case '.': {
            //передать частоту и S-meter 3 раза
            FreqAndSmeterTransmitt();
        } break;
        //Запрос текущих значений S-метра
        case ',': {
            index = 5;
            while(index--) SmeterTransmitt();//5 раз S-meter
        } break;
        //Установка напряжения регулирования усиления (Uару ) селектора каналов KS-H-148
        case 'U': {
            radioadd.AGC = _AGC_MAN;
            radioadd.LevAGC = ((usart.buffRX[usart.numbRX - 1] * 100) +
                            (usart.buffRX[usart.numbRX - 2] * 10) +
                            (usart.buffRX[usart.numbRX - 3])) / (1024 / _LINERES);
            UagcSet();//АРУ ШИМ
        } break;
        //Включение / выключение встроенного усилителя селектора каналов KS-H-148
        case 'G': {
            radioadd.Gain = (~val) & 0x1;
            SetRADIOfreq();
        } break;
        //Установка вида модуляции приемника
        case 'M': {
            radioadd.Modulation = val & 0x3;
            SetRADIOfreq();
        } break;
        //Включение / выключение динамика приемника
        case 'L': {
            if (val == 0) usart.MuteCntr = 1;//вкл. всегда
            if (val == 1) usart.MuteCntr = 2;//откл. громкость
            if (val == 2) usart.MuteCntr = 0;//авто управление
        } break;
        //Установка шага частоты настройки приемника
        case 'S': {
            radioadd.FreqStep = val & 0x7;
        } break;
    }//end switch
    //end
    putchar(0x0D);
    putchar('>');
    LCDviewMainData();//обновить данные на LCD
    usart.endRX = 0;//сбросить флаг окончания приема
    usart.numbRX = 0;//сбросить счетчик байт
    usart.startRX = 0;//сбросить флаг начала отсчета временного интервала
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/