#include <Servo.h>
#include <Ticker.h>

bool ehParaMoverServo = false;
bool ehParaDesconectarServo = false;
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
	unsigned int servoMoveCount;
	const unsigned int maxServoMoveCount;
	const unsigned long servoMoveDuration;
	const int servoPin;
	const int buttonPin;
	bool buttonPressed;
	int servoAngleMin;
	int servoAngleMax;
	const int setupServoPosMin = 500;
	const int setupServoPosMax = 2400;

	Ticker servoTicker;
	Ticker servoTickerControll;
protected:
  	static void moverServo() {
		if ( ! ehParaMoverServo ){
			Serial.println("moverServo acionado...");
			ehParaMoverServo = true;
		}
	}
	static void marcarParaDesconectarServo(){
		ehParaDesconectarServo = true;
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

	
	void close(){
		servoMoveCount = 1;
		if ( ! servoMotor.attached()) {
			if ( servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax, servoAngleMax) != 0 ){
				Serial.println("- Servo Conectado...");
				//yield();
			}
			servoPos = servoAngleMax;
			servoTickerControll.attach(2, marcarParaDesconectarServo);
		}
		servoMoveCount = maxServoMoveCount;
	}
	void open(){
		servoMoveCount = 1;
		if ( ! servoMotor.attached()) {
			if ( servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax, servoAngleMin) != 0 ){
				Serial.println("- Servo Conectado...");
			//	yield();
			}
			servoPos = servoAngleMin;
			servoTickerControll.attach(2, marcarParaDesconectarServo);
		}
		servoMoveCount = maxServoMoveCount;
	}
	~ServoController(){
		servoTicker.~Ticker();
		if (servoMotor.attached()){
			servoMotor.detach();
		}
		servoMotor.~Servo();
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
			servoTicker.attach(servoMoveDuration, moverServo);
		}
	} 
	bool isMoving(){
		return servoMoveCount != maxServoMoveCount;
	}
	ServoController(
		int _servoPin, 
		unsigned int maxMoveCount, 
		int minAngle,
		int maxAngle)
	: servoPos(minAngle),
		servoState(IDLE),
		servoMoveCount((maxMoveCount+1)*2),
		maxServoMoveCount((maxMoveCount+1)*2),
		servoMoveDuration(2.5),
		servoPin(_servoPin),
		buttonPin(0),
		buttonPressed(false),
		servoAngleMin(minAngle),
		servoAngleMax(maxAngle)
	{
		//servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax);
		//pinMode(buttonPin, INPUT_PULLUP);
	}
	ServoController(
		int _servoPin, 
		unsigned int maxMoveCount, 
		unsigned long moveDuration, 
		int btnPin, 
		int minAngle,
		int maxAngle)
	: servoPos(minAngle),
		servoState(IDLE),
		servoMoveCount((maxMoveCount+1)*2),
		maxServoMoveCount((maxMoveCount+1)*2),
		servoMoveDuration(moveDuration),
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
			servoTicker.attach(servoMoveDuration, moverServo);
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
		if ( ehParaDesconectarServo ){
			if (servoMotor.attached()){
				servoMotor.detach();
				Serial.println("servoMotor detach...");	
			}
			if (servoTickerControll.active()){
				servoTickerControll.detach();
			}
		}
	}
};
