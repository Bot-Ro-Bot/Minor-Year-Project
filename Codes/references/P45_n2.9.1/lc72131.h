//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//управление синтезатором LC72131
void LC72131Set(void)
 {
    UCHAR step;
    UINT  fPCH2, FrcvPCH2, delta;

    delta = (UINT)(radio.FreqCurrent - ((radio.FreqCurrent / (ULONG)50) * (ULONG)50));
    FrcvPCH2 = (UINT)(IF1 - _RCVPCH2) - delta;
 //если шаг = 1к√ц
 //   if (ts[radioadd.FreqStep] == 1)
 //    {
    	fPCH2 = FrcvPCH2;
        step = 0x9D;
 //    }
 //дл€ всех остальных режимов перестройки частоты с шагом > 1к√ц
 //   else
 //    {
 //   	fPCH2 = FrcvPCH2 / (UINT)5;
 //       step = 0x95;
 //    }
    //*******************************************************
    _INT_OFF;
    SPIsend(0x14, 0);//LC72131_IN1

    //chip enable
    _NOP;_NOP;_NOP;
    _LC72131_LATCH = 1;

    SPIsend((UCHAR)fPCH2, 1);
    SPIsend((UCHAR)(fPCH2 >> 8), 1);

	//Lc72131 IN1
    SPIsend(step, 0);

    //chip disable
    _LC72131_LATCH = 0;
    _NOP;_NOP;_NOP;

	//Lc72131 IN2
    SPIsend(0x94, 0);//LC72131_IN2

    //chip enable
    _NOP;_NOP;_NOP;
    _LC72131_LATCH = 1;

    //данные дл€ установки вида модул€ции
    SPIsend(Modes[radioadd.Modulation], 0);
    SPIsend(0x00, 0);
    SPIsend(0x08, 0);

    //chip disable
    _LC72131_LATCH = 0;

	//разрешить прерывани€
	_INT_ON;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
