#pragma once

#include <DadosNotificacao.h>
#include <ESPAsyncWiFiManager.h>
class WifiDadosNotificacao : public DadosNotificacao {
public:
   
	AsyncWiFiManager* WM;
	IPAddress IP;
	String SSID;
	WifiDadosNotificacao(const char* msg) : DadosNotificacao(TipoNotificacao::WIFI, msg){
	}
	
};
