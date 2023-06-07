#include <Servo.h>
#include <Ticker.h>

bool ehParaMoverServo = false;

class ServoController {
private:
	enum ServoState {
		IDLE,
		STARTING,
		MOVING,
		PAUSED,
		FINISHED
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

	Ticker servoTicker;
protected:
  
	
  
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

	~ServoController(){
		servoTicker.~Ticker();
		if (servoMotor.attached()){
			servoMotor.detach();
		}
		servoMotor.~Servo();
	}
	static void moverServo() {
		if ( ! ehParaMoverServo ){
			Serial.println("moverServo acionado...");
			ehParaMoverServo = true;
		}
	}
	bool checkConnections(){
		Serial.println("Iniciando check dos servos...");
		if (! servoMotor.attached() ){
			servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax, servoAngleMin);
		} 
		Serial.println(" -- Servo em " + String(servoAngleMin) + " ...");
		servoMotor.write(servoAngleMin);
		delay(1000);
		Serial.println(" -- Servo em " + String(servoAngleMax) + " ...");
		servoMotor.write(servoAngleMax);
		delay(1000);
		Serial.println(" -- Servo em " + String(servoAngleMin) + " ...");
		servoMotor.write(servoAngleMin);
		delay(1000);
		if (! servoMotor.attached() ){
			servoMotor.detach();
		} 
		Serial.println("Fim da verificação dos servos...");
		return true;
	}
	void resetMoveCount(){
		if ( !isMoving()){
			servoMoveCount = 0;
		//	Serial.println("Ticker acionado...");
			servoTicker.attach(2.5, moverServo);
		}
	} 
	bool isMoving(){
		return servoMoveCount != maxServoMoveCount;
	}
	ServoController(int _servoPin, unsigned int maxMoveCount, unsigned long moveDuration, 
	unsigned long pauseDuration, unsigned long delayDuration, int btnPin, int minAngle, int maxAngle)
	: servoPos(minAngle),
		servoState(IDLE),
		servoMoveStartTime(0),
		servoMoveCount((maxMoveCount+1)*2),
		maxServoMoveCount((maxMoveCount+1)*2),
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
		//pinMode(buttonPin, INPUT_PULLUP);
	}
  
	void update() {
		if ( ehParaMoverServo ){
			ehParaMoverServo = false;
			if (servoTicker.active()){
				servoTicker.detach();
				Serial.println("servoTicker detach...");
			}
			
			//Serial.println("Update ehParaMoverServo...");
			if ( ! servoMotor.attached()) {
				if ( servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax) != 0 ){
					Serial.println("- Servo Conectado...");
					yield();
				} 
			}
			int atual = servoMotor.read();
			servoMotor.write(servoPos);
			Serial.println("- Servo indo de " + String (atual) + " -> "+ String(servoPos) + " move: " + String(servoMoveCount));
			servoPos = (servoPos == servoAngleMin) ? servoAngleMax : servoAngleMin; // Alterna entre 0 e 180 graus
			servoMoveCount++;
			Serial.println("Ticker acionado...");
			servoTicker.attach(2, moverServo);
			//yield();
		}
		
		if ( ! isMoving() ){
				//Serial.println("! isMoving :D ");
			if (servoTicker.active()){
				servoTicker.detach();
				Serial.println("servoTicker detach...");
			}
			if (servoMotor.attached()){
				servoMotor.detach();
				Serial.println("servoMotor detach...");
			}
		}
		
	}
};
