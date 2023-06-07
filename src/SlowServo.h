/*
   Slow Servo Movements
   based on request of https://forum.arduino.cc/t/i-want-the-servo-to-be-slower-how-do-i-control-the-speed-of-the-servo-servo-speed-control/1012695/
   
   by noiasca http://werner.rothschopf.net
   2022-07-15
*/

#include <Servo.h>


//constexpr uint8_t servoPin = 8;   // GPIO for Servo

// make your own servo class
class SlowServo {
	private:
		uint16_t minPosServo = 544;
		uint16_t maxPosServo = 2400;
		byte servoPin = D4;
  protected:
    byte target = 0;       // target angle
    byte current = 0;      // current angle
    uint8_t interval = 50;      // delay time
    uint32_t previousMillis = 0;
  public:
    Servo servo;
	bool moving = false;
	uint16_t getTarget(){ return target;}       // target angle
    uint16_t getCurrent(){return current;}
	bool isMoving(){
		return moving;
	}
	void begin(byte pin, int  minServo, int maxServo)
    {
		servoPin = pin;
		minPosServo = minServo;
		maxPosServo = maxServo;
		if( servo.attach(servoPin, minPosServo, maxPosServo, current) != 0 ){
			servo.write(target);
			delay(500);
			//servo.detach();
			Serial.println("Servo conectado.");
		}
	}
    void begin(byte pin)
    {
		begin(pin, 544, 2400);
    }

    void setSpeed(uint8_t newSpeed)
    {
      interval = newSpeed;
    }

    void set(uint16_t newTarget)
    {
		if (!isMoving()){
			Serial.println("Set...");
			//delay(2000);
			moving = true;
			servo.write(newTarget);
			delay(20);
      		target = newTarget;
			Serial.println("Set - ok, new delay(15)...");
			//delay(2000);
			//delay(15);
		}else{
			Serial.println("Aguarde o servo chegar no destino");
		}
    }

	void update()
	{
		Serial.print("       - isMoving: " ); Serial.println(isMoving());

		if (moving && millis() - previousMillis > interval)
		{
#if ! defined(DEBUG_SERVOLIB) 
		Serial.print("Servo update() " );
		Serial.print(getCurrent());
		Serial.print(" to " );
		Serial.println(getTarget());
		Serial.print("       - previousMillis: " ); Serial.println(previousMillis);
		Serial.print("       - isMoving: " ); Serial.println(isMoving());
		Serial.print("       - servo.attached(): " ); Serial.println(servo.attached());
		
#endif
			previousMillis = millis();
			current = servo.read();
			moving = getTarget() != getCurrent();
		Serial.print("       - new isMoving: " ); Serial.println(moving);
			// // if (! isMoving() ){
			// // 	if (servo.attached()){
			// // 		servo.detach();
			// // 	}
			// // } else {
			// // 	if (!servo.attached()){
			// // 		servo.attach(servoPin, minPosServo, maxPosServo, target);
			// // 	}
			// // }
			// int position = getCurrent();
			// if (target < position)
			// {
			// 	current--;
			// 	servo.write(current);
			// }
			// else if (target > position)
			// {
			// 	current++;
			// 	servo.write(current);
			// }
		}
    }
};