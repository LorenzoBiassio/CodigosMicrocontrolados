#include <Keypad.h>
#include <LiquidCrystal.h>
#include <sevenSegmentDisplay.h>


/*------------Global declaration for LCD--------------*/
const int rs = PM_0, rw = PM_1, en = PM_2, d0 = PK_0, d1 = PK_1, d2 = PK_2,
          d3 = PK_3, d4 = PK_4, d5 = PK_5, d6 = PK_6,
          d7 = PK_7; // pin declaration for LCD
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);


/*----------Global declaration for keypad driving----------*/
const int L0 = PL_0, L1 = PL_1, L2 = PL_2, L3 = PL_3, C0 = PM_4, C1 = PM_5, C2 = PM_6, C3 = PM_7; // pin declaration for keypad
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


/*----------Global declaration for LED------------*/
int leds[] = {PA_7, PA_6, PA_5, PA_4, PQ_3, PQ_2, PQ_1, PQ_0};


/*----------Global declaration for Stepper Motor------------*/
int stepperClockwise1[] = {PH_0, PH_1, PH_2, PH_3};
int stepperCounterClockwise1[] = {PH_3, PH_2, PH_1, PH_0};
int stepperClockwise2[] = {PE_0, PE_1, PE_2, PE_3};
int stepperCounterClockwise2[] = {PE_3, PE_2, PE_1, PE_0};
int stepsPerRev = 512;
int stepDelay = 2;
float halfStepTime = (8 * stepDelay);
float fullStepTime = (4 * stepDelay);
float angularVelRPM = 0;
int stepsCounter = 0;
int rotationCounter = 0;


/*----------Global declaration for 7 segment display------------*/
const int segPinA = PQ_0, segPinB = PQ_1, segPinC = PQ_2, segPinD = PQ_3, segPinE = PA_4, segPinF = PA_5, segPinG = PA_6, segPinDP = PA_7; // segment pin declaration
const int digPin1 = PB_4, digPin2 = PB_5; // pin declaration for digits
const int digCount = 2;

sevenSegmentDisplay sevSeg1(COMMON_CATHODE, segPinA, segPinB, segPinC, segPinD, segPinE, segPinF, segPinG, segPinDP);


/*----------Global variables------------*/
int input = 0;
char* stepperMode = "halfStep";
bool stepperIsClockwise = true;
bool isRotating = false;
int numberOfQuarterRotations = 0;
int numberOfHalfRotations = 0;


void LCDDisplayMsg(char *firstLine = "", float secondLine = 0.0, int delayTime = 100) {
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

  // setup for led
  for(int i = 0; i < 8; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }


  // setup for 7 seg library
  pinMode(digPin1, OUTPUT);
  pinMode(digPin2, OUTPUT);

  digitalWrite(digPin1, HIGH);
  digitalWrite(digPin2, LOW);


  // setup stepper motor
  for (int i = 0; i < 4; i++) {
    pinMode(stepperClockwise1[i], OUTPUT);
    pinMode(stepperCounterClockwise1[i], OUTPUT);
    pinMode(stepperClockwise2[i], OUTPUT);
    pinMode(stepperCounterClockwise2[i], OUTPUT);
  }
}


void setStepperCoilsMode(int stepperCoils[], int coilsMode[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(stepperCoils[i], coilsMode[i]);
  }
}


void fullStep(bool isClockwise) {
  int coilState[] = {0, 0, 0, 0};

  if(isClockwise) {
    coilState[0] = 1;
    coilState[1] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);
    
    coilState[0] = 0;
    coilState[2] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);
    
    coilState[1] = 0;
    coilState[3] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);
    
    coilState[2] = 0;
    coilState[0] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);
    
  } else if(!isClockwise) {
    coilState[0] = 1;
    coilState[1] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);
    
    coilState[0] = 0;
    coilState[2] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);
    
    coilState[1] = 0;
    coilState[3] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);
    
    coilState[2] = 0;
    coilState[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);
  }
}


