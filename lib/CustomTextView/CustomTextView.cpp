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

CustomTextView& CustomTextView::setFollowCursor(bool followCursor) {
    _followCursor = followCursor;
    return *this;
}

void CustomTextView::draw(Adafruit_GFX& gfx) {
    gfx.setTextColor(_textColor);
    gfx.setTextSize(_size);
    gfx.setFont(_font);
    if (_followCursor) gfx.setCursor(_x, _y);
    gfx.print(_text);
}

