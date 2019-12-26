#include <Adafruit_NeoPixel.h>
#include <ButtonEvents.h>
#include "RTClib.h"

#define LED_PIN 2
#define LED_COUNT 12

byte upButtonPin = 4;
byte downButtonPin = 3;
const byte baseBrightness = 10;
const byte maxBrightness = 60;
byte currentBrightness = 10;

enum modes {sleep, showtime, showdate, flashlight, sethours, setmins, setmonth, setday, setyear, showoff};
byte currentMode = sleep;

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



void showDate(byte wp) {
  DateTime now = rtc.now();
  clearStrip();

  byte monthLight = now.month() - 1;
  byte dateLight = dateLightSelector(now.day());

  if (monthLight == dateLight) {
    clockLights.setPixelColor(monthLight, clockLights.Color(wp, wp / 2, 0));
  } else {
    clockLights.setPixelColor(monthLight, clockLights.Color(wp, 0, 0));
    clockLights.setPixelColor(dateLight, clockLights.Color(wp, wp, 0));
  }


  byte dateOnes = (now.day() / 1U) % 10;

  byte dOneToFour = 0;

  if (dateOnes >= 0 && dateOnes <= 4) {
    dOneToFour = dateOnes;
  }
  if (dateOnes >= 5) {
    dOneToFour = dateOnes - 5;
  }

  byte lightsAlreadySet[4] = {12, 12, 12, 12};
  for (int d = 0; d < dOneToFour; d++) {
    byte randomLight = random(0, 12);
    lightsAlreadySet[d] = randomLight;

    while (randomLight == monthLight || randomLight == dateLight || randomLight == lightsAlreadySet[0] || randomLight == lightsAlreadySet[1] || randomLight == lightsAlreadySet[2] || randomLight == lightsAlreadySet[3]) {
      randomLight = random(0, 12);
    }
    lightsAlreadySet[d] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }

  clockLights.show();
}

void showHour(byte wp) {
  DateTime now = rtc.now();
  byte currentHour = 0;
  bool pm = false;
  if (now.hour() < 12) {
    currentHour = now.hour();
  } else {
    currentHour = now.hour() - 12;
    pm = true;
  }

  clearStrip();
  if (!pm)
    clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, 0));
  else
    clockLights.setPixelColor(currentHour, clockLights.Color(wp, 0, 0));
  clockLights.show();
}

void showMinutes(byte wp) {
  DateTime now = rtc.now();
  byte minuteOnes = 0;

  clearStrip();
  clockLights.setPixelColor(minuteLightSelector(now.minute()), clockLights.Color(0, 0, wp));

  minuteOnes = (now.minute() / 1U) % 10;

  byte mOneToFour = 0;

  if (minuteOnes >= 0 && minuteOnes <= 4) {
    mOneToFour = minuteOnes;
  }
  if (minuteOnes >= 5) {
    mOneToFour = minuteOnes - 5;
  }

  byte lightsAlreadySet[4] = {12, 12, 12, 12};
  for (int m = 0; m < mOneToFour; m++) {
    byte randomLight = random(0, 12);
    lightsAlreadySet[m] = randomLight;

    while (randomLight == minuteLightSelector(now.minute()) || randomLight == lightsAlreadySet[0] || randomLight == lightsAlreadySet[1] || randomLight == lightsAlreadySet[2] || randomLight == lightsAlreadySet[3]) {

      randomLight = random(0, 12);
    }
    lightsAlreadySet[m] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }
  clockLights.show();
}

void showMonth(byte wp) {
  DateTime now = rtc.now();
  clearStrip();

  byte monthLight = now.month() - 1;
  clockLights.setPixelColor(monthLight, clockLights.Color(wp, 0, 0));
  clockLights.show();
}

