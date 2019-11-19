// NYU Shanghai
// Introduction to Robotics
// 2019-11-16
// Robotario

// This code uses NeoPixel  (c) 2013 Shae Erisson released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//use the board definition Attiny85 Internal 8MHz
//upload using programmer once to flash the fuses by pressing shift and clicking on upload

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define LED           4  //Neopixel data pin
#define BAT           0  //ADC0, voltage divider to sense the battery
#define LDR           1  //ADC1, light sensor
#define IR            3  //ADC3, infrared sensor
#define M1            0  //motor 1
#define M2            1  //motor 2
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

void setup() {

  // End of trinket special code
  pinMode(BAT, INPUT);
  pinMode(LDR, INPUT);
  pinMode(IR, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  analogReference(INTERNAL2V56); //in order to asure readings across the different battery levels

}

void loop() {

  pixels.setPixelColor(0, pixels.Color(0, 0, 150)); //  bright blue color.
  pixels.show();
  delay(100);
  pixels.setPixelColor(0, pixels.Color(250, 0, 0)); // very bright red color.
  pixels.show();
  delay(100);
  pixels.setPixelColor(0, pixels.Color(0, 250, 0)); // very bright green color.
  pixels.show();
  delay(100);
  pixels.setPixelColor(0, pixels.Color(0, 0, 250)); // very bright green color.
  pixels.show();
  delay(100);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
  pixels.show();
  delay(100);
}
