#include <Arduino.h>
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

void setup() {
    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 allocation failed");
        for (;;) {}
    }

    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(30);

    display.clearDisplay();

    am_pm.setCursor(4, 4)
      .setText("PM")
      .draw(display);

    batteryBar.setBounds(108,4)
      .setPercentage(75)
      .draw(display);

    mTime.setCursor(10, 40)
      .setFontFamily(&tt_lakes_neue_trial_black12pt7b)
      .setText("19:48 ")
      .draw(display);

    seconds.setFollowCursor(false)
      .setFontFamily(&tt_lakes_neue_trial_black8pt7b)
      .setText("58")
      .draw(display);

    countryCode.setCursor(4, 56)
      .setText("IND")
      .draw(display);
    
    date.setCursor(80, 56)
      .setText("26-11-2026")
      .draw(display);

    display.display();
}

void loop() {
    
}