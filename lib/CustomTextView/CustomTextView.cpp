#include "CustomTextView.hpp"

CustomTextView& CustomTextView::setCursor(int16_t x, int16_t y) {
    _x = x;
    _y = y;
    return *this;
}

CustomTextView& CustomTextView::setTextSize(int8_t size) {
    _size = size;
    return *this;
}

CustomTextView& CustomTextView::setTextColor(int8_t textColor) {
    _textColor = textColor;
    return *this;
}

CustomTextView& CustomTextView::setText(String text) {
    _preText = _text;
    _text = text;
    return *this;
}

String CustomTextView::getText() {
    return _text;
}

CustomTextView& CustomTextView::setFontFamily(const GFXfont* font) {
    _font = font;
    return *this;
}

void CustomTextView::draw(Adafruit_GFX& gfx) {
    gfx.setTextSize(_size);
    gfx.setFont(_font);
    gfx.setCursor(_x, _y);

    if (_preText.length() > 0) {
        int16_t bx, by;
        uint16_t bw, bh;
        gfx.getTextBounds(_preText, _x, _y, &bx, &by, &bw, &bh);
        gfx.fillRect(bx, by, bw, bh, !_textColor);
    }

    gfx.setTextColor(_textColor);
    gfx.print(_text);
}

