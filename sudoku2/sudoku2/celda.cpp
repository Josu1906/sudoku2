#include <iostream>
#include <string>
#include "celda.h"

using namespace std;

tCelda::tCelda() {
	this->v = 0;
	this->estado[0] = 0;
	this->estado[1] = 0;
	this->estado[2] = 1;
}

tCelda::tCelda(int v) {
	this->v = v;
	this->estado[1] = 1;
	this->estado[0] = 0;
	this->estado[2] = 0;
}

bool tCelda::es_vacia() {
	bool salida = false;
	if (estado[2] == 1) {
		salida = true;
	}

	return salida;
}

bool tCelda::es_original() {
	bool salida = false;
	if (estado[0] == 1) {
		salida = true;
	}

	return salida;
}
bool tCelda::es_ocupada() {
	bool salida = false;
	if (estado[1] == 1) {
		salida = true;
	}

	return salida;
}

void tCelda::set_valor(int valor) {
	this->v = valor;
	set_ocupada();
}

void tCelda::set_ocupada() {
	estado[1] = true;
	estado[0] = false;
	estado[2] = false;
}
void tCelda::set_original() {
	estado[0] = true;
	estado[1] = false;
	estado[2] = false;
}
void tCelda::set_vacia() {
	estado[2] = true;
	estado[0] = false;
	estado[1] = false;
}

int tCelda::dame_valor() {

	return v;
}