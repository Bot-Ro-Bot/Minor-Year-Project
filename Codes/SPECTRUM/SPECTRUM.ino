/*****************SPECTRUM_ANALYSER_FOR_LPDA_ANTENNA(30MHz-150MHz)************************/

//software interrupt lagaune ani harek selected frequency lai read gardai euta thau maa store garne



#include "config.h"
#include "Tuner.h"
#include <Wire.h>
#include <TimerOne.h>

float spectrum[150]; //signal ko amplitude store garna ko laagi
Tuner tuner;

void setup() {
  pinMode(fSelectPin,OUTPUT);
  pinMode(signalPin,INPUT);
  Serial.begin(115200);
}

void loop() {
  int i = 0;
  while (i<150) {
    spectrum[i]=analogRead(signalPin);
    analogWrite(fSelectPin,i);          //logic to change output frequency
    i++;
  }
  //plot i vs spectrum[i]
}
