//***** BIT DEFINITIONS *****************************************************
//BITDEF //Embedded Pascal specific extention
//**********USART0*********************
//UDR0:
//UCSR0A:
#define  RXC0       7
#define  TXC0       6
#define  UDRE0      5
#define  FE0        4
#define  DOR0       3
#define  UPE0       2
#define  U2X0       1
#define  MPCM0      0
//UCSR0B:
#define  RXCIE0     7
#define  TXCIE0     6
#define  UDRIE0     5
#define  RXEN0      4
#define  TXEN0      3
#define  UCSZ02     2
#define  RXB80      1
#define  TXB80      0
//UCSR0C:
#define  UMSEL01    7
#define  UMSEL00    6
#define  UPM01      5
#define  UPM00      4
#define  USBS0      3
#define  UCSZ01     2
#define  UCSZ00     1
#define  UCPOL0     0

//**********TWI************************

//TWCR:
#define  TWINT      7
#define  TWEA       6
#define  TWSTA      5
#define  TWSTO      4
#define  TWWC       3
#define  TWEN       2
#define  TWIE       0
//TWSR:
#define  TWS7       7
#define  TWS6       6
#define  TWS5       5
#define  TWS4       4
#define  TWS3       3
#define  TWPS1      1
#define  TWPS0      0

//TWAR:
#define  TWA6       7
#define  TWA5       6
#define  TWA4       5
#define  TWA3       4
#define  TWA2       3
#define  TWA1       2
#define  TWA0       1
#define  TWGCE      0

//**********TIMER_COUNTER_1************
//TIMSK1:
#define  ICIE1      5
#define  OCIE1B     2
#define  OCIE1A     1
#define  TOIE1      0
//TIFR1:
#define  ICF1       5
#define  OCF1B      2
#define  OCF1A      1
#define  TOV1       0
//TCCR1A:
#define  COM1A1     7
#define  COM1A0     6
#define  COM1B1     5
#define  COM1B0     4
#define  WGM11      1
#define  WGM10      0
//TCCR1B:
#define  ICNC1      7
#define  ICES1      6
#define  WGM13      4
#define  WGM12      3
#define  CS12       2
#define  CS11       1
#define  CS10       0
//TCCR1C:
#define  FOC1A      7
#define  FOC1B      6

//**********TIMER_COUNTER_2************
//TIMSK2:
#define  OCIE2B     2
#define  OCIE2A     1
#define  TOIE2A     0
//TIFR2:
#define  OCF2B      2
#define  OCF2A      1
#define  TOV2       0
//TCCR2A:
#define  COM2A1     7
#define  COM2A0     6
#define  COM2B1     5
#define  COM2B0     4
#define  WGM21      1
#define  WGM20      0
//TCCR2B:
#define  FOC2A      7
#define  FOC2B      6
#define  WGM22      3
#define  CS22       2
#define  CS21       1
#define  CS20       0
//ASSR:
#define  EXCLK      6
#define  AS2        5
#define  TCN2UB     4
#define  OCR2AUB    3
#define  OCR2BUB    2
#define  TCR2AUB    1
#define  TCR2BUB    0
//GTCCR:
#define  PSRASY     1

//**********AD_CONVERTER***************
//ADMUX:
#define  REFS1      7
#define  REFS0      6
#define  ADLAR      5
#define  MUX3       3
#define  MUX2       2
#define  MUX1       1
#define  MUX0       0
//ADCH:
#define  ADCH7      7
#define  ADCH6      6
#define  ADCH5      5
#define  ADCH4      4
#define  ADCH3      3
#define  ADCH2      2
#define  ADCH1      1
#define  ADCH0      0
//ADCL:
#define  ADCL7      7
#define  ADCL6      6
#define  ADCL5      5
#define  ADCL4      4
#define  ADCL3      3
#define  ADCL2      2
#define  ADCL1      1
#define  ADCL0      0
//ADCSRA:
#define  ADEN       7
#define  ADSC       6
#define  ADATE      5
#define  ADIF       4
#define  ADIE       3
#define  ADPS2      2
#define  ADPS1      1
#define  ADPS0      0
//ADCSRB:
#define  ACME       6
#define  ATDS2      2
#define  ADTS1      1
#define  ADTS0      0
//DIDR0:
#define  ADC5D      5
#define  ADC4D      4
#define  ADC3D      3
#define  ADC2D      2
#define  ADC1D      1
#define  ADC0D      0

