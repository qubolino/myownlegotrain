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






#include <Button2.h>

#define BUTTON_1        17
Button2 btn1(BUTTON_1);
#define BUTTON_2        14
Button2 btn2(BUTTON_2);
#define BUTTON_3        12
Button2 btn3(BUTTON_3);


#include "WiFi.h"
#include <esp_now.h>


typedef struct message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} message;

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }


class ControlledDevice {
  private :
    message mess;
    int value_cap;
    int value_multiplier;
    int value_offset;

  public:
    uint8_t* address;
    int value;
    int last_value;

    ControlledDevice(uint8_t* an_address, int a_cap, int a_multiplier, int an_offset) {
      this->last_value = this->value = 0;
      this->address = an_address;
      this->value_cap = a_cap;
      this->value_multiplier = a_multiplier;
      this->value_offset = an_offset;

      strcpy(this->mess.a, "THIS IS A CHAR");
      this->mess.b = this->value;
      this->mess.c = 1.2;
      this->mess.d = "Hello";
      this->mess.e = false;
    }

    void inc_value(){
      this->value = max(min(this->value+1, this->value_cap), -this->value_cap);
      if (this->value != this->last_value) {
        // trigger change
        this->send_value();
        display_value(this->value, this->last_value);
        this->last_value = this->value;
      }
    }
    void dec_value(){
      this->value = max(min(this->value-1, this->value_cap), -this->value_cap);
      if (this->value != this->last_value) {
        // trigger change
        this->send_value();
        display_value(this->value, this->last_value);
        this->last_value = this->value;
      }
    }
    void neutral(){
      this->value = max(min(0, this->value_cap), -this->value_cap);
      if (this->value != this->last_value) {
        // trigger change
        this->send_value();
        display_value(this->value, this->last_value);
        this->last_value = this->value;
      }
    }
    void send_value(){

      int value_to_send = this->value * this->value_multiplier + (this->value>0?1:(this->value<0?-1:0)) * this->value_offset;
      this->mess.b = value_to_send;

      Serial << "sending " << value_to_send << " (value " << this->value << ") "; 

      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(this->address, (uint8_t *) &this->mess, sizeof(this->mess));
      
      Serial << ( (result == ESP_OK)? " OK\n": " ERR\n");
    }
};




// uint8_t ad[] =  {0xCC, 0x50, 0xE3, 0xA1, 0x3F, 0x50}; // DOIT
// ControlledDevice cur_device(ad, 9, 5, 155);

uint8_t ad[] =  {0x24, 0x6F, 0x28, 0x96, 0x4F, 0x54}; // TTGO T-Display
ControlledDevice cur_device(ad, 9, 5, 155);

// 24:A1:60:2E:07:89  WEMOS D1 Mini (1)
// ControlledDevice cur_device(ad, 1, 255, 1);


void stop_handler(Button2& btn) {
    switch (btn.getClickType()) {
        case SINGLE_CLICK:
            cur_device.neutral();
            break;
        // case DOUBLE_CLICK:
        //     Serial.print("double ");
        //     break;
        case TRIPLE_CLICK:
            Serial.println("stopping all devices");
            break;
        case LONG_CLICK:
            cur_device.neutral();
            break;
    }
}


void setup(void) {

  Serial.begin(115200);


  btn1.setPressedHandler([](Button2 & b) {
    cur_device.dec_value();
  });
  btn3.setPressedHandler([](Button2 & b) {
    cur_device.inc_value();
  });


  btn2.setClickHandler(stop_handler);
  btn2.setLongClickHandler(stop_handler);
  btn2.setDoubleClickHandler(stop_handler);
  btn2.setTripleClickHandler(stop_handler);


  WiFi.mode(WIFI_MODE_STA);
  if (esp_now_init() != ESP_OK) {
    Serial << "Error initializing ESP-NOW\n";
    return;
  }
  Serial.print("MAC "); Serial.println(WiFi.macAddress());


  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, cur_device.address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  cur_device.send_value();



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

}
  

void loop() {

  btn1.loop();
  btn2.loop();
  btn3.loop();

}



