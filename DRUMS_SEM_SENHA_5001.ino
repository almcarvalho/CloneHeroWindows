#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

// ========================================
// DRUMS - Clone Hero (ESP32)
// Portal WiFi: DRUMS (sem senha)
// UDP: 5001
// ========================================

const char* NOME_PORTAL = "DRUMS";

const int LED_AZUL = 2;

WiFiManager wm;
WiFiUDP udp;

const uint16_t PORTA_UDP = 5001;
IPAddress broadcastIP(255,255,255,255);

bool portalAberto = false;

// -------- Botões --------
const int BTN_RED    = 13;
const int BTN_YELLOW = 12;
const int BTN_BLUE   = 14;
const int BTN_GREEN  = 27;
const int BTN_ORANGE = 26;
const int BTN_KICK   = 25;
const int BTN_START  = 33;

const int TOTAL = 7;

int pinos[TOTAL] = {
  BTN_RED,
  BTN_YELLOW,
  BTN_BLUE,
  BTN_GREEN,
  BTN_ORANGE,
  BTN_KICK,
  BTN_START
};

const char* nomes[TOTAL] = {
  "DRUM_RED",
  "DRUM_YELLOW",
  "DRUM_BLUE",
  "DRUM_GREEN",
  "DRUM_ORANGE",
  "KICK",
  "START"
};

bool estadoAnterior[TOTAL];
unsigned long debounce[TOTAL];
const unsigned long TEMPO_DEBOUNCE = 30;

void configModeCallback(WiFiManager*){
  portalAberto = true;
}

void enviar(const char* botao, const char* acao){
  udp.beginPacket(broadcastIP, PORTA_UDP);
  udp.print(botao);
  udp.print(":");
  udp.print(acao);
  udp.endPacket();
}

void controlarLed(){
  static bool estado=false;
  static unsigned long t=0;

  if(WiFi.status()==WL_CONNECTED){
    portalAberto=false;
    digitalWrite(LED_AZUL,HIGH);
  }else if(portalAberto){
    if(millis()-t>300){
      t=millis();
      estado=!estado;
      digitalWrite(LED_AZUL,estado);
    }
  }else{
    digitalWrite(LED_AZUL,LOW);
  }
}

void lerBotoes(){
  for(int i=0;i<TOTAL;i++){
    bool leitura=digitalRead(pinos[i]);

    if(leitura!=estadoAnterior[i]){
      if(millis()-debounce[i]>TEMPO_DEBOUNCE){
        debounce[i]=millis();
        estadoAnterior[i]=leitura;

        if(leitura==LOW)
          enviar(nomes[i],"DOWN");
        else
          enviar(nomes[i],"UP");
      }
    }
  }
}

void setup(){

  Serial.begin(115200);

  pinMode(LED_AZUL,OUTPUT);
  digitalWrite(LED_AZUL,LOW);

  for(int i=0;i<TOTAL;i++){
    pinMode(pinos[i],INPUT_PULLUP);
    estadoAnterior[i]=digitalRead(pinos[i]);
    debounce[i]=0;
  }

  wm.setAPCallback(configModeCallback);
  wm.setConfigPortalBlocking(false);

  // Portal sem senha
  wm.autoConnect(NOME_PORTAL);

  udp.begin(PORTA_UDP);

  Serial.println("DRUMS iniciado.");
}

void loop(){

  wm.process();

  controlarLed();

  if(WiFi.status()==WL_CONNECTED){
    lerBotoes();
  }
}
