#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <queue>
#include "../Trabajo de Marp/MonticulosFibonacci.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


/* Comprobaremos las distintas operaciones comparando las salidas a entradas elatorias de nuestro montículos
con las de la cola de prioridad de la stl
*/
namespace TestMonticulosFibonacci
{		
	TEST_CLASS(TestOperaciones)
	{
	public:

		//Monticulo de máximos. Mas facil probarlo así con la stl
		TEST_METHOD(InsercionYMinimo)
		{
			srand(time(NULL));
			priority_queue <int> pq;
			familiaFib <int, greater<int>> fib;
			fib.creaMonticuloVacio();
			// Puesto que la familia nos devuelve el min de un monticulo concreto, toda la inseción se hará sobre el mismo
			// Introducimos en ambos un numero aleatorio de 0 a 9999
			for (int i = 0; i < 200000; ++i) {
				int aleatorio = rand() % 10000;
				pq.push(aleatorio);
				fib.insertar(aleatorio, 1);
				Assert::AreEqual(pq.top(), fib.minimo(1));
			}
		}
		TEST_METHOD(InsercionYExtracciónMinimo) {
			srand(time(NULL));
			priority_queue <int> pq;
			familiaFib <int, greater<int>> fib;
			fib.creaMonticuloVacio();
			// Puesto que la familia extrae el min de un monticulo concreto, toda la inseción se hará sobre el mismo
			/* Para poder borrar sin problemas, como no se insertan repetidos, se usará un conjunto que asegure
			que el random que se inserta no está repetido
			*/
			for (int i = 0; i < 200000; ++i) {
				int aleatorio = rand() % 10000;
				if (!fib.estaElemento(aleatorio)) {
					pq.push(aleatorio);
					fib.insertar(aleatorio, 1);
					Assert::AreEqual(pq.top(), fib.minimo(1));
					pq.pop();
					fib.quitarMinimo(1);
				}
			}
		}
		// Para la eliminar uno cualquiera necesitamos comprobar que esté el elemento antes
		// Las expceciones se comporbarán depués. Por ahora vamos a hace eliminados correctos
		// Para eliminar con mas frecuecia, generaremos random solo desde 0 hasta 19
		/*TEST_METHOD(EiminacionElementoCualquiera) {
			srand(time(NULL));
			priority_queue <int> pq;
			familiaFib <int, greater<int>> fib;
			fib.creaMonticuloVacio();
			for (int i = 0; i < 200000; ++i) {
				int aleatorio = rand() % 20;
				// Solo eliminamos si sí que está
				if (fib.estaElemento(aleatorio)) {
					Assert::AreEqual(pq.top(), fib.minimo(1));
				}
				else {
					pq.push(aleatorio);
					fib.insertar(aleatorio, 1);
				}
			}
		}*/
	};

	TEST_CLASS(TestDeErrores)
	{
	public:
		TEST_METHOD(MinimoInexistente)
		{
			familiaFib<int> fib;
			try {
				fib.minimo(1);
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}


	};

}