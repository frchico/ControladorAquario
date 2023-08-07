#pragma once

#ifndef Configuracao_H
#define Configuracao_H

#include <Arduino.h>
struct Configuracao {
	uint pinServo = D7;
	uint posGavetaFechada = 15;
	uint posGavetaAberta = 43;
	uint qtdVezesParaAlimentar = 2;
	uint16 totalPassos = 3;


	bool connected = false;
	String IP;
	String apName = "AP-NAME";
	String apPwd = "AP-PASSWORD";

	String nomeDispositivo = "Aquario";

};

#endif