#ifndef MonticuloFibonacci_h
#define MonticuloFibonacci_h

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>

/*
Por comodidad y para simplificar la sintaxis se añade el espacio de nombres estándar
(No estar escribiendo std:: en lo sucesivo)
*/
using namespace std;

// Genérico para el tipo de datos almacenados
template <class T>

class monticuloFib {


	/*
	A continuación se declaran los atributos de la clase.

		- Un puntero al mínimo de la lista principal
		- El número de nodos del montículo
		- Clase interna Nodo que represena el valor y los enlaces con hermanos, hijos y padres

	Se declararán como protected para permitir su manejo con herencia
	*/

protected:
	struct Nodo;
	using Link = Nodo *;
	struct Nodo {
		Nodo(T const& e, Link const& hi, Link const& hd,
			Link const& h, Link const& p, bool m, int g) :
			elem(e), hIz(hi), hDer(hd), hijo(h), padre(p), marca(m), grado(g) {}

		Nodo(T const& e) : elem(e), hIz(nullptr), hDer(nullptr), hijo(nullptr),
			padre(nullptr), marca(false), grado(0) {}

		T elem;
		Link hIz, hDer, hijo, padre;
		bool marca; // Indicará la pérdida de un hijo de este nodo
		size_t grado; // Altura del subárbol que tiene por debajo
	};

	Link min;
	size_t nelems;


private :
	void insertaEnLPrincipal(Link x) { // Se asume que
		if (min == nullptr) {
			x->hDer = x;
			x->hIz = x;
			min = x;
		}
		else {
			x->hIz = min->hIz;
			x->hDer = min;
			(min->hIz)->hDer = x;
			min->hIz = x;
			if (x->elem < min->elem) min = x;
		}
	}

	void mezclar(Link hijo, Link padre) {

	}

	void consolidar() {
		if (min != nullptr) {
			size_t D = 2 * ceil( log( double(nelems) ) );
			vector <Link> A(D, nullptr);
			Link act = min;
			Link y, guardaAct; // guardaAct me permitirá intercambiar act e y
			do {
				size_t g = act->grado;
				while (A[g] != nullptr) {
					y = A[g];
					if (act->elem > y->elem) {
						guardaAct = act;
						act = y;
						y = guardaAct;
					}
					if (y == min) min = act; // El nuevo minimo está en act
					
					mezclar(y, act);

					if (act->hDer == act) min = act;// Si solo queda un elem en la lista es el minimo
					A[g] = nullptr;
					g++;
				}

				A[g] = act;
				act = act->hDer;

			} while (act != min);

			min = nullptr;
			for (int i = 0; i <= D; ++i) insertaEnLPrincipal(A[i]);
		}
	}


//Lo siguiente que se declara es un constructor del montículo vacío


public:
monticuloFib (): min(nullptr),  nelems(0){}

size_t size() {
	return nelems;
}

// A continuación la función que inserta elementos al montículo

void inserta(T const & e) {
		Link x = new Nodo(e);
		insertaEnLPrincipal(x);
		nelems++;
}

T const& minimo() const {
	if (min == nullptr) throw domain_error("El montículo está vacío");
	else return min->elem;
}

void unir(monticuloFib<T> const& otro) {

	if (otro.min != nullptr) {
		if (min == nullptr) min = otro.min;
		else {
			Link izOrig = min->hIz;
			Link izOtro = otro.min->hIz;
			min->hIz = izOtro;
			izOrig->hDer = otro.min;
			otro.min->hIz = izOrig;
			izOtro->hDer = min;

			if (min == nullptr || (otro.min != nullptr && otro.min->elem < min->elem))
				min = otro.min;
		}

		nelems += otro.nelems;
	}
}

T const& quitarMinimo() {

	if (min != nullptr) {

		Link minimo = min;
		Link hijo = min->hijo;

		// Primero añadimos todos los hijos del minimo a la lista pricipal

		if (hijo != nullptr) {
			minimo->hIz->hDer = hijo;
			hijo->hIz->hDer = minimo;
			Link anteriorHijo = hijo->hIz;
			hijo->hIz = minimo->hIz;
			minimo->hIz = anteriorHijo;
		}

		minimo->hijo = nullptr;

		for (Link act = hijo; act != minimo; act = act->hDer) act->padre = nullptr;

		// Eliminamos el minimo de la lista principal

		minimo->hIz->hDer = minimo->hDer;
		minimo->hDer->hIz = minimo->hIz;

		if (minimo == minimo->hDer) min = nullptr;
		else {
			min = minimo->hDer;
			//consolidar();
		}
		nelems--;
		return minimo->elem;
	}
	else throw domain_error("No existe elemento minimo que quitar");

}

};

#endif
