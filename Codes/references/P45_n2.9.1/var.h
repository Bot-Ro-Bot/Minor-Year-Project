//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UINT batt;
UINT vbatt;          //���������� �������
char vcharl,vcharh;  //������� ������� �������
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

flash UCHAR mod[3][4] = {"WFM", "NFM", " AM"}; //������������ ���� ���������
//37 �������� ������
flash UCHAR vol[_LINERES] = \
{230,227,224,223,222,220,219,217,215,213,210,202,191,185,177,
 160,140,73,60,50,45,41,37,34,30,27,24,21,18,15,12,9,7,4,2,0};
//
flash UCHAR ts[8] = {1,5,10,15,20,25,50,100}; //����
flash UCHAR scanSpd[] = {1,2,3,4,5,6,7,8,10,12,15,20,25,30};//�������� ������������
//������������ ������ �� ����������
flash UCHAR KeysArray[16] = {_KEY_SEL,      _KEY_7_WRITEM,  _KEY_4_SCANL,   _KEY_1_STEP, \
                             _KEY_FREQ_SET, _KEY_9_READM,   _KEY_6_SCANH,   _KEY_3_UAGC, \
                             _KEY_0_STOP_ST,_KEY_8_SCANM,   _KEY_5_SCANF,   _KEY_2_SQL, \
                             _KEY_AM,       _KEY_NFM,       _KEY_WFM,       _KEY_GAIN};
UINT UAGC;//��� ���...
//��� ���������������
flash UCHAR symbForConv[10] = {'0','1','2','3','4','5','6','7','8','9'};
//������ LC72131
flash UCHAR Modes[3] = {0xC0,0xC2,0xC1};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����
struct MENU
 {
    UCHAR curr;
    UCHAR timeout; //������� ����������� ����
    UCHAR Fnumb;   //������� ���-�� ���� ��� ����� �������
    UCHAR Fval[6]; //�������� �������� ��� ����� �������
    UCHAR Fdig[7];  //�������� �������� ��� ����������� ������� + �����
    UINT  adrWMEM; //
    SINT  adrRMEM;
}  menu;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�����
struct FLAG
 {
    UCHAR Kbrd;
    UCHAR Smeter;
    UCHAR ScanTime;
    UCHAR ReadMEM;
    UCHAR Beep;
    UCHAR Menu;
    UCHAR Mute;     //����� MUTE
    UCHAR Scan;     //������������
    UCHAR ScanDir;  //"�����������" ������������
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
// ��������� ������ ��������
struct ENCODER
 {
	UCHAR CurrA;	//������� �������� �� ������ �
	UCHAR CurrB;	//������� �������� �� ������ B
	UCHAR PrevA;	//���������� �������� �� ������ �
	UCHAR PrevB;	//���������� �������� �� ������ �
	UCHAR Key;   	//��������� ������ ��������
	UCHAR Count;	//���������� ��������
	UCHAR CountOld; //���������� �������� ���������
	UCHAR Dir;      //����������� �������� (1-����, 2-�����)
 } encoder;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct RADIO
 {
    ULONG FreqCurrent;  //������� �������
 } radio;
struct RADIOADD
 {
    UCHAR Volume;       //�������� ������ ���������
    ULONG FreqScanMin;  //����������� ������� ��� ������ ������������
    ULONG FreqScanMax;  //������������ ������� ��� ������ ������������
    UCHAR FreqStep;     //��� ��������� (����� � ������� ts[])
    UCHAR SQL;          //�������� ��������� SQL
    UCHAR Pause;        //�������� ����� ��� ������������ (0-��� �����!!!)
    UCHAR Speed;        //�������� �������� ������������ (����� � �������)
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
//��������� ������ S-�����
struct SMETER
 {
    UCHAR curr;
    UCHAR old;
    UCHAR oldAGC;
 } smeter;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

