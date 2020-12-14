#include <Arduino.h>
#include "utils.h"

#include <Servo.h>
Servo servo;
// #define SERVO_PIN 2  // D4 ~\o/~
#define SERVO_PIN 13


int target_angle = 0;
int angle = 0;

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <espnow.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <esp_now.h>
#endif

message mess;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&mess, incomingData, sizeof(mess));
  Serial.print("Bytes received: ");
  Serial.println(len);

  target_angle = mess.b;
  Serial.print("target_angle: "); Serial.println(target_angle);

}

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
    // esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);

    Serial.print("Testing servo");
    servo.attach(SERVO_PIN);
    for(int i=1;i < 67;i++){
        servo.write(i);
        Serial.print(".");
        delay(25);
    }
    for(int i=66;i >0;i--){
        servo.write(i);
        Serial.print(".");
        delay(25);
    }
    Serial.println("done");

}

void loop() {
  if (angle != target_angle){
      servo.write(angle);
      angle = target_angle;
      Serial.print("angle: "); Serial.println(angle);
  }
}



