#include <Servo.h>

Servo servo; // Variável Servo
int pos; // Posição Servo
 
void alimentarPeixes(){
	Serial.write("Pos = 0 \n");
	servo.write(0);
	delay(200);
	
	servo.write(45);
	delay(200);
	
	servo.write(90);
	delay(200);
	
	servo.write(180);
	delay(200);

	servo.write(100);
	delay(200);	


	servo.write(180);
	delay(200);

	servo.write(90);
	delay(200);

	servo.write(45);
	delay(200);

	servo.write(0);
	delay(200);

	servo.write(25);
	delay(200);

	servo.write(0);
	delay(200);
}

void setupAlimentadorPeixe(int SERVO_PIN){
	servo.attach(SERVO_PIN);
	servo.write(0); // Inicia motor posição zero
}