#include "stdafx.h"
#include "CppUnitTest.h"
#include <climits>
#include "../Trabajo de Marp/MonticuloFibonacci.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestMonticulosFibonacci
{		
	TEST_CLASS(TestConjunto)
	{
	public:
		TEST_METHOD(InsercionYMinimoYBorrado)
		{
			// Comprobamos que efectivamente inserta actualizando el minimo, lo saca correctamente y o elimina
			mFib<int> fib;
			for (int i = 20000; i >= 0; --i) fib.inserta(i);

			for (int j = 0; j <= 20000; ++j) {
				Assert::AreEqual(j, fib.minimo());
				fib.quitarMinimo();
			}
		}
		

	};

	TEST_CLASS(TestDeErrores)
	{
	public:
		TEST_METHOD(MinimoInexistente)
		{
			mFib<int> fib;
			try {
				fib.minimo();
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}


	};

}