void showDay(byte wp) {
  DateTime now = rtc.now();
  clearStrip();
  byte dateLight = dateLightSelector(now.day());
  clockLights.setPixelColor(dateLight, clockLights.Color(wp, wp, 0));


  byte dateOnes = (now.day() / 1U) % 10;

  byte dOneToFour = 0;

  if (dateOnes >= 0 && dateOnes <= 4) {
    dOneToFour = dateOnes;
  }
  if (dateOnes >= 5) {
    dOneToFour = dateOnes - 5;
  }

  byte lightsAlreadySet[4] = {12, 12, 12, 12};
  for (byte d = 0; d < dOneToFour; d++) {
    byte randomLight = random(0, 12);
    lightsAlreadySet[d] = randomLight;

    while (randomLight == dateLight || randomLight == lightsAlreadySet[0] || randomLight == lightsAlreadySet[1] || randomLight == lightsAlreadySet[2] || randomLight == lightsAlreadySet[3]) {
      randomLight = random(0, 12);
    }
    lightsAlreadySet[d] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }
  clockLights.show();
}

void showYear(byte wp) {
  DateTime now = rtc.now();
  clearStrip();
  int currentYear = now.year() % 100;
  int yearLight = (currentYear / 10) % 10;
  clockLights.setPixelColor(yearLight, clockLights.Color(wp, 0, wp));

  int yearOnes = (currentYear / 1U) % 10;

  byte lightsAlreadySet[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
  for (byte y = 0; y < yearOnes; y++) {
    byte randomLight = random(0, 12);
    lightsAlreadySet[y] = randomLight;

    while (randomLight == yearLight || randomLight == lightsAlreadySet[0] || randomLight == lightsAlreadySet[1] || randomLight == lightsAlreadySet[2] || randomLight == lightsAlreadySet[3] || randomLight == lightsAlreadySet[4] || randomLight == lightsAlreadySet[5] || randomLight == lightsAlreadySet[6] || randomLight == lightsAlreadySet[7] || randomLight == lightsAlreadySet[8] || randomLight == lightsAlreadySet[9]) {
      randomLight = random(0, 12);
    }
    lightsAlreadySet[y] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }
  clockLights.show();
}

void showTime(byte wp) {
  DateTime now = rtc.now();
  byte currentHour = 0;
  byte minuteOnes = 0;
  bool pm = false;
  if (now.hour() < 12) {
    currentHour = now.hour();
  } else {
    currentHour = now.hour() - 12;
    pm = true;
  }

  clearStrip();
  if (!pm)
    clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, 0));
  else
    clockLights.setPixelColor(currentHour, clockLights.Color(wp, 0, 0));

  clockLights.setPixelColor(minuteLightSelector(now.minute()), clockLights.Color(0, 0, wp));

  if (currentHour == minuteLightSelector(now.minute())) {
    if (!pm)
      clockLights.setPixelColor(currentHour, clockLights.Color(0, wp, wp));
    else
      clockLights.setPixelColor(currentHour, clockLights.Color(wp, 0, wp));
  }
  minuteOnes = (now.minute() / 1U) % 10;

  byte mOneToFour = 0;

  if (minuteOnes >= 0 && minuteOnes <= 4) {
    mOneToFour = minuteOnes;
  }
  if (minuteOnes >= 5) {
    mOneToFour = minuteOnes - 5;
  }

  byte lightsAlreadySet[4] = {12, 12, 12, 12};
  for (byte m = 0; m < mOneToFour; m++) {
    byte randomLight = random(0, 12);
    lightsAlreadySet[m] = randomLight;

    while (randomLight == currentHour || randomLight == minuteLightSelector(now.minute()) || randomLight == lightsAlreadySet[0] || randomLight == lightsAlreadySet[1] || randomLight == lightsAlreadySet[2] || randomLight == lightsAlreadySet[3]) {

      randomLight = random(0, 12);
    }
    lightsAlreadySet[m] = randomLight;
    clockLights.setPixelColor(randomLight, clockLights.Color(wp, wp, wp));
  }

  clockLights.show();
}

void setFlashlight(byte brightness) {

  if (brightness == 0) {
    clockLights.setBrightness(currentBrightness);
    clearStrip();
    return;
  }

  clockLights.setBrightness(brightness);
  for (byte led = 0; led < LED_COUNT; led++) {
    clockLights.setPixelColor(led, clockLights.Color(255, 255, 255));
  }
  clockLights.show();
}

