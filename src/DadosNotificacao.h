#pragma once

#ifndef DadosNotificacao_H
#define DadosNotificacao_H
#include <Arduino.h>

// Enum para representar os tipos de notificação
enum class TipoNotificacao {
    Mensagem,
    Acao,
	Servo,
	WIFI,
    OutroTipo
};

class DadosNotificacao {
public:
    TipoNotificacao tipo;
    bool status;
    String mensagem;
	uint passoAtual = 0;
	uint qtdTotalPassos = 0;
	uint getPercentualTarefa(){
		return qtdTotalPassos ==0 ? 0 : passoAtual/qtdTotalPassos*100;
	}

    DadosNotificacao(const String& msg) : tipo(TipoNotificacao::Mensagem), status(false), mensagem(msg) {}
    DadosNotificacao(TipoNotificacao tipo, const String& msg) : tipo(tipo), status(false), mensagem(msg) {}
    DadosNotificacao(TipoNotificacao tipo) : tipo(tipo), status(false), mensagem("") {}

    // Função virtual para retornar um identificador de tipo único
    virtual int getTipoID() const { return static_cast<int>(tipo); }

    // Destrutor virtual
    virtual ~DadosNotificacao() {}
};


#endif