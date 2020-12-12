#include <Arduino.h>

#if defined(ESP32)
#include "WiFi.h"
#include <esp_now.h>
#include <analogWrite.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <espnow.h>
#endif

// uint8_t remoteAddress[] = {0x24, 0x6F, 0x28, 0x96, 0x4F, 0x54}; // TTGO T-Display
uint8_t remoteAddress[] = {0x8C, 0xAA, 0xB5, 0x86, 0x87, 0xE4}; // TTGO LoRa
// uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA8, 0x3D, 0x94}; // DOIT
uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA1, 0x3F, 0x50}; // DOIT

typedef struct message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} message;
message mess;

int target_speed = 0;
int speed = 0;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&mess, incomingData, sizeof(mess));
  Serial.print("Bytes received: ");
  Serial.println(len);

  target_speed = mess.b;
  Serial.print("target_speed: "); Serial.println(target_speed);

}

int INA = 2;
int INB = 15;


void setup(void) {

  Serial.begin(115200);

  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);


  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
}


void loop() {
  if (speed != target_speed){
    if(target_speed == 0){
      speed = 0;
    } else {
      speed = target_speed;
    }
    analogWrite(INA, max(0, speed));
    analogWrite(INB, max(0, -speed));
  }
  delay(100);
  Serial.print("speed: "); Serial.println(speed);
}



