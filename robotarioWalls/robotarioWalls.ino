// NYU Shanghai
// Introduction to Robotics
// 2019-11-16
// Robotario

// This code uses NeoPixel  (c) 2013 Shae Erisson released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//use the board definition Attiny85 Internal 8MHz
//upload using programmer once to flash the fuses by pressing shift and clicking on upload

#define FAST    150   //value of the motor speed normal movement
#define SLOW    90   //value of the motor speed slowest movement
#define TCHECK   30  //time in ms to explore 
#define TCHARGE  10   //time in minutes to recharge
#define TSURE    5    //n times that it checked to be on the base
#define TZIGZAG  30   //time between curves for a Zig Zag
#define LIGHT   800   //value from ADC that indicates LDRs under the reflector
#define BAT     512   //value that indicates low battery
#define BASE    200   //value that indicates possible recharging base found
#define WALL    900   //value that indicates wall ahead


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

int state = START; // 0 for starting
int IRsensorValue;
int LDRsensorValue;
int BATsensorValue;
int charging = 0;

int onState = LOW;

unsigned long previousMillis = 0;        // will store last time on movement was updated


// constants won't change:
const long interval = 100;           // interval at which to blink (milliseconds)
const long stepTurn = 500;

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//here we should get ready the watchdog and timer


void setup() {
  pinMode(BAT, INPUT);
  pinMode(LDR, INPUT);
  pinMode(IR, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  analogReference(INTERNAL2V56);
  state = START;
}

void loop() {

  updateStatus();

  //  switch (state) {
  //    case START:
  //      restingAndWaiting();             // wait for light
  //      break;
  //    case RUNAWAY:
  //      woodGo();             // wood  then go
  //      break;
  //    case TURNING:
  //      turn();              // when you sense the black tape, turn to avoid collision with the wall
  //      state = START;
  //      break;
  //    case CONFIRM:               // make sure that it is a base station
  //      confirmCharge();
  //    //  state = START;
  //      break;
  //    case CHARGE:
  //      waitCharge();        // wait and charge
  //      state = START;
  //      break;
  //      //    case REST:
  //      //      //restingAndWaiting();  // resting and waiting for light
  //      //      break;
  //      //    case EXPLORE:
  //      //      break;
  //      //    case RUNAWAY:
  //      //      //runAway();           // when you are exposed to light, wait for a few seconds and then run away
  //      //      break;
  //      //    case LOWBAT:
  //      //      // lowBat();
  //      //      break;
  //
  //
  //      //      //confirmBase();
  //      //    case CHARGED:
  //      //      waitCharge();        // wait and charge
  //      //      break;
  //  }
  //
  //  //  pixels.show();
  //}
}
void updateStatus() {
  IRsensorValue = analogRead(IR); // read the value from the IR sensor
  LDRsensorValue = analogRead(LDR); // read the value from the LDR sensor
  BATsensorValue = analogRead(BAT); // read the value from the BAT sensor


  //  if ((IRsensorValue > 0 && IRsensorValue <= 200) && (LDRsensorValue < 400 )) {
  //    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  //    state = START;
  //  } else (IRsensorValue > 200 &&
  if ( IRsensorValue >= 900) {
    pixels.setPixelColor(0, pixels.Color(100, 0, 0)); // Moderately bright green color.
    pixels.show();
    turn();
  }
  //  } else if ((IRsensorValue <= 400)  && (LDRsensorValue > 400 )) {
  //    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
  //    state = START;
  //  } else if ((IRsensorValue > 400 && IRsensorValue <= 900) && (state != CONFIRM)) {
  //    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
  //    state = CONFIRM;
  //  } else if ((IRsensorValue > 400 && IRsensorValue <= 900) && (state == CONFIRM)) {
  //    pixels.setPixelColor(0, pixels.Color(75, 0, 75)); // Moderately bright purple color.
  //    state = CHARGE;
  else if (IRsensorValue < 900) {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
    pixels.show();
    int n = random ( 0, interval / 2);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= n) {
      previousMillis = currentMillis;
      if (onState == LOW) {
        onState = HIGH;
        analogWrite(M1, FAST);
        analogWrite(M2, 0);
      } else {
        onState = LOW;
        analogWrite(M1, 0);
        analogWrite(M2, FAST);
      }
    }
  }

}


