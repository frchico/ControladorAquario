#pragma once

#ifndef REDE_H
#define REDE_H


#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


#include <ESPAsyncWiFiManager.h> 

#include <ESP8266mDNS.h>


#include <Ticker.h>


#include "NotificadorDeTarefa.h"
#include "Observador.h"
#include "DadosNotificacao.h"
#include "AcaoDadosNotificacao.h"
#include "WifiDadosNotificacao.h"

#include "Configuracao.h"

class Rede : 
 	public NotificadorDeTarefa<WifiDadosNotificacao>, 
	public IObservador<DadosNotificacao> {
		const uint PORTA = 80;
		// #ifdef DEBUG_SERVOCONTROLL
		// 	#define DEBUG_SERVOCONTROLL_PRINTLN(x) Serial.println(x)
		// #else
		// 	#define DEBUG_SERVOCONTROLL_PRINTLN(x) 
		// #endif

	private:
		// Tela tela;
		// Ticker relogioTicker;
		AsyncWebServer server;
		DNSServer dns;
		AsyncWiFiManager* wm;
		// String msg;
		Configuracao* config;
		
		

		void configModeCallback(AsyncWiFiManager  &myWiFiManager);
	public:
		Rede();
		

		void mostrarConnectando(){
			// unsigned int counter = relogioTicker.counter;
			// Serial.print(".");
			// tela.display->clearDisplay();
			// tela.display->drawString(64, 10, "Connecting to WiFi");
			// tela.display->drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbol : inactiveSymbol);
			// tela.display->drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbol : inactiveSymbol);
			// tela.display->drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbol : inactiveSymbol);
			// tela.display->drawString(64, 45, msg);
			// tela.display->display();		
		}
		

		void onReceberNotificacao(const DadosNotificacao& dados);
		Configuracao* setup();

		
// void printAddress(DeviceAddress deviceAddress)
// { 
//   for (uint8_t i = 0; i <  8; i++)
//   {
//     Serial.print("0x");
//     if (deviceAddress[i] < 0x10) Serial.print("0");
//     Serial.print(deviceAddress[i], HEX);
//     if (i < 7) Serial.print(", ");
//   }
//   Serial.println("");
// }

};

#endif
