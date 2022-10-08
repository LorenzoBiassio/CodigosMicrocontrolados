
#define LED1 PN_1
#define LED2 PN_0
#define LED3 PF_4
#define LED4 PF_0


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);  
}

void loop() {
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
