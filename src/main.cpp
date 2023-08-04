//#include <Servo.h>
#include <Arduino.h>
#include <Tela.h>
#include <Ticker.h>
#include <ServoController.h>
#include <Configuracao.h>
#include <Observador.h>
#include <DadosNotificacao.h>
#include <ServoDadosNotificacao.h>

void DEBUG_PROGRAM_PRINTLN(String x);

#ifndef STASSID
#define STASSID ""
#define STAPSK ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

Tela* tela;

void mostrarTelaConexao();

Ticker relogio(mostrarTelaConexao, 1000, 0, MILLIS);

bool situacao = true;

Observador* observadorServo;

// Função para imprimir os valores observados na tela
void imprimirValores(const ServoDadosNotificacao& dados) {
    Serial.println("Posição do Servo: " + String(dados.mensagem));
}

ServoController* servo;
Configuracao config;
void setupServoController(){
	observadorServo = new Observador();
	servo = new ServoController(config.pinServo, config.qtdVezesParaAlimentar, config.posGavetaAberta, config.posGavetaFechada);
	servo->addObserver(observadorServo);
}
void mostrarTelaConexao(){
	Serial.print("Counter ");
  	Serial.println(relogio.counter());
	if ( relogio.counter() % 10 == 0){
		servo->alimentarPeixes();
	} else if ( relogio.counter() % 20 == 0){
		tela->mostrarConnectando(situacao);
		situacao = !situacao;
	}
}


void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  tela->display.setFont(ArialMT_Plain_10);
  tela->display.setTextAlignment(TEXT_ALIGN_CENTER);
  tela->display.drawString(64 + x, 38 + y, "Tempo hoje");

  tela->display.setFont(ArialMT_Plain_24);
  tela->display.setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = "30 °C";
  tela->display.drawString(60 + x, 5 + y, temp);

  tela->display.setTextAlignment(TEXT_ALIGN_CENTER);

//  display.drawXbm(0+x, 0+y, 60, 36,img_allArray[imgAtual]); 
}
void drawTela2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  tela->display.setFont(ArialMT_Plain_10);
  tela->display.setTextAlignment(TEXT_ALIGN_CENTER);
  tela->display.drawString(64 + x, 38 + y, "Tempo hoje");

  tela->display.setFont(ArialMT_Plain_24);
  tela->display.setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = "30 °C";
  tela->display.drawString(60 + x, 5 + y, temp);

  tela->display.setTextAlignment(TEXT_ALIGN_CENTER);

//  display.drawXbm(0+x, 0+y, 60, 36,img_allArray[imgAtual]); 
}



void setupDisplay(){
	DEBUG_PROGRAM_PRINTLN("Create Tela\n");
	tela = new Tela();
	DEBUG_PROGRAM_PRINTLN("Create Tela - ok\n");
	tela->exibirTexto("Verifique a tela");

	tela->addFrameCallback(drawCurrentWeather);
	tela->addFrameCallback(drawTela2);
	DEBUG_PROGRAM_PRINTLN("Funções ok.\n");
	tela->exibirTexto("Funções ok.");
	delay(2000);
	tela->init();

}

void DEBUG_PROGRAM_PRINTLN(String x) { 
	
	Serial.println(x);
	
}


void setup() {

	Serial.begin(115200);
	setupDisplay();
	setupServoController();
	tela->mostrarConnectando(true, "WIFIEIAS");	
	relogio.start();

	DEBUG_PROGRAM_PRINTLN("Setup OK\n");
	
}

void loop() {
	tela->loop();
	relogio.update();
	servo->loop();
}
