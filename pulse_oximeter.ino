#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;
float HR;
float W;
int T;
int A;
String sex;
String wms = "please enter your weight";
String a = "please enter your age";
String s = "please enter your sex: m for male and f for female";
String t = "please enter the duration in minutes";
// Callback (registered below) fired when a pulse is detected
String test="";

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  Serial.begin(9600);

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
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
  T = test.toInt();
  Serial.println(T);
  
 Serial.println(s);
  while (Serial.available() == 0) {
  }
  sex = Serial.readString();
  Serial.println(sex);

  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    HR = pox.getHeartRate();
    Serial.print(HR);
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    Serial.println();


    if (sex == "m" && HR > 0) {

      ((-55.0969 + (0.6309 * HR) + (0.1988 * W) + (0.2017 * A)) / (4.184 * 60 * T));
    }

    if (sex == "f" && HR > 0) {
      ((-20.4022 + (0.4472 * HR) - (0.1263 * W) + (0.1263 * A)) / (4.184 * 60 * T));
    }
    tsLastReport = millis();
  }
}