void halfStep(bool isClockwise) {
  int coilState[] = {0, 0, 0, 0};

  if (isClockwise) {
    coilState[0] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[1] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[0] = 0;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[2] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[1] = 0;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[3] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[2] = 0;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);

    coilState[0] = 1;
    setStepperCoilsMode(stepperClockwise1, coilState);
    setStepperCoilsMode(stepperClockwise2, coilState);
    delay(stepDelay);
  } else if (!isClockwise) {
    coilState[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[1] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[0] = 0;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[2] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[1] = 0;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[3] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[2] = 0;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);

    coilState[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise1, coilState);
    setStepperCoilsMode(stepperCounterClockwise2, coilState);
    delay(stepDelay);
  }
}


void rotateStepperD(char* mode, bool isClockwise, int degrees) {
  int steps = (degrees * 512) / 360;

  if(mode == "halfStep") {
    for (int i = 0; i < steps; i++) {
      digitalWrite(leds[0], HIGH);
      halfStep(isClockwise);
      digitalWrite(leds[0], HIGH);
    }
  } else if(mode == "fullStep") {
    for (int i = 0; i < steps; i++) {
      digitalWrite(leds[0], HIGH);
      fullStep(isClockwise);
      digitalWrite(leds[0], HIGH);
    }
  }
}


void checkStepperVariables(int input) {

  if(input == 32) {
    isRotating = !isRotating;
  }

  else if (input == 87 || input == 119) {
    // W: rise angular velocity
    if(stepDelay >= 7) {
      stepDelay-=5;
    }
  }

  else if (input == 83 || input == 115) {
      stepDelay+=5;
  }

  else if (input == 65 || input == 97) {
    // A: spin clockwise
    stepperIsClockwise = true;
  }

  else if (input == 68 || input == 100) {
    // D: spin counter-clockwise
    stepperIsClockwise = false;
  }

  else if (input == 67 || input == 99) {
    // C: full-step mode
    stepperMode = "fullStep";
  } 
  
  else if (input == 77 || input == 109) {
    // M: half-step mode
    stepperMode = "halfStep";
  }

  halfStepTime = (8 * stepDelay);
  angularVelRPM = 60/((stepsPerRev * halfStepTime)/1000);

  if(stepperMode == "fullStep") {
    fullStepTime = (4 * stepDelay);
    angularVelRPM = 60/((stepsPerRev * fullStepTime)/1000);
  }


  if(rotationCounter >= 9){
    rotationCounter = 0;
  }

  sevSeg1.set(rotationCounter);
  

  if(stepperIsClockwise){
    if(stepperMode == "fullStep") {
      LCDDisplayMsg("HO  P. Completo", angularVelRPM, 0);
    } else {
      LCDDisplayMsg("HO   Meio Passo", angularVelRPM, 0);
    }
  } else {
    if(stepperMode == "fullStep") {
      LCDDisplayMsg("AH  P. Completo", angularVelRPM, 0);
    } else {
      LCDDisplayMsg("AH   Meio Passo", angularVelRPM, 0);
    }
  }

}


void loop() {
  input = Serial.read();

  while(isRotating) {
    
    checkStepperVariables(input);

    // Rotation Command
    if(stepperMode == "halfStep") {
      digitalWrite(leds[0], HIGH);
      halfStep(stepperIsClockwise);
      digitalWrite(leds[0], LOW);
    } 
    
    else if(stepperMode == "fullStep") {
      digitalWrite(leds[0], HIGH);
      fullStep(stepperIsClockwise);
      digitalWrite(leds[0], LOW);
    }

    stepsCounter++;

    if( (stepsCounter % stepsPerRev) == 0 ) {
      stepsCounter = 0;
      rotationCounter++;      
    }

    input = Serial.read();

  }

  checkStepperVariables(input);

  if(input == 81 || input == 113) {
    // Q: rotates 90 degrees
    if(!isRotating) {
      rotateStepperD(stepperMode, stepperIsClockwise, 90);
      numberOfQuarterRotations++;
    }

    if( (numberOfQuarterRotations % 4) == 0 ) {
      numberOfQuarterRotations = 0;
      rotationCounter++;      
    }
  } 
  
  else if (input == 69 || input == 101) {
    // E: rotates 180 degrees
    if(!isRotating) {
      rotateStepperD(stepperMode, stepperIsClockwise, 180);
      numberOfHalfRotations++;
    }

    if( (numberOfHalfRotations % 2) == 0 ) {
      numberOfHalfRotations = 0;
      rotationCounter++;      
    }
  } 
}