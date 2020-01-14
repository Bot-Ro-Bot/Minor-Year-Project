//*******************************************************************************
// Попдрограмма обработки данных квадратурного энкодера
//*******************************************************************************
// 							Reading point
//                                 V
//              ___     ___     ___     ___     ___     ___     ___     _
// Stream A|___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|
//            ___     ___     ___     ___     ___     ___     ___     ___
// Stream B__|   |___|   |___|   |___|   |___|   |___|   |___|   |___|
//
//           Counter-clockwise <--|--> Clockwise
//
//  (A;B)            Current:
// Previous | (0;0) (0;1) (1;0) (1;1)
//          |------------------------
//  (0;0)   |   NC    CW   cCW   Err        NC = No change
//  (0;1)   |  cCW    NC   Err    CW        CW = Clockwise
//  (1;0)   |   CW   Err    NC   cCW       cCW = Counter-clockwise
//  (1;1)   |  Err   cCW    CW    NC       Err = Error (ignored)
//
//*******************************************************************************
// Энкодер подключен к портам RD2 и RD3
//*******************************************************************************
//определяю направление вращения...
UCHAR RotaryDirection(void)
{
	if ((!encoder.PrevA) && (!encoder.PrevB))	//Prev (0;0)
	{
		if ((!encoder.CurrA) && (encoder.CurrB))  encoder.Count++; 	//Curr (0;1)
		if ((encoder.CurrA) && (!encoder.CurrB))  encoder.Count--; 	//Curr (1;0)
   		return(1);
	}//end if
	else
	//********************************************************************
	if ((!encoder.PrevA) && (encoder.PrevB))	//Prev (0;1)
	{
 		if ((!encoder.CurrA) && (!encoder.CurrB)) encoder.Count--; 	//Curr (0;0)
  		if ((encoder.CurrA) && (encoder.CurrB))   encoder.Count++; 	//Curr (1;1)
   		return(1);
  	}//end if
  	else
	//********************************************************************
	if ((encoder.PrevA) && (!encoder.PrevB))	//Prev (1;0)
	{
  		if ((!encoder.CurrA) && (!encoder.CurrB)) encoder.Count++; 	//Curr (0;0)
   		if ((encoder.CurrA) && (encoder.CurrB))   encoder.Count--; 	//Curr (1;1)
   		return(1);
   	}//end if
   	else
	//********************************************************************
	if ((encoder.PrevA) && (encoder.PrevB))		//Prev (1;1)
	{
   		if ((!encoder.CurrA) && (encoder.CurrB))  encoder.Count--; 	//Curr (0;1)
   		if ((encoder.CurrA) && (!encoder.CurrB))  encoder.Count++; 	//Curr (1;0)
   		return(1);
  	}//end if
  	return(0);
}
//*******************************************************************************
#define _ENC_NOROT  0
#define _ENC_RIGHT  1
#define _ENC_LEFT   2
//опрос энкодера
void RotaryEncoder(void)
{
  	static UCHAR quadR, quadL;

  	//состояние вывода А
  	encoder.CurrA = _ENCA;
	if (RotaryDirection()) encoder.PrevA = encoder.CurrA;

    //состояние вывода В
  	encoder.CurrB = _ENCB;
	if (RotaryDirection()) encoder.PrevB = encoder.CurrB;

    //изменилось ли положение энкодера?
	if (encoder.Count != encoder.CountOld)
	{
            LED=1;
            timeLED=0;
            if (encoder.Count > encoder.CountOld) quadR++;
	    if (encoder.Count < encoder.CountOld) quadL++;
	    encoder.CountOld = encoder.Count;
	    //делю на 4 (для плавности...)
	    if (quadR >= 4) {quadR = 0; quadL = 0; encoder.Dir = _ENC_RIGHT;}
	    if (quadL >= 4) {quadL = 0; quadR = 0; encoder.Dir = _ENC_LEFT;}
	}


}
//*******************************************************************************
