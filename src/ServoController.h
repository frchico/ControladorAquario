#include <Servo.h>
#include <Ticker.h>


bool ServoController__ehParaMovimentarServo = false;
bool ServoController__ehParaDesconectarServo = false;


void marcarParaMovimentarServo(){
	ServoController__ehParaMovimentarServo = true;
	Serial.println("- Chamou marcarParaMovimentarServo");
}

class ServoController {
private:
	Servo servoMotor;
	int servoPos = 0;
	int servoMoveCount = 0;
	int maxServoMoveCount = 0;
	unsigned int servoPin = 0;
	int servoAnguloAberto = 0;
	int servoAnguloFechado = 50;
	int setupServoPosMin = 500;
	int setupServoPosMax = 2400;
	Ticker TickerServo;

	
	void setServoTimer(bool newStatus){
		setServoTimer(newStatus, .5);
	}
	void setServoTimer(bool newStatus, float seconds){
		TickerServo.detach();
		if ( newStatus ){
			TickerServo.once(1,marcarParaMovimentarServo);
			Serial.println("Servo agendado....");
		}
	}

protected:
	
public:
	void printStatus(){
		Serial.println(" servoPos: " + String(servoPos));
		Serial.println(" servoMoveCount: " + String(servoMoveCount));
		Serial.println(" maxServoMoveCount: " + String(maxServoMoveCount));
		Serial.println(" servoPin: " + String(servoPin));
		Serial.println(" servoAnguloAberto: " + String(servoAnguloAberto));
		Serial.println(" servoAnguloFechado: " + String(servoAnguloFechado));
		Serial.println(" isMoving: " + String(isMoving()));
		Serial.println(" -- Servo OK --");
	}
	ServoController(
		unsigned int _servoPin, 
		int maxMoveCount, 
		int anguloPortaAberta,
		int anguloPortaFechada
		)
	{
		servoPos = anguloPortaAberta;
		servoMoveCount = maxMoveCount*2;
		maxServoMoveCount = servoMoveCount;
		servoPin = _servoPin;
		servoAnguloAberto = anguloPortaAberta,
		servoAnguloFechado = anguloPortaFechada;
		printStatus();
	}

	
	bool isMoving(){
		return servoMoveCount < maxServoMoveCount;
	}

	
	void resetMoveCount(){
		Serial.println("ResertMoveCount invocado...");
		if ( ! isMoving()  ){
			servoMoveCount = 0;
			setServoTimer(true);
		}
		else{
			Serial.println(" servoMoveCount: " + String(servoMoveCount));
			Serial.println(" maxServoMoveCount: " + String(maxServoMoveCount));
			Serial.println(" isMoving: " + String(isMoving()));
		}
	}

	void open(){
		if ( isMoving() == false){
			servoPos = servoAnguloAberto;
			servoMoveCount = maxServoMoveCount-1;
			setServoTimer(true);
		}
	}
	void close(){
		if ( isMoving() == false){
			servoPos = servoAnguloFechado;
			servoMoveCount = maxServoMoveCount-1;
			setServoTimer(true);
		}
	}
	
	void checkConnections(){
		if ( isMoving() == false){
			servoPos = servoAnguloAberto;
			servoMoveCount = maxServoMoveCount > 3 ? maxServoMoveCount-3: 0;
			setServoTimer(true);
		}
	}
	void update(){
		if ( isMoving() ){
			if (! ServoController__ehParaMovimentarServo ){
				return;
			}
			Serial.println("Update is moving...");
			ServoController__ehParaMovimentarServo = false;
			if (! servoMotor.attached() ){
				servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax);
				Serial.println("Servo conectado");
			}
			int posAntiga = servoMotor.read();
			int novaPosicao = servoPos;
			Serial.println("- Movendo servo de " + String(posAntiga) + " para " + String(novaPosicao) + " id = " + String(servoMoveCount));
			servoMotor.write(novaPosicao);
			if ( isMoving() ){
				servoPos = (servoPos == servoAnguloAberto) ? servoAnguloFechado : servoAnguloAberto;
			}
			setServoTimer(isMoving(), 1.5);
			servoMoveCount++;
		}
	}
};