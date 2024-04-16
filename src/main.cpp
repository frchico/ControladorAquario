#include <Servo.h>

void DEBUG_PROGRAM_PRINTLN(String x);

#ifndef STASSID
#define STASSID ""
#define STAPSK ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


#include <ESPAsyncWiFiManager.h> 

AsyncWebServer server(80);
DNSServer dns;


#define ESPALEXA_ASYNC //it is important to define this before #include <Espalexa.h>!
#include <Espalexa.h>
Espalexa espalexa;


int posGavetaAberta = 15;
int posGavetaFechada = 43;
int qtdVezesParaAlimentar = 2;


bool ehParaAlimentarPorIntervaloTempo = true;
ushort intervaloAlimentacao = 60* 60 * 4 ; // 6 hr em segundos = 6 * 60 * 60  
ulong ultimaAlimentacao = 0;

#include <ServoController.h>
//ServoController servoController(D5, qtdVezesParaAlimentar, posGavetaAberta, posGavetaFechada); // Instância do ServoController
ServoController* servoController; 


#include <Relogio.h>

// #include <Ticker.h>
//Ticker ledTicker;
const int pinDS18B20 =D7; // Pino onde o DS18B20 está conectado
const int pinServo = D5;
const int LedAlimentarPIN = LED_BUILTIN; // Ligado no pino D4 do wemos d1 r1 //5;// LED_BUILTIN; // Pino do LED do ESP8266
const int ledPinVerde = 4;
bool ledState = false;


bool ehParaNotificarAlexaNoTermino = false;

bool telaLigada = true;
String ligadoDesde;

float temperatureC = 0;

#include <jled.h>
// turn builtin LED on after 1 second.
auto led = JLed(LedAlimentarPIN).Blink(1000, 500).Forever();


void alimentarPeixes();
void toggleLed() {
  ledState = !ledState;
  digitalWrite(LedAlimentarPIN, ledState);
}


#include <EasyButton.h>
#define pinButtonAlimentar 0 //D3
// Number of presses.
int pressesButton = 3;
// Timeout.
int timeoutButton = 2000;
// Duration.
int durationButton = 2000;
// Button.
EasyButton *buttonAlimentarPeixes;
// Callback.
void onSequenceFactoryButtonMatched() {
	Serial.println("Botao factory clicado");
	alimentarPeixes();
}
void onPressedForDuration(){
	Serial.println("Button has been pressed for the given duration!");
	servoController->checkConnections();	 
}


EspalexaDevice* alexaServoDevice;
EspalexaDevice* alexaAquaModoAuto;



Relogio relogio;

bool ehParaExibirRelogio = false;


void checkTimeParaAlimentar(){
	if ( ehParaAlimentarPorIntervaloTempo ){
		alimentarPeixes();
	}
	ultimaAlimentacao = millis();
}
Ticker alimentacaoTicker;


/**
 * Configura ou reinicia o tempo de alimentação.
*/
void setupAlimentarTicker(){
	DEBUG_PROGRAM_PRINTLN("Configurando a alimentação automática baseada no tempo.");
	ultimaAlimentacao = millis();
	alimentacaoTicker.attach(intervaloAlimentacao, checkTimeParaAlimentar);
	DEBUG_PROGRAM_PRINTLN("Ticker alimentação ativado...");

}


void alimentarPeixes(){
	if ( ! servoController->isMoving() ){
		//ledTicker.attach(0.5, toggleLed);
		led.Reset();
		alexaServoDevice->setPercent(100);
		ehParaNotificarAlexaNoTermino = true;
		servoController->resetMoveCount();
		ultimaAlimentacao = millis();
	}
	Serial.println("O Servo foi acionado...");
}

void alexaAlimentarAquario(EspalexaDevice* d){
	if (d == nullptr) return; //this is good practice, but not required
	Serial.print("Alexa informou o status");
	Serial.print(d->getValue());
	Serial.print(" | ");
	Serial.println(d->getPercent());
	if ( d->getPercent() == 100 ){
		Serial.println("Alexa chamou alimentar peixes");
		alimentarPeixes();
	} 
}

void modoAutonomoChanged(EspalexaDevice* d){
	ehParaAlimentarPorIntervaloTempo = d->getPercent() == 100;
}

ulong getProxTimeProxAlimentacao(){
	return (intervaloAlimentacao - (millis() - ultimaAlimentacao)/1000);
}

