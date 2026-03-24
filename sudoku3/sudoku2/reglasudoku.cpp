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

//void ReglasSudoku::mapeo(int f, int c) {
//
//
//	int dimension = this->tablero.dimension();
//	bool es_posible = false;
//	int dir_x[2] = { 1, 0 };
//	int dir_y[2] = { 0, 1 };
//	int raiz_perfecta = sqrt(dimension);
//	int lf = (f / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
//	int lc = (c / raiz_perfecta) * raiz_perfecta;
//
//	for (int i = 0; i < 2; i++) { // se procede hacer el analisis en base a las direcciones señaladas (columna y vertical) hacia adelante y luego hacia atras
//
//		int nf = f * dir_x[i];
//		int nc = c * dir_y[i];
//
//
//			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {
//
//				if (nf >= lf && (nf < (lf + raiz_perfecta)) && dir_x[i] == 0) {
//					nf += raiz_perfecta;
//				}
//				else if (nc >= lc && (nc < (lc + raiz_perfecta)) && dir_y[i] == 0) {
//					nc += raiz_perfecta;
//				}
//				else {
//					if (this->tablero.get_celda(nf, nc).dame_valor() > 0) { //primero para ver si el valor es posible hay que analizar los valores a su alrededor para calcular la multiplicidad de sus apariciones
//						//cout << "dirrecion: " << nf << " " << nc << "valor: " << this->tablero.get_celda(nf, nc).dame_valor() << endl;
//						//cout << endl;
//						//cout << "fila: " << nf << " columna: " << nc << "mapeo: " << this->tablero.get_celda(nf, nc).dame_valor() << " elementos: " << this->info_valores_no_validos.no_validos[f][c].dame_num_elems() << endl;
//						info_valores_no_validos.no_validos[f][c].eliminar(this->tablero.get_celda(nf, nc).dame_valor());
//						info_valores_no_validos.no_validos[f][c].insertar(this->tablero.get_celda(nf, nc).dame_valor());
//					}
//					nc += dir_x[i];
//					nf += dir_y[i];
//				}
//
//			}
//	}
//
//		int i = lf;
//
//		while ((i >= lf) && (i < (lf + raiz_perfecta))) { // una vez culminado ese tramo, se procede a analizar los valores que afectan a la celda en la cuadricula
//			int j = lc;
//			while ((j >= lc) && (j < (lc + raiz_perfecta))) {
//
//				//cout << "i: " << i << "j: " << j << endl;
//				if (this->tablero.get_celda(i, j).dame_valor() > 0) {
//					info_valores_no_validos.no_validos[f][c].eliminar(this->tablero.get_celda(i, j).dame_valor());
//					info_valores_no_validos.no_validos[f][c].insertar(this->tablero.get_celda(i, j).dame_valor());
//
//				}
//
//				j++;
//			}
//
//			i++;
//			// //cout << endl;
//		}
//
//}

bool ReglasSudoku::es_valor_posible(int f, int c, int v) {


	// primero para ver si el valor es posible, nos declaramos las variables fundamentales (direcciones del recorritdo, la dimension, si esta repetido y si es admitido o no)

	int dimension = this->tablero.dimension();
	bool es_posible = false;
	int k = 0;
	int nro_elms = this->info_valores_no_validos.no_validos[f][c].dame_num_elems();


	if (v > 0 && v <= dimension && this->tablero.get_celda(f, c).es_vacia()) { // se añade un campo de verificacion, que comprueba si el valor que se esta añadiendo esta dentro del rango posible y la celda en que se esta añadiendo esta vacia (no puede estar ni en estado original o ocupado)

			/*if (v == this->info_valores_no_validos.no_validos[f][c].pertenece(v)) {
				
			}*/
			if (!this->info_valores_no_validos.no_validos[f][c].pertenece(v)) {
				es_posible = true;
			}

		//mapeo(f, c);

		// una vez mapeado todo el alredeedor de la celda, se verifica si el valor que se quiere insertar puede pertenecer
	}

	return es_posible;
}

