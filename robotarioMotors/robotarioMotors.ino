// NYU Shanghai
// Introduction to Robotics
// 2019-11-16
// Robotario

// This code uses NeoPixel  (c) 2013 Shae Erisson released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//use the board definition Attiny85 Internal 8MHz
//upload using programmer once to flash the fuses by pressing shift and clicking on upload

#define FAST    200    //value of the motor speed normal movement
#define SLOW    100    //value of the motor speed slowest 
#define TCHECK   100   //time in ms to explore 
#define TCHARGE  10    //time in minutes to recharge
#define TSURE    10    //n times that it checked to be on the base
#define TZIGZAG  30   //time between curves for a Zig Zag
#define LIGHT   800   //value from ADC that indicates LDRs under the reflector
#define LOWBAT  512  //value that indicates low battery
#define BASE    200  //value that indicates possible recharging base found
#define WALL    600  //value that indicates wall ahead


#define START   0     // starting state of the device
#define REST    1     // for resting and waiting for light
#define EXPLORE 2     // after detecting light, it expores around
#define RUNAWAY 3     // for running away
#define TURNING 4     // turns away from the walls to avoid a collision
#define LOWBAT  5     // with low battery and looking for a charging station
#define CONFIRM 6     // confirming it's a charging station
#define CHARGE  7    // it's charging it's battery
#define CHARGED 8    // it's fully charged and it should go to a dark spot

#define PIN            4  //Neopixel data pin               (pin 3 on DIP8)
#define BAT            0  //ADC0, battery voltage divider   (pin 1 on DIP8)
#define LDR            1  //ADC1, light sensor              (pin 7 on DIP8)
#define IR             3  //ADC3, infrared sensor           (pin 2 on DIP8)
#define M1             0  //motor 1                         (pin 5 on DIP8)
#define M2             1  //motor 2                         (pin 6 on DIP8)

#define NUMPIXELS      1 // only one LED 

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(BAT, INPUT);
  pinMode(LDR, INPUT);
  pinMode(IR, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  analogReference(INTERNAL2V56);
}

void loop() {
  //moves in zig zag
  analogWrite(M1, SLOW * random(0, 2));
  analogWrite(M2, FAST );
  delay (TZIGZAG * random(0,5));
  analogWrite(M1, FAST );
  analogWrite(M2, SLOW * random(0, 2));
  delay (TZIGZAG * random(0,5));
}
