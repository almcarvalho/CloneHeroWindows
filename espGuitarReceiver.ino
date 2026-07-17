#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
 
//====================================================
// ESP-NOW RECEPTOR
// ESP32 Core 2.0.14
//
// Recebe 1 byte via ESP-NOW
// Repassa o byte para a Serial
// Pisca LED ao receber
//====================================================

const byte LED_AZUL = 2;

volatile bool piscar = false;
unsigned long tempoPisca = 0;

//====================================================

void OnDataRecv(const uint8_t *mac,
                const uint8_t *incomingData,
                int len)
{
    Serial.print("RX MAC: ");

    for (int i = 0; i < 6; i++)
    {
        if (mac[i] < 16)
            Serial.print("0");

        Serial.print(mac[i], HEX);

        if (i < 5)
            Serial.print(":");
    }

    Serial.print("  LEN=");
    Serial.print(len);

    if (len == 1)
    {
        Serial.print("  BYTE=");
        Serial.println(incomingData[0]);

        Serial.write(incomingData[0]);

        piscar = true;
        tempoPisca = millis();
        digitalWrite(LED_AZUL, HIGH);
    }
    else
    {
        Serial.println("  Pacote inválido");
    }
}

//====================================================

void setup()
{
    Serial.begin(115200);

    pinMode(LED_AZUL, OUTPUT);
    digitalWrite(LED_AZUL, LOW);

    WiFi.mode(WIFI_STA);

    WiFi.disconnect();

    WiFi.setSleep(false);

    // Mesmo canal do transmissor
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    Serial.println();
    Serial.println("==========================");
    Serial.println("ESP-NOW RECEIVER");
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.println("==========================");

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ERRO AO INICIAR ESP-NOW");

        while (true)
        {
            delay(100);
        }
    }

    esp_now_register_recv_cb(OnDataRecv);

    Serial.println("Aguardando pacotes...");
}

//====================================================

void loop()
{
    if (piscar && millis() - tempoPisca >= 50)
    {
        piscar = false;
        digitalWrite(LED_AZUL, LOW);
    }
}
