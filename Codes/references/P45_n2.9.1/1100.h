//****************************************************************************
//NOKIA LCD
//Function libruary
//Build by Panda
//Based on source from http://www.sunbizhosting.com/~spiral/
//19.04.2009
/*
��������:
���������� ������� �� 2-� ������:
- ������� ������ �� ������� �� Panda, ����� ������ ������������ ����� � ������ �������;
- ������� ������ �� ������� �� ������� ������������ ������ ����������� ���������������,
  ����� ������ ������������ � ����� ����������������, � ����� ���� ������� ������������ � �������
  �������� LCD_refresh ().
  ��� ���������� ������ ������ ����� ����� � ��������� ������� �������� �������� ����� ������ ���,
  ����� ��� ������ ������� � ���������� ���� ���������� ������ ���������������� ( ��������,
  ����� (96 � 64)/8 + 100 == ��� - 868.
         _______                                _______
		|  1  2  3  4  5  6  7  8  9   |       |
        |       |______________________________|       |
		|                                      |
		|                                      |
		|                                      |
		|            ��� � �������� �������    |
		|                                      |
		|                                      |
		|                                      |
		|                                      |
         ----------------------------------------------

1 - Reset
2 - CS
3 - Gnd
4 - SDA
5 - SCLK
6 - VDDI
7 - VDD
8 - Led +
9 - �� ������������


������� ����� �������� � ��������� ���������� 3 - 5 � ��� ���������� ������
������� ������� VDD � VDDI
��� ������� ����� ����������� �������� ��������� � LCD_init() ��� ��������� ���������

� ���������� ������������ ��������� ����� �������.
��������� �� ��� ������������������, �� ��� ��������������� ��������� ��������� ����� ��������������.
����� �� Panda 8�5 ������
����� �� ������ 8�8 ����������, ��������� ������� ������
����� �� MCV ���������� �����, ������ , ����� ��� ����� 3310
��������� ������� ������ ������ �������.

����� � �������������� ����� �������� �����-������  �������-����, ����� �������� ������ ��������� - ���� ���������

�������� ������ � ������� ����.

� ������� ����������� ���� ���������� 2 �����: 1100.h � 1100.lib , � ����������� ��� ����� � ����� �������.

*/
//****************************************************************************

#define CMD	0
#define DATA	1
sfrb LCD_PORT=0x05;   // PORT C
//sfrb LCD_PORT=0x12; // PORT D
// ����� ����� ����� ������ � Datasheet

// ����� ����� ������ ����� ����� ��
#define sclk         5; //bit number 0 ... 7
#define sda          3; //bit number
//#define cs           5; //bit number
//#define rst          6; //bit number
//#define _NOP		#asm ("nop")
#define _INT_ON     #asm ("sei")
#define _INT_OFF    #asm ("cli")
extern unsigned char posx, posy;
extern eeprom unsigned char disp;

// ������� �� ����������� ������ � ���

// library function prototypes
void LCD_init(void); // ++   ������������� �������
//void LCD_refresh (void);  // ++  ���������� ������� (����������� �������)
//void LCD_clear (void); // ++  �������� ������
void LCD_clear_all (void); // ++ �������� ������ � �������
//void LCD_pixel (unsigned char x, unsigned char y, unsigned char color); //++ ������ �������. ��� ���������  �������� ����� ���
//void LCD_line ( char x1, char y1, char x2, char y2, char mode ); // ++ ��������� �����, mode=1 - �����, 0 - �������
// LCD_line (2,2,5,2,1) - �������������� ����� ������ ����� � 3 ����� ������
//void LCD_dashline ( char x1, char y1, char x2, char y2); // ++ ���������� �����
// LCD_dashline (2,2,15,2) - �������������� ���������� ����� ������ ����� � 13 ����� ������ �� 3 ����� ������ � ������
//void LCD_rectangle(char x, char y, char x1, char y1,unsigned char color);  //  ++ ������������� color =1 - �����, 0 - �������
// LCD_rectangle( 3,2,9,8,1) - ������������� ������ ����� 6�6 �� 3 ����� ������ � 4 ������
//void LCD_fillrect  (char x, char y, char x1, char y1,unsigned char color); // ++ ����������� �������������
// LCD_fillrect( 3,2,9,8,1) - ����������� ������������� ������ ����� 6�6 �� 3 ����� ������ � 4 ������
//void LCD_setxy(char x,unsigned char y);        // ++ ��������� ��������� �������
//void LCD_deg12x16(unsigned char c, unsigned char color);    // // ++ ����� ������� color =1 - �����, 0 - �������(���������)
// LCD_deg12x16(5, 1); ������� ����� 5 ������ �����
//void LCD_frame(unsigned char x); // ++ ������������ ��������� ��� ������ ��������� ��������
//unsigned char LCD_get_char_len(unsigned char ch); // // ++  ������������ ��������� ��� ����������� ������ �������
//void LCD_char(unsigned char ch, unsigned char color);    // ++ ����� ������� �� MCV
// LCD_char(125,1); - ������� ������ �125 ����� ������
// LCD_char(min[0]+48, 1); - ������� ����� min[0] ������ ����� , ��� 48 - ����� ������� ����� �� ����
// LCD_char('A',1); - ���������
//void  LCD_wrs (unsigned char*str,unsigned char color); // ++ ����� ������ �� ���������
// LCD_setxy(10,30); ��������� ��������� ������
// LCD_wrs(tmp, 0);  ����� ����������� ���������� tmp ������� ������ ( �������� )
//void LCD_wrsf (unsigned char flash*str,unsigned char color); // ++ ����� ������ �� �����
// LCD_wrsf("Friquency",1);  ����� ����� Friquency ����� ������

// ������������ ���� �������� ����������

//void LCD_char_F(unsigned char c, unsigned char color);    // ++ ����� �������  �� ������ !!!!!
//void LCD_wrs_F (unsigned char *str, unsigned char color); // ++ ����� ������ �� ���������
//void LCD_wrsf_F (unsigned char flash*str, unsigned char color); // ++ ����� ������ �� �����

//void LCD_bold_frame(unsigned char x);
//void LCD_bold_char(unsigned char ch, unsigned char color);    // ++ ����� �������� ������� ������� �� MCV
//void LCD_bold_wrs (unsigned char*str,unsigned char color); // ++ ����� ������ ������ �� ���������
//void LCD_bold_wrsf (unsigned char flash*str,unsigned char color); // ++ ����� ������ ������ �� �����

//void LCD_boldU_char(unsigned char ch, unsigned char color);    // ++ ����� �������� ������� ������� ����� 16�10
//void LCD_boldU_wrs (unsigned char*str,unsigned char color); // ++ ����� ������ ������ �� ���������  ����� 16�10
//void LCD_boldU_wrsf (unsigned char flash*str,unsigned char color); // ++ ����� ������ ������ �� ����� ����� 16�10




// ������� �� Panda


//void lcd_init(void);
void lcd_write(char cd,unsigned char c); // ++  �������� �������������,  ������ ������ � ����
//void lcd_clear(void);
//void setx(char x); // ++
//void sety(char y); // ++
void gotoxy(char x,char y); // ++
// gotoxy(20,2); // ������ ������ �� 20 ����� ������ ��������
//void vline(char x,char y,char on); // ++
//vline(10,52,0); // ������������ ����� ����� �� 10 ����� �������� ������ � ������ 52 �����
//void line(unsigned char x,unsigned char y,unsigned char y2,unsigned char on); // ++
//line(40,55,20,1); // ����� ������������ ����� �� 40 ����� ��������� ������
void print_message(flash char* message); // ++
//print_message("Hello");   // ��������
void print_char(char c); // ++
//gotoxy(70,4);
//print_char('A'); // ������� ������ A � 5 ������ �� 70 ����� ������
void print_string(char * message);  // ++
//void char_24_32(char c,char x, char y); // ++
// char_24_32(8,35, 3); // ������� ������� ������ "8" � 4 ������ � ��������� �� 35 ����� ������
void char_12_16(char c,char x, char y); // ++
//---------------------------------------------
//void SPIsendLCD(unsigned char byte, unsigned char dord);//���������� SPI



#pragma library 1100.lib

