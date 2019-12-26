#include <Adafruit_NeoPixel.h>
#include <ButtonEvents.h>
#include "RTClib.h"

#define LED_PIN 2
#define LED_COUNT 12

int upButtonPin = 4;
int downButtonPin = 3;
const int baseBrightness = 3;
const int maxBrightness = 60;

enum modes {sleep, showtime, showdate, flashlight, sethours, setmins, setmonth, setday, setyear};
int currentMode = sleep;

ButtonEvents upButton;
ButtonEvents downButton;

RTC_DS3231 rtc;

Adafruit_NeoPixel clockLights(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void clearStrip() {
  for (int i = 0; i <= LED_COUNT; i++) {
    clockLights.setPixelColor(i, clockLights.Color(0, 0, 0));
  }
  clockLights.show();
}

void showDate(int wp) {
  DateTime now = rtc.now();
  clearStrip();
  
  int monthLight = now.month() - 1;
  int dateLight = dateLightSelector(now.day());

  if(monthLight == dateLight){
    clockLights.setPixelColor(monthLight, clockLights.Color(wp, wp/2, 0));
  } else{
      clockLights.setPixelColor(monthLight, clockLights.Color(wp, 0, 0));
      clockLights.setPixelColor(dateLight, clockLights.Color(wp, wp, 0));
  }

  
  int dateOnes = (now.day() / 1U) % 10;

  int dOneToFour = 0;

  if (dateOnes >= 0 && dateOnes <= 4) {
    dOneToFour = dateOnes;
  }
  if (dateOnes >= 5) {
    dOneToFour = dateOnes - 5;
  }

  int lightsAlreadySet[4] = {12,12,12,12};
  for (int d = 0; d < dOneToFour; d++) {
    int randomLight = random(0, 12);
    lightsAlreadySet[d] = randomLight;

    while (randomLight == monthLight || randomLight == dateLight || randomLight == lightsAlreadySet[0]|| randomLight == lightsAlreadySet[1]|| randomLight == lightsAlreadySet[2]|| randomLight == lightsAlreadySet[3]) {
       randomLight = random(0, 12);
    }
    lightsAlreadySet[d] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }

  clockLights.show();
}

void showHour(int wp){
  DateTime now = rtc.now();
  int currentHour = 0;
  bool pm = false;
  if (now.hour() < 12) {
    currentHour = now.hour();
  } else {
    currentHour = now.hour() - 12;
    pm = true;
  }

  clearStrip();
  if(!pm)
    clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, 0));
  else
    clockLights.setPixelColor(currentHour, clockLights.Color(wp, 0, 0));
  clockLights.show();
}

void showMinutes(int wp){
  DateTime now = rtc.now();
  int minuteOnes = 0;
  
  clearStrip();
  clockLights.setPixelColor(minuteLightSelector(now.minute()), clockLights.Color(0, 0, wp));
  
  minuteOnes = (now.minute() / 1U) % 10;

  int mOneToFour = 0;

  if (minuteOnes >= 0 && minuteOnes <= 4) {
    mOneToFour = minuteOnes;
  }
  if (minuteOnes >= 5) {
    mOneToFour = minuteOnes - 5;
  }

  int lightsAlreadySet[4] = {12,12,12,12};
  for (int m = 0; m < mOneToFour; m++) {
    int randomLight = random(0, 12);
    lightsAlreadySet[m] = randomLight;

    while (randomLight == minuteLightSelector(now.minute()) || randomLight == lightsAlreadySet[0]|| randomLight == lightsAlreadySet[1]|| randomLight == lightsAlreadySet[2]|| randomLight == lightsAlreadySet[3]) {
      
      randomLight = random(0, 12);
    }
    lightsAlreadySet[m] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }
  clockLights.show();
}

