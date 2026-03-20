#include <iostream>
#include <string>
#include <fstream>
#include "tablero.h"
#include "celda.h"
#include "reglasudoku.h"
#include <cmath>

using namespace std;

int ReglasSudoku::dame_dimension() {

	return this->tablero.dimension();
}

tCelda ReglasSudoku::dame_celda(int f, int c) {

	return this->tablero.get_celda(f, c);
}

bool ReglasSudoku::terminado() {

	int dimension = this->tablero.dimension();
	bool done = false;

	if ((dimension * dimension) == this->cont) {
		done = true;
	}

	return done;
}

bool ReglasSudoku::bloqueo() {

	bool blocked = false;

	if (this->pos_bloqueadas.cont > 0) {
		blocked = true;
	}

	return blocked;
}

int ReglasSudoku::dame_num_celdas_bloqueadas() {

	return this->pos_bloqueadas.cont;
}

tCelda ReglasSudoku::dame_celda_bloqueada(int p, int& f, int& c) {

	f = this->pos_bloqueadas.lista_de_bloqueados[p].f;
	c = this->pos_bloqueadas.lista_de_bloqueados[p].c;

	return this->tablero.get_celda(f, c);
}

bool ReglasSudoku::es_valor_posible(int f, int c, int v) {


	// primero para ver si el valor es posible, nos declaramos las variables fundamentales (direcciones del recorritdo, la dimension, si esta repetido y si es admitido o no)

	int dir_x[4] = { 1, 0, -1, 0 };
	int dir_y[4] = { 0, 1, 0, -1 };
	int dimension = this->tablero.dimension();
	bool repetido = false;
	bool admitido = false;

	if (v > 0 && v <= dimension) {
		// //cout << endl;
		// //cout << "THE ANALYSED VALUES IN THE ROW FIRST AND AFTER THAT IN THE COLUMN ARE: " << endl;
		for (int i = 0; i < 4; i++) {

			// se procede a recorrer por la fila y columna en donde se quiere poner el valor

			int nf = f + dir_x[i];
			int nc = c + dir_y[i];
			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension && !repetido) {

				// //cout << this->tablero.get_celda(nf, nc).dame_valor();
				// //cout << " ";

				if (v == this->tablero.get_celda(nf, nc).dame_valor()) {
					repetido = true;
				}
				else {
					nf += dir_x[i];
					nc += dir_y[i];
				}
			}

			/*//cout << endl;*/

		}

		// //cout << endl;

		// si despues de hacer el recorrido no se encuentra nunca un valor identico, se procede a hacer el analisis de la cuadricula

		if (!repetido) {
			int raiz_perfecta = sqrt(dimension);
			int lf = (f / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
			int lc = (c / raiz_perfecta) * raiz_perfecta;


			// //cout << "THE ANALYSED VALUES IN THE GRID ARE: " << endl;

			int i = lf;

			while ((i >= lf) && (i < (lf + raiz_perfecta)) && !repetido) {
				/*//cout << i << " ";*/
				int j = lc;
				while ((j >= lc) && (j < (lc + raiz_perfecta)) && !repetido) {

					/*//cout << this->tablero.get_celda(i, j).dame_valor();
					//cout << endl;*/


					if (v == this->tablero.get_celda(i, j).dame_valor()) {
						////cout << "el numero: " << v << " esta repetido" << endl;
						repetido = true;
					}


					j++;
				}

				i++;
				// //cout << endl;
			}

			if (!repetido) { // si no esta repitido, esta admitido
				admitido = true;
			}

		}

	}


	return admitido;

}

