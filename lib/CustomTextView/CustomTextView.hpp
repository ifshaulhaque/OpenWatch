#pragma once

#include<Arduino.h>
#include<Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class CustomTextView {
    public:
        CustomTextView& setBounds(int16_t xStart, int16_t yStart, int16_t xEnd, int16_t yEnd);

        CustomTextView& setTextSize(int8_t size);
        int8_t getTextSize();

        CustomTextView& setTextColor(int8_t textColor);
        int8_t getTextColor();

        CustomTextView& setText(String text);  
        String getText();

        CustomTextView& setFontFamily(const GFXfont* font);

        void draw(Adafruit_GFX& gfx);
        
    private:
        int16_t _xStart;
        int16_t _yStart;
        int16_t _xEnd;
        int16_t _yEnd;
        String _text;
        const GFXfont* _font = nullptr;
        int8_t _size = 1;
        int8_t _textColor = SSD1306_WHITE;

        String _preText;

        void draw(Adafruit_GFX& gfx, String text, uint8_t color);
};