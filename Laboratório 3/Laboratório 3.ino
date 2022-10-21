#include <Keypad.h>
#include <LiquidCrystal.h>


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


/*----------Global declaration for LED--------------------*/
const int LED = PL_4;


/*----------Global declaration for LDR--------------------*/
const int LDR = PD_4;


/*------------Global declaration for Buffer--------------*/
int bufferSize = 64;
int buffer[64]; //create buffer
int bufferIndex = 0; //index for counting buffer readyness


/*------------Global declaration for setting transfer speed--------------*/
unsigned long BUFFERpreviousMicros = 0;       
long BUFFERinterval = 1000;       //sets time interval (in microsec) between each data read


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

  // matrix Keypad input declaration
  for (int i = 0; i <= 3; i++) {
    pinMode(rowPins[i], INPUT);
    pinMode(colPins[i], INPUT);
  }

  // Set LED to output
  pinMode(LED, OUTPUT);

  // Set LDR to input
  pinMode(LDR, INPUT);

}


bool bufferBuild(float valueRead, unsigned long currentMicros){
  if(currentMicros - BUFFERpreviousMicros >= BUFFERinterval){
    BUFFERpreviousMicros = currentMicros;  

    buffer[bufferIndex] = valueRead;
    bufferIndex++;

    if (bufferIndex >= bufferSize){
      bufferIndex = 0;
      return 1;
    }
  }
  return 0;
}


void getBufferInterval() {
  LCDDisplayMsg("Nova a freq. de am.", "", 100);

  char key;       // key pressed by user
  int freq = 0, count = 0; //inputed password and counter

  while(key != 'A' && key != 'B' && key != 'C' && key != '#' && key != '*' ){

    key = customKeypad.getKey();

    if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#' && key != '*' ){

      Serial.println(key); //*debug*
      freq = (freq * 10) + (key - 48);


      lcd.setCursor(count,1);
      lcd.print(key);
      count++;

      delay(50);
    }

    else if(key && key == 'D'){


      if(freq < 10 && count == 1) {
        freq = 0;
        count = 0;
      } else {
        freq = freq / 10;
        count--;
      }

      lcd.setCursor(count,1);
      lcd.print(" ");

      delay(50);
    }
  }

  BUFFERinterval = freq * 1000;
}


int getCurrentMode() {
  LCDDisplayMsg("1-Aquis.de sinal", "2-Alt. freq. amost.", 0);
  int key = customKeypad.getKey() - 48;
  if(key == - 48) return 1;
  return key;
}


void loop() {
  int currentMode = getCurrentMode();

  unsigned long currentMicros = micros(); //using microsec precision reduces the code velocity to ~=10380 cicles per sec

  float Vp7 = analogRead(LDR) * (5 / 4096.0); // Sensor values in volts (5: supply voltage; 4096: adc resolution)    
  float RLDR = (Vp7 * 10000) /(5 - Vp7); // LDR resistor resistance

  float Lumens = -(pow(10, -3) * 5 * RLDR ) + 50; // More reasenable values
  float RealLumens = -(pow(10, -6) * 1.22 * RLDR ) + 50; // Real 

  analogWrite(LED, -(255 * Lumens / 50) + 255);

  bool bufferFlag = 0;

  bufferFlag = bufferBuild(Lumens, currentMicros);   //call the buffer builder function every loop cicle, now using raw analog input and microsec precision

  if(bufferFlag == 1){
    // Enviar dados do buffer
  }

  if(currentMode == 2) {
    getBufferInterval();
    LCDDisplayMsg("Nova frequencia adicionada", "com sucesso", 2000);
  }

}
