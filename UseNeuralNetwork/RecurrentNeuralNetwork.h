// RecurrentNeuralNetwork.h
#pragma once

#include "Matrixes3.h"
using namespace Matrixes3;

namespace NeuralNetwork 
{
	// ***** СТРУКТУРА РЕКУРЕНТНОЙ НЕЙРОННОЙ СЕТИ ПО УМОЛЧАНИЮ: *****

	const int	N = 6,					// Размерность входного вектора, т.е. число нейронов входного слоя или число входов ПО УМОЛЧАНИЮ.
				M = 10;					// Число нейронов скрытого слоя ПО УМОЛЧАНИЮ.

	double tanh( double );

	double ConvertData( double d );		// Функция преобразования данных.
	double RecoveryData( double d );	// Функция восстановления данных.

	class RecurrentNeuralNetwork
	{
	private:
		Matrix	*w,								// Матрица весовых коэффициентов связывающих нейроны входного и промежуточного слоёв.
				*delta_w_prev,					// Матрица изменений весовых коэффициентов связывающих n нейронов входного и m нейронов скрытого слоёв на предыдущем шаге обучения.
				*wc,							// Матрица весовых коэффициентов связывающих контекстные нейроны с нейронами промежуточного слоёв.
				*delta_wc_prev;					// Матрица изменений весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя на предыдущем шаге обучения.
		Vector	*T,								// Вектор-столбец порогов активации нейронов скрытого слоя.
				*delta_T_prev,					// Вектор изменений порогов активации нейронов скрытого слоя на предыдущем шаге обучения.
				*p,								// Вектор текущих выходных значений нейронов скрытого слоя.
				*p_prev,						// Вектор выходных значений нейронов скрытого слоя, хранящий эти значения на предыдущем шаге.
				*v,								// Вектор синаптических связей от нейронов скрытого слоя к выходному нейрону.
				*delta_v_prev;					// Вектор изменений синаптических связей от нейронов скрытого слоя к выходному нейрону на предыдущем шаге обучения.
		int		n,								// Число входов нейронной сети.
				m,								// Число нейронов скрытого слоя.
				numOfTrainingDatum,				// Число тренировочных данных.
				numOfTrainingExamples,			// Число тренировочных примеров.
				numOfTestDatum,					// Число тестовых данных.
				numOfTestExamples,				// Число тестовых примеров.
				numOfCompletedTrainingCycles;	// Количество завершенных тренировочных циклов обучения.
		double	a,								// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
				mu,								// Момент (инерция) обучения нейронной сети ( 0 <= mu <= 1.0 ). Его введение позволяет выходить из небольших локальных минимумов.
				sumOfErrorsSquares,				// Сумма квадратов ошибок на тренировочных данных.
				TT,								// Порог активации выходного нейрона.
				delta_TT_prev,					// Изменение порога активации выходного нейрона на предыдущем шаге обучения.
				minValue,						// Минимальное значение из тренировочного ряда (набора данных).
				maxValue;						// Максимальное значение из тренировочного ряда (набора данных).
	public:
		double	*trainingDatum,					// Тренировочные данные.
				*testDatum;						// Тестовые данные.
	private:
		char	*filenameOfTrainingData,		// Текстовый файл хранящий тренировочные данные.
				*filenameOfTestData;			// Текстовый файл хранящий тестовые данные.

	public:
		RecurrentNeuralNetwork(); // Конструктор по умолчанию.
		RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int n, int m );	// Инициализация нейронной сети,
		// где a - это скорость обучения, mu - момент инерции обучения, n - число нейронов входного слоя, m - число нейронов скрытого слоя.
		RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu );
		RecurrentNeuralNetwork( const RecurrentNeuralNetwork& srcRnn );	// Конструктор копирования, полностью копирующий состояние исходной рекурентной нейронной сети, являющейся аргументом.
		~RecurrentNeuralNetwork();										// Деструктор класса.
		void CopyState( const RecurrentNeuralNetwork& srcRnn );			// Функция копирования, полностью копирующая только состояние исходной рекурентной нейронной сети, но не её данные (обучающие и тестовые), которые должны быть одинаковыми для обеих сетей.
		int NumOfTrainingDatum() { return numOfTrainingDatum; };		// Возвращает количество данных в тренировочном наборе.
		int NumOfTrainingExamples() { return numOfTrainingExamples; };	// Возвращает количество примеров содержащихся в тренировочном наборе.
		int NumOfTestDatum() { return numOfTestDatum; };				// Возвращает количество данных в тестовом наборе.
		int NumOfTestExamples() { return numOfTestExamples; };			// Возвращает количество примеров содержащихся в тестовом наборе.
		int NumOfCompletedTrainingCycles() { return numOfCompletedTrainingCycles; };	// Возвращает количество завершенных тренировочных циклов обучения.
		double SumOfErrorsSquares() { return sumOfErrorsSquares; };		// Возвращает сумму квадратов ошибок на тренировочных данных.
		double ErrorSquare( Vector x, double e ); // Функция реализующая 1 шаг алгоритма обратного распространения ошибки и возвращающая ошибку 
		// для пары - текущего вектора входных значений и эталонного значения нейронной сети соответствующего входному вектору.
		void Train( double maxSumOfErrorsSquares, double& sumOfErrorsSquares, int maxNumOfTrainingCycles, int& numOfTrainingCycles ); // Эта функция выполняет обучение нейронной сети.
		void Train( int numOfTrainingCycles ); // Эта функция производит обучение нейронной сети, выполняя ровно numOfTrainingCycles циклов обучения.
		void Train( double maxSumOfErrorsSquares ); // Эта функция выполняет обучение нейронной сети до тех пор пока sumOfErrorsSquares на тренировочных данных больше, чем аргумент функции maxSumOfErrorsSquares.
		double y( Vector x );		// Выходное значение нейронной сети. Эта функция применяется для уже обученной нейронной сети.
	};

}