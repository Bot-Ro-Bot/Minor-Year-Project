//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define UCHAR		unsigned char
#define SCHAR		signed char
#define UINT		unsigned int
#define SINT		signed int
#define ULONG		unsigned long
#define SLONG		signed long
#define FLOAT		float
#define DOUBLE		double
//
#define _NOP		#asm ("nop")
#define _WDT_RESET 	#asm ("wdr")
#define _INT_ON     #asm ("sei")
#define _INT_OFF    #asm ("cli")
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define _FCPU    8000000 //частота тактировани€ ћ 
//
#define _RCVPCH1 37300  //37.3 MHz
#define _IFCORR  50
#define _RCVPCH2 10700  //10.7 MHz
#define _RCV_MAX 930000 //максимальна€ частота
#define _RCV_MIN 40000  //минимальна€ частота
//
//адрес в EEPROM дл€ сохранени€ частот
#define  _ADR_EE_FREQS  (UINT) 32
//
//максимальное количество записей в EEPROM
#define  _MAX_EE_SAVES  ((UINT)(512 - _ADR_EE_FREQS)/(sizeof(struct RADIO)))
//кол-во "полосочег"
#define _LINERES       (12*3)
//кол-во положений SQL
#define _LINERES_SQL   13
//минимальное значение громкости
#define _VOL_MIN_VAL   0x180 //1.25 Volt
//размер приемного буфера USART
//#define _USARTRXBUFFLEN 16
//
#define _FREQ          0
#define _MEMORY        1
//
#define _SMETERLN      0
#define _VOLUMELN      1
#define _SQLLN         2
#define _UAGCLN        3
//
#define _PAUSES        20 //максимальна€ пауза дл€ сканировани€
//AGC
#define _AGC_OFF       0
#define _AGC_AUTO      1
#define _AGC_MAN       2
//I2C
#define _I2C_BitClk    PORTC.5   //SCL
#define _I2C_BitDat    PORTC.4   //SDA
//SPI
#define _SPI_CLK       PORTB.5   //CLK
#define _SPI_DATA      PORTB.3   //MOSI
//LCD
#define _LCD_LATCH     PORTD.5   //LCD 595 LATCH
//beeper
#define _BEEPER        PORTB.4   //beep
//LC72131
#define _LC72131_LATCH PORTD.6   //LC LATCH
//KEYBOARD
#define _KBD_LATCH     PORTD.7   // KEYS 595 LATCH
#define _KBD_IN1       PINB.0
#define _KBD_IN2       PINC.0
//IR
//#define _IR_IN         PIND.2
//кнопки
#define _KEY_0_STOP_ST 1    //останов сканировани€, настройка задержки сканиров.
#define _KEY_1_STEP    2    //установка шага
#define _KEY_2_SQL     3    //установка порога "шумоподавител€"   weak signal booster
#define _KEY_3_UAGC    4    //режим ј–”
#define _KEY_4_SCANL   5    //нижн€€граница диапазона сканировани€
#define _KEY_5_SCANF   6    //сканирование в заданном диапазоне частот
#define _KEY_6_SCANH   7    //верхн€€ граница диапазона сканировани€
#define _KEY_7_WRITEM  8    //сохранить текущую частоту в пам€ть
#define _KEY_8_SCANM   9    //сканировать по частотам из пам€ти
#define _KEY_9_READM   10   //установить частоту из пам€ти
#define _KEY_FREQ_SET  11   //пр€мой ввод частоты
#define _KEY_SEL       12   //выбор режима: частота/уровень громкости
#define _KEY_GAIN      13   //weak signal booster
#define _KEY_WFM       14   //модул€ци€ WFM
#define _KEY_NFM       15   //модул€ци€ NFM
#define _KEY_AM        16   //модул€ци€ AM
//encoder
#define _ENCA          PIND.3
#define _ENCB          PIND.4
//ѕодсветка
#define LED            PORTC.1//PORTC.1
#define Butt           PINC.2
//
#define _WFM           0
#define _NFM           1
#define _AM            2
//
#define _MENU_MAIN     0    //основное меню
#define _MENU_VOL      1    //меню установки уровн€ громкости
#define _MENU_SQL      2    //меню установки SQL
#define _MENU_TS       3    //меню установки шага настройки
#define _MENU_SCANL    4    //меню ввода нижней границы сканировани€
#define _MENU_SCANF    5    //сканирование в заданном диапазоне частот
#define _MENU_SCANH    6    //меню ввода верхней границы сканировани€
#define _MENU_FSET     7    //меню ввода частоты
#define _MENU_UAGC     8    //меню установки ј–”
#define _MENU_ST       9    //меню установки паузы при сканировании
#define _MENU_SPD      10   //меню установки скорости сканировани€
#define _MENU_SCAN     11
#define _MENU_BANK     12
//
#define _UPSCAN        0
#define _DOWNSCAN      1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define Powerkey       PINB.6
#define PowerSwith     PORTB.7
#define Off            0
#define On             1
#define PowerkeyPress  0
//
#define time           0
#define sql            1
#define down           0
#define up             1
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//начальные настройки в EEPROM
eeprom ULONG FreqCurrent = 106600;  //текуща€ частота (к√ц)
eeprom UCHAR Modulation = 0;        //модул€ци€ (0-WFM/1-NFM/2-AM)
eeprom UCHAR AGC = 1;               //ј–” (0-выкл/1-авто/2-ручное)
eeprom UCHAR LevAGC = 22;           //настройка ј–”
eeprom UCHAR Gain = 0;              //gain (0-выкл./1-вкл.)
//
eeprom UCHAR Volume = 10;           //значение уровн€ громкости
eeprom ULONG FreqScanMin = 144000;  //минимальна€ частота дл€ режима сканировани€
eeprom ULONG FreqScanMax = 146000;  //максимальна€ частота дл€ режима сканировани€
eeprom UCHAR FreqStep = 5;          //шаг настройки (номер в массиве ts[])
eeprom UCHAR SQL = 1;               //значение настройки SQL
eeprom UCHAR Pause = 3;             //значение паузы при сканировании (0-нет паузы!!!)
eeprom UCHAR Speed = 20;             //значение скорости сканировании (номер в массиве)
eeprom SCHAR bank = 0;
eeprom UCHAR scan = sql;
eeprom UCHAR disp = up;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
eeprom UINT  IF1 = 37300;           //перва€ ѕ„
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

