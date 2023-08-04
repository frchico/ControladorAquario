#pragma once

#include <vector>
#include <Observador.h>

// Classe NotificadorDeTarefa
template <typename T>
class NotificadorDeTarefa {
private:
    std::vector<IObservador<T>*> observadores;

public:
    void addObserver(IObservador<T>* observador) {
        observadores.push_back(observador);
    }

    void notificar(const T& dados) {
        for (IObservador<T>* observador : observadores) {
            observador->onReceberNotificacao(dados);
        }
    }
};