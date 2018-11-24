/*
Autor: Jorge Villarrubia Elvira
*/

#ifndef MonticulosFibonacci_h
#define MonticulosFibonacci_h

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
/* Clase con la familia de montículos, que maneja el usuario
*/
class familiaFib {
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

	Comparator antesFam;
	size_t nelemsTotal;


	template <class T = int>
	// Clase interna que representa cada monticulo de Fibonacci
	class mFib {
		/*
		A continuación se declaran los atributos de la clase.

			- Un puntero al mínimo de la lista principal
			- El número de nodos del montículo
			- Clase interna Nodo que represena el valor y los enlaces con hermanos, hijos y padres

		Se declararán como protected para permitir su manejo con herencia
		*/

	protected:

		Link min;

		size_t nelems;
		unordered_map<T, Link> elementos;

		Comparator antes;

		// Permitimos a la familia acceder a los atributos y funciones privadas y protegidas de cada montículo

		friend class familiaFib;

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
				size_t D = 2 * ceil(log(double(nelems)));
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

	protected:
		mFib(Comparator c = Comparator()) : antes(c), min(nullptr), nelems(0) {}

		size_t size() {
			return nelems;
		}

		// A continuación la función que inserta elementos al montículo
		// Solo se inserta un elemento que no este ya. No admitimos duplicados
		Link insertar(T const & e) {
			Link x = new Nodo(e);
			insertaEnLPrincipal(x);
			nelems++;
			return x;
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

	protected:

		using mDir = mFib<T> * ;
		/* Llevamos un diccionario no ordenado para acceder de forma constante a cada
		elemento de la familia y saber en que montículo se encuentra, con un puntero a él
		*/
		unordered_map<T, pair<Link, mDir>> elementos;

		// Diccionario que me permite acceder a un montículo a partir de su nº
		unordered_map<size_t, mDir> monticulos;

public:
	familiaFib(Comparator c = Comparator()) : antesFam(c), nelemsTotal(0) {}

	size_t size() {
		return nelemsTotal;
	}
	// Tamaño de un montículo en concreto
	size_t size(size_t pos) {
		if (pos > monticulos.size()) throw domain_error("El montículo cuyo tamaño has solicitado no existe");
		else return monticulos[pos]->size();
	}

	// Esta función permite insertar en un montículo concreto de la familia
	void insertar(T const& e, int pos) {
		// Tenemos disponibles montículos desde la posición 1 a la size()
		if (pos > monticulos.size() || pos <= 0) throw domain_error("El montículo donde querías insertar no existe. Consulta los disponibles (desde 1 hasta size())");
		// Solo insertamos si el elemento no está para no tener repetidos
		else if (!elementos.count(e)) {
			Link x = monticulos[pos]->insertar(e);
			nelemsTotal++;
			elementos.insert({ e, {x, monticulos[pos]} });
		}
	}
	/* Si la familia no tiene montículos o se quiere crear uno nuevo, esta función creo uno con un solo nodo
	 que pertenecerá a la familia */
	void insertar(T const& e){
		if (!elementos.count(e)) {
			mDir m = new mFib<T>(antesFam);
			Link x = m->insertar(e);
			monticulos.insert({ monticulos.size() + 1, m });
			elementos.insert({ e, {x, m} });
			nelemsTotal++;
		}
	}
	T const& minimo(int pos) const {
		// Tenemos disponibles montículos desde la posición 1 a la size()
		if (pos > monticulos.size() || pos <= 0) throw domain_error("El montículo donde querías insertar no existe. Consulta los disponibles (desde 1 hasta size())");
		else {
			try {
				return monticulos[pos]->minimo();
			}
			catch (domain_error d) {
				throw;
			}
		}
	}

	/* La unión se realiza sobre el primer montículo de la llamada. 
	El segundo montículo sigue por tanto en la familia, pero queda vacío sin nodos
	*/
	void unir(int pos1, int pos2) {
		if (pos1 > monticulos.size() || pos1 <= 0 ) throw domain_error("El montículo 1 al que querías unir, no existe. Consulta los disponibles (desde 1 hasta size())");
		if (pos2 > monticulos.size() || pos2 <= 0) throw domain_error("El montículo 2 que querías unir, no existe. Consulta los disponibles (desde 1 hasta size())");
		if(pos1 == pos2) throw domain_error("No puedes unir un montículo consigo mismo. No admitimos claves duplicadas");

		monticulos[pos1]->unir(*(monticulos[pos2]));

		/* Cambio el montículo 2, que he unido, por uno vació. 
		El usuario seguirá teniendo el mismo nº de montículos disponibles. 
		No es necesario cambiar el puntero a monticulo de cada elemento del monticulo 2. Este puntero solo se usará
		en decrementarClave, para llamar sobre un montículo que contenga la clave, y por tanto me sigue valiendo
		el mismo puntero al montículo2 que ya tenía almacenado
		*/
		monticulos[pos2] = new mFib<T>(antesFam);		
	}

	T const& quitarMinimo(int pos) {
		if (pos > monticulos.size() || pos <= 0) throw domain_error("El montículo donde querías insertar no existe. Consulta los disponibles (desde 1 hasta size())");
		else {
			try {
				return monticulos[pos]->quitarMinimo();
			}
			catch (domain_error d) {
				throw;
			}
		}
	}


};



#endif
