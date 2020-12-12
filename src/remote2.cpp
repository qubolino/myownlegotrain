#include <Arduino.h>
#include <Wire.h>

#define SSD1306

#if defined(ST7735)
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS 16
#define TFT_RST -1  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to -1!
#define TFT_DC 17

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 5   // set these to be whatever pins you like!
#define TFT_MOSI 23   // set these to be whatever pins you like!
Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

#define TFT_BL      27

#elif defined(SSD1306)
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

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

#endif


#include <Button2.h>

#define BUTTON_1        17
Button2 btn1(BUTTON_1);
#define BUTTON_2        14
Button2 btn2(BUTTON_2);
#define BUTTON_3        12
Button2 btn3(BUTTON_3);


#include "WiFi.h"
#include <esp_now.h>

// uint8_t remoteAddress[] = {0x24, 0x6F, 0x28, 0x96, 0x4F, 0x54}; // TTGO T-Display
// uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA8, 0x3D, 0x94}; // DOIT DEAD
// uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0x95, 0xA3, 0xA0}; // DOIT DEAD
// uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA1, 0x3F, 0x50}; // DOIT
// future remote? 24:62:AB:DD:CD:10 LILYGO TS
// uint8_t switchAddress[] =  {0x8C, 0xCE, 0x4E, 0xC7, 0xAF, 0xB0}; // 8C:CE:4E:C7:AF:B0 ESP8266 `wemos w/ battery

#define TRAIN

#if defined(TRAIN)
uint8_t trainAddress[] =  {0xCC, 0x50, 0xE3, 0xA1, 0x3F, 0x50}; // DOIT
// uint8_t trainAddress[] = {0x24, 0x6F, 0x28, 0x96, 0x4F, 0x54}; // TTGO T-Display
#elif defined(SWITCH)
// uint8_t trainAddress[] =  {0x8C, 0xCE, 0x4E, 0xC7, 0xAF, 0xB0}; // 8C:CE:4E:C7:AF:B0 ESP8266 `wemos w/ battery
uint8_t trainAddress[] =  {0x24, 0x6F, 0x28, 0x96, 0x4F, 0x54}; // TTGO T-Display
#endif

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
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}


int last, value;


void display_value(int value, int real_value) {
#if defined(ST7735)
  screen.fillScreen(ST7735_BLACK);
#elif defined(SSD1306)
  screen.clearDisplay();
#endif

  screen.setCursor(0, 0);
  screen.setTextSize(3);
  screen.print("   "); screen.println(abs(value));


  screen.setTextSize(1);
  if(value < 0){
    screen.print(" ");
    for (int i=-9;i<value;i++){
      screen.print(" ");
    }
    for(int j=value; j<0; j++){
      screen.print("<");
    }
  }
  else if (value > 0){
    screen.print("           ");
    for(int i=0;i<value;i++){
      screen.print(">");
    }
  }

  screen.println();
  screen.println(real_value);

  screen.display();

}

void setup(void) {

  Serial.begin(115200);


  btn1.setPressedHandler([](Button2 & b) {
    Serial.println("button 1 --> --");
    value--;
  });
  btn3.setPressedHandler([](Button2 & b) {
    value++;
    Serial.println("button 3 -> ++");
  });

  btn2.setPressedHandler([](Button2 & b) {
    value = 0;
    Serial.println("button 2 -> 0");
  });

  WiFi.mode(WIFI_MODE_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print("MAC "); Serial.println(WiFi.macAddress());


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



#if defined(ST7735)
  // Use this initializer if you're using a 1.8" TFT
  // screen.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 1.44" TFT
  screen.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  //screen.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display

  Serial.println("Initialized");

    if (TFT_BL > 0) {
        pinMode(TFT_BL, OUTPUT);
        digitalWrite(TFT_BL, HIGH);
    }


  uint16_t time = millis();
  screen.fillScreen(ST7735_BLACK);
  time = millis() - time;

#elif defined(SSD1306)
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!screen.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  screen.clearDisplay();

#endif


  screen.setCursor(0, 0);
  screen.setTextColor(WHITE);
  screen.println("Hello Dear");
  screen.println("Initialized");
  screen.print("MAC "); screen.println(WiFi.macAddress());
  screen.display();

  // sleep(2000);





}
  

void loop() {

  
  btn1.loop();
  btn2.loop();
  btn3.loop();

  if (value != last) {
    Serial.println(value);
    last = value;

#if defined(TRAIN)
    // value -20 --> 20
    value =  min(9,max(-9, value));
    int speed = 5 * value + (value>0?1:(value<0?-1:0)) * 155;
#elif defined(SWITCH)
    value =  min(1,max(0, value));
    int speed = 65*value+1;
#endif

    send(speed);
    display_value(value, speed);

  }

}



