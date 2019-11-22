// NYU Shanghai
// Introduction to Robotics
// 2019-11-16
// Robotario

// This code uses NeoPixel  (c) 2013 Shae Erisson released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//use the board definition Attiny85 Internal 8MHz
//upload using programmer once to flash the fuses by pressing shift and clicking on upload

#define FAST    140   //value of the motor speed normal movement
#define SLOW    100   //value of the motor speed slowest movement
#define TCHECK   30  //time in ms to explore 
#define TCHARGE  10   //time in minutes to recharge
#define TSURE    5    //n times that it checked to be on the base
#define TZIGZAG 100   //time between curves for a Zig Zag
#define TEXPLORE 3000   //time between curves for a Zig Zag
#define TRUN   3000   //time between curves for a Zig Zag

#define LIGHT   400   //value from ADC that indicates LDRs under the reflector
#define BAT     512   //value that indicates low battery
#define BASE    200   //value that indicates possible recharging base found
#define WALL    900   //value that indicates wall ahead


#define START   0     // starting state of the device
#define REST    1     // for resting and waiting for light
#define EXPLORE 2     // after detecting light, it expores around
#define RUN     3     // for running away
#define TURN    6     // turns away from the walls to avoid a collision
#define LOWBAT  5     // with low battery and looking for a charging station
#define CONFIRM 6     // confirming it's a charging station
#define CHARGING  7    // it's charging it's battery
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
unsigned long stepMillis = 0;        // will store last time on that step of state


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

  updateSensors();

  switch (state) {
    case START:
      stepMillis = millis();
      state = EXPLORE;
      break;
    case REST:
      restingAndWaiting();             // wait for light
      break;
    case RUN:
      runAway();             // wood  then go
      break;
    case TURN:
      turning();              // when you sense the black tape, turn to avoid collision with the wall
      break;
    //  case CONFIRM:               // make sure that it is a base station
    //  confirmCharge();
    //  state = START;
    //  break;
    // case CHARGE:
    //  waitCharge();        // wait and charge
    // state = START;
    // break;
    //    case REST:
    //      //restingAndWaiting();  // resting and waiting for light
    //      break;
    case EXPLORE:
      woodGo();
      break;
      //    case RUNAWAY:
      //      //runAway();           // when you are exposed to light, wait for a few seconds and then run away
      //      break;
      //    case LOWBAT:
      //      // lowBat();
      //      break;


      //      //confirmBase();
      //    case CHARGED:
      //      waitCharge();        // wait and charge
      //      break;
  }

  //  pixels.show();
}

void updateSensors() {
  IRsensorValue = analogRead(IR); // read the value from the IR sensor
  LDRsensorValue = analogRead(LDR); // read the value from the LDR sensor
  BATsensorValue = analogRead(BAT); // read the value from the BAT sensor


  //  if ((IRsensorValue > 0 && IRsensorValue <= 200) && (LDRsensorValue < 400 )) {
  //    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright red color.
  //    state = START;
  //  } else (IRsensorValue > 200 &&
  //  if ( (IRsensorValue <= 400)  && (LDRsensorValue <= 400 ) && (state == REST)) {
  //    pixels.setPixelColor(0, pixels.Color(75, 0, 0)); // Moderately bright green color.
  //    state = RUNAWAY;
  //  } else if ((IRsensorValue <= 400)  && (LDRsensorValue > 400 )  && (state == REST)) {
  //    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
  //    state = START;
  //  } else if ((IRsensorValue > 400 && IRsensorValue <= 900) && (state != CONFIRM)) {
  //    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
  //    state = CONFIRM;
  //  } else if ((IRsensorValue > 400 && IRsensorValue <= 900) && (state == CONFIRM)) {
  //    pixels.setPixelColor(0, pixels.Color(75, 0, 75)); // Moderately bright purple color.
  //    state = CHARGE;
  //  } else if (IRsensorValue > 900) {
  //    pixels.setPixelColor(0, pixels.Color(25, 0, 0)); // Moderately bright green color.
  //    state = TURNING;
  //  }
  // pixels.show();
}

