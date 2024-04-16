#pragma once

#include <Arduino.h>
#include <ServoController.h>
#include <SituacaoAlimentacao.h>
#include <ServoInfo.h>
#include <Ticker.h>

class AlimentarPeixesManager
{
private:
	ServoController* servoController;

	byte qtdDoses = 0;
	Ticker TickerAlimentarPeixesManager;
	//Monitor Monitor;

	std::function<void(AlimentarPeixesInfo)> alimentarPeixesCallback;

protected:
	void onAlimentarPeixes(AlimentarPeixesInfo info){
		if (alimentarPeixesCallback != nullptr )
		{
			alimentarPeixesCallback(info);
		}  
	}

	void onServoNotificarAlimentacao(ServoInfo servoInfo){
		AlimentarPeixesInfo info;
		info.percentual = 0;

		switch (servoInfo.status)
		{
			case StatusServo::Iniciando:{
				info.descricao = "Preparando alimentação";
				info.situacao = SituacaoAlimentacao::Iniciada;
				break;
			}
			case StatusServo::Alimentando: {
				info.descricao = "Alimentando";
				info.percentual = servoInfo.passoAtual/servoInfo.passoTotal*100;
				info.situacao = SituacaoAlimentacao::EmAndamento;
				
			} 
			case StatusServo::CheckConnections:
			case StatusServo::Fechado:
			case StatusServo::Aberto:{
				info.descricao = "Verificando Motores";
				info.situacao = SituacaoAlimentacao::Cancelada;
			}
			/* code */
		
		default:
			break;
		} 
		onAlimentarPeixes(info);

	}

	
public:
	AlimentarPeixesManager(	unsigned int _servoPin, 
		byte quantidadeDoses, 
		int anguloPortaAberta,
		int anguloPortaFechada )
	{ 
		qtdDoses = quantidadeDoses;
		servoController = new ServoController(_servoPin, qtdDoses, anguloPortaAberta, anguloPortaFechada);
	}
	~AlimentarPeixesManager(){
		if ( servoController != nullptr ){
			servoController->~ServoController();
		}
	}
	
	void setAlimentarPeixesCallback(std::function<void(AlimentarPeixesInfo)> callback){
		alimentarPeixesCallback = callback; 
	}

	
	void alimentarPeixes(){
		if ( ! servoController->isMoving() ){
			//ledTicker.attach(0.5, toggleLed);
			servoController->resetMoveCount();
		}
		AlimentarPeixesInfo info;
		info.descricao = "Iniciando alimentação dos peixes";
		info.situacao = SituacaoAlimentacao::Iniciada;
		onAlimentarPeixes(info);
	}

	void loop(){
		servoController->loop();
	}
	
};

