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



int posGavetaAberta = 0;
int posGavetaFechada = 52;
int qtdVezesParaAlimentar = 10;

#include <ServoController.h>
ServoController servoController(D4, qtdVezesParaAlimentar, 1000, 1000, 500, D2, posGavetaAberta, posGavetaFechada); // Instância do ServoController

#include <Ticker.h>
Ticker ledTicker;
const int ledPin = LED_BUILTIN; // Pino do LED do ESP8266
bool ledState = false;
bool ehParaNotificarAlexaNoTermino = false;


void toggleLed() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}


EspalexaDevice* alexaServoDevice;


void alimentarPeixes(){
	if ( ! servoController.isMoving() ){
		ledTicker.attach(0.2, toggleLed);
		alexaServoDevice->setValue(100);
		ehParaNotificarAlexaNoTermino = true;
		servoController.resetMoveCount();
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
void configurarRotasDePaginas(){
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
	espalexa.addDevice("Alimentador aquario", alexaAlimentarAquario, EspalexaDeviceType::onoff); //simplest definition, default state off
	alexaServoDevice = espalexa.getDevice(0); 
	alexaServoDevice->setValue(0);
	// espalexa.addDevice("Lâmpada UV", lampadaUVChanged); //simplest definition, default state off
	// espalexa.addDevice("Resfriar aquario", firstLightChanged); //simplest definition, default state off
	// espalexa.addDevice("Bomba de ar", bombaArChanged); //simplest definition, default state off

	
}
Card feedButton(&dashboard, BUTTON_CARD, "Alimentar");


void setup() {

	Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wm(&server,&dns);

	char buffer[10];

	AsyncWiFiManagerParameter custom_servo_min_param("SMin", "Alimentador Fechado", itoa(posGavetaFechada, buffer,10), 3);
	wm.addParameter(&custom_servo_min_param);

	AsyncWiFiManagerParameter custom_servo_max_param("SMax", "Alimentador Aberto", itoa(posGavetaAberta, buffer,10), 3);
	wm.addParameter(&custom_servo_max_param);

	AsyncWiFiManagerParameter custom_qtd_vezes_alimentar("SQtd", "Quantas Vezes", itoa(qtdVezesParaAlimentar, buffer,10), 3);
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
    res = wm.autoConnect("AutoConnectAP","123456"); // password protected ap

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

		pinMode(ledPin, OUTPUT);

		configurarRotasDePaginas();

		espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
		//server.begin(); //omit this since it will be done by espalexa.begin(&server)
  		// Inicia o servidor
		espalexa.setDiscoverable(true);
		servoController.checkConnections();

		feedButton.attachCallback([&](int value){
			Serial.println("[Card1] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			if ( value == 1 ){
				Serial.println("Botao clicado");
				alimentarPeixes();
			}
			feedButton.update(value);
			dashboard.sendUpdates();
		});
	}
	Serial.println(":::::::: ESP Pronto :::::::: "); 
	
 	 	
}
void piscarLed(){
	// Piscar o LED sempre que o servo estiver se movendo
	if ( !servoController.isMoving()) {
		if ( ledTicker.active()) {
			ledTicker.detach();	
			digitalWrite(ledPin, LOW);
		}
		if ( ehParaNotificarAlexaNoTermino ){
			Serial.println("Alexa set off");
			ehParaNotificarAlexaNoTermino = false;
			alexaServoDevice->setValue(0);
		}
	}
}
void loop() {
	//dashboard.sendUpdates();
	piscarLed();
 	servoController.update(); // Atualiza o controle do servo
	espalexa.loop();
	//yield();
}
