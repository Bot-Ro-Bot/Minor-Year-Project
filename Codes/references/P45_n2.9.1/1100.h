//****************************************************************************
//NOKIA LCD
//Function libruary
//Build by Panda
//Based on source from http://www.sunbizhosting.com/~spiral/
//19.04.2009
/*
Описание:
библиотека состоит из 2-х частей:
- функции вывода на дисплей от Panda, когда данные записываются сразу в память дисплея;
- функции вывода на дисплей от проекта самодельного кодера передатчика радиоуправления,
  когда данные записываются в буфер микроконтроллера, а потом всем экраном записываются в дисплей
  функцией LCD_refresh ().
  Для правильной работы второй части нужно в свойствах проекта изменить величину стека данных так,
  чтобы для буфера дисплея и переменных было достаточно памяти микроконтроллера ( например,
  более (96 х 64)/8 + 100 == ОЗУ - 868.
         _______                                _______
		|  1  2  3  4  5  6  7  8  9   |       |
        |       |______________________________|       |
		|                                      |
		|                                      |
		|                                      |
		|            Вид с обратной стороны    |
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
9 - не используется


Дисплей может работать в интервале напряжений 3 - 5 В при соединённых вместе
выводах питания VDD и VDDI
При желании можно попробовать поменять параметры в LCD_init() для улучшения контраста

В библиотеке присутствуют несколько видов шрифтов.
Некоторые из них закомментированные, но при соответствующем изменении программы могут использоваться.
Шрифт от Panda 8х5 тонкий
Шрифт от Фокуса 8х8 утолщённый, смотрится намного виднее
Шрифт от MCV переменной шрины, тонкий , хорош для Нокии 3310
Различные большие шрифты хорошо заметны.

Линии и прямоугольники лучше рисовать слева-сверху  направо-вниз, иначе возможны ошибки рисования - надо проверить

Описание команд и функций ниже.

В проекте обязательно надо подключить 2 файла: 1100.h и 1100.lib , и скопировать эти файлы в папку проекта.

*/
//****************************************************************************

#define CMD	0
#define DATA	1
sfrb LCD_PORT=0x05;   // PORT C
//sfrb LCD_PORT=0x12; // PORT D
// Номер порта можно узнать в Datasheet

// Далее задаём номера ножек порта МК
#define sclk         5; //bit number 0 ... 7
#define sda          3; //bit number
//#define cs           5; //bit number
//#define rst          6; //bit number
//#define _NOP		#asm ("nop")
#define _INT_ON     #asm ("sei")
#define _INT_OFF    #asm ("cli")
extern unsigned char posx, posy;
extern eeprom unsigned char disp;

// Функции от передатчика Фокуса и МСВ

// library function prototypes
void LCD_init(void); // ++   ИНИЦИАЛИЗАЦИЯ ДИСПЛЕЯ
//void LCD_refresh (void);  // ++  Обновление дисплея (отображение буффера)
//void LCD_clear (void); // ++  стирание буфера
void LCD_clear_all (void); // ++ стирание буфера и дисплея
//void LCD_pixel (unsigned char x, unsigned char y, unsigned char color); //++ Рисуем пиксель. Все остальные  работают через нее
//void LCD_line ( char x1, char y1, char x2, char y2, char mode ); // ++ Рисование линии, mode=1 - тёмная, 0 - светлая
// LCD_line (2,2,5,2,1) - горизонтальная линия тёмного цвета в 3 точки длиной
//void LCD_dashline ( char x1, char y1, char x2, char y2); // ++ Пунктирная линия
// LCD_dashline (2,2,15,2) - горизонтальная пунктирная линия тёмного цвета в 13 точки длиной на 3 точки сверху и вправо
//void LCD_rectangle(char x, char y, char x1, char y1,unsigned char color);  //  ++ Прямоугольник color =1 - тёмный, 0 - светлый
// LCD_rectangle( 3,2,9,8,1) - прямоугольник тёмного цвета 6х6 на 3 точки сверху и 4 вправо
//void LCD_fillrect  (char x, char y, char x1, char y1,unsigned char color); // ++ закрашенный прямоугольник
// LCD_fillrect( 3,2,9,8,1) - закрашенный прямоугольник тёмного цвета 6х6 на 3 точки сверху и 4 вправо
//void LCD_setxy(char x,unsigned char y);        // ++ установка координат курсора
//void LCD_deg12x16(unsigned char c, unsigned char color);    // // ++ вывод символа color =1 - тёмный, 0 - светлый(инверсный)
// LCD_deg12x16(5, 1); выводит цифру 5 тёмного цвета
//void LCD_frame(unsigned char x); // ++ Используется автоматом при выводе инверсных символов
//unsigned char LCD_get_char_len(unsigned char ch); // // ++  Используется автоматом для определения ширины символа
//void LCD_char(unsigned char ch, unsigned char color);    // ++ вывод символа от MCV
// LCD_char(125,1); - выводит символ №125 тёмным цветом
// LCD_char(min[0]+48, 1); - выводит число min[0] тёмного цвета , где 48 - сдвиг таблицы фонта до цифр
// LCD_char('A',1); - проверить
//void  LCD_wrs (unsigned char*str,unsigned char color); // ++ ВЫВОД СТРОКИ ИЗ ОПЕРАТИВЫ
// LCD_setxy(10,30); установка координат вывода
// LCD_wrs(tmp, 0);  вывод содержимого переменной tmp светлым цветом ( инверсия )
//void LCD_wrsf (unsigned char flash*str,unsigned char color); // ++ ВЫВОД СТРОКИ ИЗ ФЛЕША
// LCD_wrsf("Friquency",1);  вывод слова Friquency тёмным цветом

