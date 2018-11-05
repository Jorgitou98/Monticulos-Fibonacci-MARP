#ifndef MonticuloFibonacci_h
#define MonticuloFibonacci_h

#include <iostream>
#include <unordered_set>

/*
Por comodidad y para simplificar la sintaxis se a�ade el espacio de nombres est�ndar
(No estar escribiendo std:: en lo sucesivo)
*/
using namespace std;

// Gen�rico para el tipo de datos almacenados
template <class T>

class monticuloFib {


	/*
	A continuaci�n se declaran los atributos de la clase.

		- Un puntero al m�nimo de la lista principal
		- El n�mero de nodos del mont�culo
		- Clase interna Nodo que represena el valor y los enlaces con hermanos, hijos y padres

	Se declarar�n como protected para permitir su manejo con herencia
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
		bool marca; // Indicar� la p�rdida de un hijo de este nodo
		int grado; // Altura del sub�rbol que tiene por debajo
	};

	Link min;
	int nelems;


private :
	void insertaEnLPrincipal(Link x) {
		x->hIz = min->hIz;
		x->hDer = min;
		(min->hIz)->hDer = x;
		min->hIz = x;
	}

//Lo siguiente que se declara es un constructor del mont�culo vac�o


public:
monticuloFib (): min(nullptr),  nelems(0){}

// A continuaci�n la funci�n que inserta elementos al mont�culo

void inserta(T const & e) {
		Link x = new Nodo(e);
		if (min == nullptr) {
			x->hDer = x;
			x->hIz = x;
			min = x;
		}
		else {
			insertaEnLPrincipal(x);
			if (x->elem < min->elem) min = x;
		}
		nelems++;
}

T const& minim() const {
	if (min == nullptr) throw domain_error("El mont�culo est� vac�o");
	else return min->elem;
}

void unir(monticuloFib<T> const& otro) {
	Link izOrig = min->hIz;
	Link izOtro = otro->min->hIz;
	min->hIz = izOtro;
	izOrig->hDer = otro->min;
	otro->min->hIz = izOrig;
	izOtro->hDer = min;

	if (min == nullptr || (otro->min != nullptr && otro->min->elem < min->elem))
		min = otro->min;

	nelems += otro->nelems;
}





};



#endif
