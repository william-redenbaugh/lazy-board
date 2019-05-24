#include <Keyboard.h>
#include "Tlc5940.h"

byte row[] = {7, 8, 0, 20} ;//col grounds
byte col[] = {4, 1, 6, 21} ;//input pullups
byte rowSize = sizeof(row);
byte colSize = sizeof(col);
byte keysState[4][4];

//Edit this
//change the key for each switch presse
//refer to https://www.arduino.cc/en/Reference/KeyboardPress
//modifier keys - https://www.arduino.cc/en/Reference/KeyboardModifiers
char keys[4][4] =  {{'q', 'w', 'e', 'r'},
                    {'a','s','d','f'},
                    {'z', 'x', 'c', 'v'},
                    {NULL, NULL, NULL, ' '}};

//ledMap[1] refers to the top left. ledMap[2] refers to the next one
int ledMap[] = {7,6,4,5,11,10,9,8,15,14,13,12,0,1,2,3};


void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  initalize(); //sets up pins
  Tlc.init(0);
}

void loop() {
  //Serial.println(millis());
  readBoard(); //reads and stores which switches are released and pressed 
  pressKeys(); //sends keypress to device
  checkLightMode();
  runLighting();
  //printState(); //for debugging
  delay(2); //to not over load the arduino
}

//KEYBOARD ROUTINES

//reads the keyboard matrix
void readBoard(){
  for(int i = 0; i < rowSize; i++){ //iterate over each row
    pinMode(row[i], OUTPUT); 
    digitalWrite(row[i], LOW); //allows current to flow through row
    for(int j = 0; j < colSize; j++){ //interate over each switch in the row
      pinMode(col[j], INPUT_PULLUP); //open the column pin up for reading
      keysState[j][i] = digitalRead(col[j]); //checks the state of the switch in the row
      //and stores it in keysState
      pinMode(col[j], INPUT);//closes the switch for reading
    } //runs until it has checked all switches in the row
    //finishes checking the row
    pinMode(row[i], INPUT); //stop current from flowing through the row
    //move to the next row
  }
}

//sends key presses
void pressKeys(){
  //interate over the keyState array
  for(int i = rowSize - 1; i >= 0; i--){
    for(int j = colSize - 1; j >= 0; j--){
      //switch at current point is pressed
      if(keysState[j][i] == 0){
        //Serial.print("Pressed: ");
        //Serial.println(keys[i][j]);
        //sends pressed value based keys array
        Keyboard.press(keys[i][j]); 
      }
      
      else{ //switch at current point is not pressed
        //Serial.print("Released: ");
        //Serial.println(keys[i][j]);
        //sends release value based on keys array
        Keyboard.release(keys[i][j]);
      }
    }
  }
}

void  initalize(){
  //resets all pins
  for(int i = 0; i < rowSize+ 1; i++){
    pinMode(row[i], INPUT);
  }
  for(int i = 0; i < colSize; i++){
    pinMode(col[i], INPUT_PULLUP);
  }
}

//prints the keysState array
void printState(){
  for(int i = 0; i < rowSize; i++){
    Serial.print(i);
    Serial.print("   ");
    for(int j = 0; j < colSize; j++){
      Serial.print(keysState[j][i]);
    }
    Serial.println();
  }
}

//LIGHTING ROUTINES

int ledMode = 0;
//0 = Fade
//1 = Light pressed
//2 = Light all except pressed

//Detects key presses to change light mode
void checkLightMode(){
  if(keysState[0][3] == 0){
    ledMode = 0;
  }
  else if(keysState[1][3] == 0){
    ledMode = 1;
  }

  else if(keysState[2][3] == 0){
    ledMode = 2;
  }
}

//Chooses lighting pattern
void runLighting(){
  switch(ledMode){
    case 0: lightsAllFade();
            break;
    case 1: lightReact(0);
            break;
    case 2: lightsColFade();
            break;
  }
}

//SETS Column c to specific value
void setCol(int c, int val){
  for(int i = c; i <= 16; i+=4){
    Tlc.set(ledMap[i], val);
  }
}
//Sets all LEDS to specific value
void setAll(int x){
  for (int i = 0; i < 16; i++) {
    Tlc.set(i, x);
  }
}

//LIGHTS: FADES COLUMNS IN WAVE PATTERN
int current1 = 500; //edit the currenti values for offsets
int current2 = 1000;
int current3 = 2000;
int current4 = 3000;
int increment5 = 100; //edit to slow or speed wave

int inc1 = 1;
int inc2 = 1;
int inc3 = 1;
int inc4 = 1;

void lightsColFade(){
  if(current1<=0) inc1 = 1;
  else if(current2<=0) inc2 = 1;
  else if(current3<=0) inc3 = 1;
  else if(current4<=0) inc4 = 1;
  else if(current1 >=4000-increment5) inc1 = -1;
  else if(current2 >=4000-increment5) inc2 = -1;
  else if(current3 >=4000-increment5) inc3 = -1;
  else if(current4 >=4000-increment5) inc4 = -1;
  current1+= inc1*increment5;
  current2+= inc2*increment5;
  current3+= inc3*increment5;
  current4+= inc4*increment5;
  setCol(0, current1);
  setCol(1, current2);
  setCol(2, current3);
  setCol(3, current4);
  Tlc.update();
}

//LIGHTS: Fade whole board in and out

int current = 0;
int increment = 25; //edit to slow or speed the wave
int inc = increment;

void lightsAllFade(){
  current += inc;
  if(current <= 0){
    inc = increment;
  }
  else if(current >= 4000-increment){
    inc = inc * -1;
  }
  setAll(current);
  Serial.println(current);
  Tlc.update();
}

//LIGHTS: REACTS TO if KEY IS PRESSED

//pres = 1 Lights all but pressed key
//pres = 0 Lights only pressed key
void lightReact(int pres){
  int lightIndex = 0;
  for (int i = 0; i < rowSize; i++){
    for(int j = 0; j < colSize; j++){
      if(keysState[j][i] == pres){
        
        Tlc.set(ledMap[lightIndex], 4000);
      }
      else{
        Tlc.set(ledMap[lightIndex], 0);
      }
      lightIndex++;
    }
  }
  Tlc.update();
}

