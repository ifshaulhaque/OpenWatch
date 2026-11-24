#pragma once

#include<Arduino.h>
#include<Adafruit_GFX.h>

class CustomBatteryBar {
    public:
        CustomBatteryBar& setBounds(int16_t x, int16_t y);

        CustomBatteryBar& setPercentage(uint8_t value);
        uint8_t getPercentage();

        void draw(Adafruit_GFX& gfx);
        
    private:
        int16_t _x;
        int16_t _y;
        int16_t _w = 16;
        int16_t _h = 7;
        uint8_t _percentage;
        static unsigned char battery_icon[7 * 2];

        void updateBatteryIcon(uint8_t percentage);
};