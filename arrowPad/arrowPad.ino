#include <Keyboard.h>

byte row[] = {7, 8, 10} ;//col grounds
byte col[] = {3, 5, 6} ;//input pullups
byte keysState[3][3];

//Edit this
//change the key for each switch presse
//refer to https://www.arduino.cc/en/Reference/KeyboardPress
//modifier keys - https://www.arduino.cc/en/Reference/KeyboardModifiers
char keys[3][3] =  {{'q', 'w', 'e'},
                    {'a', 's', 'd'},
                    {'z', 'x', ' '}};

byte rowSize = sizeof(row);
byte colSize = sizeof(col);

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  initalize(); //sets up pins
}

void loop() {
  readBoard(); //reads and stores which switches are released and pressed 
  pressKeys(); //sends keypress to device
  //printState(); //for debugging
  delay(2); //to not over load the arduino
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
        Serial.print("Pressed: ");
        Serial.println(keys[i][j]);
        //sends pressed value based keys array
        Keyboard.press(keys[i][j]); 
      }
      else{ //switch at current point is not pressed
        Serial.print("Released: ");
        Serial.println(keys[i][j]);
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
  
