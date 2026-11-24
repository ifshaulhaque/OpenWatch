#include "CustomButton.hpp"

CustomButton& CustomButton::setLabel(const char* label) {
    _label = label ? label : "";
    return *this;
}

CustomButton& CustomButton::setBounds(int16_t x, int16_t y, int16_t w, int16_t h) {
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    return *this;
}

CustomButton& CustomButton::setFocused(bool focused) {
    _focused = focused;
    return *this;
}

bool CustomButton::isFocused() const {
    return _focused;
}

CustomButton& CustomButton::setPressed(bool pressed) {
    _pressed = pressed;
    return *this;
}

bool CustomButton::isPressed() const {
    return _pressed;
}

CustomButton& CustomButton::setEnabled(bool enabled) {
    _enabled = enabled;
    return *this;
}

bool CustomButton::isEnabled() const {
    return _enabled;
}

bool CustomButton::contains(int16_t px, int16_t py) const {
    return (px >= _x) && (px < (_x + _w)) && (py >= _y) && (py < (_y + _h));
}

void CustomButton::draw(Adafruit_GFX& gfx, uint16_t activeColor, uint16_t bgColor) const {
    gfx.setTextSize(1);
    gfx.setTextWrap(false);

    // Inverted fill when pressed; double outline when focused; dashed/hollow when disabled
    if (_pressed && _enabled) {
        gfx.fillRoundRect(_x, _y, _w, _h, 3, activeColor);
        gfx.setTextColor(bgColor);
    } else {
        gfx.fillRoundRect(_x, _y, _w, _h, 3, bgColor);
        if (_enabled) {
            gfx.drawRoundRect(_x, _y, _w, _h, 3, activeColor);
            if (_focused) {
                gfx.drawRoundRect(_x + 1, _y + 1, _w - 2, _h - 2, 2, activeColor);
            }
        } else {
            // Dotted/sparse border for disabled state
            for (int16_t cx = _x; cx < _x + _w; cx += 3) {
                gfx.drawPixel(cx, _y, activeColor);
                gfx.drawPixel(cx, _y + _h - 1, activeColor);
            }
        }
        gfx.setTextColor(activeColor);
    }

    // Center text inside the button bounds (default 5x7 font glyph size)
    int16_t len = static_cast<int16_t>(strlen(_label));
    int16_t textW = len * 6; // 5px glyph + 1px spacing
    int16_t textH = 7;

    int16_t textX = _x + (_w - textW) / 2;
    int16_t textY = _y + (_h - textH) / 2;

    if (textX < _x + 2) textX = _x + 2;
    if (textY < _y + 2) textY = _y + 2;

    gfx.setCursor(textX, textY);
    gfx.print(_label);
}