#include <Arduino.h>
#include "rg15arduino.h"

RG15Arduino rg15;

void setup() {
    Serial.begin(9600);
    Serial.println("setup");
    Serial1.begin(9600);
    rg15.setStream(&Serial1);
}

void loop() {
      if(rg15.poll()) {
          Serial.print("Parsed:");
          Serial.print(rg15.acc,3);
          Serial.write(',');
          Serial.print(rg15.unit);
          Serial.write(',');
          Serial.print(rg15.eventAcc,3);
          Serial.write(',');
          Serial.print(rg15.totalAcc,3);
          Serial.write(',');
          Serial.println(rg15.rInt,3);
    } else {
       Serial.println("Timeout!");
    }
    delay(1000);
}
