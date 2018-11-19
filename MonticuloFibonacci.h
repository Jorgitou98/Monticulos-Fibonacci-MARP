/*

Autor: Jorge Villarrubia Elvira

*/



#ifndef MonticuloFibonacci_h

#define MonticuloFibonacci_h



#include <iostream>

#include <stdexcept>

#include <vector>

#include <cmath>

#include <unordered_map>



/*

Por comodidad y para simplificar la sintaxis se añade el espacio de nombres estándar

(No estar escribiendo std:: en lo sucesivo)

*/

using namespace std;



// Genérico para el tipo de datos almacenados

template <class T = int, typename Comparator = std::less<T>>



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

	using Link = Nodo * ;

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

	unordered_map<T, Link> elementos;

	Comparator antes;



private:

	void insertaEnLPrincipal(Link const & x) {

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

				if (antes(x->elem, min->elem)) min = x;

			}

		}

	}



	void cortar(Link const& hijo, Link const& padre) {

		// Quito al hijo de la lista doblemente enlazada en que está

		hijo->hIz->hDer = hijo->hDer;

		hijo->hDer->hIz = hijo->hIz;

		padre->grado--;

		insertaEnLPrincipal(hijo);

		hijo->padre = nullptr;

		hijo->marca = false;



	}

	void cortarEnCascada(Link const& padre) {

		Link z = padre->padre;

		if (z != nullptr) {

			if (!padre->marca) padre->marca = true;

			else {

				cortar(padre, z);

				cortarEnCascada(z);

			}

		}

	}





	void mezclar(Link const& hijo, Link const& padre) {

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

			size_t D = 2 * floor(log(double(nelems)));

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

	mFib(Comparator c = Comparator()) : antes(c), min(nullptr), nelems(0) {}

	size_t size() {

		return nelems;

	}



	// A continuación la función que inserta elementos al montículo

	// Solo se inserta un elemento que no este ya. No admitimos duplicados

	void insertar(T const & e) {

		if (!elementos.count(e)) {

			Link x = new Nodo(e);

			insertaEnLPrincipal(x);

			nelems++;

			elementos.insert({ e, x });

		}

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



				if (min == nullptr || (otro.min != nullptr && antes(otro.min->elem, min->elem)))

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



			elementos.erase(minimo->elem);

			nelems--;

			T minBorrado = minimo->elem;

			delete minimo;

			return minBorrado;

		}

		else throw domain_error("No existe elemento minimo que quitar");

	}



	void decrementarClave(T const& eAntiguo, T const& eNuevo) {

		if (antes(eAntiguo, eNuevo)) throw invalid_argument("El valor de la nueva clave es mayor que el anterior");



		else if (!elementos.count(eAntiguo)) throw domain_error("El valor de la clave antigua aun no esta");



		/* Como no se admiten repetidos, si un elemento ya se decrece a otro que ya está

		basta con eliminar dicho elemento

		*/



		else if (elementos.count(eNuevo)) eliminar(eAntiguo);

		else {

			Link eCambiar = elementos[eAntiguo];

			eCambiar->elem = eNuevo;

			Link padre = eCambiar->padre;

			if (padre != nullptr && antes(eCambiar->elem, padre->elem)) {

				cortar(eCambiar, padre);

				cortarEnCascada(padre);

			}

			if (antes(eCambiar->elem, min->elem)) min = eCambiar;

			elementos.erase(eAntiguo);

			elementos.insert({ eNuevo , eCambiar });

		}

	}

	void eliminar(T const& e) {

		if (nelems > 0) {

			if (e == min->elem) quitarMinimo();

			else if (elementos.count(e)) {

				T minimo = min->elem;

				quitarMinimo();

				decrementarClave(e, minimo);

				quitarMinimo();

				insertar(minimo);

			}

		}

	}

};

#endif