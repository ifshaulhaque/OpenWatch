#include <Arduino.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <tt_lakes_neue_trial_black18.h>
#include <tt_lakes_neue_trial_black12.h>
#include "CustomBatteryBar.hpp"
#include "CustomTextView.hpp"

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;
constexpr uint8_t OLED_ADDR = 0x3C;

// Adjust I2C pins to match your ESP32-C3 wiring
constexpr int I2C_SDA = 4;
constexpr int I2C_SCL = 5;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CustomBatteryBar batteryBar;
CustomTextView mTime;
CustomTextView seconds;
CustomTextView am_pm;
CustomTextView countryCode;
CustomTextView date;

uint32_t lastTickMs = 0;

void seedInitialTime(int year, int month, int day, int hour, int min, int sec) {
    setenv("TZ", "IST-5:30", 1);
    tzset();

    tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min  = min;
    t.tm_sec  = sec;

    time_t epoch = mktime(&t);
    timeval tv = { .tv_sec = epoch, .tv_usec = 0 };
    settimeofday(&tv, nullptr);
}

void renderTimeDate(const tm& timeinfo) {
    char hmBuf[8];
    char secBuf[4];
    char dateBuf[12];

    snprintf(hmBuf, sizeof(hmBuf), "%02d:%02d ", timeinfo.tm_hour, timeinfo.tm_min);
    snprintf(secBuf, sizeof(secBuf), "%02d", timeinfo.tm_sec);
    snprintf(dateBuf, sizeof(dateBuf), "%02d-%02d-%04d",
             timeinfo.tm_mday,
             timeinfo.tm_mon + 1,
             timeinfo.tm_year + 1900);

    const char* period = (timeinfo.tm_hour >= 12) ? "PM" : "AM";

    am_pm.setText(period)
      .draw(display);

    mTime.setText(hmBuf)
      .draw(display);

    seconds.setText(secBuf)
      .draw(display);

    date.setText(dateBuf)
      .draw(display);

    display.display();
};

void setup() {
    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 allocation failed");
        for (;;) {}
    }

    seedInitialTime(2026, 9, 26, 23, 58, 37);

    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(30);

    display.clearDisplay();

    am_pm.setBounds(4, 4, -1, -1)
      .setText("PM")
      .draw(display);

    batteryBar.setBounds(108,4)
      .setPercentage(75)
      .draw(display);

    mTime.setBounds(-1, -1, 90, 56)
      .setFontFamily(&tt_lakes_neue_trial_black12pt7b)
      .setText("19:48 ")
      .draw(display);

    seconds.setBounds(95, 30, -1, -1)
      .setFontFamily(&tt_lakes_neue_trial_black8pt7b)
      .setText("58")
      .draw(display);

    countryCode.setBounds(4, 56, -1, -1)
      .setText("IND")
      .draw(display);
    
    date.setBounds(-1 , -1, 124, 63)
      .setText("26-11-2026")
      .draw(display);

    display.display();
}

void loop() {
    uint32_t now = millis();
    if (now - lastTickMs >= 1000) {
        lastTickMs = now;

        tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            renderTimeDate(timeinfo);
        }
    }
}