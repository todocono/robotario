// NYU Shanghai
// Introduction to Robotics
// 2019-11-16
// Robotario

// This code uses NeoPixel  (c) 2013 Shae Erisson released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//use the board definition Attiny85 Internal 8MHz
//upload using programmer once to flash the fuses by pressing shift and clicking on upload

#define FAST    200   //value of the motor speed normal movement
#define SLOW    100   //value of the motor speed slowest movement
#define TCHECK   30  //time in ms to explore 
#define TCHARGE  10   //time in minutes to recharge
#define TSURE    5    //n times that it checked to be on the base
#define TZIGZAG  30   //time between curves for a Zig Zag
#define LIGHT   800   //value from ADC that indicates LDRs under the reflector
#define LOWBAT  512   //value that indicates low battery
#define BASE    700   //value that indicates possible recharging base found
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

int status = START; // 0 for starting

int charging = 0;


#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//here we should get ready the watchdog and timer




void setup() {
  pinMode(BAT, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  analogReference(INTERNAL2V56);
  status = LOWBAT;
}

void loop() {
  int IRsensorValue = analogRead(IR); // read the value from the IR sensor
  int LDRsensorValue = analogRead(LDR); // read the value from the LDR sensor
  int BATsensorValue = analogRead(BAT); // read the value from the LDR sensor

  if ( IRsensorValue > WALL ) {
    status = TURNING;
  }

  switch (status) {
    case START:
      // starting
      // if (IRsensorValue > 0 && IRsensorValue <= 200) {}
      woodGo();             // wood  then go
    case REST:
      restingAndWaiting();  // resting and waiting for light
      break;
    case EXPLORE:
      break;
    case RUNAWAY:
      runAway();           // when you are exposed to light, wait for a few seconds and then run away
      break;
    case TURNING:
      turn();              // when you sense the black tape, turn to avoid collision with the wall
      status = LOWBAT;  //to test if it recharges
      break;
    case LOWBAT:
      pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
      pixels.show();
      charging = 0;
      if (IRsensorValue > BASE) {                      // if the value indicates presence of wall ahead
        // turn();              // when you sense the black tape, turn to avoid collision with the wall
        status = CONFIRM;
      } else {
        findCharge();       //  when you sense the blue tape, (make sure that it is blue), stop
      }
      break;
    case CONFIRM:               // make sure that it is blue

      if ( IRsensorValue > BASE) {
        pixels.setPixelColor(0, pixels.Color(75, 0, 75)); // Moderately bright purple color.
        pixels.show();
        delay (500);
        pixels.setPixelColor(0, pixels.Color(0, 75, 75)); // Moderately bright yellow color.
        pixels.show();
        delay (500);

        if (charging ++ > TSURE) {
          status = CHARGE;
        }
        confirmCharge(); //move a little bit to confirm position
      } else {
        charging = 0;
        status = LOWBAT; // do we go back to state lowbat?
      }
      break;
    case CHARGE:
      waitCharge();        // wait and charge
      break;
    case CHARGED:
      waitCharge();        // wait and charge
      break;
  }
  pixels.show();
}

void woodGo() {
  analogWrite(M1, 100);
  analogWrite(M2, 80);
}

void restingAndWaiting() {
}

void runAway() {
}

void turn() {
  pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Moderately bright purple color.
  pixels.show();
  int n = round(random(1, 2));  //TODO: sometimes turn one way, sometimes the other
  analogWrite(M1, FAST);
  analogWrite(M2, 0); //(n - 2)*FAST);                         //                  { turn
  delay(500 + (random(500)));                                   //                    turn for 500 milliseconds }
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright purple color.
  pixels.show();
}

void findCharge() {
  //moves
  analogWrite(M1, SLOW * random(0, 3));
  analogWrite(M2, SLOW * random(0, 3));
  // if it finds the charging marker, it goes to the next state
}

boolean confirmCharge() {
  //moves a bit
  analogWrite(M1, SLOW);
  analogWrite(M2, SLOW);                            //                  { slow both motors down by changing speed from 100 to 70
  delay(TCHECK);
  analogWrite(M1, 0);
  analogWrite(M2, 0);                            //                  { slow both motors down by changing speed from 100 to 70
  //if it has been long enough in the marker, it moves to the next state
  //  delay (10);

}

void waitCharge() {
  for (int t = 1; t < 100; t++) {     //stays in this loop
    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
    pixels.show();
    delay (1000);
    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
    pixels.show();
    delay (1000);
    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
    pixels.show();
    delay (1000);
  }
}
/*
  if (IRsensorValue > 200 && IRsensorValue <= 400) {     // if the value indicates charging station
    analogWrite(M1, 70);
    analogWrite(M2, 70);                            //                  { slow both motors down by changing speed from 100 to 70
    delay(10);
  }                                     //                    allows the motors to move a little ahead }
  }
  /*
  void waitCharge() {
  pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  pixels.show();
  delay (10000);
  }
  }
  /*
  int sensorValue = analogRead(IR); // read the value from the sensor
  if (sensorValue > 0 && sensorValue <=200){               // wood  then go
    analogWrite(M1, 100);
    analogWrite(M2, 80);
      }
  else if (sensorValue > 200 && sensorValue <=400){       // blue tape then stops
    analogWrite(M1, 0);
    analogWrite(M2, 0);
    }
  else if (sensorValue > 900){                            // black tape then turn
    analogWrite(M1, 0);
    analogWrite(M2, 100);
    delay(500);
  }  }
  /*
  if (sensorValue > 0 && sensorValue <= 100){
    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  }else if (sensorValue > 100 && sensorValue <= 200){
    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
  }else if (sensorValue > 200 && sensorValue <= 300){
    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
  }else if (sensorValue > 300 && sensorValue <= 400){
    pixels.setPixelColor(0, pixels.Color(0, 0, 150)); // bright blue color.
  }else if (sensorValue > 400 && sensorValue <= 500){
    pixels.setPixelColor(0, pixels.Color(0, 150, 0)); // Moderately bright green color.
  }else if (sensorValue > 500 && sensorValue <= 600){
    pixels.setPixelColor(0, pixels.Color(0, 0, 150)); // Moderately bright green color.
  }else if (sensorValue > 600 && sensorValue <= 700){
    pixels.setPixelColor(0, pixels.Color(250, 0, 0)); // Moderately bright green color.
  }else if (sensorValue > 700 && sensorValue <= 800){
    pixels.setPixelColor(0, pixels.Color(0, 250, 0)); // Moderately bright green color.
  }else if (sensorValue > 800 && sensorValue <= 900){
    pixels.setPixelColor(0, pixels.Color(0, 0, 250)); // Moderately bright green color.
  }else if (sensorValue > 900){
    pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
  }
  pixels.show();
  delay(10);
  }
  /* digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  pixels.show();
  delay(100);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
  pixels.show();
  delay(100);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright green color.
  pixels.show();
  delay(100);
  /*
   int sensorValue = analogRead(BAT); // read the value from the sensor
  if (sensorValue > 0 && sensorValue <= 100){
    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  }else if (sensorValue > 100 && sensorValue <= 200){
    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
  }else if (sensorValue > 200 && sensorValue <= 300){
    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
  }else if (sensorValue > 300 && sensorValue <= 400){
    pixels.setPixelColor(0, pixels.Color(0, 0, 150)); // Moderately bright ... color.
  }else if (sensorValue > 400 && sensorValue <= 500){
    pixels.setPixelColor(0, pixels.Color(0, 150, 0)); // Moderately bright green color.
  }else if (sensorValue > 500 && sensorValue <= 600){
    pixels.setPixelColor(0, pixels.Color(0, 0, 150)); // Moderately bright green color.
  }else if (sensorValue > 600 && sensorValue <= 700){
    pixels.setPixelColor(0, pixels.Color(250, 0, 0)); // Moderately bright green color.
  }else if (sensorValue > 700 && sensorValue <= 800){
    pixels.setPixelColor(0, pixels.Color(0, 250, 0)); // Moderately bright green color.
  }else if (sensorValue > 800 && sensorValue <= 900){
    pixels.setPixelColor(0, pixels.Color(0, 0, 250)); // Moderately bright green color.
  }else if (sensorValue > 900){
    pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
  }
*/
