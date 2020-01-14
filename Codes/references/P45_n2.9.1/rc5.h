//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
void RC5error(void)
{
    //разрешить прерывания от таймера 2
    TIMSK2 |= (1 << TOIE2A);
    //разрешить прерывание от USART
    UCSR0B |= (1 << RXCIE0);
	//остановить таймер 0
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TIMSK0 &=~ (1 << TOIE0);
	//очистить данные RC5
	rc5.data = 0;
	rc5.bitcnt = 0;
	rc5.mbitHalf = 0;
    //разрешить прерывания от INT0
	EIMSK |= (1 << INT0);
	EIFR |= (1 << INTF0);//сбрасываем флаг прерывания
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//обработка данных дистанционного пульта управления
//#define _RC5_MANCH_BIT  		/* 889 us   */ 144
//#define _RC5_MANCH_BIT_HALF  	/* 444.5 us */ 200
//RC5--------------------------------------------------------------------------
//
//  format: S S T A0 A1 A2 A3 A4 C0 C1 C2 C3 C4 C5 (14 bit)
//
//     manchester
// <-1-> <-1->
//---+  +--+  +--+
//   |  |  |  |  |
//  1| 0| 1| 0| 1|
//   |__|  |__|  |_xxxxxx
//
// <- start -> <- data.....->
//
//   <-INT0 (falling)
//
//   | |  |  |  |
//   <-444.5 (first)
//     <-889 .........
//
// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    //запретить прерывания INT0
	EIMSK &=~ (1 << INT0);
	EIFR &=~ (1 << INTF0);
	//запретить остальные прерывания
	TIMSK2 &=~ (1 << TOIE2A);
	UCSR0B &=~ (1 << RXCIE0);
	//
	rc5.mbitF = 1;//первая половина бита будет 1
	rc5.mbitHalf = 1;//ждать вторую половину...
	//запуск таймера на время 1/2 бита (444.5 uS)
	TCNT0 = _RC5_MANCH_BIT_HALF;
	TIFR0 |= (1 << TOV0);
	TCCR0A = 0x00;
	TCCR0B = (1 << CS01) | (1 << CS00);//запускаем таймер  1:64
	TIMSK0 |= (1 << TOIE0);//разрешить прерывание от Таймера 0
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    TCNT0 = _RC5_MANCH_BIT;//обновить таймер (889 uS)
    TIFR0 |= (1 << TOV0);

    if (_IR_IN) rc5.irPin = 1;//запомнить состояние входа
    else rc5.irPin = 0;
    //принимаю 14 бит...
	if (rc5.bitcnt < 14)
	{
		if (rc5.mbitHalf++ & 0x1)//вторая половина
		{
			//вторая половина
			rc5.mbitS = rc5.irPin;
			//проверяю на соответствие манчестерскому коду
			if (rc5.mbitF != rc5.mbitS)
			{
				rc5.data <<= 1;//к след.биту
				// 01 -> log.1 (после приемника ИНВЕРСНО!!!)
				if ((!rc5.mbitF) && (rc5.mbitS)) rc5.data |= 0x1;
				//+1 к счетчику бит
				rc5.bitcnt++;
			}
			else RC5error();//ошибка!!!
		}
		//первая половина
		else rc5.mbitF = rc5.irPin;
	}
	else
	{
        //принятые данные от ПДУ
		rc5.trig =    (UCHAR)((rc5.data & 0b100000000000) >> 11);//trigger
		rc5.addr =    (UCHAR)((rc5.data & 0b011111000000) >> 6);//address
		rc5.command = (UCHAR)((rc5.data & 0b000000111111));//command
        //имитирую нажатие кнопок...
        if (rc5.trig != rc5.trigOld)
        {
            //исключаю повторения нажатий...
            rc5.trigOld = rc5.trig;
            //флаг принятой команды
            rc5.newCmdFlag = 1;
        }
		RC5error();//сбросить для след.цикла
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//программирование ПДУ
void RCprogramming(void)
{
    UCHAR index = 0;
    //если не удерживается кнопка GAIN, то выйти
    delay_ms(300);
    if (keybrd.press != _KEY_GAIN) return;
    keybrd.press = 0;
    //вывести сообщение
    gotoxy(0,0);
    print_string("Remote Program: ");
    //все кнопки по-порядку
    while(1)
    {
        _WDT_RESET;
        //вывести название команды
        gotoxy(0,1);
        print_message(rcCMD[index]);
        //команда ПДУ
        gotoxy(14,1);
        print_char((UCHAR)(rc5.command/10) + 0x30);
        print_char((UCHAR)(rc5.command%10) + 0x30);
        //ждать нажатия кнопки подтверждения
        if (keybrd.press == _KEY_AM)
        {
            //сохранить в EEPROM
            if (index < 16) RC5Array[rcdest[index]] = rc5.command;//массив кнопок
            else
            {
                //доп. команды
                switch(index)
                {
                    case 16: {RC5_ENC_RIGHT = rc5.command; break;}
                    case 17: {RC5_ENC_LEFT  = rc5.command; break;}
                    case 18: {RC5_VOL_UP    = rc5.command; break;}
                    case 19: {RC5_VOL_DOWN  = rc5.command; break;}
                }
            }
            //если все кнопки - выйти
            if (++index >= 20) return;
            rc5.command = 0;
            keybrd.press = 0;
        }
        //ждать нажатия кнопки выхода
        if (keybrd.press == _KEY_SEL) return;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
