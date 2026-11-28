#include "CustomTextView.hpp"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

CustomTextView &CustomTextView::setBounds(int16_t xStart, int16_t yStart, int16_t xEnd, int16_t yEnd)
{
    _xStart = xStart;
    _yStart = yStart;
    _xEnd = xEnd;
    _yEnd = yEnd;
    return *this;
}

CustomTextView &CustomTextView::setTextSize(int8_t size)
{
    _size = size;
    return *this;
}

CustomTextView &CustomTextView::setTextColor(int8_t textColor)
{
    _textColor = textColor;
    return *this;
}

CustomTextView &CustomTextView::setText(String text)
{
    _preText = _text;
    _text = text;
    return *this;
}

String CustomTextView::getText()
{
    return _text;
}

CustomTextView &CustomTextView::setFontFamily(const GFXfont *font)
{
    _font = font;
    return *this;
}

void CustomTextView::draw(Adafruit_GFX &gfx)
{
    if (_preText != _text)
    {
        draw(gfx, _preText, SSD1306_BLACK);
    }
    draw(gfx, _text, SSD1306_WHITE);
    _preText = _text;
}

void CustomTextView::draw(Adafruit_GFX &gfx, String text, uint8_t color)
{
    if (text.length() == 0)
        return;

    bool needMeasure = (_xStart < 0 || _xEnd < 0 || _yStart < 0 || _yEnd < 0);

    uint16_t textW = 0, textH = 0;
    int16_t x1 = 0, y1 = 0;

    if (needMeasure)
    {
        GFXcanvas1 probe(1, 1);
        probe.setTextSize(_size);
        probe.setTextWrap(false);
        probe.setFont(_font);
        probe.getTextBounds(text, 0, 0, &x1, &y1, &textW, &textH);
    }

    int16_t glyphOffsetX = (needMeasure && x1 < 0) ? -x1 : 0;
    int16_t glyphOffsetY = (needMeasure && y1 < 0) ? -y1 : 0;
    int32_t measuredW = static_cast<int32_t>(textW) + glyphOffsetX;
    int32_t measuredH = static_cast<int32_t>(textH) + glyphOffsetY;

    int32_t drawX = 0;
    int32_t canvasW = 0;

    if (_xStart >= 0 && _xEnd >= 0)
    {
        drawX = _xStart;
        canvasW = static_cast<int32_t>(_xEnd) - _xStart + 1;
    }
    else if (_xEnd < 0)
    {
        int32_t startX = (_xStart < 0) ? 0 : _xStart;
        drawX = startX;
        int32_t maxAvailW = static_cast<int32_t>(SCREEN_WIDTH) - startX;
        canvasW = (measuredW < maxAvailW) ? measuredW : maxAvailW;
    }
    else
    {
        int32_t maxAvailW = static_cast<int32_t>(_xEnd) + 1;
        canvasW = (measuredW < maxAvailW) ? measuredW : maxAvailW;
        drawX = static_cast<int32_t>(_xEnd) - canvasW + 1;
    }

    int32_t drawY = 0;
    int32_t canvasH = 0;

    if (_yStart >= 0 && _yEnd >= 0)
    {
        drawY = _yStart;
        canvasH = static_cast<int32_t>(_yEnd) - _yStart + 1;
    }
    else if (_yEnd < 0)
    {
        int32_t startY = (_yStart < 0) ? 0 : _yStart;
        drawY = startY;
        int32_t maxAvailH = static_cast<int32_t>(SCREEN_HEIGHT) - startY;
        canvasH = (measuredH < maxAvailH) ? measuredH : maxAvailH;
    }
    else
    {
        int32_t maxAvailH = static_cast<int32_t>(_yEnd) + 1;
        canvasH = (measuredH < maxAvailH) ? measuredH : maxAvailH;
        drawY = static_cast<int32_t>(_yEnd) - canvasH + 1;
    }

    if (canvasW < 1)
        canvasW = 1;
    if (canvasH < 1)
        canvasH = 1;

    GFXcanvas1 canvas(static_cast<uint16_t>(canvasW), static_cast<uint16_t>(canvasH));
    canvas.setTextSize(_size);
    canvas.setTextWrap(false);
    canvas.setFont(_font);
    canvas.setTextColor(SSD1306_WHITE);
    canvas.setCursor(glyphOffsetX, glyphOffsetY);
    canvas.print(text);

    // 6-arg drawBitmap draws only set bits in `color`, avoiding background box artifacts
    gfx.drawBitmap(static_cast<int16_t>(drawX),
                   static_cast<int16_t>(drawY),
                   canvas.getBuffer(),
                   static_cast<uint16_t>(canvasW),
                   static_cast<uint16_t>(canvasH),
                   color);
}