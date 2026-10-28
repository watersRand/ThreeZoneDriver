#include <ThreeZoneDriver.h>

// Pin mapping matching your U2 Header:
// U2-4 (SERIALDS)    -> Pin 2
// U2-3 (REGISTERCLK) -> Pin 3
// U2-2 (SHIFTCLK)    -> Pin 4
// U2-1 (PWM)         -> Pin 5 (PWM capable)
ThreeZoneDriver motors(2, 3, 4, 5, 50); // 50 microseconds deadtime

void setup() {
  motors.begin();
  motors.setSpeed(200); // Set global speed (~78%)
}

void loop() {
  // Drive all 3 zones forward
  motors.setZone(1, FORWARD);
  motors.setZone(2, FORWARD);
  motors.setZone(3, FORWARD);
  delay(3000);

  // Reverse all zones (Deadtime automatically applied)
  motors.setZone(1, REVERSE);
  motors.setZone(2, REVERSE);
  motors.setZone(3, REVERSE);
  delay(3000);

  // Independent zone maneuver
  motors.setZone(1, FORWARD);
  motors.setZone(2, STOP);
  motors.setZone(3, REVERSE);
  delay(2500);

  // Stop everything
  motors.stopAll();
  delay(2000);
}
