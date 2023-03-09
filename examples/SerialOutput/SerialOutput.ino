#include <Arduino.h>
#include <SoftwareSerial.h>
#include "rg15arduino.h"

SoftwareSerial mySerial(2, 3); // RX, TX

RG15Arduino rg15;

void setup() {
    Serial.begin(9600);
    Serial.println("setup"); 
    mySerial.begin(9600);
    rg15.setStream(&mySerial);
}

void loop() {
    if(rg15.poll()) {
        Serial.print("Accumulation: ");
        Serial.print(rg15.acc,3);
        Serial.print(rg15.unit);
        Serial.write(", Event Accumulation: ");
        Serial.print(rg15.eventAcc,3);
        Serial.print(rg15.unit);
        Serial.write(", Total Accumulation: ");
        Serial.print(rg15.totalAcc,3);          
        Serial.print(rg15.unit);
        Serial.write(", IPH: ");
        Serial.println(rg15.rInt,3);
    } else {
       Serial.println("Timeout!");
    }
    delay(1000);
}