byte dateLightSelector(byte dayValue) {
  byte dayLight = 0;
  if (dayValue < 5) {
    dayLight = 0;
    return dayLight;
  }
  if (dayValue >= 5 && dayValue < 10) {
    dayLight = 1;
    return dayLight;
  }
  if (dayValue >= 10 && dayValue < 15) {
    dayLight = 2;
    return dayLight;
  }
  if (dayValue >= 15 && dayValue < 20) {
    dayLight = 3;
    return dayLight;
  }
  if (dayValue >= 20 && dayValue < 25) {
    dayLight = 4;
    return dayLight;
  }
  if (dayValue >= 25 && dayValue < 30) {
    dayLight = 5;
    return dayLight;
  }
  if (dayValue >= 30) {
    dayLight = 6;
    return dayLight;
  }
}

byte minuteLightSelector(byte minuteValue) {
  byte minuteLight = 0;
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

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return clockLights.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return clockLights.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return clockLights.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < clockLights.numPixels(); i++) {
      clockLights.setPixelColor(i, Wheel(((i * 256 / clockLights.numPixels()) + j) & 255));
    }
    clockLights.show();
    delay(wait);
    if (!digitalRead(upButtonPin)) {
      clearStrip();
      currentMode = sleep;
      return;
    }
  }
}

void setHour() {
  byte currentHour = 0;
  showHour(255);
  while (currentMode == sethours) {
    upButton.update();
    downButton.update();
    if (upButton.tapped()) {
      DateTime now = rtc.now();
      currentHour = now.hour();
      currentHour ++;
      if (currentHour > 23)
        currentHour = 0;
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), currentHour, now.minute(), now.second()));
      showHour(255);
    }
    if (downButton.tapped()) {
      DateTime now = rtc.now();
      currentHour = now.hour();
      currentHour --;
      if (currentHour < 0)
        currentHour = 23;
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), currentHour, now.minute(), now.second()));
      showHour(255);
    }
    if (upButton.held()) {
      clearStrip();
      return;
    }
  }
}

void setMinute() {
  byte currentMinute = 0;
  showMinutes(255);
  while (currentMode == setmins) {
    upButton.update();
    downButton.update();
    if (upButton.tapped()) {
      DateTime now = rtc.now();
      currentMinute = now.minute();
      currentMinute ++;
      if (currentMinute > 59)
        currentMinute = 0;
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), currentMinute, 0));
      showMinutes(255);
    }
    if (downButton.tapped()) {
      DateTime now = rtc.now();
      currentMinute = now.minute();
      currentMinute --;
      if (currentMinute < 0)
        currentMinute = 59;
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), currentMinute, 0));
      showMinutes(255);
    }
    if (upButton.held()) {
      clearStrip();
      return;
    }

  }
}

void setMonth() {
  byte currentMonth = 0;
  showMonth(255);
  while (currentMode == setmonth) {
    upButton.update();
    downButton.update();
    if (upButton.tapped()) {
      DateTime now = rtc.now();
      currentMonth = now.month();
      currentMonth ++;
      if (currentMonth > 12)
        currentMonth = 1;
      rtc.adjust(DateTime(now.year(), currentMonth, now.day(), now.hour(), now.minute(), now.second()));
      showMonth(255);
    }
    if (downButton.tapped()) {
      DateTime now = rtc.now();
      currentMonth = now.month();
      currentMonth --;
      if (currentMonth < 1)
        currentMonth = 12;
      rtc.adjust(DateTime(now.year(), currentMonth, now.day(), now.hour(), now.minute(), now.second()));
      showMonth(255);
    }
    if (downButton.held()) {
      clearStrip();
      return;
    }
  }
}

void setDay() {
  byte currentDay = 0;
  showDay(255);
  while (currentMode == setday) {
    upButton.update();
    downButton.update();
    if (upButton.tapped()) {
      DateTime now = rtc.now();
      currentDay = now.day();
      currentDay ++;
      if (currentDay > 31)
        currentDay = 1;
      rtc.adjust(DateTime(now.year(), now.month(), currentDay, now.hour(), now.minute(), now.second()));
      showDay(255);
    }
    if (downButton.tapped()) {
      DateTime now = rtc.now();
      currentDay = now.day();
      currentDay --;
      if (currentDay < 1)
        currentDay = 31;
      rtc.adjust(DateTime(now.year(), now.month(), currentDay, now.hour(), now.minute(), now.second()));
      showDay(255);
    }
    if (downButton.held()) {
      clearStrip();
      return;
    }
  }
}

