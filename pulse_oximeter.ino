/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
float HR;
float W;
float sp;
float spsum;
float hrsum;
float calories;
int counter=0;
float T;
int A;
int sex;
String wms = "please enter your weight";
String a = "please enter your age";
String s = "please enter your sex: 1 for male and 0 for female";
String t = "please enter the duration in hours";
// Callback (registered below) fired when a pulse is detected
String test="";



// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
    
}

void setup()
{
    Serial.begin(115200);

    Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        HR=pox.getHeartRate();
        Serial.print(HR);
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");


        if(HR>0){
        Serial.println(wms);
  while (Serial.available() == 0) {
  }

  test=Serial.readString();
  W = test.toFloat();
  Serial.println(W);

  Serial.println(a);
  while (Serial.available() == 0) {
  }
   test=Serial.readString();
  A = test.toInt();
  Serial.println(A);

 

  Serial.println(t);
  while (Serial.available() == 0) {
  }
  test=Serial.readString();
  T = test.toFloat();
  Serial.println(T);
  
 Serial.println(s);
  while (Serial.available() == 0) {
  }
   test=Serial.readString();
  sex = test.toInt();

    Serial.println(sex);


    if (sex == 1 ) {

      calories = (float)((-55.0969 + (0.6309 * HR) + (0.1988 * W) + (0.2017 * A)) / (4.184 * 60 * T))*1000;
      Serial.print("Heart rate: ");
      Serial.println(HR);
      Serial.print("bpm / SpO2: ");
      Serial.println(sp);
      Serial.print("Calories=");
      Serial.println(calories);
      delay(3000);
    }

    if (sex == 0) {
      calories =(float) ((-20.4022 + (0.4472 * HR) - (0.1263 * W) + (0.1263 * A)) / (4.184 * 60 * T))*1000;
      Serial.print("Heart rate: ");
      Serial.print(HR);
      Serial.print("bpm / SpO2: ");
      Serial.print(sp);
      Serial.print("Calories");
      Serial.print(calories);
      delay(3000);
      
    }
        }

        tsLastReport = millis();
    }


    
}
