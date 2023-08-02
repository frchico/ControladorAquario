#include "Tela.h"
#include "Images.h"


#define I2C_DISPLAY_ADDRESS 0x3c

Tela::Tela() : display(I2C_DISPLAY_ADDRESS, 4, 5) {
	has_display = display.init();
	display.clear();
  	display.display();
}

bool Tela::init(){
	// Configurações iniciais do display
    if ( has_display ){
		display.flipScreenVertically();
		display.setFont(ArialMT_Plain_10);
		display.setTextAlignment(TEXT_ALIGN_LEFT);

		// Inicializa a UI com o display
		ui = new OLEDDisplayUi(&display);
		ui->setActiveSymbol(activeSymbol);
		ui->setInactiveSymbol(inactiveSymbol);
		ui->setIndicatorPosition(BOTTOM);
		ui->setIndicatorDirection(LEFT_RIGHT);
		ui->setFrameAnimation(SLIDE_LEFT);
		ui->setTargetFPS(30);

		if ( frameCallbacks.size() > 0 ){
			Serial.println("Adicionando frameCallbacks");
			// Configura o vetor de overlays na UI
			ui->setFrames(frameCallbacks.data(), frameCallbacks.size());
		}
		// Adicione os overlays no vector
		Serial.println("Adicionando os overlays no vector");
		

		if ( overlays.size() <= 0){
			overlays.push_back(drawHeaderOverlay);
		} 
			// Configura o vetor de overlays na UI
		ui->setOverlays(overlays.data(), overlays.size());
		Serial.println("Configurado o vetor de overlays na UI");
	
		ui->init();

		
		relogioTicker = new Ticker([this](){this->drawTelaConnectando();}, 500, 0, MILLIS);
		relogioTicker->stop();
		return true;
	}
	return false;
}

void Tela::addFrameCallback(FrameCallback frame){
	if ( frame ){ 
		frameCallbacks.push_back(frame);
	}
}
void Tela::addOverlayCallback(OverlayCallback  overlay){
	if ( overlay ){
		overlays.push_back(overlay);
	}	
}

// void Tela::staticHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
//     // Chama o membro de função para desenhar o Overlay
//     Tela* telaObj = reinterpret_cast<Tela*>(state->userData);
//     if (telaObj) {
//         telaObj->drawHeaderOverlay(display, state);
//     }
// }



void Tela::exibirTexto(const String& texto) {
    display.clear();
  	display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(64, 128, texto);
    display.display();
}

void Tela::atualizar() {
    ui->update();
}

void Tela::exibirBarraProgresso(int progresso, const String& label) {

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, label);
    display.drawProgressBar(2, 28, 124, 10, progresso);
    display.display();
}




void Tela::drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->drawHorizontalLine(0, 52, 128);
  display->setColor(WHITE);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 54, "dd/MM/yyyy");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  String temp = "15 °C";
  display->drawString(128, 54, temp);
}



bool Tela::hasDisplay(){
	return has_display;
}


// Função para alternar para o próximo frame após o tempo definido
unsigned long Tela::updateFrame() {
  unsigned long tempo = FRAME_DURATION;

  OLEDDisplayUiState* uiState = ui->getUiState();
  if ( 0 == uiState->currentFrame ){
    tempo = tempoIndex0;
  }
  ui->setTimePerFrame(tempo);
  /*
  unsigned long remainingTime = millis() - uiState->lastUpdate;
  if (remainingTime >= tempo) {
    // Avança para o próximo frame
    currentFrameIndex = (currentFrameIndex + 1) % numberOfFrames;

    // Atualiza o tempo do último frame
    lastFrameTime = millis();
  }
   display.clear();
  // draw the current demo method
  demos[currentFrameIndex]();
  */
  return millis()-tempo;
}

/* ======================================================================
  Function: loop
  Purpose : you should know ;-)
  ====================================================================== */
void Tela::loop()
{
	relogioTicker->update();

	if (has_display) {

		if ( ehParaExibirMensagemConexao ){
			//drawTelaConnectando();
			return;
		}
		
		if (readyForUpdate && ui->getUiState()->frameState == FIXED) {
			//updateData(&display);
		}

		updateFrame();


		int remainingTimeBudget = ui->update();

		if (remainingTimeBudget > 0) {
			// You can do some work here
			// Don't do stuff if you are below your
			// time budget.
			delay(remainingTimeBudget);
		} else {
		//	imgAtual = ( imgAtual +1 )% img_allArray_LEN;
		}


		
	} else {
		if (readyForUpdate ) {
			//updateData(&display);
		}
	}
}

