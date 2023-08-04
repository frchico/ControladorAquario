#pragma once

#include <Servo.h>
#include <Ticker.h>
#include <NotificadorDeTarefa.h>
#include <Observador.h>
#include <DadosNotificacao.h>
#include <ServoDadosNotificacao.h>
#include <AcaoDadosNotificacao.h>

#ifdef DEBUG_SERVOCONTROLL
	#define DEBUG_SERVOCONTROLL_PRINTLN(x) Serial.println(x)
#else
	#define DEBUG_SERVOCONTROLL_PRINTLN(x) 
#endif

class ServoController : 
	public Servo, 
	public NotificadorDeTarefa<ServoDadosNotificacao>, 
	public IObservador<DadosNotificacao> {
private:
	uint32_t servoPos = 0;
	uint32_t servoMoveCount = 0;
	uint32_t maxServoMoveCount = 0;
	unsigned int servoPin = 0;
	uint32_t servoAnguloAberto = 0;
	uint32_t servoAnguloFechado = 50;
	uint32_t setupServoPosMin = 500;
	uint32_t setupServoPosMax = 2400;

	bool ehParaMovimentarServo = false;

	Ticker* TickerServo;
	Ticker* TickerAlternarPosicao;
	
	bool podeAlimentarPeixes = false;
	
	void alternarPosicao();

public:
	ServoController(unsigned int _servoPin, 
		uint32_t maxMoveCount, 
		uint32_t anguloPortaAberta,
		uint32_t anguloPortaFechada
		);
	bool alimentarPeixes();
	void loop();
	void onReceberNotificacao(const DadosNotificacao& dados);
};