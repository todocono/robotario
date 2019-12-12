#define FAST    140     //value of the motor speed normal movement
#define SLOW    100     //value of the motor speed slowest movement
#define TCHECK   30     //time in ms to explore 
#define TCHARGE  10     //time in minutes to recharge
#define TSURE    5      //n times that it checked to be on the base
#define TZIGZAG 100     //time between curves for a Zig Zag
#define TEXPLORE 1500   //time between curves for a Zig Zag
#define TRUN   3000     //time between curves for a Zig Zag
#define TSLEEPS  12     //times it goes to sleep between breathing    

#define LIGHT   900     //value from ADC that indicates LDRs under the reflector
#define BAT     512     //value that indicates low battery
#define BASE    200     //value that indicates possible recharging base found
#define WALL    900     //value that indicates wall ahead


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
int sleeps = 0;
int turns = 0;


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

#include <avr/sleep.h>

// Routines to set and claer bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() {
  pinMode(BAT, INPUT);
  pinMode(LDR, INPUT);
  pinMode(IR, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  analogReference(INTERNAL2V56);
  state = START;

  setup_watchdog(5); // approximately 8 seconds sleep
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
    case EXPLORE:
      woodGo();
      break;
  }
}

void updateSensors() {
  IRsensorValue = analogRead(IR); // read the value from the IR sensor
  LDRsensorValue = analogRead(LDR); // read the value from the LDR sensor
  BATsensorValue = analogRead(BAT); // read the value from the BAT sensor
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
    turns = 0;
    state = TURN;
  }
}

void restingAndWaiting() {
  analogWrite(M1,  0);
  analogWrite(M2, 0 );
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // off
  pixels.show();
  delay(10);
  if ( sleeps ++ >= TSLEEPS) {
    breathe();
    sleeps = 0;
  }
  if (LDRsensorValue < LIGHT) {     // sense light to reactivate (it should go to sleep)
    stepMillis = millis();
    state = RUN;
  } else {
    system_sleep();  // Send the unit to sleep
  }
}

void runAway() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= TZIGZAG * 10) {
    previousMillis = currentMillis;
    if (onState == LOW) {
      pixels.setPixelColor(0, pixels.Color(25, 0, 40));
      pixels.show();
      onState = HIGH;
      analogWrite(M1, FAST);
      analogWrite(M2, 0);
    } else {
      pixels.setPixelColor(0, pixels.Color(40, 25, 0));
      pixels.show();
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
    turns = 0;
  }
}

void turning() {
  pixels.setPixelColor(0, pixels.Color(45, 0, 0)); // light red
  pixels.show();
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(100);
  if (turns < 10) {
    analogWrite(M1, FAST);
    analogWrite(M2, 0);
  } else {
    analogWrite(M1, 0);
    analogWrite(M2, FAST);
  }
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
  }
}
void lowBat() {

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



// set system into the sleep state
// system wakes up when wtchdog is timed out
void system_sleep() {
  cbi(ADCSRA, ADEN);                   // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System actually sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out
  sbi(ADCSRA, ADEN);                   // switch Analog to Digitalconverter ON
}

void breathe() {
  int flag1 = 0;
  int d = random(2, 10);
  for (int i = 0; i < random(0, 5); i++) {
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
      pixels.setPixelColor(0, pixels.Color(fadeValue, fadeValue, fadeValue)); // Moderately bright purple color.
      pixels.show();
      if (flag1 ==0) {
        analogWrite(M1, SLOW);
      }
      delay(d);
      }
      analogWrite(M1, 0);
      
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      pixels.setPixelColor(0, pixels.Color(fadeValue, fadeValue, fadeValue)); // Moderately bright purple color.
      pixels.show();
            if (flag1 == 0) {
        analogWrite(M2, SLOW);
      }
      delay(d);
    }
    analogWrite(M2, 0);
    flag1++;
  }
}

void setup_watchdog(int ii) {
  // if ii 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
  // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
  byte bb;
  int ww;
  if (ii > 9 ) ii = 9;
  bb = ii & 7;
  if (ii > 7) bb |= (1 << 5);
  bb |= (1 << WDCE);
  ww = bb;

  MCUSR &= ~(1 << WDRF);
  WDTCR |= (1 << WDCE) | (1 << WDE);  // start timed sequence
  WDTCR = bb;          // set new watchdog timeout value
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  //do nothing f_wdt = 1; // set global flag

}
