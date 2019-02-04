#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

//ULN2003 driver wiring
//IN1 -> PD5 = 5
//IN2 -> PD4 = 4
//IN3 -> PD3 = 3
//IN4 -> PD2 = 2

//Buttons wiring
//A0 - run
//A1 - reverse run
//A2 - increase angle
//A3 - decrease angle

#define DELAY 100

short int PORTDcodes[] = {12, 8, 24, 16, 48, 32, 36, 4}; 

int angleToStep(int a) {
  return int(64.0/45.0*double(a));
}

void _step(short int l, short int r, int dir) {
  for (short int i = l; i != r; i += dir) {
    PORTD = *(PORTDcodes + i);
    delayMicroseconds(DELAY);
  }
}

void doSteps(int number = 1, short int dir = 1) {
  short int l = (dir == 1? 0 : 7);
  short int r = (dir == 1? 8 : -1);
  for (int i = 0; i < number; ++i) {
    _step(l, r, dir);
  }
  PORTD = 0;
}

LiquidCrystal_PCF8574 lcd(0x3F); //if it doesn't work try 0x20 or 0x27

int v = 180;
int M = 360;
int m = 0;

void setup() {
  DDRD |= B00111100;
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  lcd.begin(16,2);                       
  lcd.setBacklight(64);                 
  lcd.setCursor(0,0);              
  lcd.print("Hello, World!");  
  delay(100);
  lcd.clear();
  lcd.print(v);
}

void loop() {
  if (digitalRead(A0)==HIGH) {
    lcd.clear();
    lcd.print("RUN->");
    lcd.print(v);
    doSteps(angleToStep(v), -1);
    lcd.clear();
    lcd.print(v);
  }
  if (digitalRead(A1)==HIGH) {
    lcd.clear();
    lcd.print("RUN<-");
    lcd.print(v);
    doSteps(angleToStep(v), 1);
    lcd.clear();
    lcd.print(v);
  }
  if (digitalRead(A2)==HIGH) {
    v = (v + 5 > M ? M : v + 5);
    lcd.clear();
    lcd.print(v);
  }
  if (digitalRead(A3)==HIGH) {
    v = (v - 5 < m ? m : v - 5);
    lcd.clear();
    lcd.print(v);
  }
  delayMicroseconds(7000);
}