void showMonth(int wp){
  DateTime now = rtc.now();
  clearStrip();
  
  int monthLight = now.month() - 1;
  clockLights.setPixelColor(monthLight, clockLights.Color(wp, 0, 0);
  clockLights.show();
}

void showTime(int wp) {
  DateTime now = rtc.now();
  int currentHour = 0;
  int minuteOnes = 0;
  bool pm = false;
  if (now.hour() < 12) {
    currentHour = now.hour();
  } else {
    currentHour = now.hour() - 12;
    pm = true;
  }

  clearStrip();
  if(!pm)
    clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, 0));
  else
    clockLights.setPixelColor(currentHour, clockLights.Color(wp, 0, 0));
    
  clockLights.setPixelColor(minuteLightSelector(now.minute()), clockLights.Color(0, 0, wp));

  if (currentHour == minuteLightSelector(now.minute())) {
    if(!pm)
      clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, wp));
    else
      clockLights.setPixelColor(currentHour, clockLights.Color(wp, 0, wp));
  }
  minuteOnes = (now.minute() / 1U) % 10;

  int mOneToFour = 0;

  if (minuteOnes >= 0 && minuteOnes <= 4) {
    mOneToFour = minuteOnes;
  }
  if (minuteOnes >= 5) {
    mOneToFour = minuteOnes - 5;
  }

  int lightsAlreadySet[4] = {12,12,12,12};
  for (int m = 0; m < mOneToFour; m++) {
    int randomLight = random(0, 12);
    lightsAlreadySet[m] = randomLight;

    while (randomLight == currentHour || randomLight == minuteLightSelector(now.minute()) || randomLight == lightsAlreadySet[0]|| randomLight == lightsAlreadySet[1]|| randomLight == lightsAlreadySet[2]|| randomLight == lightsAlreadySet[3]) {
      
      randomLight = random(0, 12);
    }
    lightsAlreadySet[m] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }

  clockLights.show();
}

void setFlashlight(int brightness) {

  if (brightness == 0) {
    clockLights.setBrightness(baseBrightness);
    for (int led = 0; led < LED_COUNT; led++) {
      clockLights.setPixelColor(led, clockLights.Color(brightness, brightness, brightness));
    }
    clockLights.show();
    return;
  }

  clockLights.setBrightness(maxBrightness);
  for (int led = 0; led < LED_COUNT; led++) {
    clockLights.setPixelColor(led, clockLights.Color(brightness, brightness, brightness));
  }
  clockLights.show();
}

int dateLightSelector(int dayValue){
  int dayLight = 0;
  if(dayValue < 5){
    dayLight = 0;
    return dayLight;
  }
  if(dayValue >= 5 && dayValue < 10){
    dayLight = 12;
    return dayLight;
  }
  if(dayValue >= 10 && dayValue < 15){
    dayLight = 1;
    return dayLight;
  }
  if(dayValue >= 15 && dayValue < 20){
    dayLight = 2;
    return dayLight;
  }
  if(dayValue >= 20 && dayValue < 25){
    dayLight = 3;
    return dayLight;
  }
  if(dayValue >= 25 && dayValue < 30){
    dayLight = 4;
    return dayLight;
  }
  if(dayValue >= 30){
    dayLight = 5;
    return dayLight;
  }
}

