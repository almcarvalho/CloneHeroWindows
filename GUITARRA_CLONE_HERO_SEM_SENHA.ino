#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

//=============================
// GUITARRA CLONE HERO ESP32
// Portal WiFi SEM SENHA
// UDP PORT 5000
//=============================

const char* NOME_PORTAL = "GUITARRA_CLONE_HERO";

const int LED_AZUL = 2;

WiFiManager wm;
WiFiUDP udp;

const uint16_t PORTA_UDP = 5000;
IPAddress broadcastIP(255,255,255,255);

bool portalAberto=false;

const int BTN_VERDE=13;
const int BTN_VERMELHO=12;
const int BTN_AMARELO=14;
const int BTN_AZUL=27;
const int BTN_LARANJA=26;
const int BTN_PALHETA=25;
const int BTN_START=33;

const int totalBotoes=7;

int botoes[totalBotoes]={13,12,14,27,26,25,33};

const char* nomes[totalBotoes]={
"GREEN","RED","YELLOW","BLUE","ORANGE","STRUM","START"
};

bool estadoAnterior[totalBotoes];
unsigned long ultimoDebounce[totalBotoes];
const unsigned long debounceDelay=30;

void configModeCallback(WiFiManager*){
  portalAberto=true;
}

void enviarComando(const char* botao,const char* acao){
  udp.beginPacket(broadcastIP,PORTA_UDP);
  udp.print(botao);
  udp.print(":");
  udp.print(acao);
  udp.endPacket();
}

void controlarLed(){
  static unsigned long ultimo=0;
  static bool estado=false;

  if(WiFi.status()==WL_CONNECTED){
    portalAberto=false;
    digitalWrite(LED_AZUL,HIGH);
  }else if(portalAberto){
    if(millis()-ultimo>300){
      ultimo=millis();
      estado=!estado;
      digitalWrite(LED_AZUL,estado);
    }
  }else{
    digitalWrite(LED_AZUL,LOW);
  }
}

void lerBotoes(){
  for(int i=0;i<totalBotoes;i++){
    bool leitura=digitalRead(botoes[i]);

    if(leitura!=estadoAnterior[i]){
      if(millis()-ultimoDebounce[i]>debounceDelay){
        ultimoDebounce[i]=millis();
        estadoAnterior[i]=leitura;
        enviarComando(nomes[i], leitura==LOW ? "DOWN":"UP");
      }
    }
  }
}

void setup(){

  Serial.begin(115200);

  pinMode(LED_AZUL,OUTPUT);
  digitalWrite(LED_AZUL,LOW);

  for(int i=0;i<totalBotoes;i++){
    pinMode(botoes[i],INPUT_PULLUP);
    estadoAnterior[i]=digitalRead(botoes[i]);
    ultimoDebounce[i]=0;
  }

  wm.setAPCallback(configModeCallback);
  wm.setConfigPortalBlocking(false);

  // Portal SEM senha
  wm.autoConnect(NOME_PORTAL);

  udp.begin(PORTA_UDP);

  Serial.println("Guitarra iniciada.");
}

void loop(){

  wm.process();

  controlarLed();

  if(WiFi.status()==WL_CONNECTED){
    lerBotoes();
  }

}
