#pragma once
#ifndef AcaoDadosNotificacao_H
#define AcaoDadosNotificacao_H
class AcaoDadosNotificacao : public DadosNotificacao {

public:
    enum class Acao {
        Alimentar,
        AbrirPorta,
        FecharPorta,
        TestarServo,
		ReconectarWIFI,
        Nenhuma
    };

    Acao executarAcao;

    AcaoDadosNotificacao(Acao acao) : DadosNotificacao(TipoNotificacao::Acao, "Executar ação"), executarAcao(acao) {}

    // Sobrescrevendo a função para retornar o identificador de tipo único desta classe
    int getTipoID() const override { return static_cast<int>(TipoNotificacao::Acao); }
};


#endif