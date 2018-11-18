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

class mFib {


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
public:
mFib (): min(nullptr),  nelems(0){}

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

void unir(mFib<T> const& otro) {

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
	};

#endif