// подпрограммы ниже работают аналогично

//void LCD_char_F(unsigned char c, unsigned char color);    // ++ вывод символа  от Фокуса !!!!!
//void LCD_wrs_F (unsigned char *str, unsigned char color); // ++ ВЫВОД СТРОКИ ИЗ ОПЕРАТИВЫ
//void LCD_wrsf_F (unsigned char flash*str, unsigned char color); // ++ ВЫВОД СТРОКИ ИЗ ФЛЕША

//void LCD_bold_frame(unsigned char x);
//void LCD_bold_char(unsigned char ch, unsigned char color);    // ++ вывод двойного жирного символа от MCV
//void LCD_bold_wrs (unsigned char*str,unsigned char color); // ++ ВЫВОД ЖИРНОЙ СТРОКИ ИЗ ОПЕРАТИВЫ
//void LCD_bold_wrsf (unsigned char flash*str,unsigned char color); // ++ ВЫВОД ЖИРНОЙ СТРОКИ ИЗ ФЛЕША

//void LCD_boldU_char(unsigned char ch, unsigned char color);    // ++ вывод двойного жирного символа поуже 16х10
//void LCD_boldU_wrs (unsigned char*str,unsigned char color); // ++ ВЫВОД ЖИРНОЙ СТРОКИ ИЗ ОПЕРАТИВЫ  поуже 16х10
//void LCD_boldU_wrsf (unsigned char flash*str,unsigned char color); // ++ ВЫВОД ЖИРНОЙ СТРОКИ ИЗ ФЛЕША поуже 16х10




// функции от Panda


//void lcd_init(void);
void lcd_write(char cd,unsigned char c); // ++  работает автоматически,  запись данных в дисп
//void lcd_clear(void);
//void setx(char x); // ++
//void sety(char y); // ++
void gotoxy(char x,char y); // ++
// gotoxy(20,2); // вторая строка на 20 точек вправо сдвинуто
//void vline(char x,char y,char on); // ++
//vline(10,52,0); // Вертикальная линия снизу на 10 точек сдвинута вправо и длиной 52 точки
//void line(unsigned char x,unsigned char y,unsigned char y2,unsigned char on); // ++
//line(40,55,20,1); // Гасит вертикальную линию на 40 точек сдвинутую вправо
void print_message(flash char* message); // ++
//print_message("Hello");   // Работает
void print_char(char c); // ++
//gotoxy(70,4);
//print_char('A'); // Выводит символ A в 5 строке на 70 точек вправо
void print_string(char * message);  // ++
//void char_24_32(char c,char x, char y); // ++
// char_24_32(8,35, 3); // Выводит большой символ "8" в 4 строке и сдвинутый на 35 точек вправо
void char_12_16(char c,char x, char y); // ++
//---------------------------------------------
//void SPIsendLCD(unsigned char byte, unsigned char dord);//аппаратный SPI



#pragma library 1100.lib

