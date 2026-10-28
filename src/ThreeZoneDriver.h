#ifndef THREE_ZONE_DRIVER_H
#define THREE_ZONE_DRIVER_H

#include <Arduino.h>

enum Direction {
  STOP = 0,
  FORWARD = 1,
  REVERSE = 2
};

class ThreeZoneDriver {
  private:
    uint8_t _dsPin;
    uint8_t _shclkPin;
    uint8_t _rclkPin;
    uint8_t _pwmPin;
    
    uint8_t _shiftRegisterByte;
    uint8_t _currentSpeed;
    uint16_t _deadtimeUs;

    void updateShiftRegister();

  public:
    ThreeZoneDriver(uint8_t dsPin, uint8_t shclkPin, uint8_t rclkPin, uint8_t pwmPin, uint16_t deadtimeUs = 50);

    void begin();
    void setZone(uint8_t zone, Direction dir);
    void setSpeed(uint8_t speed); // 0 - 255
    void stopAll();
};

#endif