//**********ANALOG_COMPARATOR**********
//ACSR:
#define  ACD        7
#define  ACBG       6
#define  ACO        5
#define  ACI        4
#define  ACIE       3
#define  ACIC       2
#define  ACIS1      1
#define  ACIS0      0
//DIDR1:
#define  AIN1D      1
#define  AIN0D      0

//**********PORTB**********************
//PORTB:
#define  PORTB7     7
#define  PORTB6     6
#define  PORTB5     5
#define  PORTB4     4
#define  PORTB3     3
#define  PORTB2     2
#define  PORTB1     1
#define  PORTB0     0
//DDRB:
#define  DDRB7      7
#define  DDRB6      6
#define  DDRB5      5
#define  DDRB4      4
#define  DDRB3      3
#define  DDRB2      2
#define  DDRB1      1
#define  DDRB0      0
//PINB:
#define  PINB7      7
#define  PINB6      6
#define  PINB5      5
#define  PINB4      4
#define  PINB3      3
#define  PINB2      2
#define  PINB1      1
#define  PINB0      0

//**********PORTC**********************
//PORTC:
#define  PORTC6     6
#define  PORTC5     5
#define  PORTC4     4
#define  PORTC3     3
#define  PORTC2     2
#define  PORTC1     1
#define  PORTC0     0
//DDRC:
#define  DDRC6      6
#define  DDRC5      5
#define  DDRC4      4
#define  DDRC3      3
#define  DDRC2      2
#define  DDRC1      1
#define  DDRC0      0
//PINC:
#define  PINC6      6
#define  PINC5      5
#define  PINC4      4
#define  PINC3      3
#define  PINC2      2
#define  PINC1      1
#define  PINC0      0

//**********PORTD**********************
//PORTD:
#define  PORTD7     7
#define  PORTD6     6
#define  PORTD5     5
#define  PORTD4     4
#define  PORTD3     3
#define  PORTD2     2
#define  PORTD1     1
#define  PORTD0     0
//DDRD:
#define  DDRD7      7
#define  DDRD6      6
#define  DDRD5      5
#define  DDRD4      4
#define  DDRD3      3
#define  DDRD2      2
#define  DDRD1      1
#define  DDRD0      0
//PIND:
#define  PIND7      7
#define  PIND6      6
#define  PIND5      5
#define  PIND4      4
#define  PIND3      3
#define  PIND2      2
#define  PIND1      1
#define  PIND0      0

//**********TIMER_COUNTER_0************
//OCR0B:
//OCR0A:
//TCNT0:
//TCCR0B:
#define  FOC0A      7
#define  FOC0B      6
#define  WGM02      3
#define  CS02       2
#define  CS01       1
#define  CS00       0
//TCCR0A:
#define  COM0A1     7
#define  COM0A0     6
#define  COM0B1     5
#define  COM0B0     4
#define  WGM01      1
#define  WGM00      0
//TIMSK0:
#define  OCIE0B     2
#define  OCIE0A     1
#define  TOIE0      0
//TIFR0:
#define  OCF0B      2
#define  OCF0A      1
#define  TOV0       0
//GTCCR:
#define  TSM        7
#define  PSRSYNC    0

//**********EXTERNAL_INTERRUPT*********
//EICRA:
#define  ISC11      3
#define  ISC10      2
#define  ISC01      1
#define  ISC00      0
//EIMSK:
#define  INT1       1
#define  INT0       0
//EIFR:
#define  INTF1      1
#define  INTF0      0
//PCMSK2:
#define  PCINT23    7
#define  PCINT22    6
#define  PCINT21    5
#define  PCINT20    4
#define  PCINT19    3
#define  PCINT18    2
#define  PCINT17    1
#define  PCINT16    0
//PCMSK1:
#define  PCINT14    6
#define  PCINT13    5
#define  PCINT12    4
#define  PCINT11    3
#define  PCINT10    2
#define  PCINT9     1
#define  PCINT8     0
//PCMSK0:
#define  PCINT7     7
#define  PCINT6     6
#define  PCINT5     5
#define  PCINT4     4
#define  PCINT3     3
#define  PCINT2     2
#define  PCINT1     1
#define  PCINT0     0
//PCIFR:
#define  PCIF2      2
#define  PCIF1      1
#define  PCIF0      0

