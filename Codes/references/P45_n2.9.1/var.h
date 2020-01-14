//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT batt;
UINT vbatt;          //напряжение батареи
char vcharl,vcharh;  //текущие символы батареи
UCHAR tauAGC;
SCHAR step;
UCHAR ktout;
UCHAR ktout2;
UCHAR SaveADRmem;
eeprom UCHAR *pEE;
unsigned int timeLED;
UINT pauseLED;
SINT memMIN;
UINT memMAX;
UCHAR *blank = {"                "};

flash UCHAR mod[3][4] = {"WFM", "NFM", " AM"}; //отображаемые виды модуляции
//37 значений уровня
flash UCHAR vol[_LINERES] = \
{230,227,224,223,222,220,219,217,215,213,210,202,191,185,177,
 160,140,73,60,50,45,41,37,34,30,27,24,21,18,15,12,9,7,4,2,0};
//
flash UCHAR ts[8] = {1,5,10,15,20,25,50,100}; //шаги
flash UCHAR scanSpd[] = {1,2,3,4,5,6,7,8,10,12,15,20,25,30};//скорости сканирования
//соответствие кнопок на клавиатуре
flash UCHAR KeysArray[16] = {_KEY_SEL,      _KEY_7_WRITEM,  _KEY_4_SCANL,   _KEY_1_STEP, \
                             _KEY_FREQ_SET, _KEY_9_READM,   _KEY_6_SCANH,   _KEY_3_UAGC, \
                             _KEY_0_STOP_ST,_KEY_8_SCANM,   _KEY_5_SCANF,   _KEY_2_SQL, \
                             _KEY_AM,       _KEY_NFM,       _KEY_WFM,       _KEY_GAIN};
UINT UAGC;//для АРУ...
//для конвертирования
flash UCHAR symbForConv[10] = {'0','1','2','3','4','5','6','7','8','9'};
//режимы LC72131
flash UCHAR Modes[3] = {0xC0,0xC2,0xC1};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//меню
struct MENU
 {
    UCHAR curr;
    UCHAR timeout; //таймаут отображения меню
    UCHAR Fnumb;   //счетчик кол-ва цифр при вводе частоты
    UCHAR Fval[6]; //значения разрядов при вводе частоты
    UCHAR Fdig[7];  //значения разрядов при отображении частоты + точка
    UINT  adrWMEM; //
    SINT  adrRMEM;
}  menu;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//флаги
struct FLAG
 {
    UCHAR Kbrd;
    UCHAR Smeter;
    UCHAR ScanTime;
    UCHAR ReadMEM;
    UCHAR Beep;
    UCHAR Menu;
    UCHAR Mute;     //режим MUTE
    UCHAR Scan;     //сканирование
    UCHAR ScanDir;  //"напрваление" сканирования
    UCHAR NextWait;
    UCHAR batt;
 } flag;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct KEYBRD
 {
    UCHAR press;
 } keybrd;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// структура данных энкодера
struct ENCODER
 {
	UCHAR CurrA;	//текущее значение на выводе А
	UCHAR CurrB;	//текущее значение на выводе B
	UCHAR PrevA;	//предыдущее значение на выводе А
	UCHAR PrevB;	//предыдущее значение на выводе В
	UCHAR Key;   	//состояние кнопки энкодера
	UCHAR Count;	//изменяемое значение
	UCHAR CountOld; //изменяемое значение последнее
	UCHAR Dir;      //направление вращения (1-лево, 2-право)
 } encoder;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct RADIO
 {
    ULONG FreqCurrent;  //текущая частота
 } radio;
struct RADIOADD
 {
    UCHAR Volume;       //значение уровня громкости
    ULONG FreqScanMin;  //минимальная частота для режима сканирования
    ULONG FreqScanMax;  //максимальная частота для режима сканирования
    UCHAR FreqStep;     //шаг настройки (номер в массиве ts[])
    UCHAR SQL;          //значение настройки SQL
    UCHAR Pause;        //значение паузы при сканировании (0-нет паузы!!!)
    UCHAR Speed;        //значение скорости сканировании (номер в массиве)
    SCHAR bank;
    UCHAR scan;
  //------------------------------------------------------------------------
    UCHAR AGC;
    UCHAR LevAGC;
    UCHAR Gain;
    ULONG Modulation;
  //------------------------------------------------------------------------
 } radioadd;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//структура данных S-метра
struct SMETER
 {
    UCHAR curr;
    UCHAR old;
    UCHAR oldAGC;
 } smeter;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

