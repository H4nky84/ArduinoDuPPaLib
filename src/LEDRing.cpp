//
//    FILE: LEDRing.c
// VERSION: 1.0..
// PURPOSE: Library for LEDRing from DUPPA
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

// datasheet of the controller: https://www.lumISSI3745l.com/assets/pdf/core/IS31FL3745_DS.pdf

#include "LEDRing.h"
#include <Wire.h>

LEDRing::LEDRing(uint8_t add, TwoWire *theWire) {
  _add = add;
  _wire = theWire;

}


void LEDRing::LEDRing_PWM_MODE(void) {
  selectBank(ISSI3745_PAGE0);
}

void LEDRing::LEDRing_Configuration(uint8_t conf) {
  selectBank(ISSI3745_PAGE2);
  writeRegister8(ISSI3745_CONFIGURATION, conf);
}

void LEDRing::LEDRing_SetScaling(uint8_t led_n, uint8_t scal) {
  selectBank(ISSI3745_PAGE1);
  writeRegister8(led_n, scal);
}

void LEDRing::LEDRing_SetScaling(uint8_t scal) {
  selectBank(ISSI3745_PAGE1);

  for (uint8_t i = 1; i < 145; i++) {
    writeRegister8(i, scal);
  }
}

void LEDRing::LEDRing_GlobalCurrent(uint8_t curr) {
  selectBank(ISSI3745_PAGE2);
  writeRegister8(ISSI3745_GLOBALCURRENT, curr);
}

void LEDRing::LEDRing_PULLUP_DOWN(uint8_t pull) {
  selectBank(ISSI3745_PAGE2);
  writeRegister8(ISSI3745_PULLUPDOWM, pull);
}

uint8_t LEDRing::LEDRing_Temperature(void) {
  selectBank(ISSI3745_PAGE2);
  return (readRegister8(ISSI3745_TEMPERATURE));
}

void LEDRing::LEDRing_SpreadSpectrum(uint8_t spread) {
  selectBank(ISSI3745_PAGE2);
  writeRegister8(ISSI3745_SPREADSPECTRUM, spread);
}

void  LEDRing::LEDRing_Reset(void) {
  selectBank(ISSI3745_PAGE2);
  writeRegister8(ISSI3745_RESET_REG, 0xAE);

}

void  LEDRing::LEDRing_Set_RGB(uint8_t led_n, uint32_t color) {

  writeRegister8(issi_led_map[0][led_n], ((color >> 16) & 0xFF));
  writeRegister8(issi_led_map[1][led_n], ((color >> 8) & 0xFF));
  writeRegister8(issi_led_map[2][led_n], (color & 0xFF));

}

void  LEDRing::LEDRing_Set_RED(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[0][led_n], color);

}

void  LEDRing::LEDRing_Set_GREEN(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[1][led_n], color);
}

void  LEDRing::LEDRing_Set_BLUE(uint8_t led_n, uint8_t color) {

  writeRegister8(issi_led_map[2][led_n], color);

}


void  LEDRing::LEDRing_ClearAll(void) {
  uint8_t i;

  LEDRing_PWM_MODE();

  for (i = 1; i < 145; i++) {
	  writeRegister8(i,0);
  }

}

void  LEDRing::selectBank(uint8_t b) {
  writeRegister8(ISSI3745_COMMANDREGISTER_LOCK, ISSI3745_ULOCK_CODE);
  writeRegister8(ISSI3745_COMMANDREGISTER, b);
}

void LEDRing::writeRegister8(uint8_t reg, uint8_t data) {
  _wire->beginTransmission(_add);
  _wire->write(reg);
  _wire->write(data);
  _wire->endTransmission();
}

void LEDRing::writeBuff(uint8_t reg, uint8_t *data, uint8_t dim) {
  _wire->beginTransmission(_add);
  _wire->write(reg);
  _wire->write(data, dim);
  _wire->endTransmission();

}

uint8_t LEDRing::readRegister8(uint8_t reg) {
  byte rdata = 0xFF;

  _wire->beginTransmission(_add);
  _wire->write(reg);
  _wire->endTransmission();
  _wire->requestFrom(_add, (uint8_t) 1);
  if (_wire->available()) {
    rdata = _wire->read();
  }
  return rdata;
}
