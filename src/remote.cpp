#include <Arduino.h>

// #define TFT

#ifdef TFT
/*  
 Test the tft.print() viz embedded tft.write() function
 This sketch used font 2, 4, 7
 
 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.
 Note that yield() or delay(0) must be called in long duration for/while
 loops to stop the ESP8266 watchdog triggering.
 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI display = TFT_eSPI(135, 240); // Invoke custom library

#define TFT_GREY 0x5AEB // New colour
#define TFT_BL          4  // Display backlight control pin

#define TFT_BACKLIGHT_ON HIGH

#else
//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
#endif

void display_value(int val){

    val = min(9, max(-9, val));

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("   "); display.println(abs(val));


    display.setTextSize(1);
    if(val > 0){
      display.print(" ");
      for (int i=9;i>val;i--){
        display.print(" ");
      }
      for(int j=val; j>0; j--){
        display.print("<");
      }
    }
    else if (val < 0){
      display.print("           ");
      for(int i=0;i>val;i--){
        display.print(">");
      }
    }

    display.display();
}


#define VBAT_PIN 34
float v_bat;  // battery voltage from ESP32 ADC read
#define LED_BLUE 2

#include <Button2.h>

#define BUTTON_1        17
Button2 btn1(BUTTON_1);


#include "WiFi.h"
#include <esp_now.h>

// uint8_t remoteAddress[] = {0x24, 0x6F, 0x28, 0x96, 0x4F, 0x54}; // TTGO T-Display
uint8_t remoteAddress[] = {0x8C, 0xAA, 0xB5, 0x86, 0x87, 0xE4}; // TTGO LoRa
// uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA8, 0x3D, 0x94}; // DOIT
// uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0x95, 0xA3, 0xA0}; // DOIT
uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA1, 0x3F, 0x50}; // DOIT

typedef struct message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} message;
message mess;

void send(int a_speed){
    // Set values to send
  strcpy(mess.a, "THIS IS A CHAR");
  mess.b = a_speed;
  mess.c = 1.2;
  mess.d = "Hello";
  mess.e = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(trainAddress, (uint8_t *) &mess, sizeof(mess));
   
  if (result == ESP_OK) {
    // Serial.println("Sent with success");
    digitalWrite(LED_BLUE, 1);
    delay(100);
    digitalWrite(LED_BLUE, 0);

  }
  else {
    Serial.println("Error sending the data");
  }
}


// Rotary Encoder 
 #define PIN_A 13
 #define PIN_B 14

#include <ESP32Encoder.h>
ESP32Encoder encoder;



int last, value;


void setup(void) {

  Serial.begin(115200);

#ifdef TFT
  tft.init();
  tft.setRotation(1);

  if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
  }
#else
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Initializing...");
  display.display();
  
#endif


  pinMode(VBAT_PIN, INPUT);
  pinMode(LED_BLUE, OUTPUT);

  btn1.setPressedHandler([](Button2 & b) {
	  encoder.setCount(0);
    last = value = 0;
    send(value);
    display_value(value/2);
  });

  WiFi.mode(WIFI_MODE_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  display.print("MAC "); display.println(WiFi.macAddress());
  display.display();


  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, trainAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  send(0);


	encoder.attachHalfQuad(PIN_A,PIN_B);
}
  

void loop() {

  value = encoder.getCount();
  if (value != last) {
    last = value;

    // value -20 --> 20
    value =  min(20,max(-20, value));
    encoder.setCount(value);

    int speed = -5 * value - (value>0?1:(value<0?-1:0)) * 155;


    send(speed);
    display_value(value/2);

  }
  
  btn1.loop();

}