void setupHttpServer(){
	// Configuração das rotas da página web
	
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

		String html = "<html><body>";
		html += "<h1>Alimentador de peixes</h1><hr />";
		html += "<h3>Temperatura:" + String(temperatureC) + "</h3><hr />";
		html += "<h3>Pr&oacute;xima execu&ccedil;&atilde;o:";
		if ( ehParaAlimentarPorIntervaloTempo ){
			uint tempo = getProxTimeProxAlimentacao(); 
			html += Relogio::formatarDataHora(tempo);
			html += "<button onclick=\"daf()\">Desativar Alimentação Automática</button><br><br>";
			
		} else {
			html += "<h3>Pr&oacute;xima execu&ccedil;&atilde;o: &lsqb;OFF&rsqb;";
			html += "<button onclick=\"laf()\">Ligar Alimentação Automática</button><br><br>";

		}
		html += "</h3><hr />";

		html += "<button onclick=\"toggleRango()\">Alimentar peixes</button><br><br>";
		if ( telaLigada ){
			html += "<button onclick=\"toggleTela()\">Desativar tela</button><br><br>";
		} else {
			html += "<button onclick=\"toggleTela()\">Ligar tela</button><br><br>";
		}
		html += "<hr />";
		html += "<h5>Ligado desde: ";
		html += ligadoDesde;
		html += "</h5>";
		html += "<h5>Hora Atual: " + relogio.getDataHora() + "</h5>";

		// html += "<button onclick=\"toggleRelay(1)\">Acionar Relé 1</button><br><br>";
		// html += "<button onclick=\"toggleRelay(2)\">Acionar Relé 2</button><br><br>";
		html += "<script>";
		html += " function toggleRango(){fetch('/a', {method: 'POST'});}";
		html += " function toggleTela(){fetch('/t', {method: 'POST'});}";
		html += " function daf(){fetch('/daf', {method: 'POST'});}";
		html += " function lad(){fetch('/laf', {method: 'POST'});}";

		// html += " function toggleRelay(relay){fetch('/relay/' + relay, {method: 'POST'});}";
				
		html += "</script>";
		
		html += "</body></html>";

		request->send(200, "text/html", html);
	});
	
	// Rota para acionar o servo
	server.on("/a", HTTP_POST, [](AsyncWebServerRequest *request) {
		alimentarPeixes();
		request->send(200);
	});
	server.on("/t", HTTP_POST, [](AsyncWebServerRequest *request) {
		 telaLigada = !telaLigada;
		request->send(200);
	});
	
	server.on("/daf", HTTP_POST, [](AsyncWebServerRequest *request) {
		ehParaAlimentarPorIntervaloTempo = false;
		alexaAquaModoAuto->setValue(0);
		request->send(200);
	});
	server.on("/laf", HTTP_POST, [](AsyncWebServerRequest *request) {
		ehParaAlimentarPorIntervaloTempo = true;
		alexaAquaModoAuto->setValue(100);
		request->send(200);
	});
	

	// // Rota para acionar os relés
	// server.on("/relay/<int>", HTTP_POST, [](AsyncWebServerRequest *request) {
	// 	int relay = request->pathArg(0).toInt();
	// 	Serial.println("O relé "+ String(relay) + " foi aceionado"); 
	// 	// Lógica para acionar o relé conforme a variável relay

	// 	request->send(200);
	// });

	server.onNotFound([](AsyncWebServerRequest *request){
		if (!espalexa.handleAlexaApiCall(request)) //if you don't know the URI, ask espalexa whether it is an Alexa control request
		{
			//whatever you want to do with 404s
			request->send(404, "text/plain", "Not found");
		}
	});
}

bool estahChecandoConexoes = false;

#include <OneWire.h>
#include <DallasTemperature.h>


OneWire oneWire(pinDS18B20);
DallasTemperature sensors(&oneWire);
Ticker temperaturaTicker;
bool ehParaLerTemperatura = false;
bool ehParaDeixarLedLigado = false;
ushort tempoLeituraTemperatura = 30; //tempo em segundos
void marcarParaLerTemperatura(){
	ehParaLerTemperatura = true;
}

void printAddress(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i <  8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("");
}

void checkSensorTemperatura(){
	// locate devices on the bus
	Serial.println("Locating devices...");
	Serial.print("Found ");
	int deviceCount = sensors.getDeviceCount();
	Serial.print(deviceCount, DEC);
	Serial.println(" devices.");
	Serial.println("");
	DeviceAddress Thermometer;
	Serial.println("Printing addresses...");
	for (int i = 0;  i  < deviceCount;  i++)
	{
		Serial.print("Sensor ");
		Serial.print(i+1);
		Serial.print(" : ");
		sensors.getAddress(Thermometer, i);
		printAddress(Thermometer);
	}
}

