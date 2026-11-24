#include "CustomBatteryBar.hpp"

CustomBatteryBar& CustomBatteryBar::setBounds(int16_t x, int16_t y) {
    _x = x;
    _y = y;
    return *this;
}

CustomBatteryBar& CustomBatteryBar::setPercentage(uint8_t value) {
  _percentage = value;
  return *this;
}

// Battery Icon: 16 pixels wide, 7 pixels tall.
// Each row takes 2 bytes (Byte 1: columns 0-7, Byte 2: columns 8-15).
// Initialized to 100% state.
unsigned char CustomBatteryBar::battery_icon[7 * 2] = {
  0b11111111, 0b11111100, // Top border
  0b10000000, 0b00000100, // Inner hollow row 1
  0b10111111, 0b11110111, // Inner fill row 2 (100% full)
  0b10111111, 0b11110111, // Inner fill row 3 (100% full)
  0b10111111, 0b11110111, // Inner fill row 4 (100% full)
  0b10000000, 0b00000100, // Inner hollow row 5
  0b11111111, 0b11111100  // Bottom border
};

// Function to update the battery icon array based on percentage (100 down to 0)
void CustomBatteryBar::updateBatteryIcon(uint8_t percentage) {
  // Clamp percentage between 0 and 100
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;

  // Map percentage to how many inner columns are filled (0 to 10 columns wide)
  // 100% = 10 columns filled, 0% = 0 columns filled
  int filledColumns = (percentage * 10) / 100;

  // Rebuild the 3 middle filling rows (rows index 2, 3, and 4)
  for (int r = 2; r <= 4; r++) {
    // Start with empty inner space: 10 pixels of 0s inside the casing
    uint16_t innerBits = 0;
    
    // Set bits to 1 based on current percentage
    for (int c = 0; c < filledColumns; c++) {
      innerBits |= (1 << (13 - c)); // Fill from left to right inside the battery
    }

    // Combine casing edges: Left border (bit 15) + Inner fill + Right wall (bit 5) + Terminal tip (bits 3-2)
    uint16_t fullRow = 0b1000000000000111 | innerBits;

    // Split the 16-bit row back into two 8-bit unsigned char bytes
    battery_icon[r * 2]     = (fullRow >> 8) & 0xFF; // High byte
    battery_icon[r * 2 + 1] = fullRow & 0xFF;        // Low byte
  }
}

void CustomBatteryBar::draw(Adafruit_GFX& gfx) {
  updateBatteryIcon(_percentage);
  gfx.drawBitmap(_x, _y, battery_icon, _w, _h, 1);
}