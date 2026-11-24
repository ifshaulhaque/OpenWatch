#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>

class CustomButton {
public:

    CustomButton& setLabel(const char* label);
    CustomButton& setBounds(int16_t x, int16_t y, int16_t w, int16_t h);

    CustomButton& setFocused(bool focused);
    bool isFocused() const;

    CustomButton& setPressed(bool pressed);
    bool isPressed() const;

    CustomButton& setEnabled(bool enabled);
    bool isEnabled() const;

    // Optional: call on touch/point input to check if coordinates hit the button
    bool contains(int16_t px, int16_t py) const;

    // Renders onto any Adafruit_GFX-compatible display (SSD1306, ST7789, etc.)
    void draw(Adafruit_GFX& gfx, uint16_t activeColor = 1, uint16_t bgColor = 0) const;

private:
    int16_t _x;
    int16_t _y;
    int16_t _w;
    int16_t _h;
    const char* _label;

    bool _focused = false;
    bool _pressed = false;
    bool _enabled = true;
};