void setupSensorTemperatura(){
	sensors.begin(); // Inicializa o sensor DS18B20
	
	DEBUG_PROGRAM_PRINTLN("Carregando sensores de temperatura...");
	checkSensorTemperatura();
	DEBUG_PROGRAM_PRINTLN("Fim da carga dos sensores de temperatura.");
	temperaturaTicker.attach(tempoLeituraTemperatura, marcarParaLerTemperatura);
	delay(1000);
	ehParaLerTemperatura = true;
}
void setupAlexa(){
	espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
	
	espalexa.setDiscoverable(true);

	espalexa.addDevice("Alimentador aquário [DEV]", alexaAlimentarAquario, EspalexaDeviceType::onoff); //simplest definition, default state off
	alexaServoDevice = espalexa.getDevice(0); 
	alexaServoDevice->setValue(0);

	espalexa.addDevice("Alimentador Aquario Modo Autonomo", modoAutonomoChanged, EspalexaDeviceType::onoff); //simplest definition, default state off
	alexaAquaModoAuto = espalexa.getDevice(1);
	uint alexaValue = alexaAquaModoAuto->getPercent();
	Serial.println("Leu o valor da alexa para o modo automático: ");
	Serial.println(alexaValue);
;	ehParaAlimentarPorIntervaloTempo = alexaValue != 100;
	// espalexa.addDevice("Lâmpada UV", lampadaUVChanged); //simplest definition, default state off
	// espalexa.addDevice("Resfriar aquario", firstLightChanged); //simplest definition, default state off
	// espalexa.addDevice("Bomba de ar", bombaArChanged); //simplest definition, default state off
}
bool setupWifi(){
	//WiFiManager, Local intialization. Once its business is done, there is no need to keep it around

	uint16 totalPassos = 3;
	DEBUG_PROGRAM_PRINTLN("Carregando WI-FI (1/" + String(totalPassos) + ")");

    AsyncWiFiManager wm(&server,&dns);

	char buffer[10];

	DEBUG_PROGRAM_PRINTLN("Carregando WI-FI (2/" + String(totalPassos) + ")");

	AsyncWiFiManagerParameter custom_servo_min_param("SMin", "Alimentador Fechado", itoa(posGavetaFechada, buffer,10), 3);
	wm.addParameter(&custom_servo_min_param);

	AsyncWiFiManagerParameter custom_servo_max_param("SMax", "Alimentador Aberto", itoa(posGavetaAberta, buffer,10), 3);
	wm.addParameter(&custom_servo_max_param);

	AsyncWiFiManagerParameter custom_qtd_vezes_alimentar("SVezes", "Quantas Vezes", itoa(qtdVezesParaAlimentar, buffer,10), 3);
	wm.addParameter(&custom_qtd_vezes_alimentar);


    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

	DEBUG_PROGRAM_PRINTLN("Carregando WI-FI (3/" + String(totalPassos) + " - Tentando conectar em [" + wm.getConfigPortalSSID() + "])");

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","12345678"); // password protected ap
	
    if(!res) {
        DEBUG_PROGRAM_PRINTLN("Failed to connect WI-FI " + wm.getConfiguredSTASSID());
        // ESP.restart();
    } 
    else {


        //if you get here you have connected to the WiFi    
        DEBUG_PROGRAM_PRINTLN("connected...yeey :)");
		DEBUG_PROGRAM_PRINTLN("IP Address: " + WiFi.localIP().toString());
		server.begin();
		posGavetaFechada = atoi(custom_servo_min_param.getValue());
		posGavetaAberta = atoi(custom_servo_max_param.getValue());
		qtdVezesParaAlimentar = atoi(custom_qtd_vezes_alimentar.getValue());
		
	}
	return res;
	
}
void setupServo(){
	servoController = new ServoController(pinServo, qtdVezesParaAlimentar, posGavetaAberta, posGavetaFechada); // Instância do ServoController

}
void setupEspDash(){
	// feedButton.attachCallback([&](int value){
	// 		Serial.println("[feedButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
	// 		servoController->printStatus();
	// 		if ( value == 1 ){
	// 			Serial.println("Botao clicado");
	// 			alimentarPeixes();
	// 		}
	// 		checkServoButton.update(0);
	// 		feedButton.update(value);
	// 		abrirButton.update(0);
	// 		fecharButton.update(0);
	// 		dashboard.sendUpdates();

	// 	});

	// 	abrirButton.attachCallback([&](int value){
	// 		servoController->printStatus();
			
	// 		led.Stop();
	// 		Serial.println("[abrirButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
	// 		if ( value == 1 ){
	// 			Serial.println("Botao clicado");
	// 			servoController->open();
	// 			digitalWrite(LedAlimentarPIN, 1);
	// 		}
	// 		checkServoButton.update(0);
	// 		feedButton.update(0);
	// 		abrirButton.update(value);
	// 		fecharButton.update(0);
	// 		dashboard.sendUpdates();


	// 	});


	// 	fecharButton.attachCallback([&](int value){
	// 		servoController->printStatus();
		

	// 		led.Stop();
	// 		Serial.println("[fecharButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
	// 		if ( value == 1 ){
	// 			Serial.println("Botao clicado");
	// 			servoController->close();
	// 			digitalWrite(LedAlimentarPIN, 1);
				
	// 		}
	// 		checkServoButton.update(0);
	// 		feedButton.update(0);
	// 		abrirButton.update(0);
	// 		fecharButton.update(value);
	// 		dashboard.sendUpdates();

	// 	});

		// checkServoButton.attachCallback([&](int value){
		// 	servoController->printStatus();
		// 	estahChecandoConexoes = false;

		// 	Serial.println("[checkServoButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
		// 	if ( value == 1 ){
		// 		led.Reset();
		// 		Serial.println("Botao clicado");
		// 		servoController->checkConnections();
		// 		estahChecandoConexoes = true;
		// 	}
		// 	checkServoButton.update(value);
		// 	feedButton.update(0);
		// 	abrirButton.update(0);
		// 	fecharButton.update(0);
		// 	dashboard.sendUpdates();

		// });

		// piscarLedButton.attachCallback([&](int value){
		// 	int status = digitalRead(ledPinVerde);
		// 	Serial.println("[piscarLedButton] Button Callback Triggered: "+String((value == 1)?"true":"false") + " E estava = "+ String (status));
		// 	led.Stop();
		// 	if (value) {
		// 		led.Reset();
		// 	}
		// 	ehParaDeixarLedLigado = value;
		// 	piscarLedButton.update(value);
		// 	dashboard.sendUpdates();

		// });

}

