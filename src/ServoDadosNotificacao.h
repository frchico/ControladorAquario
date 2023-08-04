#pragma once

#include <DadosNotificacao.h>
class ServoDadosNotificacao : public DadosNotificacao {
public:
    unsigned int posicaoAnterior = 0;
	unsigned int posicaoDestino = 0;

	unsigned int nroMovimento = 0;
	unsigned int nroMovimentoTotais = 0;

	ServoDadosNotificacao(const char* msg) : DadosNotificacao(TipoNotificacao::Servo, msg){
	}
	bool alimentacaoEstahFinalizada(){
		return nroMovimento == nroMovimentoTotais; 
	}
	
};
