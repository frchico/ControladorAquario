#include <Servo.h>
#include <Ticker.h>
#include <ServoInfo.h>

#ifdef DEBUG_SERVOCONTROLL
	#define DEBUG_SERVOCONTROLL_PRINTLN(x) Serial.println(x)
#else
	#define DEBUG_SERVOCONTROLL_PRINTLN(x) 
#endif

class ServoController {
private:

	Servo servoMotor;
	unsigned int servoPos = 0;
	byte servoMoveCount = 0;
	byte maxServoMoveCount = 0;
	unsigned int servoPin = 0;
	unsigned int servoAnguloAberto = 0;
	unsigned int servoAnguloFechado = 50;
	unsigned int setupServoPosMin = 500;
	unsigned int setupServoPosMax = 2400;
	Ticker TickerServo;

  	bool ehParaMovimentarServo = false;
	
	void setServoTimer(bool newStatus){
		setServoTimer(newStatus, .5);
	}
	void setServoTimer(bool newStatus, float seconds){
		TickerServo.detach();
		if ( newStatus ){
			TickerServo.once(1, [this]() { 
				ehParaMovimentarServo = true;
				DEBUG_SERVOCONTROLL_PRINTLN("Chamou ehParaMovimentarServo...");
			 });
			DEBUG_SERVOCONTROLL_PRINTLN("Servo agendado....");
		}
	}

	std::function<void(ServoInfo)> servoMoveCallback;

protected:
	void onServoMove(ServoInfo info){
		if (servoMoveCallback != nullptr )
		{
			servoMoveCallback(info);
		}  
	}
	
public:
	void setServoMoveCallback(std::function<void(ServoInfo)> callback){
		servoMoveCallback = callback; 
	}
	
	void printStatus(){
		Serial.println(" servoPos: " + String(servoPos) +
						"\n servoMoveCount: " + String(servoMoveCount) +
						"\n maxServoMoveCount: " + String(maxServoMoveCount) +
						"\n servoPin: " + String(servoPin) +
						"\n servoAnguloAberto: " + String(servoAnguloAberto) +
						"\n servoAnguloFechado: " + String(servoAnguloFechado) +
						"\n isMoving: " + String(isMoving()) +
						"\n -- Servo OK --");
	}
	~ServoController(){
		if ( servoMotor.attached() ){
			servoMotor.detach();
		}
	}
	ServoController(
		unsigned int _servoPin, 
		byte maxMoveCount, 
		unsigned int anguloPortaAberta,
		unsigned int anguloPortaFechada
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
		DEBUG_SERVOCONTROLL_PRINTLN("ResertMoveCount invocado...");
		if ( ! isMoving()  ){
			servoMoveCount = 0;
			setServoTimer(true);
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
	void loop(){
		if ( isMoving() ){
			if (! ehParaMovimentarServo ){
				return;
			}
			ServoInfo info;
			info.passoAtual = 0;
			info.passoTotal = maxServoMoveCount;

			DEBUG_SERVOCONTROLL_PRINTLN("Update is moving...");
			ehParaMovimentarServo = false;
			if (! servoMotor.attached() ){
				servoMotor.attach(servoPin, setupServoPosMin, setupServoPosMax);
				info.status = StatusServo::Iniciando;
				onServoMove(info);
				DEBUG_SERVOCONTROLL_PRINTLN("Servo conectado");
			}
			
			DEBUG_SERVOCONTROLL_PRINTLN("- Movendo servo de " + String(servoMotor.read()) + " para " + String(servoPos) + " id = " + String(servoMoveCount));
			info.status = StatusServo::Alimentando;
			info.passoAtual = servoMoveCount;
			onServoMove(info);
			servoMotor.write(servoPos);
			if ( isMoving() ){
				servoPos = (servoPos == servoAnguloAberto) ? servoAnguloFechado : servoAnguloAberto;
			}
			setServoTimer(isMoving(), 1.5);
			servoMoveCount++;
			if ( servoMoveCount == maxServoMoveCount){
				info.status = StatusServo::Finalizado;
				info.passoAtual = servoMoveCount;
				onServoMove(info);
			}
		}
	}
};