void woodGo() {         //moves in zig zag
  pixels.setPixelColor(0, pixels.Color(0, 25, 0)); // Moderately bright green color.
  pixels.show();
  //int n = random ( interval / 2, interval );
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= TZIGZAG) {
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
  if ( currentMillis  - stepMillis > TEXPLORE) {
    stepMillis = currentMillis;
    state = REST;
  }
  if (IRsensorValue > WALL) {
    stepMillis = currentMillis;
    state = TURN;
  }
}

void restingAndWaiting() {
  analogWrite(M1,  0);
  analogWrite(M2, 0 );
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // off
  pixels.show();


  if (LDRsensorValue < LIGHT) {     // sense light to reactivate (it should go to sleep)
    stepMillis = millis();
    state = RUN;
  }
  //  if ( currentMillis  - stepMillis > TEXPLORE) {
  //    stepMillis = millis();
  //    state = REST;
  //  }
}

void runAway() {
  pixels.setPixelColor(0, pixels.Color(0, 0, 125)); // blue
  pixels.show();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= TZIGZAG * 10) {
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
  if ( currentMillis  - stepMillis > TRUN) {
    stepMillis = currentMillis;
    state = EXPLORE;
  }
  if (IRsensorValue > WALL) {
    stepMillis = currentMillis;
    state = TURN;
  }
}

void turning() {
  pixels.setPixelColor(0, pixels.Color(45, 0, 0)); // light red
  pixels.show();
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(100);
  analogWrite(M1, FAST);
  analogWrite(M2, 0);
  delay(random(50, 200));
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(100);
  state = START;
}

void findCharge() {
  //moves
  //  analogWrite(M1, SLOW * random(0, 3));
  //  analogWrite(M2, SLOW * random(0, 3));
  // if it finds the charging marker, it goes to the next state
}

boolean confirmCharge() { //moves a bit
  analogWrite(M1, SLOW);
  analogWrite(M2, SLOW);
  delay(TCHECK);
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(TCHECK);
  analogWrite(M1, SLOW);
  analogWrite(M2, SLOW);
  delay(TCHECK);
  analogWrite(M1, 0);
  analogWrite(M2, 0);
}

void waitCharge() {
  analogWrite(M1, 0);
  analogWrite(M2, 0); //stop motors
  for (int t = 1; t < 3; t++) {     //stays in this loop
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright purple color.
    pixels.show();
    delay (1000);
    pixels.setPixelColor(0, pixels.Color(0, 75, 0)); // Moderately bright green color.
    pixels.show();
    delay (10);
    //    pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
    //    pixels.show();
    //    delay (100);
  }
}
void lowBat() {
  //  pixels.setPixelColor(0, pixels.Color(0, 0, 75)); // Moderately bright blue color.
  //  pixels.show();
  //  charging = 0;
  //  if (IRsensorValue > BASE) {                      // if the value indicates presence of wall ahead
  //    // turn();              // when you sense the black tape, turn to avoid collision with the wall
  //    status = CONFIRM;
  //  } else {
  //    findCharge();       //  when you sense the blue tape, (make sure that it is blue), stop
  //  }
}

void confirmBase() {
  //  if ( IRsensorValue > BASE) {
  //    pixels.setPixelColor(0, pixels.Color(75, 0, 75)); // Moderately bright purple color.
  //    pixels.show();
  //    delay (500);
  //    pixels.setPixelColor(0, pixels.Color(0, 75, 75)); // Moderately bright yellow color.
  //    pixels.show();
  //    delay (500);
  //
  //    if (charging ++ > TSURE) {
  //      status = CHARGE;
  //    }
  //    confirmCharge(); //move a little bit to confirm position
  //  } else {
  //    charging = 0;
  //    status = LOWBAT; // do we go back to state lowbat?
  //  }
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
