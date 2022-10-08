#define LED1 PN_1
#define LED2 PN_0
#define LED3 PF_4
#define LED4 PF_0


void setup() {
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT); 
  
  pinMode(PJ_0, INPUT_PULLUP);
  pinMode(PJ_1, INPUT_PULLUP);
}
void loop(){
  

  if(digitalRead(PJ_0) == HIGH && digitalRead(PJ_1) == LOW){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }
  if(digitalRead(PJ_0) == LOW && digitalRead(PJ_1) == HIGH){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }
  if(digitalRead(PJ_0) == LOW && digitalRead(PJ_1) == LOW){
    digitalWrite(LED1, HIGH);  
    
    delay(300);
  
    digitalWrite(LED2, HIGH);  
    delay(300);
  
    digitalWrite(LED3, HIGH);  
    delay(300);
  
    digitalWrite(LED4, HIGH);  
    delay(300);

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
  }
  if(digitalRead(PJ_0) == HIGH && digitalRead(PJ_1) == HIGH){
 digitalWrite(LED1, HIGH);  
  delay(300);               
  digitalWrite(LED1, LOW);  
  delay(300);

  digitalWrite(LED2, HIGH);  
  delay(300);               
  digitalWrite(LED2, LOW);  
  delay(300);

  digitalWrite(LED3, HIGH);  
  delay(300);               
  digitalWrite(LED3, LOW);  
  delay(300);

  digitalWrite(LED4, HIGH);  
  delay(300);               
  digitalWrite(LED4, LOW);  
  delay(300);

  digitalWrite(LED3, HIGH);  
  delay(300);               
  digitalWrite(LED3, LOW);  
  delay(300);

  digitalWrite(LED2, HIGH);  
  delay(300);               
  digitalWrite(LED2, LOW);  
  delay(300);

  digitalWrite(LED1, HIGH);  
  delay(300);               
  digitalWrite(LED1, LOW);  
  delay(300);
  }
  delay(100);
}
