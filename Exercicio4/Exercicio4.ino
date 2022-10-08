#include <LiquidCrystal.h>

const int rs = PM_0,rw = PM_1, en = PM_2,d0 = PK_0, d1 = PK_1, d2 = PK_2, d3 = PK_3, d4 = PK_4, d5 = PK_5, d6 = PK_6, d7 = PK_7;
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);

const int Button = PUSH1;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.display();

  pinMode(Button, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(Button)) {
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("UTFPR");
    lcd.setCursor(0,1);
    lcd.print("Seu nome e idade");

    delay(100);
  } else {
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("Lorenzo 20 Pedro 20");
    lcd.setCursor(0,1);
    lcd.print("Microcontroladores");

    delay(500);
    for(int i=0; i<4; i++) {
      lcd.scrollDisplayLeft();
      delay(500);
    }

    delay(100);
  }
}
