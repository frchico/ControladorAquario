#include <ESP8266WiFi.h>
#include <Servo.h>


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

#include <ESPDash.h>
ESPDash dashboard(&server);

#define ESPALEXA_ASYNC //it is important to define this before #include <Espalexa.h>!
#include <Espalexa.h>
Espalexa espalexa;


int posGavetaAberta = 15;
int posGavetaFechada = 43;
int qtdVezesParaAlimentar = 2;


#include <ServoController.h>
//ServoController servoController(D5, qtdVezesParaAlimentar, posGavetaAberta, posGavetaFechada); // Instância do ServoController
ServoController *servoController; 



// #include <Ticker.h>
//Ticker ledTicker;
const int pinDS18B20 =D7; // Pino onde o DS18B20 está conectado

const int LedAlimentarPIN = LED_BUILTIN; // Ligado no pino D4 do wemos d1 r1 //5;// LED_BUILTIN; // Pino do LED do ESP8266
const int ledPinVerde = 4;
bool ledState = false;
bool ehParaNotificarAlexaNoTermino = false;


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


void alimentarPeixes(){
	if ( ! servoController->isMoving() ){
		//ledTicker.attach(0.5, toggleLed);
		led.Reset();
		alexaServoDevice->setValue(100);
		ehParaNotificarAlexaNoTermino = true;
		servoController->resetMoveCount();
	}
	Serial.println("O Servo foi acionado...");
}
void alexaAlimentarAquario(EspalexaDevice* d){
	if (d == nullptr) return; //this is good practice, but not required
	Serial.println("Alexa informou o status");
	if ( d->getValue() == 100 ){
		Serial.println("Alexa estava desligada e agora está ligada");
		alimentarPeixes();
	} 
}
void setuHttpServer(){
	// Configuração das rotas da página web
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		String html = "<html><body>";
		html += "<h1>Controle do Servo e Relés</h1>";
		html += "<button onclick=\"toggleServo()\">Acionar Servo</button><br><br>";
		html += "<button onclick=\"toggleRelay(1)\">Acionar Relé 1</button><br><br>";
		html += "<button onclick=\"toggleRelay(2)\">Acionar Relé 2</button><br><br>";
		html += "<script>function toggleServo(){fetch('/servo', {method: 'POST'});}"
				"function toggleRelay(relay){fetch('/relay/' + relay, {method: 'POST'});}</script>";
		html += "</body></html>";

		request->send(200, "text/html", html);
	});

	// Rota para acionar o servo
	server.on("/servo", HTTP_POST, [](AsyncWebServerRequest *request) {
		alimentarPeixes();
		request->send(200);
	});

	// Rota para acionar os relés
	server.on("/relay/<int>", HTTP_POST, [](AsyncWebServerRequest *request) {
		int relay = request->pathArg(0).toInt();
		Serial.println("O relé "+ String(relay) + " foi aceionado"); 
		// Lógica para acionar o relé conforme a variável relay

		request->send(200);
	});
	server.onNotFound([](AsyncWebServerRequest *request){
		if (!espalexa.handleAlexaApiCall(request)) //if you don't know the URI, ask espalexa whether it is an Alexa control request
		{
			//whatever you want to do with 404s
			request->send(404, "text/plain", "Not found");
		}
	});
}
Card feedButton(&dashboard, BUTTON_CARD, "Alimentar");
Card abrirButton(&dashboard, BUTTON_CARD, "Abrir");
Card fecharButton(&dashboard, BUTTON_CARD, "Fechar");
Card checkServoButton(&dashboard, BUTTON_CARD, "Checar Servo");
Card piscarLedButton(&dashboard, BUTTON_CARD, "LED");
Card temperaturaCard(&dashboard, TEMPERATURE_CARD, "Temperatura", "°C");

bool estahChecandoConexoes = false;

#include <OneWire.h>
#include <DallasTemperature.h>


