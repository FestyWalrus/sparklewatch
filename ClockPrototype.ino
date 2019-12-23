#include <Adafruit_NeoPixel.h>
#include <ButtonEvents.h>
#include "RTClib.h"

#define LED_PIN 2

#define LED_COUNT 12

int currentHour = 0;
int minuteOnes = 0;
int upButtonPin = 4;
int downButtonPin = 3;
const int baseBrightness = 3;
const int maxBrightness = 127;

enum modes{sleep, showtime, showdate, flashlight, sethours, setmin5s, setmin1s, setmonth, setday, setyear};
int currentMode = sleep;

ButtonEvents upButton;
ButtonEvents downButton;

RTC_DS3231 rtc;

Adafruit_NeoPixel clockLights(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void clearStrip(){
  for(int i = 0; i <=LED_COUNT; i++){
    clockLights.setPixelColor(i, clockLights.Color(0, 0, 0));
  }
  clockLights.show();
 }

void showTime(){
  DateTime now = rtc.now();
  Serial.print(now.hour());
  Serial.print(":");
  Serial.println(now.minute());
  if(now.hour() < 12){
    currentHour = now.hour();
  } else {
    currentHour = now.hour() - 12;
  }

  int wp = 255; //white point
  
  clearStrip();
  clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, 0));

  clockLights.setPixelColor(minuteLightSelector(now.minute()), clockLights.Color(0,0,wp));

  if(currentHour == minuteLightSelector(now.minute())) {
    
  clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, wp));
  }
  minuteOnes = (now.minute() / 1U) % 10;
 
  int mOneToFour = 0;
  
  if(minuteOnes >= 0 && minuteOnes <=4){
    mOneToFour = minuteOnes;
  }
  if(minuteOnes >=5){
    mOneToFour = minuteOnes - 5;
  }
  
  for(int m = 0; m < mOneToFour; m++){
    int randomLight = random(0, 12);
    
    while(randomLight == currentHour || randomLight == minuteLightSelector(now.minute())){
      randomLight = random(0,12);
    }
    clockLights.setPixelColor(randomLight, clockLights.Color(wp - 20, wp - 20, wp - 20));
  }
  
  clockLights.show();

  Serial.println(minuteOnes);
}

void setFlashlight(int brightness){

  if(brightness == 0){
    clockLights.setBrightness(baseBrightness);
    for(int led = 0; led < LED_COUNT; led++){
      clockLights.setPixelColor(led, clockLights.Color(brightness, brightness, brightness));
    }
    clockLights.show();
    return;
  }
  
  clockLights.setBrightness(maxBrightness);
  for(int led = 0; led < LED_COUNT; led++){
    clockLights.setPixelColor(led, clockLights.Color(brightness, brightness, brightness));
  }
  clockLights.show();
}

int minuteLightSelector(int minuteValue){
  int minuteLight = 0;
  //Light selector for minutes
  if(minuteValue < 5){
    minuteLight = 0;
  }
  if(minuteValue >= 5 && minuteValue < 10){
    minuteLight = 1;
    return minuteLight;
  }
  if(minuteValue >= 10 && minuteValue < 15){
    minuteLight = 2;
    return minuteLight;
  }
  if(minuteValue >= 15 && minuteValue < 20){
    minuteLight = 3;
    return minuteLight;
  }
  if(minuteValue >= 20 && minuteValue < 25){
    minuteLight = 4;
    return minuteLight;
  }
  if(minuteValue >= 25 && minuteValue < 30){
    minuteLight = 5;
    return minuteLight;
  }
  if(minuteValue >= 30 && minuteValue < 35){
    minuteLight = 6;
    return minuteLight;
  }
  if(minuteValue >= 35 && minuteValue < 40){
    minuteLight = 7;
    return minuteLight;
  }
  if(minuteValue >= 40 && minuteValue < 45){
    minuteLight = 8;
    return minuteLight;
  }
  if(minuteValue >= 45 && minuteValue < 50){
    minuteLight = 9;
    return minuteLight;
  }
  if(minuteValue >= 50 && minuteValue < 55){
    minuteLight = 10;
    return minuteLight;
  }
  if(minuteValue >= 55 && minuteValue < 60){
    minuteLight = 11;
    return minuteLight;
  }
  
}

void setup() {
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);

  upButton.attach(upButtonPin);
  downButton.attach(downButtonPin);
  
  rtc.begin();
  Serial.begin(9600);
  clockLights.begin();
  clockLights.setBrightness(3);
  clearStrip();
  clockLights.show();
}

void loop() {
  upButton.update();
  downButton.update();
  
  if(upButton.tapped()){
    currentMode = showtime;
  }
  if(downButton.tapped()){
    currentMode = showtime;
  }
  if(upButton.held()) currentMode = flashlight;

  if(downButton.held()) currentMode = setHours;

  if(currentMode == showtime){
    showTime();
    delay(3000);
    clearStrip();
    currentMode = sleep;
  }

  if(currentMode == flashlight){
    int flashlightBrightness = 20;
    setFlashlight(flashlightBrightness);
    while(currentMode == flashlight){
      upButton.update();
      downButton.update();

      if(upButton.held()){
        setFlashlight(0);
        currentMode = sleep;
      }

      if(upButton.tapped()){
        flashlightBrightness += 10;
        if(flashlightBrightness >= 256) flashlightBrightness = 255;
        setFlashlight(flashlightBrightness);
      }
      if(downButton.tapped()){
        flashlightBrightness -= 10;
        if(flashlightBrightness <= 10) flashlightBrightness = 10;
        setFlashlight(flashlightBrightness);
      }      
      if(upButton.doubleTapped()){
        flashlightBrightness += 20;
        if(flashlightBrightness >= 256) flashlightBrightness = 255;
        setFlashlight(flashlightBrightness);
      }
      if(downButton.doubleTapped()){
        flashlightBrightness -= 20;
        if(flashlightBrightness <= 10) flashlightBrightness = 10;
        setFlashlight(flashlightBrightness);
      }
      
    }
  }
}
