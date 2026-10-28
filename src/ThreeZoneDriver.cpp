#include "ThreeZoneDriver.h"

ThreeZoneDriver::ThreeZoneDriver(uint8_t dsPin, uint8_t shclkPin, uint8_t rclkPin, uint8_t pwmPin, uint16_t deadtimeUs) {
  _dsPin = dsPin;
  _shclkPin = shclkPin;
  _rclkPin = rclkPin;
  _pwmPin = pwmPin;
  _shiftRegisterByte = 0b00000000;
  _currentSpeed = 0;
  _deadtimeUs = deadtimeUs;
}

void ThreeZoneDriver::begin() {
  pinMode(_dsPin, OUTPUT);
  pinMode(_shclkPin, OUTPUT);
  pinMode(_rclkPin, OUTPUT);
  pinMode(_pwmPin, OUTPUT);

  stopAll();
}

void ThreeZoneDriver::updateShiftRegister() {
  digitalWrite(_rclkPin, LOW);
  delayMicroseconds(1); 
  shiftOut(_dsPin, _shclkPin, MSBFIRST, _shiftRegisterByte);
  digitalWrite(_rclkPin, HIGH);
  delayMicroseconds(1);
}

void ThreeZoneDriver::setZone(uint8_t zone, Direction dir) {
  if (zone < 1 || zone > 3) return;

  uint8_t shift = (zone - 1) * 2;
  uint8_t currentZoneState = (_shiftRegisterByte >> shift) & 0b11;
  
  Direction oldDir = STOP;
  if (currentZoneState == 0b01) oldDir = FORWARD;
  else if (currentZoneState == 0b10) oldDir = REVERSE;

  // Insert deadtime during direct direction reversals to prevent shoot-through
  if (oldDir != STOP && dir != STOP && oldDir != dir) {
    _shiftRegisterByte &= ~(0b11 << shift);
    updateShiftRegister();
    delayMicroseconds(_deadtimeUs);
  }

  _shiftRegisterByte &= ~(0b11 << shift);

  if (dir == FORWARD) {
    _shiftRegisterByte |= (0b01 << shift);
  } else if (dir == REVERSE) {
    _shiftRegisterByte |= (0b10 << shift);
  }

  updateShiftRegister();
}

void ThreeZoneDriver::setSpeed(uint8_t speed) {
  _currentSpeed = speed;
  analogWrite(_pwmPin, _currentSpeed);
}

void ThreeZoneDriver::stopAll() {
  _shiftRegisterByte = 0x00;
  updateShiftRegister();
  analogWrite(_pwmPin, 0);
  _currentSpeed = 0;
}
