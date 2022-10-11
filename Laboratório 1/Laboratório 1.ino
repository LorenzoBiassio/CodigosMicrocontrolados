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


/*----------Global declaration for Stepper Motor----------------*/
int stepperClockwise[] = {PH_0, PH_1, PH_2, PH_3};
int stepperCounterClockwise[] = {PH_3, PH_2, PH_1, PH_0};
int stepsPerRev = 512;
int stepDelay = 1;


/*----------Global declaration for buzzer----------------*/
const int buzPin = PL_4;


/*----------Global declaration for Users passwords----------------*/
String pswrdUser1 = "123456"; // User Lorenzo
String pswrdUser2 = "987654"; // User Pedro


void LCDDisplayMsg(char *firstLine = "", char *secondLine = "",
                   int delayTime = 0) {
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

  // set LCD initializer
  LCDDisplayMsg("Bem vindo", "ao cofre!", 3000);

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

  // setup stepper motor
  for (int i = 0; i < 4; i++) {
    pinMode(stepperClockwise[i], OUTPUT);
    pinMode(stepperCounterClockwise[i], OUTPUT);
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


int getLockerMode() {
  LCDDisplayMsg("1-Inserir senha", "2-Alterar senha", 50);
  int key = customKeypad.getKey() - 48;
  return key;
}


String getPassword() {
  LCDDisplayMsg("Digite a senha");

  char key; // key pressed by user and displayed password
  char pswrd[] = {'0', '0', '0', '0', '0', '0'}; // inputed password and counter
  int count = 0;                                 // counter

  while (count != 6 ||
         (key != 'A' && key != 'B' && key != 'C' && key != '#' && key != '*')) {

    key = customKeypad.getKey();

    if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' &&
        key != '#' && key != '*') {

      pswrd[count] = key;

      lcd.setCursor(count, 1);
      lcd.print("*");
      tone(buzPin, 349, 150); // play buzzer in Fmajor
      count++;

      delay(50);
    }

    else if (key && key == 'D') {
      count--;

      if (count < 0) {
        count = 0;
      }

      pswrd[count] = '0';

      lcd.setCursor(count, 1);
      lcd.print(" ");
      tone(buzPin, 349, 150); // play buzzer in Fmajor

      delay(50);
    }

    if (count != 6 &&
        (key == 'A' || key == 'B' || key == 'C' || key == '#' || key == '*')) {

      LCDDisplayMsg("A senha deve ter", "6 digitos", 2000);
      LCDDisplayMsg("Digite a senha");
      for (int i = 0; i < count; i++) {
        lcd.setCursor(i, 1);
        lcd.print("*");
      }
    }
  }
  return pswrd;
}


void sevSegCountDown() {
  // set 7seg display to "5"
  sevSeg.set(5);
  delay(1000);
  // set 7seg display to "4"
  sevSeg.set(4);
  delay(1000);
  // set 7seg display to "3"
  sevSeg.set(3);
  delay(1000);
  // set 7seg display to "2"
  sevSeg.set(2);
  delay(1000);
  // set 7seg display to "1"
  sevSeg.set(1);
  delay(1000);
  // set 7seg display to "0"
  sevSeg.off();
}


void correctPswrdRoutine(char *usrName) {
  // play buzzer for 1 second in Cmajor
  tone(buzPin, 294, 1000);
  LCDDisplayMsg("Seja bem vindo,", usrName);
  rotateStepperD(true, 180);
  sevSegCountDown();
  rotateStepperD(false, 180);
}


void incorrectPswrdMsg() {
  LCDDisplayMsg("Senha incorreta", "Aguarde 3 seg", 1000);
  tone(buzPin, 494, 700); // buzz on Bmajor
  delay(700);
  LCDDisplayMsg("Senha incorreta", "Aguarde 2 seg", 1000);

  tone(buzPin, 330, 700); // buzz on Emajor
  delay(700);
  LCDDisplayMsg("Senha incorreta", "Aguarde 1 seg", 1000);
  tone(buzPin, 262, 1600); // buzz on Cmajor
  delay(1600);
}


void setStepperCoilsMode(int stepperCoils[], int coilsMode[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(stepperCoils[i], coilsMode[i]);
  }
}


void onestep(bool isClockwise) {
  int coilMode[] = {0, 0, 0, 0};

  if (isClockwise) {
    coilMode[0] = 1;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[1] = 1;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[0] = 0;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[2] = 1;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[1] = 0;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[3] = 1;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[2] = 0;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);

    coilMode[0] = 1;
    setStepperCoilsMode(stepperClockwise, coilMode);
    delay(stepDelay);
  } else if (!isClockwise) {
    coilMode[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[1] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[0] = 0;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[2] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[1] = 0;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[3] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[2] = 0;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);

    coilMode[0] = 1;
    setStepperCoilsMode(stepperCounterClockwise, coilMode);
    delay(stepDelay);
  }
}


void rotateStepperD(bool isClockwise, int degrees) {
  int steps = (degrees * 512) / 360;
  for (int i = 0; i < steps; i++) {
    onestep(isClockwise);
  }
}


void loop() {

  int lockerMode = getLockerMode();

  if (lockerMode == 1) {
    String usrPswrd = getPassword();

    if (usrPswrd == pswrdUser1) {
      correctPswrdRoutine("Lorenzo");
    } else if (usrPswrd == pswrdUser2) {
      correctPswrdRoutine("Pedro");
    } else {
      incorrectPswrdMsg();
    }
  } else if (lockerMode == 2) {
    LCDDisplayMsg("Insira nova senha");
    buzzerSound("warning");

    String newPswrd = getPassword();
    pswrdUser1 = newPswrd;

    LCDDisplayMsg("Senha adicionada", "com sucesso", 50);
    buzzerSound("warning");
  }

}
