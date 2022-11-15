#include <Keypad.h>
#include <LiquidCrystal.h>
#include <sevenSegmentDisplay.h>


/*------------Global declaration for LCD--------------*/
const int rs = PM_0, rw = PM_1, en = PM_2, d0 = PK_0, d1 = PK_1, d2 = PK_2,
          d3 = PK_3, d4 = PK_4, d5 = PK_5, d6 = PK_6,
          d7 = PK_7; // pin declaration for LCD
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);


/*----------Global declaration for 7 segment display--------------------*/
const int segPinA = PQ_0, segPinB = PQ_1, segPinC = PQ_2, segPinD = PQ_3,
          segPinE = PA_4, segPinF = PA_5, segPinG = PA_6,
          segPinDP = PA_7;                // segment pin declaration
const int digPin1 = PB_4, digPin2 = PB_5; // pin declaration for digits
const int digCount = 2;

sevenSegmentDisplay sevSeg(COMMON_CATHODE, segPinA, segPinB, segPinC, segPinD,
                           segPinE, segPinF, segPinG, segPinDP);


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


/*----------Global declaration for Stepper Motor------------*/
int stepperClockwise[] = {PH_0, PH_1, PH_2, PH_3};
int stepperCounterClockwise[] = {PH_3, PH_2, PH_1, PH_0};
int stepsPerRev = 512;
int stepDelay = 2;
float halfStepTime = (8 * stepDelay);
float fullStepTime = (4 * stepDelay);
int stepsCounter = 0;
int rotationCounter = 0;


unsigned long previousMillis = 0;


/*----------Global declaration for buzzer----------------*/
const int buzPin = PL_4;


/*----------Global declaration for Gas Sensor----------------*/
const int gasSensorPin = PD_4;
int isGasDetectionOn = true;


/*----------Global declaration for Clock Variables----------------*/
int hours = 13;
int germanyHours = hours+4;
int minutes = 59;
int seconds = 50;
bool isMilitaryTime = true;
bool isPm = false;
char key;
char location;


void LCDDisplayMsg(char *firstLine = "", char *secondLine = "",
                   int delayTime = 0) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if(firstLine != "")lcd.print(firstLine);
  lcd.setCursor(0, 1);
  if(secondLine != "")lcd.print(secondLine);
  delay(delayTime);
}


void setup() {
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.display();

  // matrix Keypad input declaration
  for (int i = 0; i <= 3; i++) {
    pinMode(rowPins[i], INPUT);
    pinMode(colPins[i], INPUT);
  }

  // setup for 7 seg library
  pinMode(digPin1, OUTPUT);
  pinMode(digPin2, OUTPUT);

  digitalWrite(digPin1, HIGH);
  digitalWrite(digPin2, LOW);

  // setup buzzer pin
  pinMode(buzPin, OUTPUT);

  // setup buzzer pin
  pinMode(gasSensorPin, INPUT);

  // setup stepper motor
  for (int i = 0; i < 4; i++) {
    pinMode(stepperClockwise[i], OUTPUT);
    pinMode(stepperCounterClockwise[i], OUTPUT);
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
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);
    
    coilState[0] = 0;
    coilState[2] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);
    
    coilState[1] = 0;
    coilState[3] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);
    
    coilState[2] = 0;
    coilState[0] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);
    
  } else if(!isClockwise) {
    coilState[0] = 1;
    coilState[1] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);
    
    coilState[0] = 0;
    coilState[2] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);
    
    coilState[1] = 0;
    coilState[3] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);
    
    coilState[2] = 0;
    coilState[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);
  }
}


void halfStep(bool isClockwise) {
  int coilState[] = {0, 0, 0, 0};

  if (isClockwise) {
    coilState[0] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[1] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[0] = 0;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[2] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[1] = 0;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[3] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[2] = 0;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);

    coilState[0] = 1;
    setStepperCoilsMode(stepperClockwise, coilState);
    delay(stepDelay);
  } else if (!isClockwise) {
    coilState[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[1] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[0] = 0;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[2] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[1] = 0;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[3] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[2] = 0;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);

    coilState[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilState);
    delay(stepDelay);
  }
}


void rotateStepperD(char* mode, bool isClockwise, int degrees) {
  int steps = (degrees * 512) / 360;

  if(mode == "halfStep") {
    for (int i = 0; i < steps; i++) {
      halfStep(isClockwise);
    }
  } else if(mode == "fullStep") {
    for (int i = 0; i < steps; i++) {
      fullStep(isClockwise);
    }
  }
}

