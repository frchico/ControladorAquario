#include <Servo.h>
#include <ServoController.h>
#include <Ticker.h>

ServoController::ServoController(unsigned int _servoPin, 
	uint32_t maxMoveCount, 
	uint32_t anguloPortaAberta,
	uint32_t anguloPortaFechada
	)
{
	servoPos = anguloPortaAberta;
	servoMoveCount = maxMoveCount*2;
	maxServoMoveCount = servoMoveCount;
	servoPin = _servoPin;
	servoAnguloAberto = anguloPortaAberta,
	servoAnguloFechado = anguloPortaFechada;
	
	TickerServo = new Ticker([this]() { 
			ehParaMovimentarServo = true;
			DEBUG_SERVOCONTROLL_PRINTLN("Chamou ehParaMovimentarServo...");
			},0,1);
	TickerServo->stop();
	TickerAlternarPosicao = new Ticker([this]() { this->alternarPosicao(); },1500*1 , maxServoMoveCount, MILLIS);
	TickerAlternarPosicao->stop();
	podeAlimentarPeixes = true;
}

bool ServoController::alimentarPeixes(){
	
	DEBUG_SERVOCONTROLL_PRINTLN("Chamou alimentarPeixes...");
	if ( ! podeAlimentarPeixes ){
		return false;
	}
	podeAlimentarPeixes = false;
	ServoDadosNotificacao dadosServo("Preparando");
	dadosServo.nroMovimentoTotais = this->maxServoMoveCount;
    notificar(dadosServo);

	if (! this->attached() ){
		this->attach(servoPin, setupServoPosMin, setupServoPosMax);
		DEBUG_SERVOCONTROLL_PRINTLN("Servo conectado");
		dadosServo.mensagem = "Servo conectado";
   		notificar(dadosServo);
	}
	TickerAlternarPosicao->resume();
	dadosServo.mensagem = "Iniciando";
	dadosServo.nroMovimentoTotais = this->maxServoMoveCount;
    notificar(dadosServo);
	TickerServo->resume();
	return true;
}
void ServoController::alternarPosicao(){
	ServoDadosNotificacao dadosServo("Movendo");
	dadosServo.nroMovimentoTotais = this->maxServoMoveCount;
	dadosServo.posicaoAnterior = servoPos;
	servoPos = servoPos == servoAnguloAberto ? servoAnguloFechado : servoAnguloAberto;
	dadosServo.posicaoDestino = servoPos;
	dadosServo.nroMovimento = TickerAlternarPosicao->counter();
	this->write(servoPos);
	DEBUG_SERVOCONTROLL_PRINTLN("Chamou write...");
    notificar(dadosServo);
	
	if ( TickerAlternarPosicao->counter() == this->maxServoMoveCount ){
		this->detach();
		podeAlimentarPeixes = true;
		dadosServo.mensagem = "Concluída";
		dadosServo.status = true;
    	notificar(dadosServo);
	}

}
void ServoController::loop(){
	TickerAlternarPosicao->update();
	TickerServo->update();
}
void ServoController::onReceberNotificacao(const DadosNotificacao& dados) {
	// Realizando a distinção de tipo manualmente usando a função getTipoID()
    if (dados.getTipoID() == static_cast<int>(TipoNotificacao::Acao)) {
        const AcaoDadosNotificacao* acaoNotificacao = static_cast<const AcaoDadosNotificacao*>(&dados);
        switch (acaoNotificacao->executarAcao) {
            case AcaoDadosNotificacao::Acao::Alimentar:
                alimentarPeixes();
                break;
            default:
                break;
        }
    } else {
        // Não é uma notificação de ação
        Serial.println("Não é uma notificação de ação");
    }

}
