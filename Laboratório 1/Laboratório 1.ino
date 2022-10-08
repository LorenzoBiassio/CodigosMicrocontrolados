
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <sevenSegmentDisplay.h>

/*------------Global declaration for LCD--------------*/
const int rs = PM_0, rw = PM_1, en = PM_2,d0 = PK_0, d1 = PK_1, d2 = PK_2, d3 = PK_3, d4 = PK_4, d5 = PK_5, d6 = PK_6, d7 = PK_7; //pin declaration for LCD
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);

/*----------Global declaration for 7 segment display--------------------*/
const int segPinA = PQ_0, segPinB = PQ_1, segPinC = PQ_2, segPinD = PQ_3, segPinE = PA_4, segPinF = PA_5, segPinG = PA_6, segPinDP = PA_7; //segment pin declaration  
const int digPin1 = PB_4, digPin2 = PB_5; //pin declaration for digits
const int digCount = 2;

sevenSegmentDisplay sevSeg(COMMON_CATHODE, segPinA, segPinB, segPinC, segPinD, segPinE, segPinF, segPinG, segPinDP);

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

/*----------Global declaration for Stepper Motor----------------*/


/*----------Global declaration for buzzer----------------*/
const int buzPin = PL_4;

/*----------Global declaration for Users passwords----------------*/
int pswrdUser1 = 123456;  //User Lorenzo
int pswrdUser2 = 987654;  //User Pedro


void setup() {
  Serial.begin(9600);

  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.display();

  //set LCD initializer
  lcd.setCursor(0,0);
  lcd.print("Bem vindo ");
  lcd.setCursor(0,1);
  lcd.print("ao cofre!");
  delay(3000);
  lcd.clear();

  //matrix Keypad input declaration
  for(int i=0;i<=3; i++){
    pinMode(rowPins[i], INPUT);
    pinMode(colPins[i], INPUT);
  }
  
  //setup for 7 seg library
  pinMode(digPin1, OUTPUT);
  pinMode(digPin2, OUTPUT);

  digitalWrite(digPin1, HIGH);
  digitalWrite(digPin2, LOW);

  //setup buzzer pin
  pinMode(buzPin, OUTPUT);

  //setup stepper motor
  //*configurar home como fechado
  //*mover o motor de passo para home
}

void loop() {
  //Set LCD Header
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Digite a senha");

  //set 7 segments display
  sevSeg.off();

  char key; //key pressed by user and displayed password
  int pswrd = 0, count = 0; //inputed password and counter


  while(key != 'A' && key != 'B' && key != 'C' && key != '#' && key != '*' ){

    key = customKeypad.getKey();

    if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#' && key != '*' ){
    
      Serial.println(key); //*debug*
      pswrd = (pswrd * 10) + (key - 48);

      
      lcd.setCursor(count,1);
      lcd.print("*");
      tone(buzPin,349,150); //play buzzer in Fmajor
      count++;

      delay(50);
    }

    else if(key && key == 'D'){

      pswrd = pswrd / 10;

      count--;
      lcd.setCursor(count,1);
      lcd.print(" ");
      tone(buzPin,349,150); //play buzzer in Fmajor

      delay(50);
    }
  }
  Serial.println(pswrd); //*debug*

  if(pswrd == pswrdUser1){
    //*mover motor de passo para aberto

    //play buzzer for 1 second in Cmajor
    tone(buzPin,294,1000);

    //display message on LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Seja bem vindo:");
    lcd.setCursor(0,1);
    lcd.print("Lorenzo");

    //set 7seg display to "5"
    sevSeg.set(5);
    delay(1000);
    //set 7seg display to "4"
    sevSeg.set(4);
    delay(1000);
    //set 7seg display to "3"
    sevSeg.set(3);
    delay(1000);
    //set 7seg display to "2"
    sevSeg.set(2);
    delay(1000);
    //set 7seg display to "1"
    sevSeg.set(1);
    delay(1000);
    //set 7seg display to "0"
    sevSeg.off();

    //enter change password menu
    key = customKeypad.getKey();   //----não entra no teste do if----
    Serial.println(key);

    if(key && key == 'A'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Insira nova senha");
      delay(200);

      //play a warning sound
      tone(buzPin,330,150); //play buzzer in Emajor
      delay(150);
      tone(buzPin,349,200); //play buzzer in Fmajor
      delay(200);
      tone(buzPin,440,250); //play buzzer in Amajor
      delay(250);

      int pswrdChange = 0; count = 0; //inputed password and counter
      while(key != 'B' && key != 'C' && key != '#' && key != '*' ){
  
      key = customKeypad.getKey();

        if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#' && key != '*' ){
        
          Serial.println(key); //*debug*
          pswrdChange = (pswrdChange * 10) + (key - 48);

          
          lcd.setCursor(count,1);
          lcd.print("*");
          tone(buzPin,349,150); //play buzzer in Fmajor
          count++;

          delay(50);
        }

        else if(key && key == 'D'){

          pswrdChange = pswrdChange / 10;

          count--;
          lcd.setCursor(count,1);
          lcd.print(" ");
          tone(buzPin,349,150); //play buzzer in Fmajor

          delay(50);
        }

      }
      pswrdUser1 = pswrdChange;
      Serial.println(pswrdUser1); //*debug*

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Senha adicionada");
      lcd.setCursor(0,1);
      lcd.print("com sucesso");
      delay(400);

      //play a warning sound
      tone(buzPin,330,150); //play buzzer in Emajor
      delay(150);
      tone(buzPin,349,200); //play buzzer in Fmajor
      delay(200);
      tone(buzPin,440,250); //play buzzer in Amajor
      delay(250);
    }


    //*mover o motor de passo para home    
  }

  else if(pswrd == pswrdUser2){
    //*mover motor de passo para aberto
    
    //play buzzer for 1 second in Cmajor
    tone(buzPin,262,1000);

    //display message on LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Seja bem vindo:");
    lcd.setCursor(0,1);
    lcd.print("Pedro");    

    //set 7seg display to "5"
    sevSeg.set(5);
    delay(1000);
    //set 7seg display to "4"
    sevSeg.set(4);
    delay(1000);
    //set 7seg display to "3"
    sevSeg.set(3);
    delay(1000);
    //set 7seg display to "2"
    sevSeg.set(2);
    delay(1000);
    //set 7seg display to "1"
    sevSeg.set(1);
    delay(1000);
    //set 7seg display to "0"
    sevSeg.set(0);

    //*mover o motor de passo para home  
  }
  else{
    //display message on LCD and play a losing song on buzzer
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Senha incorreta");

    lcd.setCursor(0,1);
    lcd.print("Aguarde 3 seg");
    tone(buzPin,494,700);//buzz on Bmajor
    delay(1000);

    lcd.setCursor(0,1);
    lcd.print("Aguarde 2 seg");
    tone(buzPin,330,700); //buzz on Emajor
    delay(1000);

    lcd.setCursor(0,1);
    lcd.print("Aguarde 1 seg");
    tone(buzPin,262,1600); //buzz on Cmajor
    delay(1000);

  }

}
