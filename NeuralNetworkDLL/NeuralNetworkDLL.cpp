// NeuralNetworkDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
//#include <math.h>
//#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Matrixes3.h"
#include "RecurrentNeuralNetwork.h"

using namespace std;
using namespace NeuralNetwork;

typedef double* PtrDouble;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

extern "C"
{
	__declspec(dllexport) void __stdcall Training( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
	{
		// ОБЯЗАТЕЛЬНОЕ ПРЕДУСЛОВИЕ: во внешней программе, вызывающей функцию Training(...), по адресу res должно быть выделено место на куче для размещения функцией Training(...) ->
		// ( numOfTrainingCycles + 1 ) * numOfSimulatedDatum чисел double.
		// filenameOfTrainingData	- имя файла, содержащего тренировочные данные;
		// filenameOfTestData		- имя файла c тестовыми данными, которые должны быть согласованы с тренировочными данными;	
		// numOfTrainingCycles		- число циклов обучения;
		// numOfSimulatedDatum		- длина числового ряда, моделируемого нейронной сетью (ИНС);
		// res						- результирующий массив чисел double содержащий последовательные приближения numOfSimulatedDatum значений, моделируемых ИНС, получаемые после прохождения каждого цикла обучения ИНС.

		int n = N, k, i, j, p, numOfTrainingExamples;
		double e, x_simulated, sumOfErrorsSquares, *trainingDatum, *testDatum;
		Vector	x0_training( n ),	// - самый первый вектор входных данных из тренировочного набора данных;
				x( n ),				// - текущий вектор входных данных из тренировочного набора данных;
				x0_test( n ),		// - самый первый вектор входных данных из тестового набора данных;
				x2( n );			// - текущий вектор входных данных для моделирования ИНС данных;
		
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu ), rnn_copy( rnn );	// Инициализация рекуррентной нейронной сети.
		numOfTrainingExamples = rnn.NumOfTrainingExamples();

		trainingDatum = rnn.trainingDatum;	// Указатель на самое первое значение в ряду тренировочных данных.
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] ); // Самый первый вектор входных данных из тренировочного набора данных;

		testDatum = rnn.testDatum;	// Указатель на самое первое значение в ряду тестовых данных.
		for( j = 0; j < n; j++ ) x0_test[j] = testDatum[j]; // Самый первый вектор входных значений из тестового набора данных.

		for( p = 0; p < numOfTrainingCycles; p++ )
		{
			//sumOfErrorsSquares = 0.0;

			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );

				//sumOfErrorsSquares += rnn.ErrorSquare( x, e ); // Выполняем 1 шаг алгоритма обратного распространения ошибки.
				rnn.ErrorSquare( x, e ); // Выполняем 1 шаг алгоритма обратного распространения ошибки. Пара вектор x и значение e составляют один пример для обучения ИНС.

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				//x[n - 1] = ConvertData( trainingDatum[k + n - 1] );
				x[n - 1] = e;
			}

			// Заполняем очередной слой numOfSimulatedDatum значений, моделируемых ИНС, т.е. заполняем импортированный массив res:
			rnn_copy.CopyState( rnn ); // Копируем состояние обучаемой ИНС для избежания проявления паразитного эффекта гистерезиса рекуррентной нейро сети при последующей генерации данных.
			x2 = x0_test;	// Это самый первый вектор входных значений из тестового набора данных, с которого начинается моделирование данных ИНС.
			for( i = 0; i < numOfSimulatedDatum; i++ )
			{
				//x_simulated = rnn.y( x2 );	// Этот ошибочный оператор должен быть закоментирован, а следующий правильный оператор разкоментирован!
				x_simulated = rnn_copy.y( x2 );	// Наличие необходимости применять копию обучаемой ИНС для моделирования данных является не оспоримым доказательством наличия эффекта гистерезиса у рекуррентной ИНС.
				res[p * numOfSimulatedDatum + i] = x_simulated;	// Проявление паразитного эффекта гистерезиса у рекуррентной ИНС при моделировании данных особенно ярко проявляется в функции Training_2(...).
				for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
				x2[n - 1] = x_simulated;
			}
		}
		// По выходу из этого двойного вложенного цикла по p, k и i оказываются проинициализированы numOfTrainingCycles * numOfSimulatedDatum элементов массива res и нам остаётся проинициализировать только,
		// оставшиеся непроинициализированными, numOfSimulatedDatum элементов массива res, соответствующие необходимому кол-ву смоделированных ИНС данных - numOfSimulatedDatum.

		// Заполняем последний слой numOfSimulatedDatum значений, моделируемых ИНС, т.е. завершаем заполнять импортированный массив res:
		x2 = x0_test;		// Это самый первый вектор входных значений из тестового набора данных с которого начинается моделирование данных ИНС.
		for( i = 0; i < numOfSimulatedDatum; i++ ) 
		{
			x_simulated = rnn.y( x2 );
			res[numOfTrainingCycles * numOfSimulatedDatum + i] = x_simulated;
			for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
			x2[n - 1] = x_simulated;
		}

	}// End Training( ... )

	__declspec(dllexport) void __stdcall Training_2( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
	{
		// ОБЯЗАТЕЛЬНОЕ ПРЕДУСЛОВИЕ: во внешней программе, вызывающей функцию Training_2(...), по адресу res должно быть выделено место на куче для размещения функцией Training_2(...) ->
		// ( numOfTrainingCycles * numOfTrainingExamples + 1 ) * numOfSimulatedDatum чисел double, где переменная numOfTrainingExamples инициализируется после инициализации ИНС.
		// filenameOfTrainingData	- имя файла, содержащего тренировочные данные;
		// filenameOfTestData		- имя файла c тестовыми данными, которые должны быть согласованы с тренировочными данными;	
		// numOfTrainingCycles		- число циклов обучения;
		// numOfSimulatedDatum		- длина числового ряда, моделируемого нейронной сетью (ИНС);
		// res						- результирующий массив чисел double содержащий последовательные приближения numOfSimulatedDatum значений, моделируемых ИНС, получаемые после выполнения каждого шага алгоритма 
		// обратного распространения ошибки;

		int n = N, // Число нейронов входного слоя.
			k, i, j, p, numOfTrainingExamples;
		double e, x_simulated, sumOfErrorsSquares, *trainingDatum, *testDatum;
		Vector	x0_training( n ),	// - самый первый вектор входных данных из тренировочного набора данных;
				x( n ),				// - текущий вектор входных данных из тренировочного набора данных;
				x0_test( n ),		// - самый первый вектор входных данных из тестового набора данных;
				x2( n );			// - текущий вектор входных данных для моделирования ИНС данных;
		
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu ), rnn_copy( rnn );	// Инициализация рекуррентной нейронной сети.
		numOfTrainingExamples = rnn.NumOfTrainingExamples();

		trainingDatum = rnn.trainingDatum;	// Указатель на самое первое значение в ряду тренировочных данных.
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] ); // Самый первый вектор входных данных из тренировочного набора данных;

		testDatum = rnn.testDatum;	// Указатель на самое первое значение в ряду тестовых данных.
		for( j = 0; j < n; j++ ) x0_test[j] = testDatum[j]; // Самый первый вектор входных значений из тестового набора данных.

		for( p = 0; p < numOfTrainingCycles; p++ )
		{
			//sumOfErrorsSquares = 0.0;

			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );
			
				//sumOfErrorsSquares += rnn.ErrorSquare( x, e ); // Выполняем 1 шаг алгоритма обратного распространения ошибки.
				rnn.ErrorSquare( x, e ); // Выполняем 1 шаг алгоритма обратного распространения ошибки. Пара вектор x и значение e составляют один пример для обучения ИНС.

				// Заполняем очередной слой numOfSimulatedDatum значений, моделируемых ИНС, т.е. заполняем импортированный массив res:
				rnn_copy.CopyState( rnn );	// Копируем состояние обучаемой ИНС для избежания проявления паразитного эффекта гистерезиса рекуррентной нейро сети при последующей генерации данных.
				x2 = x0_test;					// Это самый первый вектор входных значений из тестового набора данных, с которого начинается моделирование данных ИНС.
				for( i = 0; i < numOfSimulatedDatum; i++ ) 
				{
					//x_simulated = rnn.y( x2 );	// Этот ошибочный оператор должен быть закоментирован, а следующий правильный оператор разкоментирован!
					x_simulated = rnn_copy.y( x2 );	// Наличие необходимости применять копию обучаемой ИНС для моделирования данных является не оспоримым доказательством наличия эффекта гистерезиса у рекуррентной ИНС.
					res[(p * numOfTrainingExamples + k) * numOfSimulatedDatum + i] = x_simulated;	// Проявление паразитного эффекта гистерезиса у рекуррентной ИНС при моделировании данных особенно ярко проявляется в функции Training_2(...).
					for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
					x2[n - 1] = x_simulated;
				}

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				//x[n - 1] = ConvertData( trainingDatum[k + n - 1] );
				x[n - 1] = e;
			}
		}
		// По выходу из этого тройного вложенного цикла по p, k, i оказываются проинициализированы numOfTrainingCycles * numOfTrainingExamples * numOfSimulatedDatum элементов массива res и нам остаётся 
		// проинициализировать только оставшиеся непроинициализированными numOfSimulatedDatum элементов массива res, соответствующие необходимому кол-ву смоделированных ИНС данных - numOfSimulatedDatum.

		// Заполняем последний слой numOfSimulatedDatum значений, моделируемых ИНС, т.е. завершаем заполнять импортированный массив res:
		p = numOfTrainingCycles - 1;	// Необходимое значение переменной p в этом месте программы ВЫВЕРЕНО в выкладках.
		k = numOfTrainingExamples;		// Необходимое значение переменной k в этом месте программы ВЫВЕРЕНО в выкладках.
		x2 = x0_test;					// Это самый первый вектор входных значений из тестового набора данных с которого начинается моделирование данных ИНС.
		for( i = 0; i < numOfSimulatedDatum; i++ ) 
		{
			x_simulated = rnn.y( x2 );
			res[(p * numOfTrainingExamples + k) * numOfSimulatedDatum + i] = x_simulated;
			for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
			x2[n - 1] = x_simulated;
		}

	}// End Training_2( ... )

	__declspec(dllexport) void __stdcall Apply( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, double maxSumOfErrorsSquares, double& sumOfErrorsSquares, 
		int maxNumOfTrainingCycles, int& numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
		// ОБЯЗАТЕЛЬНОЕ ПРЕДУСЛОВИЕ: во внешней программе, вызывающей функцию Apply(...), по адресу res должно быть выделено место на куче для размещения функцией Apply(...) numOfSimulatedDatum чисел double,
		// моделируемых ИНС.
	{
		int n = N, j, k;		// N - число входов нейронной сети.
		Vector x( n );			// Вектор входных данных для нейронной сети.
		double y;
		string tempStr, str;
		ifstream testStream;
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu );						// Инициализация рекуррентной нейронной сети.
		rnn.Train( maxSumOfErrorsSquares, sumOfErrorsSquares, maxNumOfTrainingCycles, numOfTrainingCycles );	// Обучение нейронной сети.

		// Инициализация вектора x первыми n значениями из тестового набора данных:
		testStream.open( filenameOfTestData );
		for( j = 0; j < n; j++ )
		{
			getline( testStream, tempStr );
			istringstream str( tempStr );
			str >> x[j];
		}
		testStream.close();

		// Заполняем импортированный массив res моделируемыми ИНС данными:
		for( k = 0; k < numOfSimulatedDatum; k++ )
		{
			y = rnn.y( x );
			res[k] = y;
			for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
			x[n - 1] = y;
		}
	}

	__declspec(dllexport) void __stdcall Apply_2( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
		// ОБЯЗАТЕЛЬНОЕ ПРЕДУСЛОВИЕ: во внешней программе, вызывающей функцию Apply(...), по адресу res должно быть выделено место на куче для размещения функцией Apply_2(...) numOfSimulatedDatum чисел double,
		// моделируемых ИНС.
	{
		int n = N, j, k;		// N - число входов нейронной сети.
		Vector x( n );			// Вектор входных данных для нейронной сети.
		double y;
		string tempStr, str;
		ifstream testStream;
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu );						// Инициализация рекуррентной нейронной сети.
		rnn.Train( numOfTrainingCycles );																		// Обучение нейронной сети.

		// Инициализация вектора x первыми n значениями из тестового набора данных:
		testStream.open( filenameOfTestData );
		for( j = 0; j < n; j++ )
		{
			getline( testStream, tempStr );
			istringstream str( tempStr );
			str >> x[j];
		}
		testStream.close();

		// Заполняем импортированный массив res моделируемыми ИНС данными:
		for( k = 0; k < numOfSimulatedDatum; k++ )
		{
			y = rnn.y( x );
			res[k] = y;
			for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
			x[n - 1] = y;
		}
	}

	__declspec(dllexport) void __stdcall Apply_3( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, double maxSumOfErrorsSquares, int numOfSimulatedDatum, PtrDouble res )
		// ОБЯЗАТЕЛЬНОЕ ПРЕДУСЛОВИЕ: во внешней программе, вызывающей функцию Apply(...), по адресу res должно быть выделено место на куче для размещения функцией Apply_2(...) numOfSimulatedDatum чисел double,
		// моделируемых ИНС.
	{
		int n = N, j, k;		// N - число входов нейронной сети.
		Vector x( n );			// Вектор входных данных для нейронной сети.
		double y;
		string tempStr, str;
		ifstream testStream;
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu );						// Инициализация рекуррентной нейронной сети.
		rnn.Train( maxSumOfErrorsSquares );																		// Обучение нейронной сети.

		// Инициализация вектора x первыми n значениями из тестового набора данных:
		testStream.open( filenameOfTestData );
		for( j = 0; j < n; j++ )
		{
			getline( testStream, tempStr );
			istringstream str( tempStr );
			str >> x[j];
		}
		testStream.close();

		// Заполняем импортированный массив res моделируемыми ИНС данными:
		for( k = 0; k < numOfSimulatedDatum; k++ )
		{
			y = rnn.y( x );
			res[k] = y;
			for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
			x[n - 1] = y;
		}
	}

}// End extern "C"