OneWire oneWire(pinDS18B20);
DallasTemperature sensors(&oneWire);
Ticker temperaturaTicker;
bool ehParaLerTemperatura = false;
bool ehParaDeixarLedLigado = false;
int tempoLeituraTemperatura = 5; //tempo em segundos
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
	
	Serial.println("Carregando sensores de temperatura...");
	checkSensorTemperatura();
	Serial.println("Fim da carga dos sensores de temperatura.");
	temperaturaTicker.attach(tempoLeituraTemperatura, marcarParaLerTemperatura);
	delay(1000);
}
void setupAlexa(){
	espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
	
	espalexa.setDiscoverable(true);

	espalexa.addDevice("Alimentador aquário", alexaAlimentarAquario, EspalexaDeviceType::onoff); //simplest definition, default state off
	alexaServoDevice = espalexa.getDevice(0); 
	alexaServoDevice->setValue(0);
	// espalexa.addDevice("Lâmpada UV", lampadaUVChanged); //simplest definition, default state off
	// espalexa.addDevice("Resfriar aquario", firstLightChanged); //simplest definition, default state off
	// espalexa.addDevice("Bomba de ar", bombaArChanged); //simplest definition, default state off
}
bool setupWifi(){
	//WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wm(&server,&dns);

	char buffer[10];

	AsyncWiFiManagerParameter custom_servo_min_param("SMin", "Alimentador Fechado", itoa(posGavetaFechada, buffer,10), 3);
	wm.addParameter(&custom_servo_min_param);

	AsyncWiFiManagerParameter custom_servo_max_param("SMax", "Alimentador Aberto", itoa(posGavetaAberta, buffer,10), 3);
	wm.addParameter(&custom_servo_max_param);

	AsyncWiFiManagerParameter custom_qtd_vezes_alimentar("SV", "Quantas Vezes", itoa(qtdVezesParaAlimentar, buffer,10), 3);
	wm.addParameter(&custom_qtd_vezes_alimentar);


    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","12345678"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {

        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
		Serial.print("IP Address: ");
  		Serial.println(WiFi.localIP());
		server.begin();
		posGavetaFechada = atoi(custom_servo_min_param.getValue());
		posGavetaAberta = atoi(custom_servo_max_param.getValue());
		qtdVezesParaAlimentar = atoi(custom_qtd_vezes_alimentar.getValue());
		
	}
	return res;
	
}
void setupServo(){
	servoController = new ServoController(D5, qtdVezesParaAlimentar, posGavetaAberta, posGavetaFechada); // Instância do ServoController

}
void setupEspDash(){
	feedButton.attachCallback([&](int value){
			Serial.println("[feedButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			servoController->printStatus();
			if ( value == 1 ){
				Serial.println("Botao clicado");
				alimentarPeixes();
			}
			checkServoButton.update(0);
			feedButton.update(value);
			abrirButton.update(0);
			fecharButton.update(0);
			dashboard.sendUpdates();

		});

		abrirButton.attachCallback([&](int value){
			servoController->printStatus();
			
			led.Stop();
			Serial.println("[abrirButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			if ( value == 1 ){
				Serial.println("Botao clicado");
				servoController->open();
				digitalWrite(LedAlimentarPIN, 1);
			}
			checkServoButton.update(0);
			feedButton.update(0);
			abrirButton.update(value);
			fecharButton.update(0);
			dashboard.sendUpdates();


		});


		fecharButton.attachCallback([&](int value){
			servoController->printStatus();
		

			led.Stop();
			Serial.println("[fecharButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			if ( value == 1 ){
				Serial.println("Botao clicado");
				servoController->close();
				digitalWrite(LedAlimentarPIN, 1);
				
			}
			checkServoButton.update(0);
			feedButton.update(0);
			abrirButton.update(0);
			fecharButton.update(value);
			dashboard.sendUpdates();

		});

		checkServoButton.attachCallback([&](int value){
			servoController->printStatus();
			estahChecandoConexoes = false;

			Serial.println("[checkServoButton] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			if ( value == 1 ){
				led.Reset();
				Serial.println("Botao clicado");
				servoController->checkConnections();
				estahChecandoConexoes = true;
			}
			checkServoButton.update(value);
			feedButton.update(0);
			abrirButton.update(0);
			fecharButton.update(0);
			dashboard.sendUpdates();

		});

		piscarLedButton.attachCallback([&](int value){
			int status = digitalRead(ledPinVerde);
			Serial.println("[piscarLedButton] Button Callback Triggered: "+String((value == 1)?"true":"false") + " E estava = "+ String (status));
			led.Stop();
			if (value) {
				led.Reset();
			}
			ehParaDeixarLedLigado = value;
			piscarLedButton.update(value);
			dashboard.sendUpdates();

		});

}

void setupEasyButton(){

	buttonAlimentarPeixes = new EasyButton(pinButtonAlimentar);
	buttonAlimentarPeixes->begin();

	// Attach callback.
	buttonAlimentarPeixes->onSequence(pressesButton, timeoutButton, onSequenceFactoryButtonMatched);
	buttonAlimentarPeixes->onPressedFor(2000, onPressedForDuration);
}
void setupLeds(){
	pinMode(LedAlimentarPIN, OUTPUT);
	pinMode(ledPinVerde, OUTPUT);


	Serial.println("LED = HIGH");
	digitalWrite(ledPinVerde, HIGH);
	digitalWrite(LedAlimentarPIN, HIGH);
	delay(1000);

	Serial.println("LED = LOW");
	digitalWrite(ledPinVerde, LOW);
	digitalWrite(LedAlimentarPIN, LOW);
	delay(1000);
}
void setup() {

	Serial.begin(115200);
    
	setupLeds();
	setupSensorTemperatura();
	setupEasyButton();

    if ( setupWifi() ){	
		setuHttpServer();
		setupAlexa();
		setupEspDash();
		Serial.println(":::::::: ESP Pronto :::::::: "); 
	} else {
		Serial.println(":::::::: ESP SEM RECURSO WIFI :::::::: "); 
	}
	
 	 	
}
void obterTemperatura(){
	if(ehParaLerTemperatura){
		ehParaLerTemperatura = false;
		sensors.requestTemperatures(); // Solicita a leitura da temperatura

		// Lê e imprime a temperatura em graus Celsius
		float temperatureC = sensors.getTempCByIndex(0);
		Serial.print("Temperatura: ");
		Serial.print(temperatureC);
		Serial.println(" °C");
		temperaturaCard.update(temperatureC);
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

}
void loop() {
	meuLoop();
	dashboard.sendUpdates();
 	servoController->loop(); // Atualiza o controle do servo
	espalexa.loop();
	buttonAlimentarPeixes->read();
	led.Update();
}
