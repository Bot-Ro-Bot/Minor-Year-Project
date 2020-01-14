//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++ ATMEGA168 By -=SY=-, 09/01/2011 +++++++++++++++++++++
//+++++++++++++++++++ CODEVISIONAVR v.1.25.9 +++++++++++++++++++++++++++
//++++++++++++++++++++++++ 19/11/2012 corrected ++++++++++++++++++++++++
//++++++++++++++++++++++++ version 2.4 +++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "mega168a_bdef.h"
#include "mega168a.h"
#include "macros.h"
#include <string.h>
#include "stdio.h"
#include <math.h>
#include "delay.h"
#include "1100.h"
#include "var.h"
#include "spiee.h"
#include "lcd.h"
#include "lc72131.h"
#include "i2c.h"
#include "encoder.h"
#include "work.h"
//#include "rs232.h"
#include "kbrd2.h"
//#include "rc5.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void main(void)
 {
    CLKPR = 0x00;//no clock prescaler
    SMCR = 0x00;//no power save mode
    PRR = 0x00;//no power reduction
    //Timer/Counter 0 initialization
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    OCR0A = 0x00;
    OCR0B = 0x00;
    //Timer/Counter 1 initialization
    //Timer1 is 2 x 10-bit PWM
    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << CS10) | (1 << WGM12);// Timer1 running on MCU clock/1, Fast PWM
    TCCR1C = 0x00;
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    ICR1H = 0x00;
    ICR1L = 0x00;
    //PWM
    OCR1AH = 0x03; OCR1AL = 0xFF;  // set PWM1 value
    OCR1BH = 0x03; OCR1BL = 0xFF;  // set PWM2 value
    //Timer/Counter 2 initialization
    ASSR = 0x00;
    TCCR2A = 0x00;
    TCCR2B = (1 << CS20) | (1 << CS22); //1:128
    TCNT2 = 0x00;
    OCR2A = 0x00;
    OCR2B = 0x00;
    //External Interrupt(s) initialization
    //int0  |_
    EICRA = (1 << ISC01);
    //enable interrupt from INT 0
    EIMSK = (1 << INT0);
    EIFR = (1 << INTF0);
    PCICR = 0x00;
    //Timer/Counter Interrupt(s) initialization
    TIMSK2 = (1 << TOIE2A);  //��������� ���������� �� ������� 2
  //Analog Comparator initialization
    ACSR = 0x80;
    ADCSRB = 0x00;
    ADMUX = 0x00;
  //����������� ������
	// 1 = �����, 0 = ����
	DDRB = 0b10111110;
	DDRC = 0b11110010;
	DDRD = 0b11100000;
  //------------------------------
	PORTB = 0x41; //�������� ������������� ��������� PB0, PC0, ��2
	PORTC = 0x03;
	PORTD = 0x04; //PD2

  // Watchdog Timer initialization
  // Watchdog Timer Prescaler: OSC/1024k
  // Watchdog Timer interrupt: Off
    #pragma optsize-
    #asm("wdr")
    WDTCSR = 0x39;
    WDTCSR = 0x29;
  //-----------------------------------------------------------------
   PowerSwith = On;
   Beep(1);
    do
    {
     delay_ms(10);
    }while(Powerkey == PowerkeyPress);
   flag.Mute = 1; //��������� ����

 //�������������� ��������� �� EEPROM
    RestoreSetupFromEEPROM();
    memMIN = arraymemmin[radioadd.bank];
    memMAX = arraymemmax[radioadd.bank];
    menu.adrRMEM = memMIN;
    VolumeSet();
    LED=On;timeLED=0;pauseLED=14000;tauAGC = 25;
    NoFreqView();

  //������������� LCD
    LCD_init();
  //��������� 595 � 00000000
    KeyboardReset();
  //���������� ��� ���������
    SetRADIOfreq();//�������/���������
    UAGC = _AGC_MAX;
    UagcSet();//���
  //enable interrupts
    _INT_ON;
 //---------------------------------------------------------------------
     ktout = 30;
     KeyboardScan();
 //���������� �������
    DisplayOrient();
 //---------------------------------------------------------------------
    //������������� ��1
    IF1correction();
    //���������� ������
    LCDviewMainData();
    LCDviewSmeter();
    VolumeSet();//���������
 //����-------------------------------------------------------------
    while(1)
	{
        //�������� WDT
        _WDT_RESET;
        //S-meter
        SmeterCalculate();
        //���������� ������ (SQL)
        MuteControl();
        //���������� ���
        UAGCcontrol();
        //��������� ������� ������ � ��������
        KbrdEnc();
        //��������� ������ ���������
        Light_LED();
        //Battery
        Battery();
        PowerKey();
    }//end while
 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define _PRS        128
#define _t0         (((_PRS * 256) * 1000) / _FCPU)
//#define _t_200ms    (200 / _t0)
#define _t_100ms    (100 / _t0)//100ms
#define _t_1s       (1000 / _t0)
#define _t_2s       (2000 / _t0)
#define _t_30ms     (30 / _t0)
//Timer 2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
 {
    static UCHAR tmr1, tmr2, tmr3, tmr4;
    static UINT tmr5;
    //������ ��������� ��� ������ ������������
    if (tmr4 >= (240 / scanSpd[radioadd.Speed])) {tmr4 = 0; flag.ScanTime = 1;}
    else tmr4++;
    //������ ��������� ~100ms ��������
    if (tmr1 >= _t_100ms) {tmr1 = 0; flag.Smeter = 1;} // S-metr 100ms
    else if (!flag.Smeter) tmr1++;
    //������ ��������� ~1s ��������
    if (tmr2 >= _t_1s) {tmr2 = 0; flag.Menu = 1;}
    else if (!flag.Menu) tmr2++;
    //2s
    if (tmr5 >= _t_2s) {tmr5 = 0; flag.batt = 1;}
    else if (!flag.batt) tmr5++;
    //������ ��������� ~30ms ��������
    if (flag.Beep)
     {
        if (tmr3 >= _t_30ms)
         {
            tmr3 = 0;
            flag.Beep = 0;
            _BEEPER = 0; //����. �����
         }//end if
        else tmr3++;
     }//end if
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //��������� �������
    RotaryEncoder();
    //��������� ����������
     if (((PINB.0 ==0)||(PINC.0 ==0)) && (ktout2 > 50))
      {
         keybrd.press = 0;
         KeyboardScan();
      }
      else
      {  ktout = 0;
         if (ktout2 < 255) ktout2++;
      }
   }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


















