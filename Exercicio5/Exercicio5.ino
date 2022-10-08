
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

/*------------Global declaration for LCD--------------*/
const int rs = PM_0, rw = PM_1, en = PM_2,d0 = PK_0, d1 = PK_1, d2 = PK_2, d3 = PK_3, d4 = PK_4, d5 = PK_5, d6 = PK_6, d7 = PK_7; //pin declaration for LCD
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);

/*----------Global declaration for keypad driving--------------------*/
const int L0 = PL_0, L1 = PL_1, L2 = PL_2, L3 = PL_3, C0 = PM_4, C1 = PM_5, C2 = PM_6, C3 = PM_7; //pin declaration for keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {L0, L1, L2, L3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {C0, C1, C2, C3}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

/*----------Global declaration for ServoMotor----------------*/
const int servoPin = PL_4;
Servo servoMotor;  // create servoMotor object to control a servoMotor

/*----------Global declaration for StepMotor-----------------*/
//const int stepsPerRevolution = 400; //defined by the chosen StepMotor 
const int whitePin = PH_0, brownPin = PH_1, yellowPin = PH_2, redPin = PH_3;
byte stepPins[4] = {whitePin, yellowPin, redPin, brownPin};

void setup(){
  Serial.begin(9600);

  //matrix Keypad input declaration
  for(int i=0;i<=3; i++){
    pinMode(rowPins[i], INPUT);
    pinMode(colPins[i], INPUT);
  }
  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.display();
  //set up servo port
  servoMotor.attach(servoPin);
  servoMotor.write(0);
  //set up stepMotor speed in rpm
    pinMode(PH_0, OUTPUT);
    pinMode(PH_1, OUTPUT);
    pinMode(PH_2, OUTPUT);
    pinMode(PH_3, OUTPUT);
}

  
void loop(){
  //set header LCD
  lcd.setCursor(0,0);
  lcd.print("Valor em Graus");

  char key; //key pressed by user
  int pos = 0; //inputed number in degrees 

  while(key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#' && key != '*' ){
    
    key = customKeypad.getKey();

    if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#' && key != '*' ){
    
      Serial.println(key);
      pos = (pos * 10) + (key - 48);

      lcd.setCursor(0,1);
      lcd.print(pos);
      delay(100);
    }
  }
  //Set ServoMotor rotation
  servoMotor.write(pos);

  //Set StepMotor rotation
  for(int j = 0; j < 10; j++) {
    for(int i = 0; i<4; i=i+2){
      digitalWrite(stepPins[i],HIGH); 
      digitalWrite(stepPins[i+1],HIGH); 
      delay(60);
      digitalWrite(stepPins[i],LOW);
      digitalWrite(stepPins[i+1],LOW);
    }
  }


  delay(500);
  lcd.clear();
}

