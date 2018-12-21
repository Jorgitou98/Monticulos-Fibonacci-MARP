#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Trabajo de Marp/IndexPQ.h"
#include <stdio.h>
#include <stdlib.h> 
#include <ctime>
#include <vector>
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
				int aleatorio = rand() % 300000;
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
				int aleatorio = rand() % 300000;
				if (!fib.estaElemento(aleatorio)) {
					pq.push(aleatorio);
					fib.insertar(aleatorio, 1);
					Assert::AreEqual(pq.top(), fib.minimo(1));
					pq.pop();
					fib.quitarMinimo(1);
				}
			}
		}

		// Unicamente probamos decrecimientos válidos. Sin considerar excepciones
		TEST_METHOD(DecrecimientoDeClaves) {
			srand(time(NULL));
			IndexPQ<int, greater<int>> pq(200000);
			familiaFib <int, greater<int>> fib;
			fib.creaMonticuloVacio();
			// Insertamos muchos elementos grandes que después decreceremos
			for (int i = 0; i < 200000; ++i) {
				int aleatorio = rand() % 300000;
				//No insertamos repetidos para evitar la excepcion de indexPQ
				if (!fib.estaElemento(aleatorio)) {
					pq.push(aleatorio, aleatorio); // Cada elemento es su propia prioridad
					fib.insertar(aleatorio, 1);
				}
			}
			for (int j = 0; j < 200000; ++j) {
				int aleatorioGrande = rand() % 300000;
				int aleatorioPequeno = rand() % 30000;

				if (fib.estaElemento(aleatorioGrande) && !fib.estaElemento(aleatorioPequeno) && aleatorioGrande < aleatorioPequeno) {
					pq.update(aleatorioGrande, aleatorioPequeno);
					fib.decrementarClave(aleatorioGrande, aleatorioPequeno);
					Assert::AreEqual(pq.top().prioridad, fib.minimo(1));
				}
			}

		}

		/* Puesto que en esta prueba es necesario saber que elementos podemos elimina o no para evitar excepciones
			no usaremos random */
			/*
		TEST_METHOD(EliminacionElementoCualquiera) {
			srand(time(NULL));
			familiaFib <int> fib;
			fib.creaMonticuloVacio();
			for (int i = 0; i < 200000; ++i) {
				fib.insertar(i, 1);
			}
			for (int i = 199999; i >= 0; --i) {
				fib.eliminar(i);
			}
			Assert::AreEqual(true, fib.emptyRaiz(1));
		}*/
		

		TEST_METHOD(EliminacionElementoCualquiera) {
			srand(time(NULL));
			vector<int> v;
			familiaFib <int> fib;
			fib.creaMonticuloVacio();
			for (int i = 0; i < 200000; ++i) {
				int aleatorio = rand() % 300000;
				if (!fib.estaElemento(aleatorio)) {
					v.push_back(aleatorio);
					fib.insertar(aleatorio, 1);
				}
			}
			while (!v.empty()) {
				fib.eliminar(v.back());
				v.pop_back();
			}
			Assert::AreEqual(true, fib.emptyRaiz(1));
		}
		TEST_METHOD(Union) {
			familiaFib <int, greater<int>> fibParesImpares;
			familiaFib <int, greater<int>> fibTodos;
			//Creamos dos monticulos en la primera familia, el 1 para pares y el 2 para impares
			fibParesImpares.creaMonticuloVacio();
			fibParesImpares.creaMonticuloVacio();
			// Creamos uno en el otro en que llevaremos la union de elementos de los anteriores
			fibTodos.creaMonticuloVacio();
			for (int i = 0; i < 200000; ++i) {
				int aleatorio = rand() % 300000;
				if (aleatorio % 2 == 0) fibParesImpares.insertar(aleatorio, 1);
				else fibParesImpares.insertar(aleatorio, 2);
				fibTodos.insertar(aleatorio, 1);
			}
			//Uno pares e impares sobre 1
			fibParesImpares.unir(1, 2);

			// Compruebo que son iguales a la union que deberia tener exrayendo minimos de la union y del fibTodos
			// y comparandolos
			while (fibTodos.size() > 0) {
				Assert::AreEqual(fibTodos.minimo(1), fibParesImpares.minimo(1));
				fibTodos.quitarMinimo(1);
				fibParesImpares.quitarMinimo(1);
			}
		}

		
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