bool ReglasSudoku::pon_valor(int f, int c, int v) {

	// primero para poner un valor, nos declaramos las variables fundamentales (direcciones del recorritdo, la dimension, si esta repetido y si es admitido o no)

	int dimension = this->tablero.dimension();
	int dir_x[4] = { 1, 0};
	int dir_y[4] = { 0, 1};

	int raiz_perfecta = sqrt(dimension);
	int lf = (f / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
	int lc = (c / raiz_perfecta) * raiz_perfecta;

	bool addit = false;

	if (this->tablero.get_celda(f, c).es_vacia() && v > 0 && v <= dimension && es_valor_posible(f, c, v)) {
		
		// para añadir el valor a la celda y aumentar su multiplicidad se debe cumplir que esta vacia la celda, que el valor entra dentro del rango, y que es un valor posible en su columna, su fila y su cuadricula
		
		info_valores_no_validos.no_validos[f][c].insertar(v);
		this->tablero.set_valor(f, c, v, "OCUPADA");
		this->cont += 1;

		addit = true;


		for (int i = 0; i < 2; i++) { // se procede hacer el analisis en base a las direcciones señaladas (columna y vertical) hacia adelante y luego hacia atras

			int nf = f * dir_x[i];
			int nc = c * dir_y[i];


			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {

				if (nf >= lf && (nf < (lf + raiz_perfecta)) && dir_x[i] == 0) {
					nf += raiz_perfecta;
				}
				else if (nc >= lc && (nc < (lc + raiz_perfecta)) && dir_y[i] == 0) {
					nc += raiz_perfecta;
				}
				else {
					
					info_valores_no_validos.no_validos[nf][nc].insertar(v);


					nc += dir_x[i];
					nf += dir_y[i];
				}

			}
		}

		int i = lf;

		while ((i >= lf) && (i < (lf + raiz_perfecta))) { // una vez culminado ese tramo, se procede a analizar los valores que afectan a la celda en la cuadricula
			int j = lc;
			while ((j >= lc) && (j < (lc + raiz_perfecta))) {

				//cout << "i: " << i << "j: " << j << endl;
				info_valores_no_validos.no_validos[i][j].insertar(v);

				j++;
			}

			i++;
			// //cout << endl;
		}

		int dir_x[4] = { 1, 0, -1, 0 };
		int dir_y[4] = { 0, 1, 0, -1 };

		for (int i = 0; i < 4; i++) { // se procede a hacer un analisis de las filas y columnas para ver si hay alguna bloqueada por ese valor

			int nf = f + dir_x[i];
			int nc = c + dir_y[i];

			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {

				if (this->tablero.get_celda(nf, nc).es_vacia()) { // se comprueba si esta bloqueada solo si la celda es vacia
					block(nf, nc);
				}

				nf += dir_x[i];
				nc += dir_y[i];
			}
		}

		i = lf;

		while ((i >= lf) && (i < (lf + raiz_perfecta))) { // el mismo proceso que se hizo para la fila y la columna, se hace para la cuadricula

			int j = lc;
			while ((j >= lc) && (j < (lc + raiz_perfecta))) {

				if (this->tablero.get_celda(i, j).es_vacia()) {
					block(i, j);
				}

				j++;
			}

			i++;
		}

	}

	return addit;

}

void ReglasSudoku::block(int nf, int nc) {

	bool block_repetido = false;
	int p = 0;
	int dimension = this->tablero.dimension();


	while (p < this->pos_bloqueadas.cont && !block_repetido) { // primero para ver si hay celdas bloqueadas, se empieza analizando si la celda en que se esta ya pertenece a la lista de bloquedas

		if (this->pos_bloqueadas.lista_de_bloqueados[p].f == nf && this->pos_bloqueadas.lista_de_bloqueados[p].c == nc) {
			block_repetido = true;
		}
		p++;
	}
	if (!block_repetido) { // si dicha celda no se encontro dentro de las posiciones bloqueadas, entonces se procede a hacer un analisis de todos sus valores posibles, y si no tiene ni uno, es que esta bloqueada

		cout << "fila: " << nf << " columna: " << nc << " nro elem: " << this->info_valores_no_validos.no_validos[nf][nc].dame_num_elems() << endl;

		if (this->info_valores_no_validos.no_validos[nf][nc].dame_num_elems() == dimension) {
				//cout << "blocked" << endl;
				this->pos_bloqueadas.lista_de_bloqueados[this->pos_bloqueadas.cont].f = nf;
				this->pos_bloqueadas.lista_de_bloqueados[this->pos_bloqueadas.cont].c = nc;
				this->pos_bloqueadas.cont += 1;
		}
	}

}

void ReglasSudoku::unlock(int nf, int nc) {

	int k = 0;
	
	if (this->tablero.get_celda(nf, nc).es_vacia()) {
		while (k < dame_num_celdas_bloqueadas()) { // se procede a hacer una busqueda entre las celdas bloqueadas para ver si ocincide con alguna que este vacia

			if (this->pos_bloqueadas.lista_de_bloqueados[k].f == nf && this->pos_bloqueadas.lista_de_bloqueados[k].c == nc) {
				//cout << "celda bloqueada: " << nf << " " << nc << endl;
				
				//cout << "mapeo: " << this->info_valores_no_validos.no_validos[nf][nc].dame_num_elems() << endl;
				
				if (this->info_valores_no_validos.no_validos[nf][nc].dame_num_elems() < this->tablero.dimension()) { // si hay una celda 

					this->pos_bloqueadas.cont -= 1;

					for (int n = k; k < (dame_num_celdas_bloqueadas() - 1); n++) { // se hace el reordenamiento
						this->pos_bloqueadas.lista_de_bloqueados[n].f = this->pos_bloqueadas.lista_de_bloqueados[n + 1].f;
						this->pos_bloqueadas.lista_de_bloqueados[n].c = this->pos_bloqueadas.lista_de_bloqueados[n + 1].c;
					}

				}
			}
			k++;

		}

	}
}

bool ReglasSudoku::quita_valor(int f, int c) {

	// primero para quitar un valor, nos declaramos las variables fundamentales (direcciones del recorritdo, la dimension, si esta repetido y si es admitido o no)

	bool deleit = false;

	int v = this->tablero.get_celda(f, c).dame_valor();
	int num_celdas_bloqueadas = dame_num_celdas_bloqueadas();

	int dimension = this->tablero.dimension();
	bool es_posible = false;
	int dir_x[2] = { 1, 0 };
	int dir_y[2] = { 0, 1 };
	int raiz_perfecta = sqrt(dimension);
	int lf = (f / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
	int lc = (c / raiz_perfecta) * raiz_perfecta;

	//cout << "THE CELL NOT UNAVAILABLE IN THE POSITION (" << f << ", " << c << ") IS AT: " << this->tablero.get_celda(f, c).es_ocupada() << endl;

	if ((this->tablero.get_celda(f, c).es_ocupada())) { // se verifica que la celda este vacia, para setearla como vacia 

		//cout << "el valor de la celda es " << tablero.get_celda(f, c).dame_valor() << endl;

		deleit = true;
		this->tablero.set_valor(f, c, 0, "VACIO");
		this->cont -= 1;

		for (int i = 0; i < 2; i++) { // se procede hacer el analisis en base a las direcciones señaladas (columna y vertical) hacia adelante y luego hacia atras

			int nf = f * dir_x[i];
			int nc = c * dir_y[i];


			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {

				if (nf >= lf && (nf < (lf + raiz_perfecta)) && dir_x[i] == 0) {
					nf += raiz_perfecta;
				}
				else if (nc >= lc && (nc < (lc + raiz_perfecta)) && dir_y[i] == 0) {
					nc += raiz_perfecta;
				}
				else {

					info_valores_no_validos.no_validos[nf][nc].eliminar(v);


					nc += dir_x[i];
					nf += dir_y[i];
				}

			}
		}

		int i = lf;

		while ((i >= lf) && (i < (lf + raiz_perfecta))) { // una vez culminado ese tramo, se procede a analizar los valores que afectan a la celda en la cuadricula
			int j = lc;
			while ((j >= lc) && (j < (lc + raiz_perfecta))) {

				//cout << "i: " << i << "j: " << j << endl;
				info_valores_no_validos.no_validos[i][j].eliminar(v);

				j++;
			}

			i++;
			// //cout << endl;
		}



		for (int i = 0; i < 2; i++) { // se procede hacer el analisis en base a las direcciones señaladas (columna y vertical) hacia adelante y luego hacia atras

			int nf = f * dir_x[i];
			int nc = c * dir_y[i];


			while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {

				if (nf >= lf && (nf < (lf + raiz_perfecta)) && dir_x[i] == 0) {
					nf += raiz_perfecta;
				}
				else if (nc >= lc && (nc < (lc + raiz_perfecta)) && dir_y[i] == 0) {
					nc += raiz_perfecta;
				}
				else {
					//cout << "valor: " << v << endl;

					unlock(nf, nc);
				
					nc += dir_x[i];
					nf += dir_y[i];
				}



			}
		}

		i = lf;

		while ((i >= lf) && (i < (lf + raiz_perfecta))) { // una vez culminado ese tramo, se procede a analizar los valores que afectan a la celda en la cuadricula
			int j = lc;
			while ((j >= lc) && (j < (lc + raiz_perfecta))) {

		 //primero para ver si el valor es posible hay que analizar los valores a su alrededor para calcular la multiplicidad de sus apariciones
				//cout << "valor: " << v << endl;

				unlock(i, j);

				j++;
			}

			i++;
			// //cout << endl;
		}
	}	

	return deleit;

}

void ReglasSudoku::autocompletar() {

	int dimension = this->tablero.dimension();

	for (int i = 0; i < dimension; i++) {

		for (int j = 0; j < dimension; j++) {

			int k = 1;

			if (this->tablero.get_celda(i, j).es_vacia()) {
				//cout << i << " " << j << " " << this->info_valores_no_validos.no_validos[i][j].dame_num_elems() << endl;

				if ((dimension - this->info_valores_no_validos.no_validos[i][j].dame_num_elems()) == 1) {
					while ((k <= dimension) && (k > 0)) {

						if (!this->info_valores_no_validos.no_validos[i][j].pertenece(k)) {
							pon_valor(i, j, k);
						}

						k++;
					}

				}

			}


		}

	}


}

bool ReglasSudoku::carga_sudoku(ifstream& archivo) {

	bool done = false;
	int dim = 0;

	int dimension = this->tablero.dimension();
	bool es_posible = false;
	int dir_x[2] = { 1, 0 };
	int dir_y[2] = { 0, 1 };
	int raiz_perfecta = sqrt(dimension);


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


					int lf = (i / raiz_perfecta) * raiz_perfecta; // calculamos la cuadricula en que esta para analizarla
					int lc = (j / raiz_perfecta) * raiz_perfecta;

					for (int l = 0; l < 2; l++) {
						int nf = i * dir_x[l];
						int nc = j * dir_y[l];

						while (nf >= 0 && nf < dimension && nc >= 0 && nc < dimension) {

							if (nf >= lf && (nf < (lf + raiz_perfecta)) && dir_x[l] == 0) {
								nf += raiz_perfecta;
							}
							else if (nc >= lc && (nc < (lc + raiz_perfecta)) && dir_y[l] == 0) {
								nc += raiz_perfecta;
							}
							else {	 //primero para ver si el valor es posible hay que analizar los valores a su alrededor para calcular la multiplicidad de sus apariciones
									//cout << "dirrecion: " << nf << " " << nc << "valor: " << this->tablero.get_celda(nf, nc).dame_valor() << endl;
									//cout << endl;
									//cout << "fila: " << nf << " columna: " << nc << "mapeo: " << this->tablero.get_celda(nf, nc).dame_valor() << " elementos: " << this->info_valores_no_validos.no_validos[f][c].dame_num_elems() << endl;
									if (v > 0) 
										info_valores_no_validos.no_validos[nf][nc].insertar(v);
									}
									cout << "fila: " << i << " columna: " << j << " nro elem: " << this->info_valores_no_validos.no_validos[i][j].dame_num_elems() << endl;
								nc += dir_x[l];
								nf += dir_y[l];
							}

						}

					int n = lf;

					while ((n >= lf) && (n < (lf + raiz_perfecta))) { // una vez culminado ese tramo, se procede a analizar los valores que afectan a la celda en la cuadricula
						int m = lc;
						while ((m >= lc) && (m < (lc + raiz_perfecta))) {

							//cout << "i: " << i << "j: " << j << endl;
							if (v > 0) {
								info_valores_no_validos.no_validos[n][m].insertar(v);
							}

							m++;
						}

						n++;
						// //cout << endl;
					}

				if (v == 0) {
					this->tablero.set_valor(i, j, v, "VACIA");


				}
				else {
					this->tablero.set_valor(i, j, v, "ORIGINAL");
					this->cont += 1;
				}
					}



			}

		}

		done = true;

		archivo.close();

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

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
		
			int nro_elementos = this->info_valores_no_validos.no_validos[i][j].dame_num_elems();
			int k = 0;
			while (nro_elementos != 0 && k < dim) {
				this->info_valores_no_validos.no_validos[i][j].eliminar(k);
				if (this->info_valores_no_validos.no_validos[i][j].dame_num_elems() < nro_elementos) {
					nro_elementos = this->info_valores_no_validos.no_validos[i][j].dame_num_elems();
					k++;
				}
			}
		
		}
	}

	this->pos_bloqueadas.cont = 0;
}
