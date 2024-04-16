#pragma once
#ifndef TELA_MANAGER_H
#define TELA_MANAGER_H

#include <Arduino.h>

#include <vector>
// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images

#include "images.h"

// Use the corresponding display class:

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);
long drawConnectingToWifi(OLEDDisplay *display, long count = 0){
	if (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		display->clear();
		display->drawString(64, 10, "Connecting to WiFi");
		display->drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
		display->drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
		display->drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
		display->display();
		return ++counter;
	}
	return -1;
}
void frame1Callback(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)  {
        	display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void frame2Callback(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)  {
        // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
		// Besides the default fonts there will be a program to convert TrueType fonts into this format
		display->setTextAlignment(TEXT_ALIGN_LEFT);
		display->setFont(ArialMT_Plain_10);
		display->drawString(0 + x, 10 + y, "Teste 10");

		display->setFont(ArialMT_Plain_16);
		display->drawString(0 + x, 20 + y, "Teste 16");

		display->setFont(ArialMT_Plain_24);
		display->drawString(0 + x, 34 + y, "Teste 24");
}


// Defina outras classes de frame da mesma forma

void overlay1Callback(OLEDDisplay *display, OLEDDisplayUiState *state)  {
        display->setTextAlignment(TEXT_ALIGN_RIGHT);
		display->setFont(ArialMT_Plain_10);
		display->drawString(128, 0, String(millis()));
}



class TelaManager :  public SSD1306Wire {

private:

	// SSD1306Wire display(0x3c, SDA, SCL);  // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
	OLEDDisplayUi* ui;

	std::vector<FrameCallback> frameCallbacks; // Vetor para armazenar callbacks de frames
    std::vector<OverlayCallback> overlayCallbacks; // Vetor para armazenar callbacks de overlays

	
	// how many frames are there?
	int frameCount = 2;

	// Overlays are statically drawn on top of a frame eg. a clock
	OverlayCallback overlays2[1];
	int overlaysCount = 1;

public:
	~TelaManager(){
	}

	void addFrameCallback(FrameCallback callback) {
        frameCallbacks.push_back(callback); // Adiciona um novo callback de frame ao vetor
    }

    void addOverlayCallback(OverlayCallback callback) {
        overlayCallbacks.push_back(callback); // Adiciona um novo callback de overlay ao vetor
    }

	TelaManager(): 
		SSD1306Wire(0x3c, SDA, SCL)
	{
		Serial.println("Construtor - ok");
		
		//  // Initialize frames array with function pointers
        // frames[0] = drawFrame1;
        // frames[1] = drawFrame2;
        // frames[2] = drawFrame3;
        // frames[3] = drawFrame4;
        // frames[4] = drawFrame5;

        // // Initialize overlays array with pointers
        // overlays[0] = msOverlay;
	}
	

	// Initialize the OLED display using Wire library
	// SH1106Wire display(0x3c, SDA, SCL);


	void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
		display->setTextAlignment(TEXT_ALIGN_RIGHT);
		display->setFont(ArialMT_Plain_10);
		display->drawString(128, 0, String(millis()));
	}

	void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
	// draw an xbm image.
	// Please note that everything that should be transitioned
	// needs to be drawn relative to x and y

	display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
	}

	void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
		// Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
		// Besides the default fonts there will be a program to convert TrueType fonts into this format
		display->setTextAlignment(TEXT_ALIGN_LEFT);
		display->setFont(ArialMT_Plain_10);
		display->drawString(0 + x, 10 + y, "Arial 10");

		display->setFont(ArialMT_Plain_16);
		display->drawString(0 + x, 20 + y, "Arial 16");

		display->setFont(ArialMT_Plain_24);
		display->drawString(0 + x, 34 + y, "Arial 24");
	}

	void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
	// Text alignment demo
	display->setFont(ArialMT_Plain_10);

	// The coordinates define the left starting point of the text
	display->setTextAlignment(TEXT_ALIGN_LEFT);
	display->drawString(0 + x, 11 + y, "Left aligned (0,10)");

	// The coordinates define the center of the text
	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->drawString(64 + x, 22 + y, "Center aligned (64,22)");

	// The coordinates define the right end of the text
	display->setTextAlignment(TEXT_ALIGN_RIGHT);
	display->drawString(128 + x, 33 + y, "Right aligned (128,33)");
	}

	void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
	// Demo for drawStringMaxWidth:
	// with the third parameter you can define the width after which words will be wrapped.
	// Currently only spaces and "-" are allowed for wrapping
	display->setTextAlignment(TEXT_ALIGN_LEFT);
	display->setFont(ArialMT_Plain_10);
	display->drawStringMaxWidth(0 + x, 10 + y, 128, "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore.");
	}

	void drawFrame5(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	}
	int16_t update(){
		if ( this->ui ){
			return this->ui->update();
		} 
		return millis();
	}
	void setup(){
				
				Serial.println("Iniciando setup");
		ui = new OLEDDisplayUi( this );

				Serial.println("Ui construído");
		// The ESP is capable of rendering 60fps in 80Mhz mode
		// but that won't give you much time for anything else
		// run it in 160Mhz mode or just set it to 30 fps
		ui->setTargetFPS(60);
				Serial.println("FPS - OK");

		// Customize the active and inactive symbol
		ui->setActiveSymbol(activeSymbol);
		ui->setInactiveSymbol(inactiveSymbol);

		// You can change this to
		// TOP, LEFT, BOTTOM, RIGHT
		ui->setIndicatorPosition(BOTTOM);

		// Defines where the first frame is located in the bar.
		ui->setIndicatorDirection(LEFT_RIGHT);

		// You can change the transition that is used
		// SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
		ui->setFrameAnimation(SLIDE_LEFT);

		// // Add frames
		// ui->setFrames(this->frames, frameCount);

		// // Add overlays
		// ui->setOverlays(this->overlays, overlaysCount);
				Serial.println("Iniciando frames e overlays");

		ui->setFrames(frameCallbacks.data(), frameCallbacks.size());
        ui->setOverlays(overlayCallbacks.data(), overlayCallbacks.size());
				Serial.println("frames e overlays - ok");

		// Initialising the UI will init the display too.
				Serial.println("ui.Init");
		ui->init();
				Serial.println("ui.Init - ok ");
				Serial.println("SETUP - ok");

	}
};



#endif // TELA_MANAGER_H
