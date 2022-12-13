

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>


/*------------Global declaration for LCD--------------*/
const int rs = PM_0, rw = PM_1, en = PM_2, d0 = PK_0, d1 = PK_1, d2 = PK_2,
          d3 = PK_3, d4 = PK_4, d5 = PK_5, d6 = PK_6,
          d7 = PK_7; // pin declaration for LCD
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);


/*----------Global declaration for keypad driving--------------------*/
const int L0 = PL_0, L1 = PL_1, L2 = PL_2, L3 = PL_3, C0 = PM_4, C1 = PM_5,
          C2 = PM_6, C3 = PM_7; // pin declaration for keypad
const byte ROWS = 4;            // four rows
const byte COLS = 4;            // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {L0, L1, L2,
                      L3}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {C0, C1, C2,
                      C3}; // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


/*----------Global declaration for buzzer----------------*/
const int buzPin = PL_4;


/*----------Global declaration for DC Motor----------------*/
const int EN1 = PF_3;
const int IN1 = PE_2;
const int IN2 = PE_3;


/*----------Global declaration for ServoMotors----------------*/
const int servoPin1 = PL_4;
const int servoPin2 = PD_4;
const int servoPin3 = PE_5;
Servo servo1;  // create servo1 object to control a servo motor
// Servo 1 max angles: 0 < theta < 120; ---> horizontal
Servo servo2;  // create servo2 object to control a servo motor
// Servo 2 max angles: 140 < theta < 180; ----> Hand
Servo servo3;  // create servo3 object to control a servo motor
// Servo 3 max angles: 0 < theta < 45; ----> Vertical


/*----------Global declaration for Encoder----------------*/
const int encoderPin = PB_4;


/*----------Global declaration for Arm Position----------------*/
int horizontal = 0;
int vertical = 0;
int rotation = 0;
bool isRotationClockwise = true;
bool isHandOpen = true; // true = open; false = closed


void LCDDisplayMsg(char *firstLine = "", char *secondLine = "", int delayTime = 10) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
  delay(delayTime);
}


void setup() {
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.display();
  LCDDisplayMsg("H0 V0 R0", "Garra Fechada", 100);

  // matrix Keypad input declaration
  for (int i = 0; i <= 3; i++) {
    pinMode(rowPins[i], INPUT);
    pinMode(colPins[i], INPUT);
  }

  // setup buzzer pin
  pinMode(buzPin, OUTPUT);

  // setup dc motor pins
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Setup servor motor pins
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

}


void openHand(bool handState) {
  if(handState == true) {
    servo2.write(180);
  }
  else {
    servo2.write(140);
  }
}


