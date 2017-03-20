#include <Wire.h>
#include "Adafruit_Trellis.h"

#define MOMENTARY 0
#define LATCHING 1

Adafruit_Trellis matrix0 = Adafruit_Trellis();
// Just one
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

#define NUMTRELLIS 1
#define numKeys (NUMTRELLIS * 16)

#define INTPIN A2

int buttons[] = {1,2,3,4};
int playerButtons[] = {0,0,0,0};
int currentIndex = 0;
bool playing = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Demo");

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  trellis.begin(0x70);  // only one  
}

void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  
  if(!playing){  
    resetGame();
    setButtons();
    playSequence();
    playing = true;
    
  }else{
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
        if (trellis.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
          // Alternate the LED
          if (trellis.isLED(i))
            trellis.clrLED(i);
          else{
            trellis.setLED(i);
            trellis.writeDisplay();
            delay(150);
            trellis.clrLED(i);
            trellis.writeDisplay();
            addButtonPress(i);
          }
        } 
      }
      
    }
  }
}

void setButtons(){
  randomSeed(analogRead(0));
  for(int i = 0;i < (sizeof(buttons)/sizeof(int));i++){
    
    int num = random(16);
    if(num != 12)
      buttons[i] = num;
    else
      i = i-1;
  }
}

void checkResults(){
  bool correct = true;
  for(int x = 0;x < (sizeof(buttons)/sizeof(int)); x++){
    if(buttons[x] != playerButtons[x])
      correct = false;
  }
  if(!correct)
    displayX();
  else
    displayFull();

  playing = false;
}

void addButtonPress(int b){
  playerButtons[currentIndex] = b;
  currentIndex++;
  if(currentIndex == (sizeof(buttons)/sizeof(int)))
    checkResults();
}

void resetGame(){
  currentIndex = 0;
  displayChase();
}

void playSequence(){
  for(int i = 0;i < (sizeof(buttons)/sizeof(int));i++){
    int num = buttons[i];
    trellis.setLED(num);
    trellis.writeDisplay();
    delay(500);
    trellis.clrLED(num);
    trellis.writeDisplay();
    delay(500);
  }
}

void displayX(){
  for(int x = 0;x < 3; x++){
    delay(200);
    for(int i = 0;i < 5;i++){
      trellis.setLED(i*5);
      trellis.writeDisplay();
    }
    for(int i = 1;i < 6;i++){
      trellis.setLED(i*3);
      trellis.writeDisplay();
    }
    delay(200);
    for(int i = 0;i < 5;i++){
      trellis.clrLED(i*5);
      trellis.writeDisplay();
    }
    for(int i = 1;i < 6;i++){
      trellis.clrLED(i*3);
      trellis.writeDisplay();
    }
  }
  delay(1000);
}

void displayFull(){
  for(int x = 0;x < 3; x++){
    delay(200);
    for(int i = 0;i < 16; i++){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
    delay(200);
    for(int i = 0;i < 16; i++){
      trellis.clrLED(i);
      trellis.writeDisplay();
    }
  }
  delay(1000);
}

void displayChase(){
  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
  delay(1000);
}
