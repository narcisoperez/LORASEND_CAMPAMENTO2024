/*
  Code Base from RadioLib: https://github.com/jgromes/RadioLib/tree/master/examples/SX126x

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>
#include <U8g2lib.h>

#define LoRa_nss 8
#define LoRa_sck 9
#define LoRa_MOSI 10
#define LoRa_MISO 11
#define LoRa_nrst 12
#define LoRa_busy 13
#define LoRa_dio1 14
#define oled_sda 17
#define oled_scl 18
#define oled_rst 21
#define LED 35

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/oled_scl, /* data=*/oled_sda, /* reset=*/oled_rst);
SX1262 radio = new Module(LoRa_nss, LoRa_dio1, LoRa_nrst, LoRa_busy);

void setup() {
  Serial.begin(9600);
  //SPI.begin(LoRa_sck, LoRa_MISO, LoRa_MOSI, LoRa_nss);

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin();
  u8g2.begin();
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop()
{
  Serial.print(F("[SX1262] Transmitting packet ... "));
  int state = radio.transmit("Satélite PUNI1 transmite: Hola ISBH!!!");
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);

  u8g2.firstPage();
  do{
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(5, 20, "SAT PUNI 1");
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 40, "PUNI1 transmite");    
    u8g2.drawStr(10, 60, "Hola ISBH (:");
  } while (u8g2.nextPage());

  if (state == RADIOLIB_ERR_NONE)
  {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

    // print measured data rate
    Serial.print(F("[SX1262] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));
  }
  else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
  {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));
  }
  else if (state == RADIOLIB_ERR_TX_TIMEOUT)
  {
    // timeout occured while transmitting packet
    Serial.println(F("timeout!"));
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // wait for a second before transmitting again
  delay(1);
}