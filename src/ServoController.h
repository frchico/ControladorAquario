#include <Servo.h>

class ServoController {
private:
  
  
  enum ServoState {
    IDLE,
    MOVING,
    PAUSED
  };

  Servo servoMotor;
  int servoPos;
  
  ServoState servoState;
  unsigned long servoMoveStartTime;
  unsigned int servoMoveCount;
  const unsigned int maxServoMoveCount;
  const unsigned long servoMoveDuration;
  const unsigned long servoPauseDuration;
  const unsigned long servoDelayDuration;
  const int servoPin;
  const int buttonPin;
  bool buttonPressed;
  int servoAngleMin;
  int servoAngleMax;
  const int setupServoPosMin = 500;
  const int setupServoPosMax = 2400;

protected:
  
	void stopServoMove() {
		servoState = PAUSED;
		Serial.println(" - stopServoMove()");
	}
  
	void resumeServoMove() {
		servoState = IDLE;
		servoMoveCount++; // Incrementa o contador de movimentos
		Serial.println("Movimento "+ String(servoMoveCount) + " de " + String(maxServoMoveCount*2));
		if ( ! isMoving() && servoMotor.attached() ) {
			Serial.println("- Servo Descontectado...");
			servoMotor.detach(); // Desconecta o servo
			buttonPressed = false;
		}
	}
  
	void checkButton() {
		if (digitalRead(buttonPin) == LOW && !buttonPressed) {
			buttonPressed = true;
			if ( ! isMoving() ){
				resetMoveCount();
			}
		} 
		// else if (digitalRead(buttonPin) == HIGH && buttonPressed) {
		// 	buttonPressed = false;
		// }
	}  
public:
	bool checkConnections(){
		Serial.println("Iniciando check dos servos...");
		if (! servoMotor.attached() ){
			servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax, servoAngleMin);
		} 
		Serial.println(" -- Servo em " + String(servoAngleMin) + " ...");
		servoMotor.write(servoAngleMin);
		delay(2000);
		Serial.println(" -- Servo em " + String(servoAngleMax) + " ...");
		servoMotor.write(servoAngleMax);
		delay(2000);
		Serial.println(" -- Servo em " + String(servoAngleMin) + " ...");
		servoMotor.write(servoAngleMin);
		delay(2000);
		if (! servoMotor.attached() ){
			servoMotor.detach();
		} 
		Serial.println("Fim da verificação dos servos...");
		return true;
	}
	void resetMoveCount(){
		servoMoveCount = 0;
	} 
	bool isMoving(){
		return servoMoveCount != maxServoMoveCount *2;
	}
	ServoController(int _servoPin, unsigned int maxMoveCount, unsigned long moveDuration, 
	unsigned long pauseDuration, unsigned long delayDuration, int btnPin, int minAngle, int maxAngle)
	: servoPos(minAngle),
		servoState(IDLE),
		servoMoveStartTime(0),
		servoMoveCount(maxMoveCount),
		maxServoMoveCount(maxMoveCount),
		servoMoveDuration(moveDuration),
		servoPauseDuration(pauseDuration),
		servoDelayDuration(delayDuration),
		servoPin(_servoPin),
		buttonPin(btnPin),
		buttonPressed(false),
		servoAngleMin(minAngle),
		servoAngleMax(maxAngle)
	{
		//servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax);
	
		pinMode(buttonPin, INPUT_PULLUP);
	}
  
	void update() {
		checkButton();
		
		switch (servoState) {
			case IDLE:
			if (isMoving() || buttonPressed) {
				Serial.println("- startServoMove()");
				startServoMove();
			}
			break;
			case MOVING:
			if (millis() - servoMoveStartTime >= (servoMoveDuration+servoDelayDuration)) {
				Serial.println("");
				Serial.println("- MOVING: stopServoMove()");
				stopServoMove();
			} else {
				Serial.print(".");
			}
			break;
			case PAUSED:
			if (millis() - servoMoveStartTime >= servoPauseDuration) {
				Serial.println("- resumeServoMove()");
				resumeServoMove();
			}
			break;
		}
	}
  
	void startServoMove() {
		if ( servoState != IDLE){
			Serial.println("O servo não está IDLE");
			return;
		} 
		
		
		servoState = MOVING;

		// Move o servo para a posição desejada
		servoPos = (servoPos == servoAngleMin) ? servoAngleMax : servoAngleMin; // Alterna entre 0 e 180 graus
		servoMoveStartTime = millis();

		if ( ! servoMotor.attached()) {
			if ( servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax,servoPos) != 0 ){
				//servoMotor.attach(servoPin, 544, 2400); // Reconecta o servo
				Serial.println("- Servo Conectado...");
				delay(10);
			}
		} else {
			int atual = servoMotor.read();
			Serial.println("- Servo indo de " + String (atual) + " -> "+ String(servoPos));
			servoMotor.write(servoPos);
			//delay(50);
		}
	    Serial.print(" Movendo o servo.");
	}
};
