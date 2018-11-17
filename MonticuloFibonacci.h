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


private :
	void insertaEnLPrincipal(Link x) {
		if (x != nullptr) {
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
	}

	void mezclar(Link hijo, Link padre) {
		// Primero quito de la lista principal el que será hijo
		(hijo->hIz)->hDer = hijo->hDer;
		(hijo->hDer)->hIz = hijo->hIz;
		// Falta aún enlazarlo correctamente con si mismo por izquierda y derecha

		if (padre->hijo == nullptr) {
			padre->hijo = hijo;
			// Ahora rompo los enlaces que le quedaban con sus antiguos hermanos
			hijo->hDer = hijo;
			hijo->hIz = hijo;
		}
		else {
			hijo->hDer = padre->hijo;
			hijo->hIz = padre->hijo->hIz;
			padre->hijo->hIz->hDer = hijo;
			padre->hijo->hIz = hijo;
		}
		// En ambos casos se convierte en hijo del padre. Luego:
		hijo->padre = padre;
		padre->grado++;

	}

	void consolidar() {
		if (min != nullptr) {
			size_t D = 2 * ceil( log( double(nelems) ) );
			vector<Link> A;
			for (int i = 0; i < D; ++i) {
				A.push_back(nullptr);
			}

			/* La varibale actSig es porque el avance no será siempre al siguiente en
			la lista principal, de forma que guardo encada caso qué nodo vistoen la siguiente
			iteración en esta variable */

			Link act = min, actSig = min->hDer; 
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
					/* Si el nuevo hijo será el que tenía hasta ahora como min, 
					y por tanto desaparecerá de la lista principal, y es el siguiente
					en visitarse hago que el siguiente en visitarse sea su siguiente.
					Lo avanzo uno de mas
					*/
					if (actSig == min && y == min) {
						actSig = y->hDer;
					}

					if (y == min) min = y->hDer;
					
					mezclar(y, act);

					A[g] = nullptr;
					g++;
				}

				A[g] = act;
				act = actSig;
				actSig = act->hDer;

			} while (act != min);

			min = nullptr;
			for (int i = 0; i < D; ++i) insertaEnLPrincipal(A[i]);
		}
	}


//Lo siguiente que se declara es un constructor del montículo vacío


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
			min = minimo;
			minimo->hijo = nullptr;
			for (Link act = hijo; act != minimo; act = act->hDer) act->padre = nullptr;
		}
		// Eliminamos el minimo de la lista principal. No es necesario actalizar su grado a 0. Va a desaparecer

		minimo->hIz->hDer = minimo->hDer;
		minimo->hDer->hIz = minimo->hIz;

		if (minimo == minimo->hDer) min = nullptr;
		else {
			min = minimo->hDer;
			consolidar();
		}
		nelems--;
		return minimo->elem;
	}
	else throw domain_error("No existe elemento minimo que quitar");

}

};

#endif
