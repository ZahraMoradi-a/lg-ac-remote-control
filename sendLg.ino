
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4; 
const uint16_t buttonpin = D3;
bool isON = false;
bool buttonstate;
bool lastbuttonstate = HIGH;
unsigned long lastdebouncetime = 0;
const unsigned long debouncedelay = 50;
IRsend irsend(kIrLed); 
uint16_t rawDataOFF[59] = {3124, 9924,  510, 1572,  484, 558,  482, 584,  458, 566,  484, 1596,  480, 576,  458, 576,  458, 564,  506, 1578,  480, 1632,  484, 550,  458, 584,  458, 560,  482, 554,  508, 528,  480, 560,  482, 576,  458, 560,  508, 550,  460, 576,  458, 552,  508, 1602,  458, 584,  460, 1624,  484, 528,  480, 584,  460, 584,  458, 1588,  482};
uint16_t rawDataON[59] = {3182, 9846,  544, 1572,  492, 552,  490, 544,  490, 544,  492, 1558,  518, 524,  520, 516,  518, 526,  516, 524,  518, 524,  518, 522,  518, 518,  516, 550,  490, 532,  518, 526,  516, 524,  520, 1558,  520, 526,  516, 542,  492, 526,  516, 520,  516, 1558,  520, 522,  520, 528,  514, 1574,  518, 1580,  496, 548,  494, 540,  494};  

uint8_t samsungState[kSamsungAcStateLength] = {
    0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
    0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

void setup() {
  pinMode(buttonpin, INPUT_PULLUP);
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  
  Serial.begin(115200, SERIAL_8N1);
#endif  
}

void loop() {
  bool reading = digitalRead(buttonpin);
  if(reading != lastbuttonstate){
    lastdebouncetime = millis();
  }
  if((millis()- lastdebouncetime) > debouncedelay){
    if(reading != buttonstate){
      buttonstate = reading;
      if(buttonstate == LOW){
        isON =! isON;

        if(isON){
          Serial.println("ON");
          irsend.sendRaw(rawDataON, 59, 38);  
        } else{
            Serial.println("OFF");
            irsend.sendRaw(rawDataOFF, 59, 38);
        }
      }
    }
  }
  lastbuttonstate = reading;
}
