#pragma once
#include <DadosNotificacao.h>
#include <ServoDadosNotificacao.h>

// Interface Observador
template <typename T> 
class IObservador {
public:
    virtual void onReceberNotificacao(const T& dados) = 0;
};

class Observador: public IObservador<ServoDadosNotificacao> {
public:
    void onReceberNotificacao(const ServoDadosNotificacao& dados) override {
        // Lógica para imprimir os dados recebidos
        Serial.print("Notificação recebida em ");
        Serial.print (millis());
        Serial.print(" Tipo [");
        Serial.print(dados.getTipoID());
        Serial.print("] Mensagem: ");
        Serial.println(dados.mensagem);

		Serial.print("De [");
        Serial.print(dados.posicaoAnterior);
        Serial.print("] para [");
        Serial.print(dados.posicaoDestino);
        Serial.print("] - ");
        Serial.print(dados.nroMovimento);
        Serial.print(" / ");
        Serial.print(dados.nroMovimentoTotais);
    }
};