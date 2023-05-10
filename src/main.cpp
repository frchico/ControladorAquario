#include <Arduino.h>
#include <Servo.h>
 
#define SERVO_PIN 2 // Porta Digital 6 PWM
 
Servo servo; // Variável Servo
int pos; // Posição Servo
 
void setup ()
{
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
  servo.write(0); // Inicia motor posição zero

  Serial.write("Pos = 0 \n");
	servo.write(0);
		delay(1000);
	
	servo.write(45);
		delay(1000);
	
	servo.write(180);
		delay(1000);

		
	
	servo.write(0);
	delay(3000);
	Serial.write("Pos = OK \n");
}
 
void loop()
{
	
	
}