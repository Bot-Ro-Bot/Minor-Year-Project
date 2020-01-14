void Light_LED(void)
 {
   if ((timeLED>=pauseLED))
   {
     LED=0;
     timeLED=0;
  } else timeLED++;
 }
//====================================================================
void SaveAll (void)
 {
    FreqCurrent = radio.FreqCurrent;
    Volume = radioadd.Volume;
    SQL = radioadd.SQL;
    FreqStep = radioadd.FreqStep;
    LevAGC = radioadd.LevAGC;
    Pause = radioadd.Pause;
    Speed = radioadd.Speed;
    Gain = radioadd.Gain;
    Modulation = radioadd.Modulation;
    AGC = radioadd.AGC;
    FreqScanMin = radioadd.FreqScanMin;
    FreqScanMax = radioadd.FreqScanMax;
    bank = radioadd.bank;
    scan = radioadd.scan;
 }
//=====================================================================

//=====================================================================
//void RCkeyCheck(void);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���������� ��� ��� ���������� ���������
void VolumeSet(void)
 {
    UINT temp;
    //���� �UTE � �������� SQL
    if (flag.Mute && radioadd.SQL) {OCR1AH = 0x03; OCR1AL = 0xFF;}
     else//����� ��������� ���������
        if (radioadd.Volume <= _LINERES)
        {
            //���������� ��� ��� ���������
            temp = 20 + (UINT)(vol[radioadd.Volume]);
            OCR1AH = (UCHAR)((temp >> 8) & 0x03);
            OCR1AL = (UCHAR)temp;
        }
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT arrmin [16] = { 0,13,33,65,79, 97,129,145,177,237,273,353,433,545,673,865};   // ������� ��� S-metr
UINT arrmax [16] = {12,32,64,78,96,128,144,176,236,272,352,432,544,672,864,1023}; //
#define _AGC_MIN    150 //����������� �������� ��� ���������� ���
#define _AGC_MAX    1023 //������������ �������� ��� ���������� ��� 840
#define _UAGC_HI    14  //������������ �������� S-�����
//��� - 0.3...4 V (0...40-45 dB)
//��� = 230...1023
//���������� ��� ��� ���
void UagcSet(void)
 {
    UINT temp;
    //���������� ��� ��� ���
    if (radioadd.AGC == _AGC_OFF) temp = 1023;//off (������������ ��������)
     else
      {
        //auto
        if (radioadd.AGC == _AGC_AUTO) temp = UAGC;
        //manual
        else temp = ((UINT)radioadd.LevAGC * (1023 / (_LINERES + 8))) + _AGC_MIN;// + �������� 1.0v
      }
    if (temp > 1023) temp = 1023;
    //���������� ���
    OCR1BH = (UCHAR)((temp >> 8) & 0x03);
    OCR1BL = (UCHAR)temp;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���
UINT ADCmeasure(void);
//
void UAGCcontrol(void)
 { UINT smet;UCHAR ok;UCHAR k;
   static UCHAR delayAGC;
    //���� ������� ����� ����
    if (radioadd.AGC == _AGC_AUTO)
    {     if (UAGC < 500) k = 5;
             else k = 20;
          if ((delayAGC >= tauAGC) || ((smeter.curr < smeter.oldAGC)))
        {
            delayAGC = 0;
            smeter.oldAGC = smeter.curr;
            //�������� ��������� S-����� �� ��������
         do {
             if (smeter.oldAGC >= (_UAGC_HI + 1))//���� ��������, �� �������� ��������
              {
               if (UAGC >= _AGC_MIN)
                {
                  UAGC -=k; //�������� ���������� ��� (�������� ��������)
                  if (UAGC < _AGC_MIN) UAGC= _AGC_MIN;
                }
               }
                else //�������� ��������� S-����� �� ���������� ������������ ���������
	         {
	          if (UAGC <= _AGC_MAX)
	           {
    	            UAGC += k;//���������� ���������� ��� (���������� ��������)
                    if (UAGC > _AGC_MAX) UAGC = _AGC_MAX;
                   }
	        }
              smet = (UINT)ADCmeasure();
               if ((smet > 900)&&(smet < 1022)) ok = 1;
         }while(!ok);
             UagcSet();
         }
        else delayAGC++;
    }
    else UAGC = _AGC_MAX;//��� ���������
 }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��������� �������
void SetRADIOfreq(void)
 {
     LC72131Set();  //��������� �����������
     KSH14xPLL();   //��������� ���������
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��������� ���
UINT ADCmeasure(void)
 {
    UINT adc;
    ADMUX = 0x03; //channel 3
    delay_us(10);
    ADCSRA = 0b10000101; // 1000 0101  ADC on, interrupt disable, MCU clock / 32
    delay_us(200);
    ADCSRA |= (1 << ADSC); // start ADC
    while(ADCSRA & (1 << ADSC));
    adc = (UINT)ADCL | ((UINT)ADCH << 8);
    ADCSRA = 0x00; //off ADC
    return(adc);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT ADCbatt(void)
 {
    ADMUX = 0x02; //channel
    delay_us(10);
    ADCSRA = 0b10000101; // 1000 0101  ADC on, interrupt disable, MCU clock / 32
    delay_us(200);
    ADCSRA |= (1 << ADSC); // start ADC
    while(ADCSRA & (1 << ADSC));
    batt = (UINT)ADCL | ((UINT)ADCH << 8);
    ADCSRA = 0x00; //off ADC
    return(batt);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 void Battery(void)
 {
    if (flag.batt == 1)
    {
      ADCbatt();
      //��� AVCC=4.7V ,AVCC(� �������)/1024*10000
       vbatt = 46*batt;
       if (vbatt <= 32000){LED = 1;LCDoff();SaveAll();Beep(5);PowerSwith = Off;delay_ms(3000);}
       if ((vbatt > 32000)&&(vbatt <= 34000)){ vcharl = 139;vcharh =144;}
       if ((vbatt > 34000)&&(vbatt <= 36000)){ vcharl = 140;vcharh =144;}
       if ((vbatt > 36000)&&(vbatt <= 37000)){ vcharl = 141;vcharh =144;}
       if ((vbatt > 37000)&&(vbatt <= 37500)){ vcharl = 142;vcharh =144;}
       if ((vbatt > 37500)&&(vbatt <= 38000)){ vcharl = 143;vcharh =144;}
       if ((vbatt > 38000)&&(vbatt <= 39000)){ vcharl = 143;vcharh =145;}
       if ((vbatt > 39000)&&(vbatt <= 41000)){ vcharl = 143;vcharh =146;}
       if (vbatt > 41000){ vcharl = 143;vcharh =147;}
        LCDviewBatt();
        flag.batt = 0;
    }
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�����
void Beep(UCHAR rep)
 {
    while(rep--)
    {
        _BEEPER = 1;
        delay_ms(100);
        _BEEPER = 0;
        delay_ms(100);
    }//end while
    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//S-meter
void SmeterCalculate(void)
 {
    ULONG smet;UCHAR lt,rt;
 //������� �������� � S-�����
    smet = (ULONG)ADCmeasure();
 //��������������� S-����
 //�������� �����
     lt =0;
     rt =15;
      while(1){                                          // ����������� ����
            if(arrmin[((lt + rt+1) / 2)] > smet)         // ���� �����������
              {                                          // ������� ������ ��������
                rt = (lt + rt) / 2;                      // �������� ������ �������
              }
                else  if(arrmax[((lt + rt) / 2)] < smet)   // ���� �����������
                {                                          // ������� ������ ��������
                 lt = (lt + rt+1) / 2;                   // �������� ����� �������
                }
                else
                {                                        // � ���� ������ ������� ������
                   break;                                // � ��������� ����
                }
         }
            smeter.curr = (lt + rt) / 2;                 // ����� ���������� �������
  //----------------------------------------------------------------------------------------
    //�������� S-���� � ������������ ��������������...
	if (((menu.curr == _MENU_MAIN) || (menu.curr == _MENU_SCANF)) && (flag.Smeter))
	{
	    //�������� ������ S-�����
       //    if (smeter.curr != smeter.old)
       //     {
              LCDviewSmeter();
       //     smeter.old = smeter.curr;
       //     }//end if
              flag.Smeter = 0;                                  //�������� ���� S-�����
        }//end if
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MUTE
void MuteControl(void)
 {
   //���� ������� ������� ��������� �����...
        if (smeter.curr >= radioadd.SQL)
        {
            //���������� ������������
            flag.Scan = 0;
            //�������� ����
            flag.Mute = 0;LED=On;timeLED=0;
        }//end if
          else
           {
              //��������� ����
              flag.Mute = 1;
           }//end else
    VolumeSet();  //���������� ���
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//"��������" ������� (�������� "����������")
void NoFreqView(void)
 {
    UCHAR jj;
    for (jj = 0; jj < 6; jj++) menu.Fval[jj] = 0xFF;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��������� ����� �������� �������
ULONG FreqNewValue(void)
 {
    ULONG Fnew;
    Fnew = (ULONG)100000 * (ULONG)menu.Fval[0] + \
           (ULONG)10000 * (ULONG)menu.Fval[1] + \
           (ULONG)1000 * (ULONG)menu.Fval[2] + \
           (ULONG)100 * (ULONG)menu.Fval[3] + \
           (ULONG)10 * (ULONG)menu.Fval[4] + \
           (ULONG)menu.Fval[5];
    return(Fnew);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�������� ��������� ����� �������
void  FreqSetControl(void)
 {
    ULONG Ftemp;
    //��������� �� LCD
    LCDviewFreqSet();
    //��������
    if (menu.Fnumb >= 5)
    {
        //��������� ����� �������� �������
        Ftemp = FreqNewValue();
        if ((Ftemp > _RCV_MAX) || (Ftemp < _RCV_MIN)) LCDerror();
         else
         {
            //���������� �������� � ����������� �� ������
            if (menu.curr == _MENU_SCANL)
             {
                //���� ������ �����������
                if (Ftemp >= radioadd.FreqScanMax) LCDerror();
                 else
                  {
                    radioadd.FreqScanMin = Ftemp;
                    delay_ms(150); Beep(2);
                   }//end else
             }//end if
               else
                if (menu.curr == _MENU_SCANH)
                 {
                  //���� ������ �����������
                   if (Ftemp <= radioadd.FreqScanMin) LCDerror();
                    else
                    {
                      radioadd.FreqScanMax = Ftemp;
                      delay_ms(150); Beep(2);
                     }//end else
                 }//end if
            else
              if (menu.curr == _MENU_FSET)
               {
                radio.FreqCurrent = Ftemp;
                 if (radio.FreqCurrent >=  66000 && radio.FreqCurrent < 108000)
                 { radioadd.Modulation = 0;
                   radioadd.FreqStep = 7;
                 }
                  else
                  { radioadd.FreqStep = 5;
                    if (radio.FreqCurrent >= 108000 && radio.FreqCurrent < 137000)  radioadd.Modulation = 2; //
                      else radioadd.Modulation = 1;
                  }                                              //
                  SetRADIOfreq();//�������/���������
                  delay_ms(150); Beep(2);
               }//end if
        }//end else
           menu.Fnumb = 0;//�������� ������� ��������
           //"��������" ������� (�������� "����������")
           NoFreqView();
           //�������� ������ �� LCD
           LCDviewMainData();
           LCDviewSmeter();
           //�������� � ��������� ����
        menu.curr = _MENU_MAIN;
    }//end if
    else menu.Fnumb++;//� ���������� �������
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ReadFreqFromMem(void);
void PowerKey (void);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������������
void Scanner(UCHAR mode)
 {
    UINT scanPause;
     while(1)
    {
        keybrd.press = 0;//�������� �������� ������...
        //���������� ���� ������ ������������
        flag.Scan = 1;pauseLED=1800;tauAGC = 1;
        //����� ������������
        if (mode == _FREQ) //�������� ������
        {
            //�������� "�����������" ������������
            if (flag.ScanDir == _UPSCAN) //�����
            {
                if(radio.FreqCurrent <  radioadd.FreqScanMax) radio.FreqCurrent += ts[radioadd.FreqStep];  //��� �����
		        else flag.ScanDir = _DOWNSCAN;
            }//end if
             else
              if (flag.ScanDir == _DOWNSCAN) //����
               {
                if(radio.FreqCurrent >  radioadd.FreqScanMin) radio.FreqCurrent -= ts[radioadd.FreqStep];  //��� ����
		        else flag.ScanDir = _UPSCAN;
               }//end if
        }//end if
          else
           if (mode == _MEMORY) //������� �� ������
            {
              //�������� �������
              ReadFreqFromMem();
            }//end else
        //***********************************************************
        SetRADIOfreq();//�������/���������
        //�������� ������ �� LCD
        LCDviewMainData();
        do
        {
            flag.ScanTime = 0;//�������� ���� ��������
            flag.NextWait = 0;//���� �������� ����������� ������������
            do {
                Battery();
                PowerKey();
                SmeterCalculate();   //S-meter
                MuteControl();
                UAGCcontrol();  //���
                Light_LED();
                if (radioadd.scan == sql)
                {if (flag.Scan == 0)
                  {
                    //���� HOLD ��� STOP, �� �����...
                    if (radioadd.Pause == 0) keybrd.press = _KEY_0_STOP_ST;
                    //����� ����...
                    flag.NextWait = 1;
                    flag.Scan = 1;
                    scanPause = (UINT)radioadd.Pause * (UINT)900;
                   }//end if
                }
                else
                  {
                    if ((flag.Scan == 0)&&(flag.NextWait == 0))
                     {
                      //���� HOLD ��� STOP, �� �����...
                      if (radioadd.Pause == 0) keybrd.press = _KEY_0_STOP_ST;
                      //����� ����...
                      flag.NextWait = 1;
                      flag.Scan = 1;
                      scanPause = (UINT)radioadd.Pause * (UINT)900;
                     }//end if
                   }
                //���� ������ ������ STOP
                //RCkeyCheck();//�������� ���
                if (keybrd.press == _KEY_0_STOP_ST)
                {
                    flag.Scan = 0;tauAGC = 25;pauseLED=14000;
                    LCDviewMainData();
                    _INT_ON;
                    return;
                }//end if
                 else keybrd.press = 0;
            //��������� �������� ��� ������������
                delay_ms(1);
                _WDT_RESET;
            } while(flag.NextWait && (scanPause--));//��� ������ ��� ����. ����� ������������...
        } while((!flag.NextWait) && (!flag.ScanTime));
    }//end while
   tauAGC = 25; pauseLED=14000;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�������� ������� ������� � ��. ��������� � ������

//----------------------------------------------------------------------
  void WriteFreqToMem(void)
  {
    UINT  jj;
    UCHAR ok = 0;
    ULONG Ftemp;
    ULONG tempBuff = 0;
    //�������� �� �������������
    for(jj = memMIN; jj < memMAX; jj++)
    {
        SaveADRmem = jj;
        ReadEEPROMbuff((UCHAR *)&tempBuff,_ADR_EE_FREQS + (jj * 3), 3);
        //���������...
        tempBuff &=~(0xFFC00000);
       if (tempBuff == radio.FreqCurrent)
              { LCDviewFrPresent();                return;              }
    }//end for
    //��� ��������� �����
     menu.adrWMEM = memMIN;
    do
    {
        //�������� �������
        ReadEEPROMbuff((UCHAR *)&Ftemp,_ADR_EE_FREQS + (menu.adrWMEM * 3), 3);
        //�������� �������� �������
        Ftemp &=~(0xFFC00000);

        if ((Ftemp > _RCV_MAX) || (Ftemp < _RCV_MIN))
        {
            //�������� ������� � ��. ���������
            Ftemp = radio.FreqCurrent;
            Ftemp |= ((radioadd.Modulation) << 0x16);
            WriteEEPROMbuff((UCHAR *)&Ftemp, _ADR_EE_FREQS + (menu.adrWMEM * 3), 3);
            ok = 1;
        }//end if
         else menu.adrWMEM++;//� ����. �����...
    } while((menu.adrWMEM < memMAX) && (!ok));
    //���� ����� �� �������...
    if (!ok) LCDviewMemFull();
     else LCDviewSaveOK();
    //�������� ������ �� LCD
    LCDviewMainData();
    LCDviewSmeter();
 }
//----------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���������� ������� �� ������

//--------------------------------------------------------------------------------------
void ReadFreqFromMem(void)
 {
    UCHAR err = 0, tout = 20;   //tout=memMAX-memMIN
    ULONG Ftemp;
    ULONG tempBuff;
    do {
         if ((menu.adrRMEM >= (memMAX - 1))&&(step==1)) menu.adrRMEM = memMIN - 1;
          else
          {
           if ((menu.adrRMEM <= memMIN)&&(step== -1)) menu.adrRMEM = memMAX;
          }
            menu.adrRMEM += step;
         //�������� �������
          ReadEEPROMbuff((UCHAR *)&Ftemp, _ADR_EE_FREQS + (UINT)(menu.adrRMEM * 3), 3);
          tempBuff = Ftemp;
          tempBuff &= (0xC00000);
          tempBuff >>=0x16;
          Ftemp &=~(0xFFC00000);
         //�������� �������� �������
        if ((Ftemp <= _RCV_MAX) && (Ftemp >= _RCV_MIN))
        {
         //�������� ���������...
            radio.FreqCurrent = Ftemp;
            radioadd.Modulation = tempBuff;
            SetRADIOfreq();//�������/���������
            err = 0;
        }//end if
         else err = 1;
    } while((tout--) && err);//������, ���� �� ������ ���������� �������� �� ���������...
    if (err == 1) menu.adrRMEM = memMIN;
 }
//--------------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������ �� ������ ��������� �������� ��������
void EraseMEM(void)
 {
    UCHAR zero = 0;
    WriteEEPROMbuff((UCHAR *)&zero, _ADR_EE_FREQS + ((menu.adrRMEM) * 3), 3);
    LCDviewErase();//�� LCD ������� ERASED
    ReadFreqFromMem();      // ��������� �� ��������� ����� � ����������� �� ����������� ��������
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���������� ��1
void IF1correction(void)
 {
    UCHAR jj;
    if (keybrd.press != _KEY_NFM) return;
    keybrd.press = 0;
    //������� ���������
    gotoxy(0,0);
    print_string("IF1 correction  ");
    while(1)
    {
        _WDT_RESET;
        if (encoder.Dir == _ENC_RIGHT)
         {
            encoder.Dir = _ENC_NOROT;
            if (IF1 < (_RCVPCH1 + _IFCORR)) IF1++;
         }
        if (encoder.Dir == _ENC_LEFT)
         {
            encoder.Dir = _ENC_NOROT;
            if (IF1 > (_RCVPCH1 - _IFCORR)) IF1--;
         }
        //�������� � �������
        LCDfreqCalculate(IF1);
        //������� �������� ��1
        gotoxy(0,1);
        //XX.XXX MHz
        for(jj = 0; jj < 7; jj++)
         {
            //�� �������� ������ '0'
            if ((jj == 0) && (menu.Fdig[jj] == '0')) print_char(' ');
             else print_char(menu.Fdig[jj]);
         }
        print_string(" MHz");
        //����� ������� ������ ������
           if (Powerkey == PowerkeyPress)
           {PowerKey() ;
            return;
         }
    }
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   void DisplayOrient(void)
  {
     if (keybrd.press != _KEY_WFM) return;
     keybrd.press = 0;
     gotoxy (0,0);
     print_string("DISPLAY ");
     if (disp == up) print_string ("UP");
     else print_string ("DOWN");

      while(1)
     {
        _WDT_RESET;
        if (encoder.Dir == _ENC_RIGHT)
         {
            encoder.Dir = _ENC_NOROT;
            gotoxy(48,0);
            if (disp == up) {disp = down; print_string ("DOWN");}
             else {disp = up;print_string ("UP  ");}
         }
        if (encoder.Dir == _ENC_LEFT)
         {
            encoder.Dir = _ENC_NOROT;
            gotoxy(48,0);
            if (disp == up) {disp = down; print_string ("DOWN");}
             else {disp = up;print_string ("UP  ");}
         }
        //����� ������� ������ ������
          if (Powerkey == PowerkeyPress)
           {PowerKey() ;
           return; }
     }
  }
