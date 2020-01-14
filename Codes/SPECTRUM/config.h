//changes in configuration is made here

#ifndef CONFIG_H
#define CONFIH_H

/******************************************TUNER_CONFIGURATIONS***************************************************/
#define tunerAddress  0x60
#define controlByte   0b11001010  //  0b11001010---1024  //  0b11001110---512  // 0b11001100---640
#define addressByte   ob11000010
#define DB1           0x00 
#define DB2           0x00
#define VHF_LO        0x04
#define VHF_HI        0x02
#define UFH           0x01
#define bandSelectByte  VHF_LO
/******************************************************************************************************************/


/******************************************ARDUINO_CONFIGURATIONS***************************************************/
#define fSelectPin 4      //pwm to vary tuning voltage 
#define signalPin A5     //pin to read signal from
/*******************************************************************************************************************/

#endif
