#include<Arduino.h>
#include "Tuner.h"
#include "config.h"
#include <Wire.h>

//constructer initializes and configures the device to our desired operation
Tuner::Tuner() {
  Wire.begin();
  Wire.beginTransmission(tunerAddress);
  //Wire.write();
  //Wire.write(0b00000000);                 
  Wire.endTransmission();
}



void Tuner::checkStatus() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}
