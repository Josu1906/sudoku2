#include "multiconjunto.h"
#include <iostream>

using namespace std;


int MultiConjunto::dame_num_elems() {

	int i = 0;

	while (this->lista[i].v != 0) {
		//cout << " elemento: " << lista[i].v << " posicin: " << i << endl;
		i++;
	}

	return i;

}

bool MultiConjunto::pertenece(int v) {

	int dim = dame_num_elems();
	int i = 0;
	bool encontrado = false;
	//cout << "v: " << v << endl;

	while (i < dim && !encontrado) {
		if (lista[i].v == v) {
			encontrado = true;
			//cout << "ENCONTRADO " << v << ": " << lista[i].multiplicidad << endl;
		}
		i++;
	}

	return encontrado;

}

bool MultiConjunto::insertar(int v){

	int cont = dame_num_elems();
	bool insertado = false;

	if (pertenece(v)) {
		bool encontrado = false;
		int i = 0;
		while (!encontrado && i < cont) {
			if (this->lista[i].v == v) {
				lista[i].multiplicidad++;
				insertado = true;
				encontrado = true;
			}
			i++;
		}
	}
	else {
		lista[cont].v = v;
		lista[cont].multiplicidad++;
		//cout << cont << endl;
		insertado = true;
	}
	return insertado;
}


bool MultiConjunto::eliminar(int v) {
	int dim = dame_num_elems();
	int index = 0;
	bool encontrado = false;

	while (index < dim && !encontrado) {
		if (lista[index].v == v) {
			encontrado = true;
			lista[index].multiplicidad--;
			if (lista[index].multiplicidad < 1) {
				for (int n = index; n < (dim - 1); n++) { // se hace el reordenamiento
					//this->lista[n].v = this->lista[n + 1].v;
					//this->lista[n].multiplicidad = this->lista[n + 1].multiplicidad;
					this->lista[n] = this->lista[n + 1];
				}
				this->lista[dame_num_elems() - 1].v = 0;
				this->lista[dame_num_elems() - 1].multiplicidad = 0;
			}


		}
		else {
			index++;
		}

	}

	return encontrado;
}