void setupEasyButton(){

	DEBUG_PROGRAM_PRINTLN("Identificando botões...");
	buttonAlimentarPeixes = new EasyButton(pinButtonAlimentar);
	buttonAlimentarPeixes->begin();

	// Attach callback.
	buttonAlimentarPeixes->onSequence(pressesButton, timeoutButton, onSequenceFactoryButtonMatched);
	buttonAlimentarPeixes->onPressedFor(2000, onPressedForDuration);
	DEBUG_PROGRAM_PRINTLN("Botões carregados...");
	delay(1000);

}
void setupLeds(){
	DEBUG_PROGRAM_PRINTLN("Inicializando leds...");
	pinMode(LedAlimentarPIN, OUTPUT);
	pinMode(ledPinVerde, OUTPUT);


	Serial.println("LED = HIGH");
	digitalWrite(ledPinVerde, HIGH);
	digitalWrite(LedAlimentarPIN, HIGH);
	delay(1000);

	Serial.println("LED = LOW");
	digitalWrite(ledPinVerde, LOW);
	digitalWrite(LedAlimentarPIN, LOW);
	DEBUG_PROGRAM_PRINTLN("Leds carregados...");
	delay(1000);
	
}

void obterTemperatura(){
	if(ehParaLerTemperatura){
		ehParaLerTemperatura = false;
		sensors.requestTemperatures(); // Solicita a leitura da temperatura

		// Lê e imprime a temperatura em graus Celsius
		Serial.print("Temperatura: ");
		Serial.print(temperatureC);
		Serial.println(" °C");
		temperatureC = sensors.getTempCByIndex(0);
		//temperaturaCard.update(temperatureC);
	}
}
#include <Tela.h>
Tela tela;
void setupDisplay(){
	tela.setup();
	
}

#include <Relogio.h>

Ticker relogioTicker;

void marcarParaExibirRelogioNaTela(){
	ehParaExibirRelogio = true;
}
String ultimaDataHora;


