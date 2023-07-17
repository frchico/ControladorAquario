#ifndef MEU__DISPLAY_CONTROL_H
#define MEU__DISPLAY_CONTROL_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class Tela {
  private:
//  SCL GPIO 5 (D1)
// SDA  GPIO 4 (D2)
    const byte SCREEN_WIDTH = 128; // OLED display width, in pixels
    const byte SCREEN_HEIGHT = 64; // OLED display height, in pixels
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    const int OLED_RESET = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
//    ADDRESS = 0x3c;
	bool online =false;
  public: 
    Adafruit_SSD1306* display;
    bool setup(){
      display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    //  i2c.setup(0, SDA, SCL, i2c.SLOW)
      // if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
      //  u8g.setColorIndex(255);     // white
      // }
      // else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
      //  u8g.setColorIndex(3);         // max intensity
      // }
      // else if ( u8g.getMode() == U8G_MODE_BW ) {
      //  u8g.setColorIndex(1);         // pixel on
      // }
      // else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
      //  u8g.setHiColorByRGB(255,255,255);
      // }
      
      // initialize with the I2C addr 0x3C
      online = display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  

      if ( online )
      {
		for( int i= 0; i< 4; i++){
			display->setRotation(i);
			show("Carregando Projeto Nemo");
			delay(500);
		}
		display->setRotation(2);
		show("Projeto Nemo - Tela OK");
		delay(1000);
      } else {
         Serial.println(F("SSD1306 allocation failed"));
      }
      return online;
    }
	bool isOnline(){
		return online;
	}
	void clearDisplay(){
		display->clearDisplay();
	}
	void mostrar(){
		 display->setTextSize(1);
        display->setTextColor(WHITE);
        display->setCursor(0,3);
		display->display();
	}
	void linha(){
		display->drawFastHLine(0, display->getCursorY(), display->width(), WHITE);
		display->println();
		
	}
	void print(String msg){
		display->print(msg);
	}
	void println(String msg){
		display->println(msg);
	}
	void show(String msg){
		// Clear the buffer.
        display->clearDisplay();
		//display->setRotation(180);
        display->setTextSize(1);
        display->setTextColor(WHITE);
        display->setCursor(0,3);
        display->println(msg);
        display->display();
	}
};

#endif