bool ReglasSudoku::pon_valor(int f, int c, int v) {

	int dimension = this->tablero.dimension();
	int dir_x[4] = { 1, 0, -1, 0 };
	int dir_y[4] = { 0, 1, 0, -1 };
	bool addit = false;

	//cout << "THE ATTEMPTED VALUE IS " << v << endl;

	//cout << "THE CELL NOT UNAVAILABLE IN THE POSITION (" << f << ", " << c << ") IS AT: " << this->tablero.get_celda(f, c).es_ocupada() << endl;

	if (!(this->tablero.get_celda(f, c).es_ocupada()) && !(this->tablero.get_celda(f, c).es_original()) && v > 0 && v <= dimension && es_valor_posible(f, c, v)) {
		this->tablero.set_valor(f, c, v, "OCUPADA");
		this->cont += 1;

		addit = true;


		for (int i = 0; i < 4; i++) { // se procede a hacer un analisis de las filas y columnas para ver si  hay alguna bloqueada

			int nf = f + dir_x[i];
			int nc = c + dir_y[i];

			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {

				int k = 1;
				bool block = true;
				bool block_repetido = false;

				// analiza los valores posibles de la fila y la columna alrededor celda que se puso el valor. si no encuentra un valor posible para una celda quiere decir que esta bloqueada

				//cout << "THE CELL EMPTY  IN THE POSITION (" << nf << ", " << nc << ") IS AT: " << this->tablero.get_celda(nf, nc).es_vacia() << endl;

				//cout << this->tablero.get_celda(nf, nc).dame_valor() << " " << endl;

				if (this->tablero.get_celda(nf, nc).es_vacia()) {
					//cout << "EVALUATING IF THERE ARE CELLS BLOCKED..." << endl;
					while (k <= dimension && block) {
						if (es_valor_posible(nf, nc, k)) {
							////cout << "THERE ARE ONE POSSIBLE VALUE" << endl;
							block = false;
						}
						k++;
					}
					if (block) {

						//cout << "THE BLOCKED CELLS ARE: ... ";
						int p = 0;
						while (p < this->pos_bloqueadas.cont && !block_repetido) {

							//cout << "(" << nf << ", " << nc << "): ";

							if (this->pos_bloqueadas.lista_de_bloqueados[p].f == nf && this->pos_bloqueadas.lista_de_bloqueados[p].c == nc) {
								//cout << "DUPLICATED, ";
								block_repetido = true;
							}
							p++;
						}
						if (!block_repetido) {
							//cout << "NEW (ANALYZED IN ROWS COLUMNS), ";
							this->pos_bloqueadas.lista_de_bloqueados[this->pos_bloqueadas.cont].f = nf;
							this->pos_bloqueadas.lista_de_bloqueados[this->pos_bloqueadas.cont].c = nc;
							this->pos_bloqueadas.cont += 1;

						}
						//cout << endl;
					}
				}


				nf += dir_x[i];
				nc += dir_y[i];
			}

		}

		int raiz_perfecta = sqrt(dimension);
		int lf = (f / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
		int lc = (c / raiz_perfecta) * raiz_perfecta;


		// //cout << "THE ANALYSED VALUES IN THE GRID ARE: " << endl;

		int i = lf;

		while ((i >= lf) && (i < (lf + raiz_perfecta))) {
			/*//cout << i << " ";*/
			int j = lc;
			while ((j >= lc) && (j < (lc + raiz_perfecta))) {

				bool block_repetido = false;
				/*//cout << this->tablero.get_celda(i, j).dame_valor();
				//cout << endl;*/
				int k = 1;
				bool block = true;


				if (this->tablero.get_celda(i, j).es_vacia()) {
					//cout << "EVALUATING IF THERE ARE CELLS BLOCKED..." << endl;
					while (k <= dimension && block) {
						if (es_valor_posible(i, j, k)) {
							//cout << "THERE ARE ONE POSSIBLE VALUE" << endl;
							block = false;
						}
						k++;
					}

					if (block) {

						//cout << "THE BLOCKED CELLS ARE: ... ";
						int p = 0;

						//cout << "(" << i << ", " << j << "): ";

						while (p < this->pos_bloqueadas.cont && !block_repetido) {

							//cout << "(" << pos_bloqueadas.lista_de_bloqueados[p].f << ", " << pos_bloqueadas.lista_de_bloqueados[p].c << ")" ;

							if (this->pos_bloqueadas.lista_de_bloqueados[p].f == i && this->pos_bloqueadas.lista_de_bloqueados[p].c == j) {
								block_repetido = true;
								//cout << "DUPLICATED, ";
							}
							p++;
						}

						if (!block_repetido) {
							//cout << "NEW IN GRID, ";
							this->pos_bloqueadas.lista_de_bloqueados[this->pos_bloqueadas.cont].f = i;
							this->pos_bloqueadas.lista_de_bloqueados[this->pos_bloqueadas.cont].c = j;
							this->pos_bloqueadas.cont += 1;
						}

						//cout << endl;
					}

				}


				j++;
			}

			i++;
			// //cout << endl;
		}

	}

	return addit;

}

bool ReglasSudoku::quita_valor(int f, int c) {

	bool deleit = false;

	int dimension = this->tablero.dimension();
	int v = this->tablero.get_celda(f, c).dame_valor();
	int dir_x[4] = { 1, 0, -1, 0 };
	int dir_y[4] = { 0, 1, 0, -1 };
	int num_celdas_bloqueadas = dame_num_celdas_bloqueadas();

	//cout << "THE CELL NOT UNAVAILABLE IN THE POSITION (" << f << ", " << c << ") IS AT: " << this->tablero.get_celda(f, c).es_ocupada() << endl;

	if ((this->tablero.get_celda(f, c).es_ocupada())) {
		deleit = true;
		this->tablero.set_valor(f, c, 0, "VACIO");
		this->cont -= 1;

		for (int i = 0; i < 4; i++) {
			int nf = f + dir_x[i];
			int nc = c + dir_y[i];

			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) { // solo puede haber una celda que este bloqueada por ese valor?

				int k = 0;

				tCelda celda_puntera = tablero.get_celda(nf, nc);

				if (celda_puntera.es_vacia()) {


					while (k < num_celdas_bloqueadas) {

						if (this->pos_bloqueadas.lista_de_bloqueados[k].f == nf && this->pos_bloqueadas.lista_de_bloqueados[k].c == nc) {
							if (es_valor_posible(nf, nc, v)) {

								this->pos_bloqueadas.cont -= 1;

								for (int n = k; k < (num_celdas_bloqueadas - 1); k++) { // se hace el reordenamiento
									this->pos_bloqueadas.lista_de_bloqueados[n].f = this->pos_bloqueadas.lista_de_bloqueados[n + 1].f;
									this->pos_bloqueadas.lista_de_bloqueados[n].c = this->pos_bloqueadas.lista_de_bloqueados[n + 1].c;
								}

							}
						}
						k++;

					}

				}

				nf += dir_x[i];
				nc += dir_y[i];

			}

		}

		int raiz_perfecta = sqrt(dimension);
		int lf = (f / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
		int lc = (c / raiz_perfecta) * raiz_perfecta;


		// //cout << "THE ANALYSED VALUES IN THE GRID ARE: " << endl;

		int i = lf;

		while ((i >= lf) && (i < (lf + raiz_perfecta))) {
			/*//cout << i << " ";*/
			int j = lc;
			while ((j >= lc) && (j < (lc + raiz_perfecta))) {
				
				int k = 0;

				tCelda celda_puntera = tablero.get_celda(i, j);

				if (celda_puntera.es_vacia()) {


					while (k < num_celdas_bloqueadas) {

						if (this->pos_bloqueadas.lista_de_bloqueados[k].f == i && this->pos_bloqueadas.lista_de_bloqueados[k].c == j) {
							if (es_valor_posible(i, j, v)) {

								this->pos_bloqueadas.cont -= 1;

								for (int n = k; k < (num_celdas_bloqueadas - 1); k++) { // se hace el reordenamiento
									this->pos_bloqueadas.lista_de_bloqueados[n].f = this->pos_bloqueadas.lista_de_bloqueados[n + 1].f;
									this->pos_bloqueadas.lista_de_bloqueados[n].c = this->pos_bloqueadas.lista_de_bloqueados[n + 1].c;
								}

							}
						}
						k++;

					}

				}
				
				j++;
			}
			i++;
		}
	}

	return deleit;

}

void ReglasSudoku::autocompletar() {

	int dimension = this->tablero.dimension();

	for (int i = 0; i < dimension; i++) {

		for (int j = 0; j < dimension; j++) {

			int num_celda_encontradas = 0;
			int k = 1;
			int v = 0;

			if (this->tablero.get_celda(i, j).es_vacia()) {

				while ((k <= dimension) && (k > 0) && (num_celda_encontradas < 2)) {

					if (es_valor_posible(i, j, k)) {
						num_celda_encontradas++;
						v = k;
					}

					k++;
				}

				if (num_celda_encontradas > 0 && num_celda_encontradas < 2) {
					pon_valor(i, j, v);
				}

			}


		}

	}


}

bool ReglasSudoku::carga_sudoku(ifstream& archivo) {

	bool done = false;
	int dim = 0;


	archivo.open("sudoku_1.txt");
	if (archivo.is_open()) {

		archivo >> dim;
		int v = 0;



		this->tablero = Tablero(dim);
		int dimension = this->tablero.dimension();

		//cout << "Dimension del sudoku: " << dimension << "x" << dimension << endl;

		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < dimension; j++) {
				archivo >> v;
				if (v == 0) {
					this->tablero.set_valor(i, j, v, "VACIA");
				}
				else {
					this->tablero.set_valor(i, j, v, "ORIGINAL");
					this->cont += 1;
				}
			}
		}

		done = true;

		archivo.close();
	}

	return done;
}

void ReglasSudoku::reset() {

	int dim = this->tablero.dimension();


	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (this->tablero.get_celda(i, j).es_ocupada()) {
				this->tablero.set_valor(i, j, 0, "VACIO");
				this->cont -= 1;
			}
		}
	}


	for (int i = 0; i < this->pos_bloqueadas.cont; i++) {
		this->pos_bloqueadas.lista_de_bloqueados[i].f = 0;
		this->pos_bloqueadas.lista_de_bloqueados[i].c = 0;
	}
	this->pos_bloqueadas.cont = 0;
}