void buzzerSound(char *feel) {
  if (feel == "warning") {
    // play a warning sound
    tone(buzPin, 330, 150); // play buzzer in Emajor
    delay(150);
    tone(buzPin, 349, 200); // play buzzer in Fmajor
    delay(200);
    tone(buzPin, 440, 250); // play buzzer in Amajor
    delay(250);
  } else if (feel == "loosing") {
    tone(buzPin, 494, 700); // buzz on Bmajor
    delay(700);
    tone(buzPin, 330, 700); // buzz on Emajor
    delay(700);
    tone(buzPin, 262, 1600); // buzz on Cmajor
    delay(1600);
  }
}


void sevSegCountDown() {

  for(int i = 10; i > 0; i--) {

    for(int j = 0; j <= 15; j++) {
      fullStep(true);
      digitalWrite(digPin1, HIGH);
      digitalWrite(digPin2, LOW);
      sevSeg.set(i/10);
      delay(1);
      fullStep(true);
      digitalWrite(digPin1, LOW);
      digitalWrite(digPin2, HIGH);
      sevSeg.set(i%10);
      delay(1);
    }
  }
  sevSeg.off();
}


void displayTime(int hour, int min, int sec, char location, bool isMilitary) {
  lcd.clear();
  lcd.setCursor(0,0);
  if(location == 'A'){
    lcd.print("AL: ");

    if(isMilitary) {
      if(germanyHours > 12) {
        if((germanyHours-12)<10)lcd.print("0");// always 2 digits
        lcd.print(germanyHours-12);
        isPm = true;
      } else {
        if(germanyHours<10)lcd.print("0");// always 2 digits
        lcd.print(germanyHours);
        isPm = false;
      }
    }
    else {
      if(germanyHours<10)lcd.print("0");// always 2 digits
      lcd.print(germanyHours);
    }

  } else {
    lcd.print("BR: ");

    if(isMilitary) {
      if(hours > 12) {
        if((hour-12)<10)lcd.print("0");// always 2 digits
        lcd.print(hour-12);
        isPm = true;
      } else {
        if(hour<10)lcd.print("0");// always 2 digits
        lcd.print(hour);
        isPm = false;
      }
    }
    else {
      if(hour<10)lcd.print("0");// always 2 digits
      lcd.print(hour);
    }

  }

  lcd.print(":");
  if(min<10)lcd.print("0");
  lcd.print(min);
  lcd.print(":");
  if(sec<10)lcd.print("0");
  lcd.print(sec);

  lcd.print("  ");
  if(isMilitary) {
    if(isPm) {
      lcd.print("PM");
    } else {
      lcd.print("AM");
    }
  }

  lcd.setCursor(0,1);
  if (isGasDetectionOn) {
      lcd.print("Sis. de gas: ON");
    } else {
      lcd.print("Sis. de gas: OFF");
    }

}


void incleaseTime() {
  seconds++;
  if(seconds >= 60){
    seconds = 0;
    minutes++;
  }
  if(minutes >= 60) {
    minutes = 0;
    hours++;
    germanyHours = hours+4;
  }
  if(germanyHours >= 24) {
    germanyHours = hours + 4 - 24;
  }
  if(hours >= 24) {
    hours = 0;
    germanyHours = hours+4;
  }
}

void loop() {

  fullStep(true);

  key = customKeypad.getKey();
  if(key == 'A') {
    location = 'A';
  } else if (key == 'B') {
    location = 'B';
  } else if (key == '*' || key == '#') {
    isMilitaryTime = !isMilitaryTime;
    delay(100);
  } else if (key == 'D') {
    isGasDetectionOn = !isGasDetectionOn;
    delay(100);
  }

  unsigned long currentMillis = millis(); //using microsec precision reduces the code velocity to ~=10380 cicles per sec

  if(currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    incleaseTime();
    displayTime(hours, minutes, seconds, location, isMilitaryTime);
  }


  if(seconds == 0 && minutes%2 == 0) {
    stepDelay = 8;
    tone(buzPin, 440, 250);
    LCDDisplayMsg("Troca de Turno", "Vel. motores reduzida");
    sevSegCountDown();
    seconds+=10;
    stepDelay = 2;
  }


  int gasReading = analogRead(gasSensorPin); //reads the analaog value from the carbon monoxide sensor's

  if(isGasDetectionOn) {
    while(gasReading < 600) {
      digitalWrite(buzPin, HIGH);
      LCDDisplayMsg("Vazamento de Gas", "Evacuar agora!", 100);

      currentMillis = millis();
      if(currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;
        incleaseTime();
      }

      gasReading = analogRead(gasSensorPin);

    }
  }

  digitalWrite(buzPin, LOW);
}
