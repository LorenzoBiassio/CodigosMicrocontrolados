#define D1 PA_7
#define D2 PA_6
#define D3 PA_5
#define D4 PA_4
#define D5 PQ_3
#define D6 PQ_2
#define D7 PQ_1
#define D8 PQ_0
#define T0 PP_5

void setup() {                
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);     
}

void loop() {
digitalWrite(D1, HIGH);  
  delay(300);              
  digitalWrite(D1, LOW);    
  delay(300);              
  digitalWrite(D2, HIGH);  
  delay(300);              
  digitalWrite(D2, LOW);    
  delay(300);              
  digitalWrite(D3, HIGH);  
  delay(300);              
  digitalWrite(D3, LOW);    
  delay(300);            
  digitalWrite(D4, HIGH);  
  delay(300);
  digitalWrite(D4, LOW);
  delay(300);             
  digitalWrite(D5, HIGH);  
  delay(300);              
  digitalWrite(D5, LOW);    
  delay(300);             
  digitalWrite(D6, HIGH);  
  delay(300);              
  digitalWrite(D6, LOW);    
  delay(300);             
  digitalWrite(D7, HIGH);  
  delay(300);              
  digitalWrite(D7, LOW);    
  delay(300);             
  digitalWrite(D8, HIGH);  
  delay(300);              
  digitalWrite(D8, LOW);    
  delay(300);                                        
}