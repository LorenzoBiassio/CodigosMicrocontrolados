#define LED1 PN_1
#define LED2 PN_0
#define LED3 PF_4
#define LED4 PF_0


void setup() {
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  pinMode(PJ_0, INPUT_PULLUP);
  pinMode(PJ_1, INPUT_PULLUP);
}
void loop(){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);

  if(digitalRead(PJ_0) == HIGH){
    digitalWrite(LED1, LOW);
  }
  
  if(digitalRead(PJ_1) == HIGH){
    digitalWrite(LED2, HIGH);
  }
  delay(100);
}
