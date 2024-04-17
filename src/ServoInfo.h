#pragma once

#ifndef SERVO_INFO_H
#define SERVO_INFO_H
#include <Arduino.h>
enum StatusServo {
	Aberto,
	Fechado,
	CheckConnections,
	Iniciando,
	Alimentando,
	Finalizado

};

// Definição da estrutura ServoInfo
struct ServoInfo{
    byte passoAtual;
    byte passoTotal;
	StatusServo status;

};
#endif // ALIMENTAR_PEIXES_INFO_H
