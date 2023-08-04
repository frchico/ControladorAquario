#pragma once

#ifndef TELA_H
#define TELA_H

#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>



#include <vector> // Adicione essa linha para utilizar std::vector
#include <functional> // Adicione essa linha para utilizar std::function

#include <Ticker.h>

#define I2C_DISPLAY_DEVICE  1

class Tela {
public:
    SSD1306Wire display;
    OLEDDisplayUi* ui;


    Tela();
	
	bool init();

    void exibirTexto(const String& texto);
    void atualizar();
    void exibirBarraProgresso(int progresso, const String& label);

	void drawProgress(OLEDDisplay *display, int percentage, String labeltop, String labelbot);
	void drawProgress(OLEDDisplay *display, int percentage, String labeltop );

	bool hasDisplay();

	void addFrameCallback(FrameCallback frame);
	void addOverlayCallback(OverlayCallback  overlay);

	void loop();

	void drawTelaConnectando();
	void mostrarConnectando(bool mostrar, const String& msg ="");

private:
	Ticker* relogioTicker;

	bool has_display = false;
	bool readyForUpdate = false;

	String mensagemConexao = "";
	bool ehParaExibirMensagemConexao = false;

	// Defina a duração de cada frame em milissegundos
	const unsigned int FRAME_DURATION = 5000; // 2 segundos
	// Defina um temporizador e uma variável para rastrear o tempo de cada frame
	unsigned long lastFrameTime = 0;
	const unsigned long tempoIndex0 =  3* 1* 1000;
	int currentFrameIndex = 0;
	unsigned long updateFrame();

	char i2c_dev[I2C_DISPLAY_DEVICE][32]; // Array on string displayed
	uint8_t i2c_scan(uint8_t address = 0xff);


	std::vector<FrameCallback> frameCallbacks;

	//static void staticFrameCallback(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

	// Vetor para armazenar os overlays
    std::vector<OverlayCallback> overlays;

    // Função estática para ser usada como Overlay
   // static void staticHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);

    // Funções para desenhar overlays
    static void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
	

};



#endif // TELA_H