void setYear() {
  int currentYear = 0;
  showYear(255);
  while (currentMode == setyear) {
    upButton.update();
    downButton.update();
    if (upButton.tapped()) {
      DateTime now = rtc.now();
      currentYear = now.year();
      currentYear ++;
      if (currentYear > 2099)
        currentYear = 2000;
      rtc.adjust(DateTime(currentYear, now.month(), now.day(), now.hour(), now.minute(), now.second()));
      showYear(255);
    }
    if (downButton.tapped()) {
      DateTime now = rtc.now();
      currentYear = now.year();
      currentYear --;
      if (currentYear < 2000)
        currentYear = 2099;
      rtc.adjust(DateTime(currentYear, now.month(), now.day(), now.hour(), now.minute(), now.second()));
      showYear(255);
    }
    if (downButton.held()) {
      clearStrip();
      return;
    }
  }

}

void flashConfirm() {
  clearStrip();
  for (byte led = 0; led < LED_COUNT; led++) {
    clockLights.setPixelColor(led, clockLights.Color(0, 255, 0));
  }
  clockLights.show();
  delay(400);
  clearStrip();
}

void flashlightMode() {
  byte flashlightBrightness = 20;
  setFlashlight(flashlightBrightness);
  while (currentMode == flashlight) {
    upButton.update();
    downButton.update();

    if (upButton.held()) {
      clockLights.setBrightness(currentBrightness);
      clearStrip();
      currentMode = sleep;
    }
    if (downButton.held()) {
      currentBrightness = flashlightBrightness;
      clockLights.setBrightness(currentBrightness);
      flashConfirm();
      currentMode = sleep;
    }

    if (upButton.tapped()) {
      flashlightBrightness += 10;
      if (flashlightBrightness >= maxBrightness) flashlightBrightness = maxBrightness;
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

void fancyShow(bool timeOrDate) {
  bool setAfter = false;
  for (byte i = 1; i < 15; i++) {
    if (timeOrDate)
      showTime(17 * i);
    else
      showDate(17 * i);
    clockLights.show();
    delay(10);
  }
  if (timeOrDate)
    showTime(255);
  else
    showDate(255);
  unsigned long startDelayMillis = millis();
  while (millis() - startDelayMillis < 3000) {
    upButton.update();
    downButton.update();
    if (downButton.held() && !timeOrDate) {
      currentMode = setmonth;
      setAfter = true;
      break;
    }
    if (upButton.held() && timeOrDate) {
      currentMode = sethours;
      setAfter = true;
      break;
    }
  }

  for (byte i = 15; i > 1; i--) {
    if (timeOrDate)
      showTime(17 * i);
    else
      showDate(17 * i);
    clockLights.show();
    delay(10);
  }
  clearStrip();
  if (!setAfter)
    currentMode = sleep;
}

void setup() {
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);

  upButton.attach(upButtonPin);
  downButton.attach(downButtonPin);

  rtc.begin();
  clockLights.begin();
  clockLights.setBrightness(currentBrightness);
  clearStrip();
  clockLights.show();

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(2000, 1, 1, 0, 0, 0));
  }
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
  if (downButton.held()) {
    currentMode = showoff;
  }

  //actual mode code starts here

  if (currentMode == sethours) {
    setHour();
    currentMode = setmins;
  }

  if (currentMode == setmins) {
    setMinute();
    flashConfirm();
    currentMode = sleep;
  }

  if (currentMode == showtime) {
    fancyShow(true); //false means show date
  }

  if (currentMode == showdate) {
    fancyShow(false); //false means show date
  }

  if (currentMode == setmonth) {
    setMonth();
    currentMode = setday;
  }

  if (currentMode == setday) {
    setDay();
    currentMode = setyear;
  }

  if (currentMode == setyear) {
    setYear();
    flashConfirm();
    currentMode = sleep;
  }

  if (currentMode == flashlight) {
    flashlightMode();
  }

  if (currentMode == showoff) {
    while (currentMode == showoff) {
      rainbowCycle(2);
    }
  }
}
