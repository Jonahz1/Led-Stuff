#include "FastLED.h"

#define LED_DT 7                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // Are they RGB, GRB or what??
#define LED_TYPE WS2812                                       // Don't forget to change LEDS.addLeds
#define NUM_LEDS 60

int soundSensor = 2; // pin for the spl sensor
struct CRGB leds[NUM_LEDS];  
uint8_t max_bright = 128;

const long interval = 15; // this is the interval for the 'falling' blue leds
unsigned long previousMillis = 0;


const long interval_hs = 100; // interval for the 'falling' red led
unsigned long previousMillis_hs = 0;


//int pos = 0;

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);    // Use this for WS2812B
  FastLED.setBrightness(max_bright); //sets max brightness of led strip
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(9600); // this was used for testing purposes to make sure the sensor was reading correctly

  pinMode(soundSensor, INPUT);
}

int max_led = 0; // the highest point that low reaches
int high_score = 0; // location of the red led 
void loop() {

  // first we want to read the sensor data 100 times
  // and take the highest value read
  // this gets rid of noise and any junk the sensor read
  int temp_Read;
  int total;
  for (int i = 0; i < 100; i++){
    temp_Read = analogRead(A0);
    if (temp_Read>total){
      total = temp_Read;
      delay(1);
    }
  }

  // the value 550 is where no noise is read by the sensor in a quiet setting
  int low = map(total, 550,1000, 0, NUM_LEDS); //this will map the analog output from 550-1200 to 0-NUM_LEDS

  // if low is below 0 then set it -1 which makes the strip of leds 'blank' when the sensor is reading 0
  if (low < 0){
    low = -1;
  }
  
  //Serial.println(max_led, DEC); // this was used to test max_led

  if (low > max_led){
  max_led = low; // keeps track of how the leds highest point
  for (int i = 0; i <= low; i++){
    leds[i] = CRGB::Blue; // we want to light up every led equal up to low
  }}

  // a delay without using delay() 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    leds[max_led] = CRGB::Black; // this makes the leds 'fall' back down
    max_led--; // we want to decrease the number by one to create the 'falling' animation
  }

  // led strip is 'blank' when sensor is reading nothing
  if(max_led <= 0){
    max_led = -1;
  }

  // whenever high_score is less than low, make it equal to low + 1
  if (high_score < low){
  high_score = low+1;
  }

  // delay without delay() but for the high_score variable
  if (currentMillis - previousMillis_hs >= interval_hs){
    previousMillis_hs = currentMillis;
    leds[high_score] = CRGB::Black;
    high_score--;
  }

  // we want the red led on the strip when nothing is playing
  if (high_score < 0){
    high_score = 0;
  }

  // i put this last so it will overwrite any blue leds in its place
  leds[high_score] = CRGB::Red;
  
  FastLED.show(); 
}


// reset function for testing that clears the led strip
void reset(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }

  FastLED.show();
}


