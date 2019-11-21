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

#define PIN            4  //Neopixel data pin               (pin 3 on DIP8)
#define BAT            0  //ADC0, battery voltage divider   (pin 1 on DIP8)
#define LDR            1  //ADC1, light sensor              (pin 7 on DIP8)
#define IR             3  //ADC3, infrared sensor           (pin 2 on DIP8)
#define M1             0  //motor 1                         (pin 5 on DIP8)
#define M2             1  //motor 2                         (pin 6 on DIP8)
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; // delay for half a second
int status = 0; // 0 for starting
// 1 for resting and waiting for light
// 2 for exploring light
// 3 for running away
// 4 to avoid a collision with a wall
// 5 with low battery and looking for a charging station
// 6 confirming it's a charging station
// 7 charging
void setup() {

  // End of trinket special code
  pinMode(BAT, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  analogReference(INTERNAL2V56);
  status = 6;
}
void loop() {
  int sensorValue = analogRead(IR); // read the value from the IR sensor

  if (sensorValue > 0 && sensorValue <= 100) {
    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  } else if (sensorValue > 100 && sensorValue <= 200) {
    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
  } else if (sensorValue > 200 && sensorValue <= 300) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
  } else if (sensorValue > 300 && sensorValue <= 400) {
    pixels.setPixelColor(0, pixels.Color(150, 0, 0)); // bright red color.
  } else if (sensorValue > 400 && sensorValue <= 500) {
    pixels.setPixelColor(0, pixels.Color(0, 150, 0)); //  bright green color.
  } else if (sensorValue > 500 && sensorValue <= 600) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 150)); //  bright blue color.
  } else if (sensorValue > 600 && sensorValue <= 700) {
    pixels.setPixelColor(0, pixels.Color(250, 0, 0)); // very bright red color.
  } else if (sensorValue > 700 && sensorValue <= 800) {
    pixels.setPixelColor(0, pixels.Color(0, 250, 0)); // very bright green color.
  } else if (sensorValue > 800 && sensorValue <= 900) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 250)); // very bright green color.
  } else if (sensorValue > 900) {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
  }
  pixels.show();
  delay(10);
}