//void woodGo() {         //moves in zig zag
//  //  pixels.setPixelColor(0, pixels.Color(0, 0, 125)); // Moderately bright green color.
//  //  pixels.show();
//  analogWrite(M1, FAST);
//  analogWrite(M2, FAST);
//  // delay (TZIGZAG * random(0, 5));  //we could do this with millis
//  //  //  pixels.setPixelColor(0, pixels.Color(0, 125, 125)); // Moderately bright purple color.
//  //  //  pixels.show();
//  //  analogWrite(M1, FAST );
//  //  analogWrite(M2, SLOW * random(0, 2));
//  // delay (TZIGZAG * random(0, 5));
//}
//
//void restingAndWaiting() {
//  analogWrite(M1,  0);
//  analogWrite(M2, 0 );
//}
//
////void runAway() {
////  analogWrite(M1,  SLOW * random(0, 2));
////  analogWrite(M2, FAST );
////}
//
void turn() {
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(100);
  analogWrite(M1, FAST);
  analogWrite(M2, 0);
  delay(random(100));
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(100);

  //  unsigned long previousMillisStep = millis();
  //  unsigned long currentMillis = millis();
  //  previousMillis = millis();
  //
  //  while (currentMillis - previousMillisStep < stepTurn) {
  //    currentMillis = millis();
  //
  //    if (currentMillis - previousMillis >= interval *3) {
  //      previousMillis = currentMillis;
  //      if (onState == LOW) {
  //        onState = HIGH;
  //        analogWrite(M1, FAST);
  //        analogWrite(M2, 0);
  //      } else {
  //        onState = LOW;
  //        analogWrite(M1, 0);
  //        analogWrite(M2, 0);
  //      }
  //    }
  //  }
}
//
//void findCharge() {
//  //moves
//  //  analogWrite(M1, SLOW * random(0, 3));
//  //  analogWrite(M2, SLOW * random(0, 3));
//  // if it finds the charging marker, it goes to the next state
//}
//
//boolean confirmCharge() { //moves a bit
//  analogWrite(M1, SLOW);
//  analogWrite(M2, SLOW);
//  delay(TCHECK);
//  analogWrite(M1, 0);
//  analogWrite(M2, 0);
//  delay(TCHECK);
//  analogWrite(M1, SLOW);
//  analogWrite(M2, SLOW);
//  delay(TCHECK);
//  analogWrite(M1, 0);
//  analogWrite(M2, 0);
//}
//
//void waitCharge() {
//  analogWrite(M1, 0);
//  analogWrite(M2, 0); //stop motors
//  for (int t = 1; t < 3; t++) {     //stays in this loop
//    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright purple color.
//    pixels.show();
//    delay (1000);
//    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
//    pixels.show();
//    delay (10);
//    //    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
//    //    pixels.show();
//    //    delay (100);
//  }
//}
//void lowBat() {
//  //  pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
//  //  pixels.show();
//  //  charging = 0;
//  //  if (IRsensorValue > BASE) {                      // if the value indicates presence of wall ahead
//  //    // turn();              // when you sense the black tape, turn to avoid collision with the wall
//  //    status = CONFIRM;
//  //  } else {
//  //    findCharge();       //  when you sense the blue tape, (make sure that it is blue), stop
//  //  }
//}
//
//void confirmBase() {
//  //  if ( IRsensorValue > BASE) {
//  //    pixels.setPixelColor(0, pixels.Color(75, 0, 75)); // Moderately bright purple color.
//  //    pixels.show();
//  //    delay (500);
//  //    pixels.setPixelColor(0, pixels.Color(0, 75, 75)); // Moderately bright yellow color.
//  //    pixels.show();
//  //    delay (500);
//  //
//  //    if (charging ++ > TSURE) {
//  //      status = CHARGE;
//  //    }
//  //    confirmCharge(); //move a little bit to confirm position
//  //  } else {
//  //    charging = 0;
//  //    status = LOWBAT; // do we go back to state lowbat?
//  //  }
//}
