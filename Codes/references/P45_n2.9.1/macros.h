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
#define _FCPU    8000000 //������� ������������ ��
//
#define _RCVPCH1 37300  //37.3 MHz
#define _IFCORR  50
#define _RCVPCH2 10700  //10.7 MHz
#define _RCV_MAX 930000 //������������ �������
#define _RCV_MIN 40000  //����������� �������
//
//����� � EEPROM ��� ���������� ������
#define  _ADR_EE_FREQS  (UINT) 32
//
//������������ ���������� ������� � EEPROM
#define  _MAX_EE_SAVES  ((UINT)(512 - _ADR_EE_FREQS)/(sizeof(struct RADIO)))
//���-�� "���������"
#define _LINERES       (12*3)
//���-�� ��������� SQL
#define _LINERES_SQL   13
//����������� �������� ���������
#define _VOL_MIN_VAL   0x180 //1.25 Volt
//������ ��������� ������ USART
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
#define _PAUSES        20 //������������ ����� ��� ������������
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
//������
#define _KEY_0_STOP_ST 1    //������� ������������, ��������� �������� ��������.
#define _KEY_1_STEP    2    //��������� ����
#define _KEY_2_SQL     3    //��������� ������ "��������������"   weak signal booster
#define _KEY_3_UAGC    4    //����� ���
#define _KEY_4_SCANL   5    //������������� ��������� ������������
#define _KEY_5_SCANF   6    //������������ � �������� ��������� ������
#define _KEY_6_SCANH   7    //������� ������� ��������� ������������
#define _KEY_7_WRITEM  8    //��������� ������� ������� � ������
#define _KEY_8_SCANM   9    //����������� �� �������� �� ������
#define _KEY_9_READM   10   //���������� ������� �� ������
#define _KEY_FREQ_SET  11   //������ ���� �������
#define _KEY_SEL       12   //����� ������: �������/������� ���������
#define _KEY_GAIN      13   //weak signal booster
#define _KEY_WFM       14   //��������� WFM
#define _KEY_NFM       15   //��������� NFM
#define _KEY_AM        16   //��������� AM
//encoder
#define _ENCA          PIND.3
#define _ENCB          PIND.4
//���������
#define LED            PORTC.1//PORTC.1
#define Butt           PINC.2
//
#define _WFM           0
#define _NFM           1
#define _AM            2
//
#define _MENU_MAIN     0    //�������� ����
#define _MENU_VOL      1    //���� ��������� ������ ���������
#define _MENU_SQL      2    //���� ��������� SQL
#define _MENU_TS       3    //���� ��������� ���� ���������
#define _MENU_SCANL    4    //���� ����� ������ ������� ������������
#define _MENU_SCANF    5    //������������ � �������� ��������� ������
#define _MENU_SCANH    6    //���� ����� ������� ������� ������������
#define _MENU_FSET     7    //���� ����� �������
#define _MENU_UAGC     8    //���� ��������� ���
#define _MENU_ST       9    //���� ��������� ����� ��� ������������
#define _MENU_SPD      10   //���� ��������� �������� ������������
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
//��������� ��������� � EEPROM
eeprom ULONG FreqCurrent = 106600;  //������� ������� (���)
eeprom UCHAR Modulation = 0;        //��������� (0-WFM/1-NFM/2-AM)
eeprom UCHAR AGC = 1;               //��� (0-����/1-����/2-������)
eeprom UCHAR LevAGC = 22;           //��������� ���
eeprom UCHAR Gain = 0;              //gain (0-����./1-���.)
//
eeprom UCHAR Volume = 10;           //�������� ������ ���������
eeprom ULONG FreqScanMin = 144000;  //����������� ������� ��� ������ ������������
eeprom ULONG FreqScanMax = 146000;  //������������ ������� ��� ������ ������������
eeprom UCHAR FreqStep = 5;          //��� ��������� (����� � ������� ts[])
eeprom UCHAR SQL = 1;               //�������� ��������� SQL
eeprom UCHAR Pause = 3;             //�������� ����� ��� ������������ (0-��� �����!!!)
eeprom UCHAR Speed = 20;             //�������� �������� ������������ (����� � �������)
eeprom SCHAR bank = 0;
eeprom UCHAR scan = sql;
eeprom UCHAR disp = up;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
eeprom UINT  IF1 = 37300;           //������ ��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

