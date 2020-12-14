#include <Arduino.h>

#include "utils.h"

#include <ESP8266WiFi.h>
#include <espnow.h>


message mess;

int target_speed = 0;
int speed = 0;

// callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&mess, incomingData, sizeof(mess));
  Serial.print("Bytes received: ");
  Serial.println(len);

  target_speed = mess.b;
  Serial.print("target_speed: "); Serial.println(target_speed);

}

int INA = 4;  // D2
int INB = 5;  // D1


void setup(void) {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
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