//**********SPI************************
//SPSR:
#define  SPIF       7
#define  WCOL       6
#define  SPI2X      0
//SPCR:
#define  SPIE       7
#define  SPE        6
#define  DORD       5
#define  MSTR       4
#define  CPOL       3
#define  CPHA       2
#define  SPR1       1
#define  SPR0       0

//**********CPU************************
//PCICR:
#define  PCIE2      2
#define  PCIE1      1
#define  PCIE0      0
//PRR:
#define  PRTWI      7
#define  PRTIM2     6
#define  PRTIM0     5
#define  PRTIM1     3
#define  PRSPI      2
#define  PRUSART0   1
#define  PRADC      0

//CLKPR:
#define  CLKPCE     7
#define  CLKPS3     3
#define  CLKPS2     2
#define  CLKPS1     1
#define  CLKPS0     0
//SREG:
//SPH:
#define  SP9        1
#define  SP8        0
//SPL:
#define  SP7        7
#define  SP6        6
#define  SP5        5
#define  SP4        4
#define  SP3        3
#define  SP2        2
#define  SP1        1
#define  SP0        0
//SPMCSR:
#define  SPMIE      7
#define  RWWSB      6
#define  RWWSRE     4
#define  BLBSET     3
#define  PGWRT      2
#define  PGERS      1
#define  SELFPRGEN  0
//MCUCR:
#define  PUD        4
#define  IVSEL      1
#define  IVCE       0
//MCUSR:
#define  WDRF       3
#define  BORF       2
#define  EXTREF     1
#define  PORF       0
//SMCR:
#define  SM2        3
#define  SM1        2
#define  SM0        1
#define  SE         0
//MONDR:
#define  MONDR7     7
#define  MONDR6     6
#define  MONDR5     5
#define  MONDR4     4
#define  MONDR3     3
#define  MONDR2     2
#define  MONDR1     1
#define  MONDR0     0
//GPIOR2:
#define  GPIOR27    7
#define  GPIOR26    6
#define  GPIOR25    5
#define  GPIOR24    4
#define  GPIOR23    3
#define  GPIOR22    2
#define  GPIOR21    1
#define  GPIOR20    0
//GPIOR1:
#define  GPIOR17    7
#define  GPIOR16    6
#define  GPIOR15    5
#define  GPIOR14    4
#define  GPIOR13    3
#define  GPIOR12    2
#define  GPIOR11    1
#define  GPIOR10    0
//GPIOR0:
#define  GPIOR07    7
#define  GPIOR06    6
#define  GPIOR05    5
#define  GPIOR04    4
#define  GPIOR03    3
#define  GPIOR02    2
#define  GPIOR01    1
#define  GPIOR00    0

//**********WATCHDOG*******************
//WDTCSR:
#define  WDIF       7
#define  WDIE       6
#define  WDP3       5
#define  WDCE       4
#define  WDE        3
#define  WDP2       2
#define  WDP1       1
#define  WDP0       0

//**********EEPROM*********************
//EEARH:
#define  EEAR8      0
//EEARL:
#define  EEAR7      7
#define  EEAR6      6
#define  EEAR5      5
#define  EEAR4      4
#define  EEAR3      3
#define  EEAR2      2
#define  EEAR1      1
#define  EEAR0      0
//EEDR:
#define  EEDR7      7
#define  EEDR6      6
#define  EEDR5      5
#define  EEDR4      4
#define  EEDR3      3
#define  EEDR2      2
#define  EEDR1      1
#define  EEDR0      0
//EECR:
#define  EEPM1      5
#define  EEPM0      4
#define  EERIE      3
#define  EEMPE      2
#define  EEPE       1
#define  EERE       0
