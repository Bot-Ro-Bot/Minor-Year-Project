//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
//�������� S-meter
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
//�������� ������� � S-meter 3 ����
void FreqAndSmeterTransmitt(void)
{
    UCHAR index, jj;
    LCDfreqCalculate(radio.FreqCurrent);//"����������� � �������"
    //������� "F "
    putchar(0x0D);
    putchar('F');
    putchar(' ');
    //������� ������� XXX.XXX
    for (index = 0; index < 7; index++) putchar(menu.Fdig[index]);
    //������� 3 ���� S-meter
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
    //��������� �� ��������� - �����...
    if (usart.endRX == 0) return;
    //-----------------------------------------------------------
    val = usart.buffRX[2] - 0x30;
    if (val > 9) return;//������ - �����...
    switch(usart.buffRX[0])
    {
        //��������� ������� ��������� ��������� XXX.XXX
        case 'F': {
            menu.Fval[5] = usart.buffRX[usart.numbRX - 1] - 0x30;
            menu.Fval[4] = usart.buffRX[usart.numbRX - 2] - 0x30;
            menu.Fval[3] = usart.buffRX[usart.numbRX - 3] - 0x30;
            //����� �����!!! [4]
            menu.Fval[2] = usart.buffRX[usart.numbRX - 5] - 0x30;
            menu.Fval[1] = usart.buffRX[usart.numbRX - 6] - 0x30;
            menu.Fval[0] = usart.buffRX[usart.numbRX - 7] - 0x30;
            if (menu.Fval[0] > 9) menu.Fval[0] = 0;
            radio.FreqCurrent = FreqNewValue();
            SetRADIOfreq();
            //�������� ������� � S-meter 3 ����
            FreqAndSmeterTransmitt();
        } break;
        //���������� ������� ��������� ��������� �� ���� ���
        case '+': {
            FreqCurrIncr();//��������� �������
            //�������� ������� � S-meter 3 ����
            FreqAndSmeterTransmitt();
        } break;
        //���������� ������� ��������� ��������� �� ���� ���
        case '-': {
            FreqCurrDecr();//��������� �������
            //�������� ������� � S-meter 3 ����
            FreqAndSmeterTransmitt();
        } break;
        //������ ������� ������� ��������� ���������
        case '.': {
            //�������� ������� � S-meter 3 ����
            FreqAndSmeterTransmitt();
        } break;
        //������ ������� �������� S-�����
        case ',': {
            index = 5;
            while(index--) SmeterTransmitt();//5 ��� S-meter
        } break;
        //��������� ���������� ������������� �������� (U��� ) ��������� ������� KS-H-148
        case 'U': {
            radioadd.AGC = _AGC_MAN;
            radioadd.LevAGC = ((usart.buffRX[usart.numbRX - 1] * 100) +
                            (usart.buffRX[usart.numbRX - 2] * 10) +
                            (usart.buffRX[usart.numbRX - 3])) / (1024 / _LINERES);
            UagcSet();//��� ���
        } break;
        //��������� / ���������� ����������� ��������� ��������� ������� KS-H-148
        case 'G': {
            radioadd.Gain = (~val) & 0x1;
            SetRADIOfreq();
        } break;
        //��������� ���� ��������� ���������
        case 'M': {
            radioadd.Modulation = val & 0x3;
            SetRADIOfreq();
        } break;
        //��������� / ���������� �������� ���������
        case 'L': {
            if (val == 0) usart.MuteCntr = 1;//���. ������
            if (val == 1) usart.MuteCntr = 2;//����. ���������
            if (val == 2) usart.MuteCntr = 0;//���� ����������
        } break;
        //��������� ���� ������� ��������� ���������
        case 'S': {
            radioadd.FreqStep = val & 0x7;
        } break;
    }//end switch
    //end
    putchar(0x0D);
    putchar('>');
    LCDviewMainData();//�������� ������ �� LCD
    usart.endRX = 0;//�������� ���� ��������� ������
    usart.numbRX = 0;//�������� ������� ����
    usart.startRX = 0;//�������� ���� ������ ������� ���������� ���������
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/