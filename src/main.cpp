#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "CustomButton.hpp"

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;
constexpr uint8_t OLED_ADDR = 0x3C;

// Adjust I2C pins to match your ESP32-C3 wiring
constexpr int I2C_SDA = 4;
constexpr int I2C_SCL = 5;

// Example physical tactile buttons (active LOW with internal pull-ups)
constexpr int BTN_NEXT_PIN = 0;
constexpr int BTN_SELECT_PIN = 1;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CustomButton btnStart(8, 22, 52, 18, "START");
CustomButton btnReset(68, 22, 52, 18, "RESET");

int focusedIndex = 0;

void updateFocus() {
    btnStart.setFocused(focusedIndex == 0);
    btnReset.setFocused(focusedIndex == 1);
}

void renderUI() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(8, 6);
    display.print("Controls");

    btnStart.draw(display);
    btnReset.draw(display);

    display.display();
}

void setup() {
    Serial.begin(115200);
    pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
    pinMode(BTN_SELECT_PIN, INPUT_PULLUP);

    Wire.begin(I2C_SDA, I2C_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 allocation failed");
        for (;;) {}
    }

    updateFocus();
    renderUI();
}

void loop() {
    static bool lastNext = HIGH;
    static bool lastSelect = HIGH;

    bool curNext = digitalRead(BTN_NEXT_PIN);
    bool curSelect = digitalRead(BTN_SELECT_PIN);

    // Cycle focus on press
    if (lastNext == HIGH && curNext == LOW) {
        focusedIndex = (focusedIndex == 0) ? 1 : 0;
        updateFocus();
        renderUI();
    }

    // Visual pressed state while held
    bool isSelectHeld = (curSelect == LOW);
    if (focusedIndex == 0) {
        btnStart.setPressed(isSelectHeld);
    } else {
        btnReset.setPressed(isSelectHeld);
    }

    // Trigger action on release
    if (lastSelect == LOW && curSelect == HIGH) {
        if (focusedIndex == 0) {
            Serial.println("START clicked");
        } else {
            Serial.println("RESET clicked");
        }
    }

    if (curSelect != lastSelect) {
        renderUI();
    }

    lastNext = curNext;
    lastSelect = curSelect;
    delay(15); // Simple debounce / frame pacing
}