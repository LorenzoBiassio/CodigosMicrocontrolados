//Laboratorio 1 Paulo e Gabriel

#include <Keypad.h>
#include <LiquidCrystal.h>
   
//ligacao do motor 
#define sai1 PH_0
#define sai2 PH_1
#define sai4 PH_2
#define sai3 PH_3 

//variaveis p/ senha
char senhap[7]="123456";
char senhag[7]="147258";
char input[7];
char backup[7];
int a,p,g;
bool abrirCofre = false, alterarSenha = false, senhacorretap=false,senhacorretag=false;


//lcd
int RS=PM_0, RW=PM_1, EN=PM_2, D0=PK_0, D1=PK_1, D2=PK_2, D3=PK_3, D4=PK_4, D5=PK_5, D6=PK_6, D7=PK_7, buzzer=PE_0;

byte customChar[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
LiquidCrystal lcd(RS, RW, EN, D0, D1, D2, D3, D4, D5, D6, D7);

//Pinos display 7 seg
#define transistor PB_5
#define A PQ_0
#define B PQ_1
#define C PQ_2
#define D PQ_3
#define E PA_4
#define F PA_5
#define G PA_6
#define DP PA_7// decimal
#define common_cathode 0


bool segMode = common_cathode; 
int seg[] {A,B,C,D,E,F,G,DP}; // segment pins
byte chars = 10; // max value in the array "Chars"
byte Chars[10][9] { 
           {'0',1,1,1,1,1,1,0,0},//0
           {'1',0,1,1,0,0,0,0,0},//1
           {'2',1,1,0,1,1,0,1,0},//2
           {'3',1,1,1,1,0,0,1,0},//3
           {'4',0,1,1,0,0,1,1,0},//4
           {'5',1,0,1,1,0,1,1,0},//5
           {'6',1,0,1,1,1,1,1,0},//6
           {'7',1,1,1,0,0,0,0,0},//7
           {'8',1,1,1,1,1,1,1,0},//8
           {'9',1,1,1,1,0,1,1,0},//9
           };
//////////////////////////////////////////////////
////////// Configuração do Teclado////////////////
//////////////////////////////////////////////////
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {PL_0, PL_1, PL_2, PL_3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {PM_4, PM_5, PM_6, PM_7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


void setup() {
  pinMode(seg[0],OUTPUT);
  pinMode(seg[1],OUTPUT);
  pinMode(seg[2],OUTPUT);
  pinMode(seg[3],OUTPUT);
  pinMode(seg[4],OUTPUT);
  pinMode(seg[5],OUTPUT);
  pinMode(seg[6],OUTPUT); 
  pinMode(seg[7],OUTPUT);
  pinMode(transistor, OUTPUT);
  pinMode(DP, OUTPUT);  //Garante que o ponto do display não esteja ligado
  digitalWrite(DP, LOW);
  lcd.createChar(1, customChar);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  lcd.display();
  Serial.begin(9600);
  inicializar();
  pinMode(sai1,OUTPUT);
  pinMode(sai2,OUTPUT);
  pinMode(sai3,OUTPUT);
  pinMode(sai4,OUTPUT);
}


void bip(int freq, int tempo) {
  float periodo = 1000.0/freq;
   for (int i = 0; i< tempo/(periodo);i++){
    digitalWrite(buzzer,HIGH);
    delayMicroseconds(periodo*500); //Metade do periodo em ms
    digitalWrite(buzzer, LOW);
    delayMicroseconds(periodo*500);
  }
}


void reseta(){
  a=1;
  abrirCofre = alterarSenha = senhacorretap = senhacorretag = false; 
  a=p=g=0;  
  memset(input,0,7);
}

void inicializar(){
  lcd.setCursor(1,0);
  lcd.print("Inicializando");
  bip(1350,100);
  delay(100);
  bip(1350,100);
  delay(100);
  bip(1350,100);
  lcd.setCursor(0,1);
  for(int i=0;i<16;i++){
    lcd.write((byte)1);
    delay(100);
  }
  reseta();
}

void alarme(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Senha incorreta");
  lcd.setCursor(0,1);
  lcd.print("Aguarde 3s");
  bip(300,3000);
  lcd.clear();
  reseta();
}

void menu(){
  delay(50);
    lcd.setCursor(0,0);
    lcd.print("1-Abrir o cofre  ");
    lcd.setCursor(0,1);
    lcd.print("2-Alterar senha  ");
    reseta();
}

void verifypw(char key){
if(key=='A')
  reseta();
if(a>7)//evita estourar o limite do char e força uma verificação
  key='#';
  if(key=='#'){
    key=NULL;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Validando senha");
    lcd.setCursor(3,1);
    lcd.print("Aguarde");
    delay(500);
    lcd.clear();
    for(int i=0;i<6;i++){
      if(input[i]==senhap[i])
      p++;
      if(input[i]==senhag[i])
      g++;}
      if(p==6){ 
      a=p=g=0;
      senhacorretap=true;}
      else if(g==6){
      a=p=g=0;
      senhacorretag=true;}
      else
      alarme();
      a=p=g=0;
   }
  if(key=='*' && a>0){
    input[a-1]=' ';
    bip(450,50);
    lcd.setCursor(a+4,1);
    lcd.print(" ");
    a--;
  }
  lcd.setCursor(0,0);
  lcd.print("Digite sua senha");
  if(key!=NULL && key!='*'){
    bip(450,50);
    lcd.setCursor(a+5,1);
    lcd.print("*");
    input[a]=key;
    a++;}
}

void changepw(char key){
 if(key=='A')
  reseta();
  if(senhacorretap == true){
    strcpy(backup , senhap);
    if(a!=0)
    lcd.clear();
    delay(50);
    a=0;
    lcd.setCursor(0,0);
    lcd.print("Digite sua nova ");
    lcd.setCursor(0,1);
    lcd.print("senha:");
    do{
    char key = customKeypad.getKey();
    if(key=='A')
    reseta();
    if(p>6){//evita estourar o limite do char e força entrada
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A senha so pode");
      lcd.setCursor(0,1);
      lcd.print("ter 6 digitos");
      delay(2000);
      strcpy(senhap , backup);
      reseta();}
    if(key=='#'){
     lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Salvando a nova ");
      lcd.setCursor(0,1);
      lcd.print("senha");
      delay(100);
      reseta();
    }
    if(key=='*' && p>0){
      input[p-1]=' ';
      bip(450,50);
      lcd.setCursor(p+5,1);
      lcd.print(" ");
      p--;
    }
    if(key!=NULL && key!='*' && key!='#'){
    bip(450,50);
    lcd.setCursor(p+6,1);
    lcd.print("*");
    senhap[p]=key;
    p++;}
  }while(alterarSenha==true);}
  if(senhacorretag == true){
    strcpy(backup , senhag);
    if(a!=0)
    lcd.clear();
    delay(50);
    a=0;
    lcd.setCursor(0,0);
    lcd.print("Digite sua nova ");
    lcd.setCursor(0,1);
    lcd.print("senha:");
    do{
    char key = customKeypad.getKey();
    if(key=='A')
    reseta();
    if(p>6){//evita estourar o limite do char e força entrada
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A senha so pode");
      lcd.setCursor(0,1);
      lcd.print("ter 6 digitos");
      delay(2000);
      strcpy(senhag , backup);
      reseta();}
    if(key=='#'){
     lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Salvando a nova ");
      lcd.setCursor(0,1);
      lcd.print("senha");
      delay(100);
      reseta();
    }
    if(key=='*' && p>0){
      input[p-1]=' ';
      bip(450,50);
      lcd.setCursor(p+5,1);
      lcd.print(" ");
      p--;
    }
    if(key!=NULL && key!='*' && key!='#'){
    bip(450,50);
    lcd.setCursor(p+6,1);
    lcd.print("*");
    senhag[p]=key;
    p++;}
  }while(alterarSenha==true);}
  if(alterarSenha==true)
  verifypw(key);
}

void abre(){
  bip(1300,850);
  for (int i = 0; i<=135; i++)
 {
 digitalWrite(sai1,HIGH);
 digitalWrite(sai2,LOW);
 digitalWrite(sai3,HIGH);
 digitalWrite(sai4,LOW);
 delay(15);
 digitalWrite(sai3,HIGH);
 digitalWrite(sai1,LOW);
 digitalWrite(sai2,HIGH);
 digitalWrite(sai4,LOW);
 delay(15); 
 digitalWrite(sai2,HIGH);
 digitalWrite(sai1,LOW);
 digitalWrite(sai4,HIGH);
 digitalWrite(sai3,LOW);
 delay(15); 
 digitalWrite(sai1,HIGH);
 digitalWrite(sai2,LOW);
 digitalWrite(sai4,HIGH);
 digitalWrite(sai3,LOW);
 delay(15); 
 }
  for(int j=5; j>=0; j--){
      digitalWrite(transistor, HIGH);        
      for(int i = 0;i<8;i++){
      digitalWrite(seg[i],Chars[j][i+1]);
      }
      delay(1000);
      digitalWrite(transistor, LOW);}
      reseta();
  lcd.setCursor(0,0); 
      lcd.clear();
      lcd.print("Fechando o cofre");
      lcd.setCursor(4,1); 
      lcd.print("Ate mais");
  for (int i = 0; i<=135; i++)
 {
 digitalWrite(sai1,HIGH);
 digitalWrite(sai2,LOW);
 digitalWrite(sai3,HIGH);
 digitalWrite(sai4,LOW);
 delay(15);
 digitalWrite(sai3,HIGH);
 digitalWrite(sai1,LOW);
 digitalWrite(sai2,HIGH);
 digitalWrite(sai4,LOW);
 delay(15); 
 digitalWrite(sai2,HIGH);
 digitalWrite(sai1,LOW);
 digitalWrite(sai4,HIGH);
 digitalWrite(sai3,LOW);
 delay(15); 
 digitalWrite(sai1,HIGH);
 digitalWrite(sai2,LOW);
 digitalWrite(sai4,HIGH);
 digitalWrite(sai3,LOW);
 delay(15); 
}
}

void loop() {
  char key = customKeypad.getKey();
  if (abrirCofre == false && alterarSenha == false) {
    menu();
    if(key!=NULL)
    bip(450,50);
    if(key=='1'){
      abrirCofre = true;
      key=NULL;
      lcd.clear();}
    if(key=='2'){
      alterarSenha = true; 
      key=NULL;
      lcd.clear();}}
    if (alterarSenha == true) {  
      changepw(key);
    }
    if (abrirCofre == true) {
      if(senhacorretap == true){
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Abrindo o cofre");
      lcd.setCursor(0,1); 
      lcd.print("Ola Sr. Paulo");
      abre();
      lcd.clear();
      }
      if(senhacorretag == true){
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Abrindo o cofre");
      lcd.setCursor(0,1); 
      lcd.print("Ola Sr. Gabriel");
      abre();
      lcd.clear();
      }
      verifypw(key);
    }
}