/* ======================================================================
  Function: i2cScan
  Purpose : scan I2C bus
  Input   : specifc address if looking for just 1 specific device
  Output  : number of I2C devices seens
  Comments: -
  ====================================================================== */
uint8_t Tela::i2c_scan(uint8_t address)
{
  uint8_t error;
  int nDevices;
  uint8_t start = 1 ;
  uint8_t end   = 0x7F ;
  uint8_t index = 0;
  char device[16];
  char buffer[32];

  if (address >= start && address <= end) {
    start = address;
    end   = address + 1;
    Serial.print(F("Searching for device at address 0x"));
    Serial.printf("%02X ", address);
  } else {
    Serial.println(F("Scanning I2C bus ..."));
  }

  nDevices = 0;
  for (address = start; address < end; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.printf("Device ");

      if (address == 0x40)
        strcpy(device, "TH02" );
      else if (address == 0x29 || address == 0x39 || address == 0x49)
        strcpy(device, "TSL2561" );

      else if (address==0x50) {
        strcpy(device, "24AA02E64" );
        // This device respond to 0x50 to 0x57 address
        address+=0x07;
      }
      else if (address == 0x55 )
        strcpy(device, "BQ72441" );
      else if (address == I2C_DISPLAY_ADDRESS || address == I2C_DISPLAY_ADDRESS + 1)
        strcpy(device, "OLED SSD1306" );
      else if (address >= 0x60 && address <= 0x62 ) {
        strcpy(device, "MCP4725_Ax" );
        device[9]= '0' + (address & 0x03);
      } else if (address >= 0x68 && address <= 0x6A ) {
        strcpy(device, "MCP3421_Ax" );
        device[9]= '0' + (address & 0x03);
      } else if (address == 0x64)
        strcpy(device, "ATSHA204" );
      else
        strcpy(device, "Unknown" );

      sprintf(buffer, "0x%02X : %s", address, device );
      if (index < I2C_DISPLAY_DEVICE) {
        strcpy(i2c_dev[index++], buffer );
      }

      Serial.println(buffer);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.printf("Unknow error at address 0x%02X", address);
    }

    yield();
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found"));
  else
    Serial.printf("Scan done, %d device found\r\n", nDevices);

  return nDevices;
}

/* ======================================================================
  Function: drawProgress
  Purpose : prograss indication
  Input   : OLED display pointer
          percent of progress (0..100)
          String above progress bar
          String below progress bar
  Output  : -
  Comments: -
  ====================================================================== */
void Tela::drawProgress(OLEDDisplay *display, int percentage, String labeltop, String labelbot) {
  if (has_display) {
    display->clear();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_10);
    display->drawString(64, 8, labeltop);
    display->drawProgressBar(10, 28, 108, 12, percentage);
    display->drawString(64, 48, labelbot);
    display->display();
  }
}

/* ======================================================================
  Function: drawProgress
  Purpose : prograss indication
  Input   : OLED display pointer
          percent of progress (0..100)
          String above progress bar
  Output  : -
  Comments: -
  ====================================================================== */
void Tela::drawProgress(OLEDDisplay *display, int percentage, String labeltop ) {
  drawProgress(display, percentage, labeltop, String(""));
}

void Tela::mostrarConnectando(bool mostrar, const String& msg){
	mensagemConexao = msg;
	ehParaExibirMensagemConexao = mostrar;
	if ( mostrar ) { relogioTicker->resume(); } else { relogioTicker->stop();}

}

void Tela::drawTelaConnectando(){
	if ( ! has_display && ! ehParaExibirMensagemConexao ){
		return;
	}
	uint32_t counter = relogioTicker->counter();
	display.clear();
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, 10, "Connecting to WiFi");
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbol : inactiveSymbol);
	display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbol : inactiveSymbol);
	display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbol : inactiveSymbol);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	mensagemConexao = String (counter) + " | " + String(counter % 3);
	display.drawString(64, 45, mensagemConexao );
	display.display();
}