void rotateArmZAxis(int degrees, bool isClockwise) {

  analogWrite(EN1, 255);

  if(isClockwise) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  int currentPosition = (analogRead(encoderPin) * 360) / (4096-275);

  while(abs(currentPosition - degrees) >= 8 ) {

    currentPosition = (analogRead(encoderPin) * 360) / (4096-275);
    delay(1);
  }

  analogWrite(EN1, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

}


void translateHorizontal(int heigth) {
  servo1.write((heigth*70)/100);
  // The angle can vary from 0 to 45 (0 < theta < 120)
}


void translateVertical(int percentage) {
  // servo3.write(abs(((percentage*45) / 100) - 45));
  servo3.write((percentage*45) / 100);
  // The angle can vary from 0 to 45 (0 < theta < 45)

}


int getPositionFromString(String userInput, char axis) {
  int startIndex = userInput.indexOf(axis);


  if (startIndex == -1) return -1;

  // Rotation direction
  if(axis == 'R') isRotationClockwise = true;
  if(axis == 'C') isRotationClockwise = false;

  // Hand State Read
  if(axis == 'G') {
    if((int) userInput[startIndex+1] - 48 == 1) {
      return 1;
    } else if ((int) userInput[startIndex+1] - 48 == 0) {
      return 0;
    }
    return -1;
  }

  // Axis Position Read
  char firstDigit = userInput[startIndex+3];
  char secondDigit = userInput[startIndex+2];
  char thirdDigit = userInput[startIndex+1];

  // Serial.println(userInput);
  // Serial.println(firstDigit);
  // Serial.println(secondDigit);
  // Serial.println(thirdDigit);

  // Reads the three digits maximum numeric user input
  int position = 0;
  if(((int) firstDigit >= 48)  && ((int) firstDigit <= 57)) {
    position += (firstDigit - 48);
    if(((int) secondDigit >= 48)  && ((int) secondDigit <= 57)) {
      position += (secondDigit - 48) * 10;
      if(((int) thirdDigit >= 48)  && ((int) thirdDigit <= 57)) {
        position += (thirdDigit - 48) * 100;
        return position;
      }
    }
  } else if(((int) secondDigit >= 48)  && ((int) secondDigit <= 57)) {
    position += (secondDigit - 48);
    if(((int) thirdDigit >= 48)  && ((int) thirdDigit <= 57)) {
      position += (thirdDigit - 48) * 10;
      return position;
    }
  } else if(((int) thirdDigit >= 48)  && ((int) thirdDigit <= 57)) {
    position += (thirdDigit - 48);
    return position;
  }

  return -1;
}


char auxiliarCharArray[] = {};
int getCoordinates() {
  int i = 0;
  int inputChar = Serial.read();

  // Read the serial input
  while(inputChar != -1) {
    auxiliarCharArray[i] = (char) inputChar;
    inputChar = Serial.read();
    i++;
    delay(10);
  }

  String coordinates = auxiliarCharArray;
  
  // if there is a serial input get the given positions
  if(i != 0 ) {
    int horizontalPosition = getPositionFromString(coordinates, 'H'); // horizontal
    int vertictalPosition = getPositionFromString(coordinates, 'V'); // vertical
    int rotationalPositionCW = getPositionFromString(coordinates, 'R'); // rotation clockwise
    int rotationalPositionCC = getPositionFromString(coordinates, 'C'); // rotation counter-clock
    int handOpenClose = getPositionFromString(coordinates, 'G'); // open/close

    coordinates = "";
    i==0;

    // clar auxiliar array
    for(int j = 0; j < sizeof(auxiliarCharArray); j++) {
      auxiliarCharArray[j] = ' ';
    }

    if (horizontalPosition != -1) horizontal = horizontalPosition; 
    if (vertictalPosition != -1) vertical = vertictalPosition; 
    if (rotationalPositionCW != -1) rotation = rotationalPositionCW; 
    if (rotationalPositionCC != -1) rotation = rotationalPositionCC;
    if (handOpenClose != -1) isHandOpen = handOpenClose; 

    // Serial.print(vertictalPosition);
    // Serial.print(", G");
    // Serial.print((bool) isHandOpen);
    // Serial.print(", H");
    // Serial.print(horizontal);
    // Serial.print(", R");
    // Serial.println(rotation);
    // HO045 V100 RH000
    return 1;
  }

  return 0;
}


void updateArmStates() {
  rotateArmZAxis(rotation, isRotationClockwise);
  translateHorizontal(horizontal);
  translateVertical(vertical);
  openHand(isHandOpen);
}


void updateLCDMessage() {
  String message1 =  "H";
  message1 += horizontal;
  message1 += " V";
  message1 += vertical;
  message1 += " R";
  message1 += rotation;

  String message2 = "Garra Fechada";
  if(isHandOpen) message2 = "Garra Aberta";

  char firstLine[message1.length()+1];
  message1.toCharArray(firstLine, message1.length()+1);

  char secondLine[message2.length()+1];
  message2.toCharArray(secondLine, message2.length()+1);

  LCDDisplayMsg(firstLine, secondLine, 100);
}


void loop() {

  if(getCoordinates()) {

    updateArmStates();
    updateLCDMessage();

  }
  
  // delay(500);
}
