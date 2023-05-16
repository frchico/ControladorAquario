/*
   Slow Servo Movements
   based on request of https://forum.arduino.cc/t/i-want-the-servo-to-be-slower-how-do-i-control-the-speed-of-the-servo-servo-speed-control/1012695/
   
   by noiasca http://werner.rothschopf.net
   2022-07-15
*/

#include <Servo.h>


constexpr uint8_t servoPin = 8;   // GPIO for Servo

// make your own servo class
class SlowServo {
  protected:
    uint16_t target = 90;       // target angle
    uint16_t current = 90;      // current angle
    uint8_t interval = 50;      // delay time
    uint32_t previousMillis = 0;
  public:
    Servo servo;

    void begin(byte pin)
    {
      servo.attach(pin);
    }

    void setSpeed(uint8_t newSpeed)
    {
      interval = newSpeed;
    }

    void set(uint16_t newTarget)
    {
      target = newTarget;
    }

    void update()
    {
      if (millis() - previousMillis > interval)
      {
        previousMillis = millis();
        if (target < current)
        {
          current--;
          servo.write(current);
        }
        else if (target > current)
        {
          current++;
          servo.write(current);
        }
      }
    }
};