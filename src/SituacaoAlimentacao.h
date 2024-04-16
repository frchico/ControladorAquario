#pragma once

#ifndef ALIMENTAR_PEIXES_INFO_H
#define ALIMENTAR_PEIXES_INFO_H

#include <Arduino.h>
enum SituacaoAlimentacao {
	Concluida,
	EmAndamento,
	Iniciada,
	Cancelando,
	Cancelada
};
// Função para obter o texto correspondente ao enum
const char* situacaoAlimentacaoToString(SituacaoAlimentacao situacao) {
    switch (situacao) {
        case Concluida: return "Concluída";
        case EmAndamento: return "Em Andamento";
        case Iniciada: return "Iniciada";
        case Cancelando: return "Cancelando";
        case Cancelada: return "Cancelada";
        default: return "Desconhecida"; // Valor desconhecido
    }
}



// Definição da estrutura AlimentarPeixesInfo
struct AlimentarPeixesInfo {
    SituacaoAlimentacao situacao;
    double percentual;
    String descricao;
};

#endif // ALIMENTAR_PEIXES_INFO_H
