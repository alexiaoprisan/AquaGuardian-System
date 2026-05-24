#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Servo.h>
#include <math.h>

// pini
#define SERVO_PIN 9
#define BUTTON_PIN 3
#define LED_BUTTON_PIN 6
#define TRIG_PIN 4
#define ECHO_PIN 5
#define LDR_PIN A0
#define NTC_PIN A1
#define RELAY_PIN 7

// SETTINGS
#define LIGHT_ON_THRESHOLD 80
#define LIGHT_OFF_THRESHOLD 500
#define WATER_ALERT_CM 5.0
#define FEED_HOUR 16
#define FEED_MINUTE 46

// OBJECTS
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
Servo feederServo;

// GLOBAL
bool alreadyFed = false;
bool manualLedMode = false;
bool ledState = false;
bool autoLedState = false;

// FEED
void feedFish()
{
    Serial.println("FEEDING...");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Feeding...");
    // rotesc la 90 de grade
    feederServo.write(90);
    delay(1000);
    feederServo.write(0);
    delay(500);
    Serial.println("DONE");
}

// HC-SR04 distance
float readDistanceCm()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // senzorul trimite unda ultrasonica
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // masor ca timp pinul echo ramane pe high
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    // calculez distanta in cm
    return duration * 0.0343 / 2.0;
}

// NTC
float readTemperature()
{
    int ntcValue = analogRead(NTC_PIN);

    // calculez rezistenta nntc-ului
    float resistance = 10000.0 * (1023.0 / ntcValue - 1.0);

    // formula temperaturii
    float tempC = 1.0 / (
            log(resistance / 10000.0) / 3950.0 +
            1.0 / (25.0 + 273.15)
        ) - 273.15;

    return tempC;
}

// SETUP - cand porneste microcontrollerul
void setup()
{
    Serial.begin(9600);

    // initializez i2c, lcd
    Wire.begin();
    lcd.init();
    lcd.backlight();
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    feederServo.attach(SERVO_PIN);
    feederServo.write(0);

    // verific daca functioneaza bine rtc
    if (!rtc.begin())
    {
        lcd.clear();
        lcd.print("RTC ERROR");
        Serial.println("RTC ERROR");
        while (1);
    }

    // RULEAZA O SINGURA DATA
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    lcd.clear();

    // mesaj de pornire
    lcd.setCursor(0, 0);
    lcd.print("AquaGuardian");
    lcd.setCursor(0, 1);
    lcd.print("System Ready");

    delay(1500);

    lcd.clear();
}

void loop()
{
    // citesc ora curenta de la rtc
    DateTime now = rtc.now();

    // citesc nivelul apei
    float distance = readDistanceCm();
    float waterLevel = 15.0 - distance;

    // evit valori negative
    if (waterLevel < 0)
    {
        waterLevel = 0;
    }

    int ldrValue = analogRead(LDR_PIN);

    float tempC = readTemperature();

    // FEED BUTTON
    static bool lastFeedButton = HIGH;

    // citesc starea curenta a butonului
    bool currentFeedButton = digitalRead(BUTTON_PIN);
    
    if (lastFeedButton == HIGH && currentFeedButton == LOW)
    {
        // debounce
        delay(50);
        // dau mancare daca apas butonul
        if (digitalRead(BUTTON_PIN) == LOW)
        {
            feedFish();
        }
    }

    lastFeedButton = currentFeedButton;

    // LED BUTTON

    static bool lastLedButton = HIGH;

    bool currentLedButton = digitalRead(LED_BUTTON_PIN);

    if (lastLedButton == HIGH && currentLedButton == LOW)
    {
        delay(50);
        if (digitalRead(LED_BUTTON_PIN) == LOW)
        {
            manualLedMode = !manualLedMode;
            if (manualLedMode) {
                ledState = !ledState;
            }
        }
    }

    lastLedButton = currentLedButton;

    // LED CONTROL

    if (manualLedMode)
    {
        digitalWrite(RELAY_PIN, ledState);
    }
    else
    {
        // aprind lumina daca e prea intuneric
        if (ldrValue < LIGHT_ON_THRESHOLD)
        {
            autoLedState = true;
        }
        // inchid ledul daca e destula lumina
        if (ldrValue > LIGHT_OFF_THRESHOLD)
        {
            autoLedState = false;
        }
        digitalWrite(RELAY_PIN, autoLedState);
    }

    // AUTO FEED

    // verific daca e timupl sa hranesc pestii
    if (now.hour() == FEED_HOUR && now.minute() == FEED_MINUTE && now.second() < 10 && !alreadyFed)
    {
        Serial.println("AUTO FEED TRIGGER");
        feedFish();
        alreadyFed = true;
    }

    if ( now.hour() != FEED_HOUR ||  now.minute() != FEED_MINUTE)
    {
        alreadyFed = false;
    }

    // LCD

    lcd.setCursor(0, 0);

    if (now.hour() < 10) lcd.print("0");

    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) 
        lcd.print("0");

    lcd.print(now.minute());
    lcd.print(":");

    if (now.second() < 10) 
        lcd.print("0");
    lcd.print(now.second());

    lcd.print(" ");

    lcd.setCursor(0, 1);

    lcd.print("T:");
    lcd.print(tempC, 1);

    lcd.print("C ");

    lcd.print("W:");

    if (distance < 0)
    {
        lcd.print("ERR");
    }
    else
    {
        lcd.print((int)waterLevel);

        lcd.print("cm ");
    }

    // SERIAL

    Serial.print("TIME: ");

    if (now.hour() < 10)
        Serial.print("0");

    Serial.print(now.hour());

    Serial.print(":");

    if (now.minute() < 10)
        Serial.print("0");

    Serial.print(now.minute());

    Serial.print(":");

    if (now.second() < 10)
        Serial.print("0");

    Serial.print(now.second());

    Serial.print(" | TEMP: ");

    Serial.print(tempC);

    Serial.print(" C");

    Serial.print(" | WATER LEVEL: ");
    Serial.print(waterLevel);

    Serial.print(" cm");

    Serial.print(" | LDR: ");

    Serial.println(ldrValue);

    if (waterLevel < 5) {
        Serial.println("ALERTA: nivel insuficient de apa!");
    }

    delay(500);
}