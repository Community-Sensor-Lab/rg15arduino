#include <Arduino.h>
#include "rg15arduino.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

uint8_t drop[8]  = {0x4, 0x4, 0xe, 0xe, 0x1f, 0x1f, 0x1f, 0xe};

//sun symbol binary data for cosmetic purposes.
uint8_t sun0[8] = {0x4,0x2,0x1,0x0,0x0,0x3,0x0,0x0};
uint8_t sun1[8] = {0x4,0x4,0x0,0xe,0x1f,0x1f,0x1f,0xe};
uint8_t sun2[8] = {0x0,0x8,0x10,0x0,0x0,0x1c,0x0,0x0};

uint8_t sun3[8] = {0x2,0x4,0x0,0x0,0x0,0x0,0x0,0x0} ;
uint8_t sun4[8] = {0x4,0x4,0x8,0x0,0x0,0x0,0x0,0x0};
uint8_t sun5[8] = {0x18,0x04,0x0,0x0,0x0,0x0,0x0,0x0};



RG15Arduino rg15;
bool lastCycleSunny;
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
  //make drop icon.
  lcd.createChar(0, drop);
  //make sun icon.
  lcd.createChar(1, sun0);
  lcd.createChar(2, sun1);
  lcd.createChar(3, sun2);
  
  lcd.createChar(4, sun3);
  lcd.createChar(5, sun4);
  lcd.createChar(6, sun5);
  
  lcd.home();
  //end sun icon

  
  mySerial.begin(9600);
  rg15.setStream(&mySerial);
}

void loop() {
  if (rg15.poll()) {
    if(rg15.eventAcc > 0) {
      lcd.clear();
      display("Event" , rg15.eventAcc, true);
      //Accumulation since last pole display.
      //Only shown if it is greater then 0.
      if(rg15.acc > 0) {
          lcd.setCursor(12,1);
          lcd.print("+");
          lcd.print(rg15.acc * 1000, 0); 
          lcd.print("mI");
      }

      //IPH display
      lcd.setCursor(8,0);
      lcd.print(rg15.rInt,3);
      lcd.print("IPH");

      delay(5000); //wait a bit before changing screens.
      
    }
    
    
    if(rg15.eventAcc > 0 or !lastCycleSunny) { // Total accumulation screen.
      //if the last screen frame shown was a sunny frame and it is still sunny,
      //we should not refresh the screen.
      lcd.clear();
      display("Total" , rg15.totalAcc, true);
      lastCycleSunny = !(rg15.eventAcc > 0);
      (lastCycleSunny) ? displaySun() : displayRain();
    }
    delay(5000);
   
    
   
  } else {
    lcd.print("Connecting...");
    delay(5000);
    lcd.clear();
  }
}

void display(String name, float value, boolean unit) {
  lcd.setCursor(0,0);
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print(value, 3);
  if (unit) {
    lcd.print(rg15.unit[0]);
    lcd.print(rg15.unit[1]);
  }
}

void displayRain() {
  lcd.setCursor(11, 0);
  lcd.write(0);
  lcd.setCursor(9, 1);
  lcd.write(0);
  lcd.setCursor(0,0);
}

void displaySun() {
  lcd.setCursor(10,0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.setCursor(10,1);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
  lcd.setCursor(0,0);
}
