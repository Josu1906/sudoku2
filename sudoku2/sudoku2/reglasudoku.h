#pragma once

#include <iostream>
#include "tablero.h"
#include <string>

using namespace std;

typedef struct {
	int f;
	int c;
} tPosBloqueada;

typedef tPosBloqueada tListaBloqueadas[(DIM_MAX * DIM_MAX)];

typedef struct {
	int cont;
	tListaBloqueadas lista_de_bloqueados;
} tBloqueada;


class ReglasSudoku {

private:
	int cont;
	Tablero tablero = Tablero(9);
	tBloqueada pos_bloqueadas;

public:

	ReglasSudoku(int dim){
		cont = 0;

		tablero = Tablero(dim);

		pos_bloqueadas.cont = 0;

		for (int i = 0; i < (DIM_MAX * DIM_MAX); i++) {
			pos_bloqueadas.lista_de_bloqueados[i].f = 0;
			pos_bloqueadas.lista_de_bloqueados[i].c = 0;
		}
	}

	ReglasSudoku() {
		cont = 0;

		pos_bloqueadas.cont = 0;

		for (int i = 0; i < (DIM_MAX * DIM_MAX); i++) {
			pos_bloqueadas.lista_de_bloqueados[i].f = 0;
			pos_bloqueadas.lista_de_bloqueados[i].c = 0;
		}
	}

	/* consultoras */
	int dame_dimension();
	tCelda dame_celda(int f, int c);
	bool terminado();
	bool bloqueo();
	int dame_num_celdas_bloqueadas();
	tCelda dame_celda_bloqueada(int p, int& f, int& c);
	bool es_valor_posible(int f, int c, int v);

	/* modificadoras */
	bool pon_valor(int f, int c, int v);
	bool quita_valor(int f, int c);
	void reset();
	void autocompletar();

	/* inicializadora */
	bool carga_sudoku(ifstream& archivo);
}; 
