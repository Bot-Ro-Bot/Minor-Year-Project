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
//строб для сканирования клавиатуры
void KeyboardStrob(void)
 {
     _NOP;_NOP;_NOP;
	 _KBD_LATCH = 1;
     _NOP;_NOP;_NOP;
	 _KBD_LATCH = 0;
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void KeyboardReset(void)
 {    SPIsend(0x00, 0); //отправляю в 595 /00000000
            KeyboardStrob();//latch
            _NOP;_NOP;_NOP;
 }
//---------------------------------------------------------------------
//сканирование клавиатуры
void KeyboardScan(void)
  { UCHAR ii, scanK;
    //если уже было обработано нажатие...
    if ((keybrd.press == 0) && (ktout >= 30))//ktout - антидребезг...
     {  ktout2 = 0;
        // 11111110
        scanK = 0xFE;
        for (ii = 0; ii < 8; ii++)
         {  //отправляю в 595
            SPIsend(scanK, 0);
            KeyboardStrob();//latch
            _NOP;_NOP;_NOP;
            if (!(_KBD_IN1))
             {  keybrd.press = KeysArray[ii];
                ktout = 0;
                _BEEPER = 1;//вкл. бипер
                flag.Beep = 1;
                LED=1;
                timeLED=0;
             }//end if
            if (!(_KBD_IN2))//аналогично _KBD_IN1
             {  keybrd.press = KeysArray[ii + 8];
                ktout = 0;
                _BEEPER = 1;//вкл. бипер
                LED=1;
                timeLED=0;
                flag.Beep = 1;
             }//end if
              scanK <<= 1;//след. бит
              scanK |= 0x1;//устанавливаю 0-ой бит в 1
        }//end for
     }//end if
     else
     {
      if (ktout < 255) ktout++;
     }//накапливаю...
            SPIsend(0x00, 0); //отправляю в 595 /00000000
            KeyboardStrob();//latch
            _NOP;_NOP;_NOP;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//округление значения чатоты в зависимости от установленного шага настройки
void FreqRoundFromStep(UCHAR dir)
 {
    ULONG Fdelta;
    Fdelta = (radio.FreqCurrent - ((radio.FreqCurrent / ((ULONG)ts[radioadd.FreqStep])) * ((ULONG)ts[radioadd.FreqStep])));
    if (Fdelta)
     {
        radio.FreqCurrent -= Fdelta;
        //если уменьшение значения частоты
        if (dir == 0) radio.FreqCurrent += ts[radioadd.FreqStep];
     }
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//увеличить частоту
void FreqCurrIncr(void)
 {
    if (radio.FreqCurrent >= _RCV_MAX) radio.FreqCurrent = _RCV_MAX;
     else
      {
        FreqRoundFromStep(1);//округление значения чатоты
        radio.FreqCurrent += ts[radioadd.FreqStep]; // + шаг
      }
    SetRADIOfreq();//частота/модуляция
    LCDviewMainData();//обновить данные на LCD
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//уменьшить частоту
void FreqCurrDecr(void)
 {
    if (radio.FreqCurrent <= _RCV_MIN) radio.FreqCurrent = _RCV_MIN;
     else
      {
        FreqRoundFromStep(0);//округление значения чатоты
        radio.FreqCurrent -= ts[radioadd.FreqStep]; // - шаг
      }
    SetRADIOfreq();//частота/модуляция
    LCDviewMainData();//обновить данные на LCD
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//обработка нажатых клавиш и энкодера
void KbrdEnc(void)
 {
        //энкодер...
        if (encoder.Dir == _ENC_RIGHT)
         {
            encoder.Dir = _ENC_NOROT;
            switch(menu.curr)
             {
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если основное меню, то меняю частоту
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
                //если меню регулировки громкости
                case(_MENU_VOL):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.Volume >= _LINERES) radioadd.Volume = _LINERES;
                    else radioadd.Volume++;//добавить
                    VolumeSet();//применить значение
                    LCDviewVolume();//отображаю на LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню регулировки SQL
                case(_MENU_SQL):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.SQL >= _LINERES_SQL) radioadd.SQL = _LINERES_SQL;
                    else radioadd.SQL++;//добавить
                    LCDviewSQL();//отображаю на LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню изменения шага настройки
                case(_MENU_TS):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.FreqStep >= (sizeof(ts) - 1)) radioadd.FreqStep = (sizeof(ts) - 1);
                     else radioadd.FreqStep++;//добавить
                    LCDviewTS();//отображаю на LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню изменения АРУ
                case(_MENU_UAGC):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.LevAGC >= _LINERES) radioadd.LevAGC = _LINERES;
                     else radioadd.LevAGC++;//добавить
                    UagcSet();//АРУ ШИМ
                    LCDviewAGC();//отображаю на LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню настройки паузы при сканировании
                case(_MENU_ST):
                  {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.Pause >= _PAUSES) radioadd.Pause = _PAUSES;
                     else radioadd.Pause++;//добавить
                    LCDviewST();//отображаю на LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню настройки скорости сканировании
                case(_MENU_SPD):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.Speed >= (sizeof(scanSpd) - 1)) radioadd.Speed = (sizeof(scanSpd) - 1);
                     else radioadd.Speed++;//добавить
                    LCDviewSPD();//отображаю на LCD

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
                //если основное меню, то меняю частоту
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
                //если меню регулировки громкости
                case(_MENU_VOL):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.Volume < 1) radioadd.Volume = 0;
                     else radioadd.Volume--;//убрать
                    VolumeSet();//применить значение
                    LCDviewVolume();//отображаю на LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню регулировки SQL
                case(_MENU_SQL):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.SQL == 0) radioadd.SQL = 0;
                     else radioadd.SQL--;//убрать
                    LCDviewSQL();//отображаю на LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню изменения шага настройки
                case(_MENU_TS):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.FreqStep == 0) radioadd.FreqStep = 0;
                     else radioadd.FreqStep--;//убрать
                    LCDviewTS();//отображаю на LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню изменения АРУ
                case(_MENU_UAGC):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.LevAGC == 0) radioadd.LevAGC = 0;
                    else radioadd.LevAGC--;//убрать
                    UagcSet();//АРУ ШИМ
                    LCDviewAGC();//отображаю на LCD

                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню настройки паузы при сканировании
                case(_MENU_ST):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.Pause == 0) radioadd.Pause = 0;
                     else radioadd.Pause--;//убрать
                    LCDviewST();//отображаю на LCD
                 } break;
                //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                //если меню настройки скорости сканировании
                case(_MENU_SPD):
                 {
                    menu.timeout = 0;//сбросить счетчик таймаута меню
                    if (radioadd.Speed == 0) radioadd.Speed = 0;
                     else radioadd.Speed--;//убрать
                    LCDviewSPD();//отображаю на LCD
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
	    //клавиатура
	    if (keybrd.press)//если нажата любая кнопка
	    {
                LED=1;
                timeLED=0;
            switch(keybrd.press)//обрабатываю нажатие кнопки
            {
	           case _KEY_GAIN:
                   {
                    //удаляет неправильно введенные цифры
                    if ((menu.curr == _MENU_FSET) || (menu.curr == _MENU_SCANL) || (menu.curr == _MENU_SCANH))
                     {
                        if (menu.Fnumb)
                        {
                            menu.Fnumb--;
                            menu.Fval[menu.Fnumb] = 0xFF;
                            LCDviewFreqSet();//отображаю на LCD
                        }
                    }
                     else
                      if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
                       {
	                    radioadd.Gain ^= 1;
	                    SetRADIOfreq();//частота/модуляция
	                    LCDviewMainData();//обновить данные на LCD

	                }//end if
	            } break;

	            case _KEY_WFM: {
                           if (flag.ReadMEM == 1)
                    {
                        //стираю считанное из памяти значение
                        EraseMEM();
                        //обновить данные на LCD
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
                        SetRADIOfreq();//частота/модуляция
	                LCDviewMainData();//обновить данные на LCD
	                }//end if
	            } break;
                //----------------------------------------------
	            case _KEY_SEL:
                    {
                     if (menu.curr == _MENU_MAIN)
                      {
                        //перехожу к меню регулировки громкости
                        menu.curr = _MENU_VOL;
                        LCDviewVolume();//отображаю на LCD
	              }//end if
	                else
                      if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                {
                        //перехожу к основному меню
                          menu.curr = _MENU_MAIN;
                          LCDviewSmeter();//отображаю S-метр на LCD
                          LCDviewAfterFreq();
	                }//end if
	            } break;
                //----------------------------------------------
                case _KEY_FREQ_SET:
                {
                    if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
                     {
                      //перехожу к меню ввода частоты
                      flag.ReadMEM = 0;
                      menu.curr = _MENU_FSET;
                      LCDviewFreqSet();//отображаю на LCD
                     }//end if
	        } break;
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case _KEY_0_STOP_ST: {
	                if ((menu.curr == _MENU_FSET) || (menu.curr == _MENU_SCANL) || (menu.curr == _MENU_SCANH))
	                 {
                           menu.Fval[menu.Fnumb] = 0;//цифра 0
                           FreqSetControl();//проверка ввода
	                 }//end if
                    else
                    if (menu.curr == _MENU_ST)
                     {
                        //перехожу к меню установки скорости сканирования
                        menu.curr = _MENU_SPD;
                        LCDviewSPD();//отображаю на LCD
                     }//end if
                      else
                      {   if (menu.curr == _MENU_SPD)
                         {
                          menu.curr = _MENU_SCAN;
                          LCDviewSCAN();
                         }
                         else
                         {
                          //перехожу к меню установки паузы при сканировании
                          menu.curr = _MENU_ST;
                          LCDviewST();//отображаю на LCD
                         }//end else
                      }
	            } break;
                //----------------------------------------------
	            case _KEY_1_STEP: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
    	                  //перехожу к меню изменения шага настройки
	                    menu.curr = _MENU_TS;
	                    LCDviewTS();//отображаю на LCD
	                 }//end if
	                else
	                {
                         menu.Fval[menu.Fnumb] = 1;//цифра 1
                         FreqSetControl();//проверка ввода
	                }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_2_SQL: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //перехожу к меню регулировки SQL
	                    menu.curr = _MENU_SQL;
	                    LCDviewSQL();//отображаю на LCD
	                 }//end if
	                else
	                {
                         menu.Fval[menu.Fnumb] = 2;//цифра 2
                         FreqSetControl();//проверка ввода
	                }//end else
	            } break;
                    //----------------------------------------------
	            case _KEY_3_UAGC: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //изменяю режим АРУ
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
	                            UagcSet();//АРУ на максимум
	                            LCDviewSmeter();

	                         }//end if
	                        else
	                         {
	                            menu.curr = _MENU_UAGC;
	                            radioadd.AGC = _AGC_MAN;
	                            UagcSet();//АРУ

	                         }//end else
	                      }//end if
	                    LCDviewAGC();//отображаю на LCD
	                 }//end if
	                else
	                 {
                          menu.Fval[menu.Fnumb] = 3;//цифра 3
                          FreqSetControl();//проверка ввода
	                 }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_4_SCANL: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //перехожу к меню ввода нижней границы сканирования
	                    menu.curr = _MENU_SCANL;
	                    LCDviewFreqSet();//отображаю на LCD
                            gotoxy(0,2);
                            print_string(blank);
	                 }//end if
	                else
	                 {
                           menu.Fval[menu.Fnumb] = 4;//цифра 4
                           FreqSetControl();//проверка ввода
	                }//end else
	             } break;
                //----------------------------------------------
	            case _KEY_5_SCANF: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //перехожу к режиму сканирования в заданном диапазоне частот
                          if ((radio.FreqCurrent < radioadd.FreqScanMin) || (radio.FreqCurrent > radioadd.FreqScanMax))\
                            radio.FreqCurrent = radioadd.FreqScanMin;
                            LCDviewSmeter();
	                    Scanner(_FREQ); //сканировать...
	                 }//end if
	                else
	                  {
                           menu.Fval[menu.Fnumb] = 5;//цифра 5
                           FreqSetControl();//проверка ввода
	                  }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_6_SCANH: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //перехожу к режиму сканирования в заданном диапазоне частот
	                    menu.curr = _MENU_SCANH;
       	                    LCDviewFreqSet();//отображаю на LCD
                            gotoxy(0,2);
                            print_string(blank);
	                  }//end if
	                   else
	                   {
                            menu.Fval[menu.Fnumb] = 6;//цифра 6
                            FreqSetControl();//проверка ввода
	                   }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_7_WRITEM: {

	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
	                    //сохраняю текущую частоту в память
	                    WriteFreqToMem();
	                 }//end if
	                  else
	                   {
                            menu.Fval[menu.Fnumb] = 7;//цифра 7
                            FreqSetControl();//проверка ввода
	                   }//end else
	            } break;
                //----------------------------------------------
	            case _KEY_8_SCANM: {
	                if ((menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
	                 {
                          step = 1;
	                  Scanner(_MEMORY);//сканировать по частотам из памяти
	                 }//end if
	                  else
	                  {
                            menu.Fval[menu.Fnumb] = 8;//цифра 8
                            FreqSetControl();//проверка ввода
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
	                    //установить частоту из памяти
	                    ReadFreqFromMem();
                            LCDviewMainData();

                           }
	                 }//end if
	                  else
	                   {
                            menu.Fval[menu.Fnumb] = 9;//цифра 9
                            FreqSetControl();//проверка ввода
	                   }//end else
	            } break;
                //----------------------------------------------
	    }//end switch
                keybrd.press = 0;//разрешаю очередное сканирование клавиатуры
  }//end if
        //****************************************************************

        //если режим НЕосновного меню и кроме меню ввода частоты,
        //то стартую отсчет времени...
        if ((menu.curr != _MENU_MAIN) && (menu.curr != _MENU_FSET) && (menu.curr != _MENU_SCANL) && (menu.curr != _MENU_SCANH))
         {

            if (flag.Menu) {flag.Menu = 0; menu.timeout++;}
            if (menu.timeout >= 5)  //~5 секунд...
             {
                menu.curr = _MENU_MAIN;
                LCDviewSmeter();//отображаю S-метр на LCD
                LCDviewAfterFreq();
             } //end if
         }//end if
        else
        menu.timeout = 0;

 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

