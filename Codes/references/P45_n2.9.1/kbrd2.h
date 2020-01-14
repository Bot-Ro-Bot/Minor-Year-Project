//----------------------------------------------------------------------
int arraymemmin [8]= {0,20,40,60,80,100,120,140};
int arraymemmax [8]= {20,40,60,80,100,120,140,160};
//----------------------------------------------------------------------

void PowerKey (void)
  {
   if (Powerkey == PowerkeyPress)
   {  LED = On;
      delay_ms(500);
      if (Powerkey == PowerkeyPress)
      {
       LCDoff();
       SaveAll();

         do
        {
         delay_ms(10);
        }while(Powerkey == PowerkeyPress);
       Beep(5);
       PowerSwith = Off;
       delay_ms(3000);
    }
  }
 }


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ��� ������������ ����������
void KeyboardStrob(void)
 {
     _NOP;_NOP;_NOP;
	 _KBD_LATCH = 1;
     _NOP;_NOP;_NOP;
	 _KBD_LATCH = 0;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void KeyboardReset(void)
 {    SPIsend(0x00, 0); //��������� � 595 /00000000
            KeyboardStrob();//latch
            _NOP;_NOP;_NOP;
 }
//---------------------------------------------------------------------
//������������ ����������
void KeyboardScan(void)
  { UCHAR ii, scanK;
    //���� ��� ���� ���������� �������...
    if ((keybrd.press == 0) && (ktout >= 30))//ktout - �����������...
     {  ktout2 = 0;
        // 11111110
        scanK = 0xFE;
        for (ii = 0; ii < 8; ii++)
         {  //��������� � 595
            SPIsend(scanK, 0);
            KeyboardStrob();//latch
            _NOP;_NOP;_NOP;
            if (!(_KBD_IN1))
             {  keybrd.press = KeysArray[ii];
                ktout = 0;
                _BEEPER = 1;//���. �����
                flag.Beep = 1;
                LED=1;
                timeLED=0;
             }//end if
            if (!(_KBD_IN2))//���������� _KBD_IN1
             {  keybrd.press = KeysArray[ii + 8];
                ktout = 0;
                _BEEPER = 1;//���. �����
                LED=1;
                timeLED=0;
                flag.Beep = 1;
             }//end if
              scanK <<= 1;//����. ���
              scanK |= 0x1;//������������ 0-�� ��� � 1
        }//end for
     }//end if
     else
     {
      if (ktout < 255) ktout++;
     }//����������...
            SPIsend(0x00, 0); //��������� � 595 /00000000
            KeyboardStrob();//latch
            _NOP;_NOP;_NOP;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���������� �������� ������ � ����������� �� �������������� ���� ���������
void FreqRoundFromStep(UCHAR dir)
 {
    ULONG Fdelta;
    Fdelta = (radio.FreqCurrent - ((radio.FreqCurrent / ((ULONG)ts[radioadd.FreqStep])) * ((ULONG)ts[radioadd.FreqStep])));
    if (Fdelta)
     {
        radio.FreqCurrent -= Fdelta;
        //���� ���������� �������� �������
        if (dir == 0) radio.FreqCurrent += ts[radioadd.FreqStep];
     }
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��������� �������
void FreqCurrIncr(void)
 {
    if (radio.FreqCurrent >= _RCV_MAX) radio.FreqCurrent = _RCV_MAX;
     else
      {
        FreqRoundFromStep(1);//���������� �������� ������
        radio.FreqCurrent += ts[radioadd.FreqStep]; // + ���
      }
    SetRADIOfreq();//�������/���������
    LCDviewMainData();//�������� ������ �� LCD
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��������� �������
void FreqCurrDecr(void)
 {
    if (radio.FreqCurrent <= _RCV_MIN) radio.FreqCurrent = _RCV_MIN;
     else
      {
        FreqRoundFromStep(0);//���������� �������� ������
        radio.FreqCurrent -= ts[radioadd.FreqStep]; // - ���
      }
    SetRADIOfreq();//�������/���������
    LCDviewMainData();//�������� ������ �� LCD
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//��������� ������� ������ � ��������
void KbrdEnc(void)
 {
        //�������...
        if (encoder.Dir == _ENC_RIGHT)
         {
            encoder.Dir = _ENC_NOROT;
            switch(menu.curr)
             {
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� �������� ����, �� ����� �������
                case(_MENU_MAIN):
                 {
                  if (flag.ReadMEM==1)
                   {
                    step = 1;
                    ReadFreqFromMem ();
                    LCDviewMainData();
                   }
                   else
                    {
                     FreqCurrIncr();
                    }
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ����������� ���������
                case(_MENU_VOL):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.Volume >= _LINERES) radioadd.Volume = _LINERES;
                    else radioadd.Volume++;//��������
                    VolumeSet();//��������� ��������
                    LCDviewVolume();//��������� �� LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ����������� SQL
                case(_MENU_SQL):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.SQL >= _LINERES_SQL) radioadd.SQL = _LINERES_SQL;
                    else radioadd.SQL++;//��������
                    LCDviewSQL();//��������� �� LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� ���� ���������
                case(_MENU_TS):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.FreqStep >= (sizeof(ts) - 1)) radioadd.FreqStep = (sizeof(ts) - 1);
                     else radioadd.FreqStep++;//��������
                    LCDviewTS();//��������� �� LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� ���
                case(_MENU_UAGC):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.LevAGC >= _LINERES) radioadd.LevAGC = _LINERES;
                     else radioadd.LevAGC++;//��������
                    UagcSet();//��� ���
                    LCDviewAGC();//��������� �� LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� ����� ��� ������������
                case(_MENU_ST):
                  {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.Pause >= _PAUSES) radioadd.Pause = _PAUSES;
                     else radioadd.Pause++;//��������
                    LCDviewST();//��������� �� LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� �������� ������������
                case(_MENU_SPD):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.Speed >= (sizeof(scanSpd) - 1)) radioadd.Speed = (sizeof(scanSpd) - 1);
                     else radioadd.Speed++;//��������
                    LCDviewSPD();//��������� �� LCD

                 } break;
                case (_MENU_SCAN):
                 {
                  menu.timeout = 0;
                  if (radioadd.scan == time) radioadd.scan = sql;
                   else radioadd.scan = time;
                  LCDviewSCAN();
                 }break;
                case (_MENU_BANK):
                 {
                 menu.timeout = 0;
                 radioadd.bank ++;
                  if (radioadd.bank > 7) radioadd.bank = 0;
                  else
                  {if (radioadd.bank < 0) radioadd.bank = 7;}
                 memMIN = arraymemmin[radioadd.bank];
                 memMAX = arraymemmax[radioadd.bank];
                 step = 0;
                 menu.adrRMEM = memMIN;
                  if (flag.ReadMEM == 1)
                  {
                   ReadFreqFromMem();
                   LCDviewMainData();
                  }
                 LCDviewSET_BANK();
                 } break;
                default: break;
            }//end switch
        }//end if RIGHT
        //--------------------------------------------------------------------------

        //----------------------------------------------------
	    if (encoder.Dir == _ENC_LEFT)
	     {
    	    encoder.Dir = _ENC_NOROT;
            switch(menu.curr)
             {
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� �������� ����, �� ����� �������
                case(_MENU_MAIN):
                 {
                  if (flag.ReadMEM==1)
                  {
                   step = (-1);
                   ReadFreqFromMem ();
                   LCDviewMainData();
                  }
                  else
                  {
                    FreqCurrDecr();
                  }
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ����������� ���������
                case(_MENU_VOL):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.Volume < 1) radioadd.Volume = 0;
                     else radioadd.Volume--;//������
                    VolumeSet();//��������� ��������
                    LCDviewVolume();//��������� �� LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ����������� SQL
                case(_MENU_SQL):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.SQL == 0) radioadd.SQL = 0;
                     else radioadd.SQL--;//������
                    LCDviewSQL();//��������� �� LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� ���� ���������
                case(_MENU_TS):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.FreqStep == 0) radioadd.FreqStep = 0;
                     else radioadd.FreqStep--;//������
                    LCDviewTS();//��������� �� LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� ���
                case(_MENU_UAGC):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.LevAGC == 0) radioadd.LevAGC = 0;
                    else radioadd.LevAGC--;//������
                    UagcSet();//��� ���
                    LCDviewAGC();//��������� �� LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� ����� ��� ������������
                case(_MENU_ST):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.Pause == 0) radioadd.Pause = 0;
                     else radioadd.Pause--;//������
                    LCDviewST();//��������� �� LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //���� ���� ��������� �������� ������������
                case(_MENU_SPD):
                 {
                    menu.timeout = 0;//�������� ������� �������� ����
                    if (radioadd.Speed == 0) radioadd.Speed = 0;
                     else radioadd.Speed--;//������
                    LCDviewSPD();//��������� �� LCD
                } break;
                case (_MENU_SCAN):
                 {
                 menu.timeout = 0;
                 if (radioadd.scan == time) radioadd.scan = sql;
                  else radioadd.scan = time;
                 LCDviewSCAN();
                }break;
                case (_MENU_BANK):
                 {
                 menu.timeout = 0;
                 radioadd.bank --;
                 if (radioadd.bank > 7) radioadd.bank = 0;
                  else
                  {if (radioadd.bank < 0) radioadd.bank = 7;}
                   memMIN = arraymemmin[radioadd.bank];
                   memMAX = arraymemmax[radioadd.bank];
                   step = 0;
                   menu.adrRMEM = memMIN;
                   if (flag.ReadMEM == 1)
                   {
                    ReadFreqFromMem();
                    LCDviewMainData();
                   }
                 LCDviewSET_BANK();
                } break;
                default: break;
            }//end switch
	    }//end if LEFT
        //****************************************************************
        //****************************************************************
        //****************************************************************
	    //����������
	    if (keybrd.press)//���� ������ ����� ������
	    {
                LED=1;
                timeLED=0;
            switch(keybrd.press)//����������� ������� ������
            {
	           case _KEY_GAIN:
                   {
                    //������� ����������� ��������� �����
                    if ((menu.curr == _MENU_FSET) || (menu.curr == _MENU_SCANL) || (menu.curr == _MENU_SCANH))
                     {
                        if (menu.Fnumb)
                        {
                            menu.Fnumb--;
                            menu.Fval[menu.Fnumb] = 0xFF;
                            LCDviewFreqSet();//��������� �� LCD
                        }
                    }
                     else
                      if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
                       {
	                    radioadd.Gain ^= 1;
	                    SetRADIOfreq();//�������/���������
	                    LCDviewMainData();//�������� ������ �� LCD

	                }//end if
	            } break;

	            case _KEY_WFM: {
                           if (flag.ReadMEM == 1)
                    {
                        //������ ��������� �� ������ ��������
                        EraseMEM();
                        //�������� ������ �� LCD
	                    LCDviewMainData();
                    }//end if
	            } break;

               //--------------------------------------------------------------------------------------------------------------

                   case _KEY_NFM:
                   {
                    if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
                     {
                       menu.curr = _MENU_BANK;
                     }
                        LCDviewAfterFreq();
                        LCDviewSET_BANK();
	            } break;
                //--------------------------------------------------------------------------------------------------------------
	            case _KEY_AM:
                    {
                     if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
                      {
                        radioadd.Modulation++;
                         if (radioadd.Modulation > 2) radioadd.Modulation = 0;
                        SetRADIOfreq();//�������/���������
	                LCDviewMainData();//�������� ������ �� LCD
	                }//end if
	            } break;
                //----------------------------------------------
	            case _KEY_SEL:
                    {
                     if (menu.curr == _MENU_MAIN)
                      {
                        //�������� � ���� ����������� ���������
                        menu.curr = _MENU_VOL;
                        LCDviewVolume();//��������� �� LCD
	              }//end if
	                else
                      if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                {
                        //�������� � ��������� ����
                          menu.curr = _MENU_MAIN;
                          LCDviewSmeter();//��������� S-���� �� LCD
                          LCDviewAfterFreq();
	                }//end if
	            } break;
                //----------------------------------------------
                case _KEY_FREQ_SET:
                {
                    if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
                     {
                      //�������� � ���� ����� �������
                      flag.ReadMEM = 0;
                      menu.curr = _MENU_FSET;
                      LCDviewFreqSet();//��������� �� LCD
                     }//end if
	        } break;
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case _KEY_0_STOP_ST: {
	                if ((menu.curr == _MENU_FSET) || (menu.curr == _MENU_SCANL) || (menu.curr == _MENU_SCANH))
	                 {
                           menu.Fval[menu.Fnumb] = 0;//����� 0
                           FreqSetControl();//�������� �����
	                 }//end if
                    else
                    if (menu.curr == _MENU_ST)
                     {
                        //�������� � ���� ��������� �������� ������������
                        menu.curr = _MENU_SPD;
                        LCDviewSPD();//��������� �� LCD
                     }//end if
                      else
                      {   if (menu.curr == _MENU_SPD)
                         {
                          menu.curr = _MENU_SCAN;
                          LCDviewSCAN();
                         }
                         else
                         {
                          //�������� � ���� ��������� ����� ��� ������������
                          menu.curr = _MENU_ST;
                          LCDviewST();//��������� �� LCD
                         }//end else
                      }
	            } break;
                //----------------------------------------------
	            case _KEY_1_STEP: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
    	                  //�������� � ���� ��������� ���� ���������
	                    menu.curr = _MENU_TS;
	                    LCDviewTS();//��������� �� LCD
	                 }//end if
	                else
	                {
                         menu.Fval[menu.Fnumb] = 1;//����� 1
                         FreqSetControl();//�������� �����
	                }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_2_SQL: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //�������� � ���� ����������� SQL
	                    menu.curr = _MENU_SQL;
	                    LCDviewSQL();//��������� �� LCD
	                 }//end if
	                else
	                {
                         menu.Fval[menu.Fnumb] = 2;//����� 2
                         FreqSetControl();//�������� �����
	                }//end else
	            } break;
                    //----------------------------------------------
	            case _KEY_3_UAGC: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //������� ����� ���
	                    if (radioadd.AGC == _AGC_OFF)
	                     {
	                        radioadd.AGC = _AGC_AUTO;

                             }
	                    else
	                     if ((radioadd.AGC == _AGC_AUTO) || (radioadd.AGC == _AGC_MAN))
	                      {
	                        if (menu.curr == _MENU_UAGC)
	                         {
	                            menu.curr = _MENU_MAIN;
	                            radioadd.AGC = _AGC_OFF;
	                            UagcSet();//��� �� ��������
	                            LCDviewSmeter();

	                         }//end if
	                        else
	                         {
	                            menu.curr = _MENU_UAGC;
	                            radioadd.AGC = _AGC_MAN;
	                            UagcSet();//���

	                         }//end else
	                      }//end if
	                    LCDviewAGC();//��������� �� LCD
	                 }//end if
	                else
	                 {
                          menu.Fval[menu.Fnumb] = 3;//����� 3
                          FreqSetControl();//�������� �����
	                 }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_4_SCANL: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //�������� � ���� ����� ������ ������� ������������
	                    menu.curr = _MENU_SCANL;
	                    LCDviewFreqSet();//��������� �� LCD
                            gotoxy(0,2);
                            print_string(blank);
	                 }//end if
	                else
	                 {
                           menu.Fval[menu.Fnumb] = 4;//����� 4
                           FreqSetControl();//�������� �����
	                }//end else
	             } break;
                //----------------------------------------------
	            case _KEY_5_SCANF: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //�������� � ������ ������������ � �������� ��������� ������
                          if ((radio.FreqCurrent < radioadd.FreqScanMin) || (radio.FreqCurrent > radioadd.FreqScanMax))\
                            radio.FreqCurrent = radioadd.FreqScanMin;
                            LCDviewSmeter();
	                    Scanner(_FREQ); //�����������...
	                 }//end if
	                else
	                  {
                           menu.Fval[menu.Fnumb] = 5;//����� 5
                           FreqSetControl();//�������� �����
	                  }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_6_SCANH: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //�������� � ������ ������������ � �������� ��������� ������
	                    menu.curr = _MENU_SCANH;
       	                    LCDviewFreqSet();//��������� �� LCD
                            gotoxy(0,2);
                            print_string(blank);
	                  }//end if
	                   else
	                   {
                            menu.Fval[menu.Fnumb] = 6;//����� 6
                            FreqSetControl();//�������� �����
	                   }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_7_WRITEM: {

	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //�������� ������� ������� � ������
	                    WriteFreqToMem();
	                 }//end if
	                  else
	                   {
                            menu.Fval[menu.Fnumb] = 7;//����� 7
                            FreqSetControl();//�������� �����
	                   }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_8_SCANM: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
                          step = 1;
	                  Scanner(_MEMORY);//����������� �� �������� �� ������
	                 }//end if
	                  else
	                  {
                            menu.Fval[menu.Fnumb] = 8;//����� 8
                            FreqSetControl();//�������� �����
	                  }//end else
	            } break;
                    //----------------------------------------------
	            case _KEY_9_READM: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
                          step = 0;
                           if (flag.ReadMEM==1)
                            {
                             flag.ReadMEM = 0;
                             LCDviewMainData();
                            }
                          else
                           {
                            flag.ReadMEM = 1;
	                    //���������� ������� �� ������
	                    ReadFreqFromMem();
                            LCDviewMainData();

                           }
	                 }//end if
	                  else
	                   {
                            menu.Fval[menu.Fnumb] = 9;//����� 9
                            FreqSetControl();//�������� �����
	                   }//end else
	            } break;
                //----------------------------------------------
	    }//end switch
                keybrd.press = 0;//�������� ��������� ������������ ����������
  }//end if
        //****************************************************************

        //���� ����� ����������� ���� � ����� ���� ����� �������,
        //�� ������� ������ �������...
        if ((menu.curr != _MENU_MAIN) && (menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
         {

            if (flag.Menu) {flag.Menu = 0; menu.timeout++;}
            if (menu.timeout >= 5)  //~5 ������...
             {
                menu.curr = _MENU_MAIN;
                LCDviewSmeter();//��������� S-���� �� LCD
                LCDviewAfterFreq();
             } //end if
         }//end if
        else
        menu.timeout = 0;

 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

