#define ESPALEXA_ASYNC //it is important to define this before #include <Espalexa.h>!
#include <Espalexa.h>

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

Espalexa espalexa;

#include <ESPAsyncWiFiManager.h> 

//#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESPDash.h>



#include <ServoEasing.hpp>

#include <Servo.h>




ServoEasing servo;
#define START_DEGREE_VALUE  0
#define SERVO1_PIN  D4
#define DISABLE_COMPLEX_FUNCTIONS     // Activating this disables the SINE, CIRCULAR, BACK, ELASTIC, BOUNCE and PRECISION easings. Saves up to 1850 bytes program memory.
#define MAX_EASING_SERVOS 1

AsyncWebServer server(80);
DNSServer dns;
ESPDash dashboard(&server);



Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");
Card feedButton(&dashboard, BUTTON_CARD, "Alimentar");
Card bombaDeArButton(&dashboard, BUTTON_CARD, "Bomba de ar");
Card lampadaUVButton(&dashboard, BUTTON_CARD, "Lâmpada UV");
Card coolerButton(&dashboard, BUTTON_CARD, "Resfriamento");

bool estaAlimentando = false;


enum StatusAlimentacao {
	Parado = 0,
	Alimentando,
	Retornando,
	AlimentacaoConcluida
} statusAlimentacao;


int posGavetaAberta = 0;
int posGavetaFechada = 90;

//callback functions
void firstLightChanged(uint8_t brightness);
void setupAlimentadorPeixe(int SERVO_PIN);
void alimentarPeixes();

unsigned long startMillis0;
unsigned currentMillis;
const unsigned long period = 1000;

void Serialprintln(String s){
	currentMillis = millis();
	if (currentMillis - startMillis0 <= period) {
		Serial.println(s);
		startMillis0 = millis();

	}
}
void setup() {
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wm(&server,&dns);

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
		
	

		setupAlimentadorPeixe(SERVO1_PIN);
		
		feedButton.update(false);

		feedButton.attachCallback([&](int value){
			Serial.println("[Card1] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			if ( value == 1 && statusAlimentacao == StatusAlimentacao::Parado ){
				Serial.println("Botao clicado");
				statusAlimentacao = StatusAlimentacao::Alimentando;
			}
			feedButton.update(value);
			dashboard.sendUpdates();
		});


		server.onNotFound([](AsyncWebServerRequest *request){
		if (!espalexa.handleAlexaApiCall(request)) //if you don't know the URI, ask espalexa whether it is an Alexa control request
		{
			//whatever you want to do with 404s
			request->send(404, "text/plain", "Not found");
		}
		});
			// Define your devices here.
		espalexa.addDevice("Alimentador aquario", firstLightChanged); //simplest definition, default state off
		espalexa.addDevice("Lâmpada UV", firstLightChanged); //simplest definition, default state off
		espalexa.addDevice("Resfriar aquario", firstLightChanged); //simplest definition, default state off
		espalexa.addDevice("Bomba de ar", firstLightChanged); //simplest definition, default state off

		espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
		//server.begin(); //omit this since it will be done by espalexa.begin(&server)
    }

}



void loop() {
	temperature.update((int)random(0, 50));
	humidity.update((int)random(0, 100));
	
	//feedButton.update(statusAlimentacao != StatusAlimentacao::Parado);
	/* Send Updates to our Dashboard (realtime) */
	dashboard.sendUpdates();
	espalexa.loop();

	alimentarPeixes();
   
	/* 
	Delay is just for demonstration purposes in this example,
	Replace this code with 'millis interval' in your final project.
	*/
	//delay(100);
	// put your main code here, to run repeatedly:   
}


//our callback functions
void firstLightChanged(uint8_t brightness) {
    Serial.print("Device 1 changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness == 255) {
      Serial.println("ON");
    }
    else if (brightness == 0) {
      Serial.println("OFF");
    }
    else {
      Serial.print("DIM "); Serial.println(brightness);
    }
}


//StatusAlimentacao statusAlimentacao = StatusAlimentacao::Parado; // 0 - parado; 1 - alimentando; 2 - retornando alimentacao;
void alimentarPeixes(){
	switch (statusAlimentacao)
	{
		case StatusAlimentacao::Parado: {
			if(servo.attached()){
				servo.detach();
				Serial.println("Servo desconectado...");
			}
			feedButton.update(false);
			dashboard.sendUpdates(true);
			break;
		}
		case StatusAlimentacao::Alimentando: { //1 - alimentando
			if(!servo.attached()){
				if (servo.attach(SERVO1_PIN, posGavetaFechada) == INVALID_SERVO) {
					Serial.println("Erro ao conectar a servo...");
					statusAlimentacao = StatusAlimentacao::Parado;
					return;
				}
			}
			feedButton.update(true);
			dashboard.sendUpdates(true);
			Serial.println("Angulo 1 = " + servo.getCurrentAngle());

			servo.startEaseToD(posGavetaAberta, 1000);
			Serial.println("Servo Alimentando...");
			statusAlimentacao = StatusAlimentacao::Retornando;
		}
		case StatusAlimentacao::Retornando: {
			//Serialprintln("Servo Retornando...");
			if ( ! servo.isMoving() ) {
				Serial.print("Angulo 2 = " );
				Serial.println( servo.getCurrentAngle());
				Serial.println("Servo Retornando...");
				servo.startEaseToD(posGavetaFechada, 1000);
				statusAlimentacao = StatusAlimentacao::AlimentacaoConcluida;
			}
		}
		case StatusAlimentacao::AlimentacaoConcluida: {
			//Serialprintln("Servo AlimentacaoConcluida...");
			if ( ! servo.isMoving()) {
				Serial.println("Servo AlimentacaoConcluida...");
				Serial.print("Angulo 3 = " );
				Serial.println( servo.getCurrentAngle());
				
				statusAlimentacao = StatusAlimentacao::Parado;
			}
		}
	}
}
void setupAlimentadorPeixe(int SERVO_PIN){
	statusAlimentacao = StatusAlimentacao::Parado;
	//servo.attach(SERVO_PIN);
	//servo.write(0); // Inicia motor posição zero
}