void atualizarTela0(){

	if ( ehParaExibirRelogio ){
		ehParaExibirRelogio = false;
		ultimaDataHora = relogio.formatarDataHora();
	}
	tela.clearDisplay();
	tela.println(ultimaDataHora);
	tela.linha();
	tela.print("Temp: ");
	tela.println(String(temperatureC));
	tela.linha();
	tela.print("IP: ");
	tela.display->println(WiFi.localIP());
	tela.mostrar();
}


uint infoTime = 0;
uint pagina = 0;
byte totPagina = 3;


void atualizarTela(){
	
	tela.setLigada(telaLigada);
	// if ( telaLigada ){
	// 	if ( horaAtual.Hour > 1 && horaAtual.Hour < 5 ){
	// 		tela.clearDisplay();
	// 		telaLigada = false;
	// 		return;
	// 	} else {
	// 		telaLigada = true;
	// 	}
	// }
	if ( ! ehParaExibirRelogio ){
		return;
	}
	ehParaExibirRelogio = false;
	tela.clearDisplay();

	tmElements_t horaAtual = relogio.getTime();

		// if ( horaAtual.hour > 2 && horaAtual.hour < 5 ){
		// 	tela.clearDisplay();
		// 	telaLigada = false;
		// }
	ultimaDataHora = relogio.getDataHora();
	infoTime++;
	if ( infoTime > 3){
		infoTime = 0;
		pagina++;
		if ( pagina > totPagina ) {
			pagina = 0;
		}
	}
		
	
	tela.println(ultimaDataHora);
	tela.linha();
	switch (pagina)
	{
		default:
		case 0: {
			uint tempo = ehParaAlimentarPorIntervaloTempo ? getProxTimeProxAlimentacao(): intervaloAlimentacao;
			tela.println("PROX: " + Relogio::formatarDataHora(tempo));
			break;
		}	
		case 1: {
			tela.print("Modo: ");
			tela.println(ehParaAlimentarPorIntervaloTempo ? " [Auto]": " [OFF]");
			break;
		}	
		
	}
	
	tela.linha();
	tela.print("IP: ");
	tela.display->println(WiFi.localIP());
	tela.linha();
	if ( servoController->isMoving() ){
		tela.println("... Alimentando ...");
	}
	tela.mostrar();
}
ushort tempoAtualizarRelogio = 1;

void setupRelogio(){
	relogio.setup();
	relogio.updateTime();
	ehParaExibirRelogio = true;
	if (tela.isOnline()){
		relogioTicker.attach(min(tempoLeituraTemperatura, tempoAtualizarRelogio), marcarParaExibirRelogioNaTela);
	}
	ligadoDesde = relogio.getDataHora();

}


void DEBUG_PROGRAM_PRINTLN(String x) { 
	
	Serial.println(x);
	if ( tela.isOnline() ){
		tela.show(x);
	}
}


void setup() {

	Serial.begin(115200);
	setupDisplay();
	setupLeds();
	setupSensorTemperatura();
	setupEasyButton();

    if ( setupWifi() ){	
		DEBUG_PROGRAM_PRINTLN("Load HTTP SERVER...");
		setupHttpServer();
		
		DEBUG_PROGRAM_PRINTLN("Load Relogio...");
		setupRelogio();
		
		DEBUG_PROGRAM_PRINTLN("Load Servo...");
		setupServo();

		DEBUG_PROGRAM_PRINTLN("Setup triggers...");
		setupAlimentarTicker();

		DEBUG_PROGRAM_PRINTLN("Load Alexa...");
		setupAlexa();
		//DEBUG_PROGRAM_PRINTLN("Load EspDash...");
		//setupEspDash();


		DEBUG_PROGRAM_PRINTLN(":::::::: ESP Pronto :::::::: "); 
	} else {
		DEBUG_PROGRAM_PRINTLN(":::::::: ESP SEM RECURSO WIFI :::::::: "); 
	}
	
 	 	
}


void meuLoop(){
	// Piscar o LED sempre que o servo estiver se movendo
	if ( !servoController->isMoving()) {
		led.Stop();
		if ( ehParaNotificarAlexaNoTermino ){
			Serial.println("Alexa set off");
			ehParaNotificarAlexaNoTermino = false;
			alexaServoDevice->setValue(0);
		}
		estahChecandoConexoes = false;
		if ( ! ehParaDeixarLedLigado){
			digitalWrite(LedAlimentarPIN, LOW);
		}
	}
	obterTemperatura();
	atualizarTela();

}
void loop() {
	meuLoop();
	//dashboard.sendUpdates();
 	servoController->loop(); // Atualiza o controle do servo
	espalexa.loop();
	buttonAlimentarPeixes->read();
	led.Update();
}
