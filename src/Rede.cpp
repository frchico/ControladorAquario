#include <Rede.h>


void Rede::configModeCallback(AsyncWiFiManager  &myWiFiManager) {
	// WifiDadosNotificacao wfn("Entered config mode");
	// wfn.WM = myWiFiManager;
	// wfn.IP = WiFi.softAPIP();
	// wfn.SSID = myWiFiManager->getConfigPortalSSID();
	// Serial.println(wfn.IP);
	// Serial.println(wfn.mensagem);
	// Serial.println(wfn.SSID);
	// notificar(wfn);
}
Rede::Rede() : server(80), dns()
{
	wm = new AsyncWiFiManager(&server, &dns);
	wm->setAPCallback([this](AsyncWiFiManager* wm) { this->configModeCallback(*wm); });
}

// Opção 2
// Rede::Rede() : server(80), dns()
// {
//     wm = new AsyncWiFiManager(&server, &dns);
//     wm->setAPCallback([&](AsyncWiFiManager& wm) { this->configModeCallback(wm); });
//     // Alterar para referência na captura ---------^
// }


void Rede::onReceberNotificacao(const DadosNotificacao& dados) {
	// // Realizando a distinção de tipo manualmente usando a função getTipoID()
	// if (dados.getTipoID() == static_cast<int>(TipoNotificacao::Acao)) {
	// 	const AcaoDadosNotificacao* acaoNotificacao = static_cast<const AcaoDadosNotificacao*>(&dados);
	// 	switch (acaoNotificacao->executarAcao) {
	// 		case AcaoDadosNotificacao::Acao::ReconectarWIFI:
				
	// 			break;
	// 		default:
	// 			break;
	// 	}
	// } else {
	// 	// Não é uma notificação de ação
	// 	Serial.println("Não é uma notificação de ação");
	// }

}


Configuracao* Rede::setup(){

	WifiDadosNotificacao dados("SETUP WIFI");
	dados.passoAtual++;
	dados.qtdTotalPassos = 7;
	
	config = new Configuracao();
	dados.WM = wm;
	char buffer[10];

	notificar(dados);

	dados.mensagem = "Configurando parâmetros";
	dados.passoAtual++;
	notificar(dados);

	AsyncWiFiManagerParameter custom_aqua_nome_param("NomeDisp", "Nome do dispositivo", config->nomeDispositivo.c_str(), 15);
	wm->addParameter(&custom_aqua_nome_param);

	AsyncWiFiManagerParameter custom_servo_min_param("SMin", "Alimentador Fechado", itoa(config->posGavetaFechada, buffer,10), 3);
	wm->addParameter(&custom_servo_min_param);

	AsyncWiFiManagerParameter custom_servo_max_param("SMax", "Alimentador Aberto", itoa(config->posGavetaAberta, buffer,10), 3);
	wm->addParameter(&custom_servo_max_param);

	AsyncWiFiManagerParameter custom_qtd_vezes_alimentar("SVezes", "Quantas Vezes", itoa(config->qtdVezesParaAlimentar, buffer,10), 3);
	wm->addParameter(&custom_qtd_vezes_alimentar);

	dados.mensagem = "Tentando conectar";
	dados.passoAtual++;
	notificar(dados);


	config->connected = wm->autoConnect(config->apName.c_str(),config->apPwd.c_str(), 10); // password protected ap
	if(!config->connected) {
		dados.mensagem = "Não foi possível conectar!";
		dados.passoAtual = dados.qtdTotalPassos;
		notificar(dados);
	} 
	else {
		config->IP = WiFi.localIP().toString();
		config->apName = wm->getConfiguredSTASSID();
		config->nomeDispositivo = String(custom_aqua_nome_param.getValue());
		config->posGavetaFechada = atoi(custom_servo_min_param.getValue());
		config->posGavetaAberta = atoi(custom_servo_max_param.getValue());
		config->qtdVezesParaAlimentar = atoi(custom_qtd_vezes_alimentar.getValue());
	
		dados.mensagem = "Conexão ok!";
		dados.passoAtual++;
		notificar(dados);
	
		dados.mensagem = "Iniciando componentes adicionais";
		dados.passoAtual++;
		notificar(dados);

		server.begin();
		dados.mensagem = "Server, ok.";
		dados.passoAtual++;
		notificar(dados);

		MDNS.begin(config->apName);
		dados.mensagem = "DNS, ok.";
		dados.passoAtual++;
		notificar(dados);
		
	}
	return config;
	// // reset settings - wipe stored credentials for testing
	// // these are stored by the esp library
	// //wm.resetSettings();

	// // Automatically connect using saved credentials,
	// // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
	// // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
	// // then goes into a blocking loop awaiting configuration and will return success result

	// // DEBUG_PROGRAM_PRINTLN("Carregando WI-FI (3/" + String(totalPassos) + " - Tentando conectar em [" + wm.getConfigPortalSSID() + "])");

	// // passoAtual ++;
	// // percentual = passoAtual / totalPassos * 100;
	// // msg = "Conectando ao WI-FI (" + String(passoAtual) + "/" + String(totalPassos) + ")";
	// // DEBUG_PROGRAM_PRINTLN(msg);
	// //tela.drawProgress(percentual, msg);


	// //relogioTicker.start();
	// bool res;
	// // res = wm.autoConnect(); // auto generated AP name from chipid
	// // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
	// res = wm->autoConnect("AutoConnectAP","12345678", 10); // password protected ap
	// configuracao.connected = res;
	// if(!res) {
	// 	// relogioTicker.stop();
	// 	// DEBUG_PROGRAM_PRINTLN("Failed to connect WI-FI " + wm.getConfiguredSTASSID());
	// 	// ESP.restart();
	// } 
	// else {
	// 	configuracao.IP = WiFi.localIP().toString();
		
	// 	// passoAtual ++;
	// 	// percentual = passoAtual / totalPassos * 100;
	// 	// msg = "Conectando IP Address: " + configuracao.IP;
	// 	// DEBUG_PROGRAM_PRINTLN(msg);
	// 	// tela.drawProgress(percentual, msg);
	// 	server.begin();
	// }
	// return configuracao;
}