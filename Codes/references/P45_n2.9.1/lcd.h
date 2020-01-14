void Beep(UCHAR rep);
//

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������!!!
void LCDerror(void)
 {
    gotoxy(0,1);
    print_string("error data");
    delay_ms(500);
    Beep(3);
    delay_ms(500);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������������� ��������� ���������� ������
void LCDviewSaveOK(void)
 {
    UCHAR ts1,ts2,ts3;
    ts3 = menu.adrWMEM + 1;
    gotoxy(0,1);
    print_string("saved in - ");
    ts1 = (UCHAR)((ts3-memMIN) / 10);
    if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
     else print_char(' ');
    ts2 = (UCHAR)((ts3-memMIN) % 10);
    if (ts2 || ts1) print_char(0x30 + ts2); //���� = 0, �� �� ����������
     else print_char(' ');

    //�����
    delay_ms(500);
    Beep(2);
    delay_ms(500);
    gotoxy(0,1);
    print_string(blank);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDviewSCAN(void)
     {
       gotoxy(0,1);
       //print_string(blank);
       print_string("SCAN MODE:");
      if (radioadd.scan == time) print_string(" TIME ");
        else print_string(" SQL  ");
     }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDviewBatt(void)

  {
          UCHAR ts1,ts2,ts3;
          gotoxy(0,6);
          print_char(0x00 + vcharl);
          gotoxy(5,6);
          print_char(0x00 + vcharh);
          ts1 = (UCHAR)(vbatt/10000);
          print_char(0x30 + ts1);
          print_char('.');
          ts2 = (UCHAR)((vbatt%10000)/1000);
          print_char(0x30 + ts2);
          ts3 = (UCHAR)((vbatt)%1000/100);
          print_char(0x30 + ts3);
          print_string("V");

    gotoxy(60,6);
    ts1 = (UCHAR)(ts[radioadd.FreqStep]/100);
    if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
     else print_char(' ');
    ts2 = (UCHAR)((ts[radioadd.FreqStep]%100)/10);
    if (ts2 || ts1) print_char(0x30 + ts2); //���� = 0, �� �� ����������
     else print_char(' ');
    ts3 = (UCHAR)(ts[radioadd.FreqStep]%10);
    if (ts3 || ts2 || ts1) print_char(0x30 + ts3); //���� = 0, �� �� ����������
     else print_char(' ');
    print_string("kHz");
  }

void LCDviewErase(void)
 {
    gotoxy(0,1);
    print_string("erase");
    //�����
    Beep(2);
    delay_ms(800);
    gotoxy(0,1);
    print_string(blank);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDviewMemFull(void)
 {
    gotoxy(0,1);
    print_string("memory full!!!");
    //�����
    Beep(3);
    delay_ms(800);
    gotoxy(0,1);
    print_string(blank);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDviewAfterFreq(void);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDviewFrPresent(void)
 {  UCHAR ts1,ts2,ts3;
    gotoxy(0,1);
    print_string("already in ch");
    ts3 = SaveADRmem +1 ;
    ts1 = (UCHAR) ((ts3 - memMIN)/10);
    if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
    else print_char(' ');
    ts2 = (UCHAR) ((ts3 - memMIN)%10);
    print_char(0x30 + ts2);
    //�����
    Beep(2);
    delay_ms(800);
    gotoxy(0,1);
    print_string(blank);
    LCDviewAfterFreq();
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� "�������" ��� S-�����, ���.���������, SQL, AGC
void LCDviewLine(UCHAR val, UCHAR mode)
 {
    UCHAR jj;
    gotoxy(0,7);
    if (mode == _UAGCLN) print_string("AGC:");
    if (mode == _VOLUMELN) print_string("VOL:");
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //"�������"...
    for (jj = 0; jj < _LINERES; jj++)
	{
	 if (jj >= (val / 3))
	  {
	   print_char((val % 3)+135);
	   break;
	  }
	   else print_char(138);
	}
	jj = ((_LINERES - jj)/3 - 1);
	while(jj--) print_char(135);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ���� ����������� ���������
void LCDviewVolume(void)
 {
    LCDviewLine(radioadd.Volume, _VOLUMELN);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ���� ����������� SQL
void LCDviewSQL(void)
 {
    UCHAR jj;
    gotoxy(0,7);
    print_string("             SQL");
    if (radioadd.SQL == 0)
    {
        //���������� ������� "OPEN"
        gotoxy(4,7);
        print_string("    OPEN    ");
        return;
    }//end if
    gotoxy(0,7);
    for (jj = 0; jj < _LINERES_SQL; jj++)
    {
        if (jj < radioadd.SQL) print_char(134);
        else print_char(148);
    }
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ������ S-����� (15 ������� - 1...9,+10,+20,+30,+40,+50,+60)
void LCDviewSmeter(void)
 {
    UCHAR jj;
    gotoxy(0,7);
    print_string("S");
    //
    for (jj = 0; jj < 15; jj++)
    {
        if (jj < smeter.curr)
        {
            if ((jj >= 0)  && (jj < 2))  print_char(128);
            if ((jj >= 2)  && (jj < 4))  print_char(129);
            if ((jj >= 4)  && (jj < 6))  print_char(130);
            if ((jj >= 6)  && (jj < 8))  print_char(131);
            if ((jj >= 8)  && (jj < 10)) print_char(132);
            if ((jj >= 10) && (jj < 12)) print_char(133);
            if ((jj >= 12) && (jj < 15)) print_char(134);
        }
        else print_char(0x20);
        }
        if ((UAGC < 1023))
        {
        gotoxy(78,7);
         if ((UAGC >= 150) && (UAGC < 350)) print_string("+50");
          if ((UAGC >= 350) && (UAGC < 500)) print_string("+40");
           if ((UAGC >= 500) && (UAGC < 600)) print_string("+30");
            if ((UAGC >= 600) && (UAGC < 800)) print_string("+20");
             if ((UAGC >= 800) && (UAGC < 1023)) print_string("+10");
        }
    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���� ��������� ����� ��� ������������
void LCDviewST(void)
 {
    UCHAR ts1,ts2;
    //
    gotoxy(0,1);
    print_string("SCAN PAUSE:");
    if (radioadd.Pause == 0)
    {
        //���������� ������� "HOLD"
        print_string("HOLD");
    }//end if
    else
    {
        ts1 = (UCHAR)(radioadd.Pause / 10);
        if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
        else print_char(' ');
        ts2 = (UCHAR)(radioadd.Pause % 10);
        if (ts2 || ts1) print_char(0x30 + ts2); //���� = 0, �� �� ����������
        else print_char(' ');
        print_string(" s");
    }//end else
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���� ��������� �������� ������������
void LCDviewSPD(void)
 {
    UCHAR ts1,ts2;
    //
    gotoxy(0,1);
    print_string("SCAN SPEED:");
    ts1 = (UCHAR)(scanSpd[radioadd.Speed] / 10);
    if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
     else print_char(' ');
    ts2 = (UCHAR)(scanSpd[radioadd.Speed] % 10);
    if (ts2 || ts1) print_char(0x30 + ts2); //���� = 0, �� �� ����������
     else print_char(' ');
    print_string(" ch");
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���� ��������� �����
void LCDviewSET_BANK(void)
 {
    gotoxy(0,1);
    print_string(blank);
    gotoxy(0,1);
    print_string("BANK: ");
    print_char(0x30 + (radioadd.bank) + 1);
 }
//����������� ������ AGC
void LCDviewAGC(void)
 {
    gotoxy(70,0);
    if (radioadd.AGC == _AGC_OFF) print_string("OF");//����.
     else
    if (radioadd.AGC == _AGC_AUTO) print_string("AU");//����
     else
    if (radioadd.AGC == _AGC_MAN)
     {
        print_string("MN"); //������ �����...
        if (menu.curr == _MENU_UAGC) LCDviewLine(radioadd.LevAGC, _UAGCLN);
     }//end if
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ���� ����� �������

//-----------------------------------------------------------------------------
    void LCDviewFreqSet(void)
 {
    UCHAR jj;
    gotoxy(0,1);
    if (menu.curr == _MENU_SCANL) print_string("SET LOW FREQ.");
     else
     if (menu.curr == _MENU_SCANH) print_string("SET HIGH FREQ.");
      else
       if (menu.curr == _MENU_FSET)  print_string("SET WORK FREQ.");
    gotoxy(0,6);
    //�������
    for (jj = 0; jj < 3; jj++)
     {
        if (menu.Fval[jj] != 0xFF) char_12_16(menu.Fval[jj],jj*12+5,3);
         else char_12_16(12,jj*12+5,3);
      }//end for
    char_12_16(11,jj*12+5,3);//�����
    for (jj = 3; jj < 6; jj++)
     {
        if (menu.Fval[jj] != 0xFF) char_12_16(menu.Fval[jj],(jj+1)*12+5,3);
         else char_12_16(12,(jj+1)*12+5,3);
     }//end for
 }

//-----------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ���� ��������� ���� ���������
void LCDviewTS(void)
 {
    UCHAR ts1,ts2,ts3;
    gotoxy(0,1);
    print_string("SET TS:");
    ts1 = (UCHAR)(ts[radioadd.FreqStep]/100);
    if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
     else print_char(' ');
    ts2 = (UCHAR)((ts[radioadd.FreqStep]%100)/10);
    if (ts2 || ts1) print_char(0x30 + ts2); //���� = 0, �� �� ����������
     else print_char(' ');
    ts3 = (UCHAR)(ts[radioadd.FreqStep]%10);
    if (ts3 || ts2 || ts1) print_char(0x30 + ts3); //���� = 0, �� �� ����������
     else print_char(' ');
    print_string(" kHz");
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//"����������� � �������"
void LCDfreqCalculate(ULONG ftemp)
 {
    UCHAR jj = 6, m;
  //���� ���������� ��������� ������� ������ ������
    do{
        if (jj == 3) menu.Fdig[jj] = '.';//�����
         else
         {
            m = ftemp % 10;//������� ������� �� ������� ����� �� ���������
            menu.Fdig[jj] = symbForConv[m];//���� ������� ���� ��������� �����
            ftemp /= 10;//��������� ����� � 10 ���
         }
      } while(jj--);
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDviewFreq(void)
 {
    UCHAR jj;
    LCDfreqCalculate(radio.FreqCurrent);
    //XXX.XXX MHz
    for(jj = 0; jj < 7; jj++)
     {
        //�� �������� ������ '0'
        if ((jj == 0) && (menu.Fdig[jj] == '0')) char_12_16(0,jj+5,3);
        if (jj == 3) char_12_16(11,jj*12+5,3);
         else char_12_16(menu.Fdig[jj]-0x30,jj*12+5,3);
     }
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void LCDoff(void)
 {
    gotoxy(0,1);
    print_string(blank);
    gotoxy(36,1);
    print_string("OFF");
 }
void LCDviewAfterFreq(void)
 {   UCHAR ts1,ts2,ts3;
     gotoxy(0,0);
    print_message(mod[radioadd.Modulation]);//���������
    if (flag.Scan)
     {
        //������� SCAN
        gotoxy(36,1);
        print_string("SCAN");
     }//end if
     else
      {
       gotoxy(0,1);
       print_string(blank);
       gotoxy(36,1);
       print_string("ch");
       ts3 = menu.adrRMEM +1 ;
       ts1 = (UCHAR) ((ts3 - memMIN)/10);
        if (ts1) print_char(0x30 + ts1); //���� = 0, �� �� ����������
         else print_char(' ');
        ts2 = (UCHAR) ((ts3 - memMIN)%10);
        print_char(0x30 + ts2);
      }
    if (flag.ReadMEM == 1)
     { gotoxy(25,0);
       print_char('M');
     }
      else
       { gotoxy(25,0);
         print_char(' ');
       }
      gotoxy(43,0);
      print_string("B");
      print_char((radioadd.bank+1)+0x30);
        //���
        LCDviewAGC();
        //GAIN
        gotoxy(88,0);
        if (radioadd.Gain) print_char('G');
         else print_char(' ');
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� �������� ������ �� LCD
void LCDviewMainData(void)
 {
    LCDviewFreq();
    LCDviewAfterFreq();//������ � ������ ������ ����� �������
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

