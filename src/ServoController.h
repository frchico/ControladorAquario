#include <Servo.h>
#include <Ticker.h>

bool ehParaMoverServo = false;
bool ehParaDesconectarServo = false;
enum ServoCheckStage {
		IDLE,
		ABERTO,
		MOVING,
		FECHADO,
		FINISHED
	};

ServoCheckStage servoCheckStage = ServoCheckStage::IDLE;
ServoCheckStage nextStage= ServoCheckStage::IDLE;
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
	
	void servoRefeshConnection(){
		if ( ! servoMotor.attached()) {
			if ( servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax) != 0 ){
				Serial.println("- Servo Conectado...");
				//yield();
			} 
		}
	}
	void desconectarServo(){
		if (servoMotor.attached()){
			servoMotor.detach();
			Serial.println("servoMotor detach...");
		}
	}

protected:
  	static void moverServo() {
		if ( ! ehParaMoverServo ){
			Serial.println("moverServo acionado...");
			ehParaMoverServo = true;
		}
	}
	static void marcarParaDesconectarServo(){
		Serial.println("Marcado para desconctar"); 
		ehParaDesconectarServo = true;
	}
	static void irParaProximoEstagio(){
		Serial.print("Indo de " + String(servoCheckStage) + " para o estágio "+ String(nextStage));
		servoCheckStage = nextStage;
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
		if ( isMoving() )
			return;
		servoPos = servoAngleMin;
		servoMoveCount = maxServoMoveCount -2;
		servoTicker.attach(servoMoveDuration, moverServo);
	}
	void open(){
		if ( isMoving() )
			return;
		servoPos = servoAngleMax;
		servoMoveCount = maxServoMoveCount -2;
		servoTicker.attach(servoMoveDuration, moverServo);
	}
	~ServoController(){
		servoTicker.~Ticker();
		desconectarServo();
		servoMotor.~Servo();
	}
	
	bool checkConnections(){
		
		if ( isMoving() )
			return false;
		servoMoveCount = maxServoMoveCount -3;
		servoPos = servoAngleMin;
		servoTicker.attach(servoMoveDuration, moverServo);
		return true;
	}
	void resetMoveCount(){
		if ( !isMoving()){
			servoMoveCount = 0;
			servoPos = servoAngleMin;
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
		if ( servoCheckStage != ServoCheckStage::IDLE ){
			if ( servoCheckStage == ServoCheckStage::MOVING){
				return;
			}
			ehParaMoverServo = false;
			ehParaDesconectarServo = false;
			if( servoTickerControll.active() ){
				servoTickerControll.detach();
			}
			servoRefeshConnection();
			int atual = servoMotor.read();
			int destino = servoAngleMin;
			switch (servoCheckStage)
			{
				case ServoCheckStage::ABERTO: {
					servoCheckStage = ServoCheckStage::MOVING;
					destino = servoAngleMin;
					nextStage = ServoCheckStage::FECHADO;
					break;
				}
				case ServoCheckStage::FECHADO: {
					servoCheckStage = ServoCheckStage::MOVING;
					destino = servoAngleMax;
					nextStage = ServoCheckStage::FINISHED;
					break;
				}
				case ServoCheckStage::FINISHED: {
					servoCheckStage = ServoCheckStage::MOVING;
					destino = servoPos;
					nextStage = ServoCheckStage::IDLE;
					ehParaDesconectarServo = true;
					servoMoveCount = maxServoMoveCount;
					break;
				}
				default:
					break;
			}
			servoMotor.write(destino);
			Serial.println("- Servo indo de " + String (atual) + " -> "+ String(destino));
			servoTickerControll.attach(servoMoveDuration, irParaProximoEstagio);
		}
		if ( ehParaMoverServo ){
			ehParaMoverServo = false;
			if (servoTicker.active()){
				servoTicker.detach();
				Serial.println("servoTicker detach...");
			}
			
			//Serial.println("Update ehParaMoverServo...");
			servoRefeshConnection();
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
			desconectarServo();			
		}
		if ( ehParaDesconectarServo ){
			desconectarServo();
			if (servoTickerControll.active()){
				servoTickerControll.detach();
			}
		}
	}
};
