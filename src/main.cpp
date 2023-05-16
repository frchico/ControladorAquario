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



#include <Servo.h>



#define PIN_SERVO D4

#include <SimpleTimer.h>
SimpleTimer timer;

Servo servo;
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


//callback functions
void firstLightChanged(uint8_t brightness);
void setupAlimentadorPeixe(int SERVO_PIN);
void alimentarPeixes();

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
		
	

		setupAlimentadorPeixe(PIN_SERVO);
		
		feedButton.update(true);


		feedButton.attachCallback([&](int value){
			Serial.println("[Card1] Button Callback Triggered: "+String((value == 1)?"true":"false"));
			feedButton.update(value);
			dashboard.sendUpdates();
			if ( value == 1 ){
				estaAlimentando = true;
				alimentarPeixes();
				feedButton.update(false);
				dashboard.sendUpdates();
			}
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
	timer.run();

	/* Send Updates to our Dashboard (realtime) */
	dashboard.sendUpdates();
	espalexa.loop();
   
	/* 
	Delay is just for demonstration purposes in this example,
	Replace this code with 'millis interval' in your final project.
	*/
delay(100);
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

void alimentarPeixes(){

		int tempo = 2000;
		timer.setTimeout(tempo, []{
			if ( ! servo.attached() ){
				servo.attach(PIN_SERVO);
		}});

		Serial.println("alimentarPeixes");
		timer.setTimeout(tempo+=2, []{
			servo.write(0); 
			Serial.println("Pos  = 0");
		});

		// ITimer.setInterval(500, []{Serial.println(90);});
		// ITimer.setInterval(500, []{Serial.println(180);});
		// ITimer.setInterval(500, []{Serial.println(0);});
		timer.setTimeout(tempo*=2, []{
			servo.write(90); 
			Serial.println("Pos  = 90");
		});
		timer.setTimeout(tempo*=2, []{
			servo.write(180); 
			Serial.println("Pos  = 180");
		});
		timer.setTimeout(tempo*=2, []{
			servo.write(90); 
			Serial.println("Pos  = 90");
		});
		timer.setTimeout(tempo*=2, []{
			servo.write(0); 
			Serial.println("Pos  = 0");
		});
		timer.setTimeout(tempo*=2, []{
			if ( servo.attached() ){
			servo.detach();
		}});
		

	

	
}
void setupAlimentadorPeixe(int SERVO_PIN){
	//servo.attach(SERVO_PIN);
	//servo.write(0); // Inicia motor posição zero
}