#pragma once

#ifndef MEU__DISPLAY_CONTROL_IMAGES_H
#define MEU__DISPLAY_CONTROL_IMAGES_H


#define WiFi_Logo_width 60
#define WiFi_Logo_height 36

const uint8_t WiFi_Logo_bits2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

const uint8_t activeSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000
};

const uint8_t inactiveSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000
};

// 'Alimentar', 60x36px
const unsigned char img_Alimentar [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xfe, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x80, 0x07, 0x00, 0x00, 
  0x00, 0xe0, 0x03, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
  0x00, 0xc0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0xc0, 0x01, 0x00, 0x1e, 0x30, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x7e, 0x70, 0x00, 0x00, 
  0x00, 0x80, 0x01, 0x00, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x80, 0x83, 0x00, 0xf8, 0x61, 0x00, 0x00, 
  0x00, 0x80, 0x83, 0x00, 0x00, 0xe1, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x01, 0x00, 0xe2, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0x03, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x00, 0x7c, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0x07, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xc0, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc3, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'Aquario', 60x36px
const unsigned char img_Aquario [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 
  0x00, 0x80, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x80, 0x60, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0xc0, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x30, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x18, 0x06, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x08, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x80, 0x01, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0xc0, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x08, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
  0x00, 0x80, 0xfc, 0xff, 0xff, 0x33, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 
  0x00, 0x80, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'AquarioPeixe', 60x36px
const unsigned char img_AquarioPeixe [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xf8, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x7f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x18, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x60, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x78, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x80, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x80, 0x01, 0x00, 0x18, 0x0e, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x18, 0x0c, 0x00, 0x00, 
  0x00, 0xc0, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xf8, 0x40, 0x1c, 0x00, 0x00, 
  0x00, 0xc0, 0x78, 0xf8, 0x41, 0x18, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xf1, 0x0f, 0x18, 0x00, 0x00, 
  0x00, 0xc0, 0xe0, 0xff, 0x17, 0x18, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xff, 0x37, 0x18, 0x00, 0x00, 
  0x00, 0xc0, 0xc0, 0xff, 0x1f, 0x18, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0x1f, 0x0c, 0x00, 0x00, 
  0x00, 0xc0, 0xc0, 0xf1, 0x07, 0x0c, 0x00, 0x00, 0x00, 0x80, 0x41, 0x70, 0x00, 0x0c, 0x00, 0x00, 
  0x00, 0x80, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x1c, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x38, 0xff, 0xe3, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x1f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'SemNet', 60x36px
const unsigned char img_SemNet [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x78, 0x20, 0x00, 0x00, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0x70, 0x06, 0xcc, 0xc1, 0x03, 0x00, 0x00, 0x78, 0xe0, 0x07, 0xfc, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0xc0, 0x03, 0x78, 0xc0, 0x03, 0x00, 0x00, 0x78, 0xc0, 0x03, 0x78, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0xe0, 0x07, 0xfc, 0xc0, 0x03, 0x00, 0x00, 0x78, 0x70, 0x06, 0xcc, 0x41, 0x03, 0x00, 
  0x00, 0x78, 0x20, 0x04, 0x84, 0x00, 0x02, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0x00, 0xf8, 0x03, 0xc0, 0x03, 0x00, 0x00, 0x78, 0x00, 0xfc, 0x0f, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0x00, 0x0c, 0x04, 0xc0, 0x03, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 
  0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 
  0x00, 0xe0, 0x3f, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'TermometroCasa', 60x36px
const unsigned char img_TermometroCasa [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xf0, 0x19, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0d, 0x06, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x10, 0x07, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x18, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x10, 0x01, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x60, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x10, 0x38, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x48, 0x80, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0x18, 0x48, 0x1e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x48, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x06, 0x48, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x03, 0x48, 0x0e, 0x18, 0x00, 0x00, 
  0x00, 0x80, 0x01, 0x48, 0x00, 0x30, 0x00, 0x00, 0x00, 0xc0, 0x08, 0x48, 0x00, 0x64, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x48, 0x0e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x48, 0x00, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x48, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x48, 0x1e, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x48, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x48, 0x00, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x48, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x48, 0x00, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x48, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x48, 0x06, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0xcc, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x86, 0xfc, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xf8, 0x82, 0xfd, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1520)
const int img_allArray_LEN = 5;
const unsigned char* ImagensAquarioList[5] = {
  img_Alimentar,
  img_Aquario,
  img_AquarioPeixe,
  img_SemNet,
  img_TermometroCasa
};


#endif