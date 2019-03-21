#include <Keyboard.h>

byte row[] = {7, 8, 10} ;//col grounds //re arrange
byte col[] = {3, 5, 6} ;//input pullups ///re arrange
byte keysState[3][3];
//Edit this
char keys[3][3] =  {{'q', 'w', 'e'},
                    {'a', 's', 'd'},
                    {'z', 'x', ' '}};

byte rowSize = sizeof(row);
byte colSize = sizeof(col);

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  initalize();
}

void loop() {
  readBoard();
  //printState();
  pressKeys();
  delay(2);
}
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

void readBoard(){
  for(int i = 0; i < rowSize; i++){
    pinMode(row[i], OUTPUT);
    digitalWrite(row[i], LOW);
    for(int j = 0; j < colSize; j++){
      pinMode(col[j], INPUT_PULLUP);
      keysState[j][i] = digitalRead(col[j]);
      pinMode(col[j], INPUT);
    }
    pinMode(row[i], INPUT);
  }
}

void pressKeys(){
  for(int i = rowSize - 1; i >= 0; i--){
    for(int j = colSize - 1; j >= 0; j--){
      if(keysState[j][i] == 0){
        Serial.print("Pressed: ");
        Serial.println(keys[i][j]);
        Keyboard.press(keys[i][j]);
      }
      else{
        Serial.print("Released: ");
        Serial.println(keys[i][j]);
        Keyboard.release(keys[i][j]);
      }
    }
  }
}

void  initalize(){
  for(int i = 0; i < rowSize+ 1; i++){
    pinMode(row[i], INPUT);
  }
  for(int i = 0; i < colSize; i++){
    pinMode(col[i], INPUT_PULLUP);
  }
}
  
