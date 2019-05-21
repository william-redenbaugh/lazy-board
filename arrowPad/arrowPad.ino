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

int ledMode = 0;
//0 = Fade
//1 = Light pressed
//2 = Light all except pressed
void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  initalize(); //sets up pins
  Tlc.init(0);
}

void loop() {
  readBoard(); //reads and stores which switches are released and pressed 
  pressKeys(); //sends keypress to device
  checkLightMode();
  runLighting();
  //printState(); //for debugging
  delay(2); //to not over load the arduino
}

void runLighting(){
  switch(ledMode){
    case 0: lights();
            break;
    case 1: lightReact(0);
            break;
    case 2: lightReact(1);
            break;
  }
}

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

//vars for lights()
int current = 0;
int increment = 50;
int inc = increment;

void lights(){
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

//if pres = 1 Lights all but pressed
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

void setAll(int x){
  for (int i = 0; i < 16; i++) {
    Tlc.set(i, x);
  }
}

void setCol(int c, int val){
  //for(int i = 0;)
}
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
  
