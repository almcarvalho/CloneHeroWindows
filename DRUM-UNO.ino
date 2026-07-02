/*
====================================================
BATERIA COM 5 PIEZOS + 2 BOTÕES - ARDUINO UNO

CAPTA O SINAL DOS PIEZOS DE ACORDO COM A VARIAÇÃO DELE (DELTA)

Envia pela Serial:
R,Y,B,G,O,K,S
====================================================
*/

const int PIEZO_RED    = A0;
const int PIEZO_YELLOW = A1;
const int PIEZO_BLUE   = A2;
const int PIEZO_GREEN  = A3;
const int PIEZO_ORANGE = A4;

const int BTN_KICK = 8;
const int BTN_START = 9;

const int BUZZER = 2;

const int DELTA_MINIMO = 20;
const unsigned long TEMPO_RETRIGGER = 300;

const int SOM_RED=200,SOM_YELLOW=400,SOM_BLUE=700,SOM_GREEN=900,SOM_ORANGE=1100;

int ultimoRed=0,ultimoYellow=0,ultimoBlue=0,ultimoGreen=0,ultimoOrange=0;
unsigned long ultimoHitRed=0,ultimoHitYellow=0,ultimoHitBlue=0,ultimoHitGreen=0,ultimoHitOrange=0;

bool ultimoKick,HultimoStart;

void setup(){
  Serial.begin(115200);
  pinMode(BUZZER,OUTPUT);
  pinMode(BTN_KICK,INPUT_PULLUP);
  pinMode(BTN_START,INPUT_PULLUP);
  ultimoKick=digitalRead(BTN_KICK);
  HultimoStart=digitalRead(BTN_START);
}

void loop(){
  int red=analogRead(PIEZO_RED);
  int yellow=analogRead(PIEZO_YELLOW);
  int blue=analogRead(PIEZO_BLUE);
  int green=analogRead(PIEZO_GREEN);
  int orange=analogRead(PIEZO_ORANGE);

  int dR=red-ultimoRed,dY=yellow-ultimoYellow,dB=blue-ultimoBlue,dG=green-ultimoGreen,dO=orange-ultimoOrange;

  Serial.print("R:");Serial.print(dR);
  Serial.print("\tY:");Serial.print(dY);
  Serial.print("\tB:");Serial.print(dB);
  Serial.print("\tG:");Serial.print(dG);
  Serial.print("\tO:");Serial.println(dO);

  if(dR>DELTA_MINIMO && millis()-ultimoHitRed>TEMPO_RETRIGGER){ultimoHitRed=millis();tone(BUZZER,SOM_RED,30);Serial.println("R");}
  if(dY>DELTA_MINIMO && millis()-ultimoHitYellow>TEMPO_RETRIGGER){ultimoHitYellow=millis();tone(BUZZER,SOM_YELLOW,30);Serial.println("Y");}
  if(dB>DELTA_MINIMO && millis()-ultimoHitBlue>TEMPO_RETRIGGER){ultimoHitBlue=millis();tone(BUZZER,SOM_BLUE,30);Serial.println("B");}
  if(dG>DELTA_MINIMO && millis()-ultimoHitGreen>TEMPO_RETRIGGER){ultimoHitGreen=millis();tone(BUZZER,SOM_GREEN,30);Serial.println("G");}
  if(dO>DELTA_MINIMO && millis()-ultimoHitOrange>TEMPO_RETRIGGER){ultimoHitOrange=millis();tone(BUZZER,SOM_ORANGE,30);Serial.println("O");}

  bool k=digitalRead(BTN_KICK);
  if(k!=ultimoKick){
    ultimoKick=k;
    Serial.println(k==LOW?"K_DOWN":"K_UP");
  }

  bool s=digitalRead(BTN_START);
  if(s!=HultimoStart){
    HultimoStart=s;
    Serial.println(s==LOW?"S_DOWN":"S_UP");
  }

  ultimoRed=red; ultimoYellow=yellow; ultimoBlue=blue; ultimoGreen=green; ultimoOrange=orange;
  delay(2);
}
