
#define TONE_OUTPUT_PIN 26
const int TONE_PWM_CHANNEL = 0;
#include "pitches.h"

#include <Adafruit_NeoPixel.h>
#define LED_PIN 15
#define LED_COUNT 12
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define BLYNK_TEMPLATE_ID "TMPL6DFllZkwq"
#define BLYNK_TEMPLATE_NAME "Take Medicine Reminder"
#define BLYNK_AUTH_TOKEN "MCID4f0_3sf-TjFSdgKacYU-jblye5sq"

#define BLYNK_PRINT Serial
// #include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "ConnectToWifi.h"
#include "GetDateTime.h"
#include "MorningMusicReminder.h"
#include "NightMusicReminder.h"
#include "MedicineActiveReminder.h"

int hourToTakeMedicineInMorning = 13;
int minutesToTakeMedicineInMorning = 0;
int hourToTakeMedicineInNight = 18;
int minutesToTakeMedicineInNight = 0;
int hourToShutDown = 22;
int morningLed = 3;
int nightLed =7;

MedicineActiveReminder morningReminder(morningLed, hourToTakeMedicineInMorning , minutesToTakeMedicineInMorning, hourToShutDown, true);
MedicineActiveReminder nightReminder(nightLed, hourToTakeMedicineInNight, minutesToTakeMedicineInNight, hourToShutDown, false);

int currentHour;
int currentMinutes;

void setup() {
  Serial.begin(115200);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)Serial.println("ESP32 Touch Test");
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
  connectToWifi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();
  currentHour = getCurrentHour();
  currentMinutes = getCurrentMinutes();
  
  morningReminder.setLedColorByTime(currentHour, currentMinutes);
  nightReminder.setLedColorByTime(currentHour, currentMinutes);

  morningReminder.playMusicReminder(currentHour, currentMinutes);
  nightReminder.playMusicReminder(currentHour, currentMinutes);

  morningReminder.tookeMedicineValidator(currentHour, 6, hourToTakeMedicineInNight);
  nightReminder.tookeMedicineValidator(currentHour, hourToTakeMedicineInNight, hourToShutDown);

  morningReminder.sendReminderToPhone(currentHour, currentMinutes);
  nightReminder.sendReminderToPhone(currentHour, currentMinutes);
  delay(50);
}

BLYNK_WRITE(V1) {
  hourToTakeMedicineInMorning = param.asInt();
  Serial.println("update morning hour for medicine, new value: ");
  Serial.println(hourToTakeMedicineInMorning);
  morningReminder.setHourToGetMedicine(hourToTakeMedicineInMorning);
}

BLYNK_WRITE(V2) {
  hourToTakeMedicineInNight = param.asInt();
  Serial.println("update night hour for medicine, new value: ");
  Serial.println(hourToTakeMedicineInNight);
  nightReminder.setHourToGetMedicine(hourToTakeMedicineInNight);
}


BLYNK_WRITE(V3) {
  minutesToTakeMedicineInMorning = param.asInt();
  Serial.println("update morning minutes for medicine, new value: ");
  Serial.println(minutesToTakeMedicineInMorning);
  morningReminder.setMinutesToGetMedicine(minutesToTakeMedicineInMorning);
}


BLYNK_WRITE(V4) {
  minutesToTakeMedicineInNight = param.asInt();
  Serial.println("update night minutes for medicine, new value: ");
  Serial.println(minutesToTakeMedicineInNight);
  nightReminder.setMinutesToGetMedicine(minutesToTakeMedicineInNight);
}