int minuteLightSelector(int minuteValue) {
  int minuteLight = 0;
  //Light selector for minutes
  if (minuteValue < 5) {
    minuteLight = 0;
    return minuteLight;
  }
  if (minuteValue >= 5 && minuteValue < 10) {
    minuteLight = 1;
    return minuteLight;
  }
  if (minuteValue >= 10 && minuteValue < 15) {
    minuteLight = 2;
    return minuteLight;
  }
  if (minuteValue >= 15 && minuteValue < 20) {
    minuteLight = 3;
    return minuteLight;
  }
  if (minuteValue >= 20 && minuteValue < 25) {
    minuteLight = 4;
    return minuteLight;
  }
  if (minuteValue >= 25 && minuteValue < 30) {
    minuteLight = 5;
    return minuteLight;
  }
  if (minuteValue >= 30 && minuteValue < 35) {
    minuteLight = 6;
    return minuteLight;
  }
  if (minuteValue >= 35 && minuteValue < 40) {
    minuteLight = 7;
    return minuteLight;
  }
  if (minuteValue >= 40 && minuteValue < 45) {
    minuteLight = 8;
    return minuteLight;
  }
  if (minuteValue >= 45 && minuteValue < 50) {
    minuteLight = 9;
    return minuteLight;
  }
  if (minuteValue >= 50 && minuteValue < 55) {
    minuteLight = 10;
    return minuteLight;
  }
  if (minuteValue >= 55 && minuteValue < 60) {
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
  //update the buttons at the start of each loop
  upButton.update();
  downButton.update();

  //mode selection code

  if (upButton.tapped()) {
    if (currentMode == sleep)
      currentMode = showtime;
    //if(currentMode == sethours)
      //0=0;
  }
  if (downButton.tapped()) {
    if (currentMode == sleep)
      currentMode = showdate;
  }
  if (upButton.held()) {
    if (currentMode == sleep)
      currentMode = flashlight;
  }
  if(upButton.doubleTapped()){
    if(currentMode == showtime) currentMode = sethours;
  }

  //actual mode code starts here

  if (currentMode == sethours) {
    Serial.println("Setting hours");
    int currentHour = 0;
    showHour(255);
    while(currentMode == sethours){
      upButton.update();
      downButton.update();
      if(upButton.tapped()){
        DateTime now = rtc.now();
        currentHour = now.hour();
        currentHour ++;
        if(currentHour > 23)
          currentHour = 0;
        Serial.print("Hour incremented: ");
        Serial.println(currentHour);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), currentHour, now.minute(), 0));
        showHour(255);
      }
      if(downButton.tapped()){
        DateTime now = rtc.now();
        currentHour = now.hour();
        currentHour --;
        if(currentHour < 0)
          currentHour = 23;
        Serial.print("Hour decremented: ");
        Serial.println(currentHour);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), currentHour, now.minute(), 0));
        showHour(255);
      }
      if(upButton.held()){
        clearStrip();
        Serial.println("Not setting hours any more");
        currentMode = setmins;
      }
    }
    
  }

  if (currentMode == setmins) {
    Serial.println("Setting minutes");
    int currentMinute = 0;
    showMinutes(255);
    while(currentMode == setmins){
      upButton.update();
      downButton.update();
      if(upButton.tapped()){
        DateTime now = rtc.now();
        currentMinute = now.minute();
        currentMinute ++;
        if(currentMinute > 59)
          currentMinute = 0;
        Serial.print("Minute incremented: ");
        Serial.println(currentMinute);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), currentMinute, 0));
        showMinutes(255);
      }
      if(downButton.tapped()){
        DateTime now = rtc.now();
        currentMinute = now.minute();
        currentMinute --;
        if(currentMinute < 0)
          currentMinute = 59;
        Serial.print("Minute decremented: ");
        Serial.println(currentMinute);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), currentMinute, 0));
        showMinutes(255);
      }
      if(upButton.held()){
        clearStrip();
        Serial.println("Sleep time");
        currentMode = sleep;
      }
    }
    
  }

  if (currentMode == showtime) {
    Serial.println("Displaying time fancily");
    bool setTimeAfter = false;
    for (int i = 1; i < 15; i++) {
      showTime(17 * i);
      clockLights.show();
      delay(10);
    }
    showTime(255);
    unsigned long startDelayMillis = millis();
    while(millis() - startDelayMillis < 3000){
      upButton.update();
      if(upButton.held()){
        currentMode = sethours;
        setTimeAfter = true;
        break;
      }
    }
    for (int i = 15; i > 1; i--) {
      showTime(17 * i);
      clockLights.show();
      delay(10);
    }
    clearStrip();
    if(!setTimeAfter){
    currentMode = sleep;
    }
  }

  if (currentMode == showdate) {
    Serial.println("Displaying date fancily");
    for (int i = 1; i < 15; i++) {
      showDate(17 * i);
      clockLights.show();
      delay(10);
    }
    delay(3000);
    for (int i = 15; i > 1; i--) {
      showDate(17 * i);
      clockLights.show();
      delay(10);
    }
    clearStrip();
    currentMode = sleep;
  }

  if (currentMode == flashlight) {
    int flashlightBrightness = 20;
    setFlashlight(flashlightBrightness);
    while (currentMode == flashlight) {
      upButton.update();
      downButton.update();

      if (upButton.held()) {
        setFlashlight(0);
        currentMode = sleep;
      }

      if (upButton.tapped()) {
        flashlightBrightness += 10;
        if (flashlightBrightness >= 256) flashlightBrightness = 255;
        setFlashlight(flashlightBrightness);
      }
      if (downButton.tapped()) {
        flashlightBrightness -= 10;
        if (flashlightBrightness <= 10) flashlightBrightness = 10;
        setFlashlight(flashlightBrightness);
      }
      if (upButton.doubleTapped()) {
        flashlightBrightness += 20;
        if (flashlightBrightness >= 256) flashlightBrightness = 255;
        setFlashlight(flashlightBrightness);
      }
      if (downButton.doubleTapped()) {
        flashlightBrightness -= 20;
        if (flashlightBrightness <= 10) flashlightBrightness = 10;
        setFlashlight(flashlightBrightness);
      }

    }
  }
}
