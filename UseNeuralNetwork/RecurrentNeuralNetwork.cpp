// Построение рекуррентной нейронной сети Элмана, имеющей 1 нейрон с линейной функцией активации в выходном слое.
// Применяется для решения задач прогнозирования временных рядов.

#define LINEAR_1	0
#define LINEAR_2	1
#define SIGMOID		2
#define TANH		3

// Выбор вида функции активации нейронов скрытого слоя:
//#define ACTIVATION_FUNCTION SIGMOID		// Сигмоидная функция в качестве функции активации нейронов скрытого слоя.
#define ACTIVATION_FUNCTION TANH			// Гиперболический тангенс в качестве функции активации нейронов скрытого слоя.

// Выбор вида функции преобразования данных для максимизации информации приносимой каждым примером:
//#define CONVERSION_FUNCTION LINEAR_1		// Линейная функция в качестве функции преобразования данных на отрезок [0, 1].
#define CONVERSION_FUNCTION LINEAR_2		// Линейная функция в качестве функции преобразования данных на отрезок [-1, 1].
//#define CONVERSION_FUNCTION SIGMOID		// Сигмоидная функция в качестве функции преобразования данных.
//#define CONVERSION_FUNCTION TANH			// Гиперболический тангенс в качестве функции преобразования данных.

/*
Сигмоидная функция в качестве функции активации нейронов скрытого слоя согласуется с линейной функцией преобразования данных 
на отрезок [0, 1].
Гиперболический тангенс в качестве функции активации нейронов скрытого слоя согласуется с линейной функцией преобразования данных 
на отрезок [-1, 1].
Поэтому константа SIGMOID согласуется с константой LINEAR_1, а константа TANH согласуется с константой LINEAR_2.
*/

#if( !defined( ACTIVATION_FUNCTION ) ) 
	#error Должен быть определен макрос ACTIVATION_FUNCTION !
#endif
#if( !defined( CONVERSION_FUNCTION ) ) 
	#error Должен быть определен макрос CONVERSION_FUNCTION !
#endif

#include <math.h>							// для функции pow()
//#include <cmath>							// для функции tanh()
#include <string>
#include <sstream>
#include <queue>							// для использования очередей
#include "Matrixes3.h"
#include "RecurrentNeuralNetwork.h"
using namespace std;
using namespace Matrixes3;

static double minValue, maxValue;

// ***** ПАРАМЕТРЫ РЕКУРРЕНТНОЙ НЕЙРОННОЙ СЕТИ: *****

double	l = 1.0;				// Коэффициент характеризующий ширину функции активации нейронов скрытого слоя.
		//a = 0.01,				// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
		//a = 0.025,			// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
		//a = 0.029,			// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
		//a = 0.030,			// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
		//mu	= 0;			// Момент (инерция) обучения нейронной сети ( 0 <= mu <= 1.0 ). Его введение позволяет выходить из небольших локальных минимумов.
		//mu	= 0.125;		// Момент (инерция) обучения нейронной сети ( 0 <= mu <= 1.0 ). Его введение позволяет выходить из небольших локальных минимумов.
		//mu	= 0.275;		// Момент (инерция) обучения нейронной сети ( 0 <= mu <= 1.0 ). Его введение позволяет выходить из небольших локальных минимумов.

double F( double S );				// Функция активации нейронов скрытого слоя.
double Derivative_F( double S );	// Производная функции активации нейронов скрытого слоя.

double ConvertData( double d );		// Функция преобразования данных для максимизации информации приносимой каждым примером.
double RecoveryData( double d );	// Функция восстановления данных является обратной по отношению к функции преобразования данных.

namespace NeuralNetwork
{

	RecurrentNeuralNetwork::RecurrentNeuralNetwork()
	{	// Конструктор по умолчанию.
		filenameOfTrainingData = nullptr;
		filenameOfTestData = nullptr;
		w = nullptr;
		delta_w_prev = nullptr;
		wc = nullptr;
		delta_wc_prev = nullptr;
		T = nullptr;
		delta_T_prev = nullptr;
		p = nullptr;
		p_prev = nullptr;
		v = nullptr;
		delta_v_prev = nullptr;
		trainingDatum = nullptr;
		testDatum = nullptr;
	}

	RecurrentNeuralNetwork::RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int n, int m )
	{	// Начальная инициализация нейронной сети:
		int i, j;
		double datum;
		queue<double> queueOfTrainingData, queueOfTestData;
		ifstream trainingStream, testStream;
		string tempStr, str;

		this->filenameOfTrainingData = filenameOfTrainingData;	// Файл хранящий тренировочные данные для обучения в текстовом формате.
		this->filenameOfTestData = filenameOfTestData;			// Файл хранящий тестовые данные для обучения в текстовом формате.
		this->n = n;						// Число нейронов входного слоя.
		this->m = m;						// Число нейронов скрытого слоя.
		this->a = a;						// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
		this->mu = mu;						// Момент (инерция) обучения нейронной сети ( 0 <= mu <= 1.0 ). Его введение позволяет выходить из небольших локальных минимумов.
		this->numOfCompletedTrainingCycles = 0;					// Количество завершенных тренировочных циклов обучения.
		this->sumOfErrorsSquares = -1;							// Сумма квадратов ошибок на тренировочном наборе данных.
		w = new Matrix( n, m );				// Матрица весовых коэффициентов связывающих нейроны входного и скрытого слоёв.
		delta_w_prev = new Matrix( n, m );	// Матрица изменений весовых коэффициентов связывающих n нейронов входного и m нейронов скрытого слоёв на предыдущем шаге обучения инициализируется нулевой матрицей.
		wc = new Matrix( m, m );			// Матрица весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя. Число контекстных нейронов совпадает с числом нейронов промежуточного (скрытого) слоя.
		delta_wc_prev = new Matrix( m, m );	// Матрица изменений весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя на предыдущем шаге обучения инициализируется нулевой матрицей.
		T = new Vector( m );				// Вектор-столбец порогов активации нейронов скрытого слоя инициализируем нуль-вектором.
		delta_T_prev = new Vector( m );		// Вектор изменений порогов активации нейронов скрытого слоя на предыдущем шаге обучения инициализируем нуль-вектором.
		p = new Vector( m );				// Вектор текущих выходных значений нейронов скрытого слоя инициализируем нулевым вектором.
		p_prev = new Vector( m );			// Вектор выходных значений нейронов скрытого слоя, хранящий эти значения на предыдущем шаге, инициализируем нуль-вектором.
		v = new Vector( m );				// Вектор синаптических связей от нейронов скрытого слоя к выходному нейрону.
		delta_v_prev = new Vector( m );		// Вектор изменений синаптических связей от нейронов скрытого слоя к выходному нейрону на предыдущем шаге обучения инициализируем нуль-вектором.
		TT = 0.0;							// Порог активации выходного нейрона инициализируем нулевым значением.
		delta_TT_prev = 0.0;				// Изменение порога активации выходного нейрона на предыдущем шаге обучения инициализируем нулевым значением.
		minValue = numeric_limits<double>::max();
		maxValue = numeric_limits<double>::lowest();

		// Инициализация матрицы весовых коэффициентов между нейронами входного слоя и нейронами скрытого слоя случайными значениями в 
		// диапазоне (0, 1):
		for(i = 0; i < n; i++)
			for(j = 0; j < m; j++)
				(*w)(i, j) = (double)rand() / RAND_MAX;

		// Инициализация матрицы весовых коэффициентов между контекстными нейронами и нейронами скрытого слоя случайными значениями в 
		// диапазоне (0, 1), а также вектора весовых коэффициентов от нейронов скрытого слоя к выходному нейрону:
		for(i = 0; i < m; i++)
		{
			(*v)[i] = (double)rand() / RAND_MAX;
			for(j = 0; j < m; j++)
				(*wc)(i, j) = (double)rand() / RAND_MAX;
		}

		trainingStream.open( filenameOfTrainingData );
		numOfTrainingDatum = 0;
		if( !trainingStream.fail() )
		{
	        getline( trainingStream, tempStr );		// считывание 1-ой строки из файла данных.
			do{										// цикл подсчёта кол-ва значений.
				istringstream str( tempStr );
				str >> datum;
				queueOfTrainingData.push( datum );
				//trainingStream >> datum; // возникает проблема с символом новой строки в последней строке данных.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTrainingDatum++;
				getline( trainingStream, tempStr );	// считывание очередной строки из файла данных.
			} while( tempStr.size() || !trainingStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		trainingStream.close();
		numOfTrainingExamples = numOfTrainingDatum - n;
		trainingDatum = new double[numOfTrainingDatum];
		for( i = 0; i < numOfTrainingDatum; i++ )
		{
			trainingDatum[i] = queueOfTrainingData.front();
			queueOfTrainingData.pop();
		}
		/*
		i = 0;
		while( !queueOfTrainingData.empty() )
		{
			trainingDatum[i] = queueOfTrainingData.front();
			queueOfTrainingData.pop();
			i++;
		}
		*/

		testStream.open( filenameOfTestData );
		numOfTestDatum = 0;
		if( !testStream.fail() )
		{
	        getline( testStream, tempStr );			// считывание 1-ой строки из файла данных.
			do{										// цикл подсчёта кол-ва значений.
				istringstream str( tempStr );
				str >> datum;
				queueOfTestData.push( datum );
				//testStream >> datum; // возникает проблема с символом новой строки в последней строке данных.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTestDatum++;
				getline( testStream, tempStr );	// считывание очередной строки из файла данных.
			} while( tempStr.size() || !testStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		testStream.close();
		numOfTestExamples = numOfTestDatum - n;
		testDatum = new double[numOfTestDatum];
		for( i = 0; i < numOfTestDatum; i++ )
		{
			testDatum[i] = queueOfTestData.front();
			queueOfTestData.pop();
		}

		// Следующие 7 строк кода для отладки:
		/*
		cout << "w  =  " << endl; w->Display( cout, 5 ); cout << endl;
		cout << "T  =  "; T->Display( cout, 5 ); cout << endl << endl;
		cout << "wc  =  " << endl; wc->Display( cout, 5 ); cout << endl;
		cout << "p  =  "; p->Display( cout, 5 ); cout << endl << endl;
		cout << "p_prev  =  "; p_prev->Display( cout, 5 ); cout << endl << endl;
		cout << "v  =  "; v->Display( cout, 5 ); cout << endl << endl;
		cout << "TT  =  "; cout << TT << endl;
		*/
	}

	RecurrentNeuralNetwork::RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu ):
		filenameOfTrainingData( filenameOfTrainingData ), filenameOfTestData( filenameOfTestData ), a( a ), mu( mu ), n( N ), m( M ), numOfCompletedTrainingCycles( 0 ), sumOfErrorsSquares( -1 )
	{	// Начальная инициализация нейронной сети (значения для n и m выбраны по умолчанию - n = 6, m = 10):
		int i, j;
		double datum;
		queue<double> queueOfTrainingData, queueOfTestData;
		ifstream trainingStream, testStream;
		string tempStr, str;

		/*
		cout << endl;
		cout << "this->filenameOfTrainingData  =  " << this->filenameOfTrainingData << endl
			<< "this->filenameOfTestData  =  " << this->filenameOfTestData << endl
			<< "this->a  =  " << this->a << endl
			<< "this->mu  =  " << this->mu << endl
			<< "this->maxNumOfTrainingCycles  =  " << this->maxNumOfTrainingCycles << endl
			<< "this->maxSumOfErrorsSquares  =  " << this->maxSumOfErrorsSquares << endl
			<< "this->n  =  " << this->n << endl
			<< "this->m  =  " << this->m << endl;
		*/

		w = new Matrix( n, m );				// Матрица весовых коэффициентов связывающих нейроны входного и скрытого слоёв.
		delta_w_prev = new Matrix( n, m );	// Матрица изменений весовых коэффициентов связывающих n нейронов входного и m нейронов скрытого слоёв на предыдущем шаге обучения инициализируется нулевой матрицей.
		wc = new Matrix( m, m );			// Матрица весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя. Число контекстных нейронов совпадает с числом нейронов промежуточного (скрытого) слоя.
		delta_wc_prev = new Matrix( m, m );	// Матрица изменений весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя на предыдущем шаге обучения инициализируется нулевой матрицей.
		T = new Vector( m );				// Вектор-столбец порогов активации нейронов скрытого слоя инициализируем нуль-вектором.
		delta_T_prev = new Vector( m );		// Вектор изменений порогов активации нейронов скрытого слоя на предыдущем шаге обучения инициализируем нуль-вектором.
		p = new Vector( m );				// Вектор текущих выходных значений нейронов скрытого слоя инициализируем нулевым вектором.
		p_prev = new Vector( m );			// Вектор выходных значений нейронов скрытого слоя, хранящий эти значения на предыдущем шаге, инициализируем нуль-вектором.
		v = new Vector( m );				// Вектор синаптических связей от нейронов скрытого слоя к выходному нейрону.
		delta_v_prev = new Vector( m );		// Вектор изменений синаптических связей от нейронов скрытого слоя к выходному нейрону на предыдущем шаге обучения инициализируем нуль-вектором.
		TT = 0.0;							// Порог активации выходного нейрона инициализируем нулевым значением.
		delta_TT_prev = 0.0;				// Изменение порога активации выходного нейрона на предыдущем шаге обучения инициализируем нулевым значением.
		minValue = numeric_limits<double>::max();
		maxValue = numeric_limits<double>::lowest();

		// Инициализация матрицы весовых коэффициентов между нейронами входного слоя и нейронами скрытого слоя случайными значениями в 
		// диапазоне (0, 1):
		for(i = 0; i < n; i++)
			for(j = 0; j < m; j++)
				(*w)(i, j) = (double)rand() / RAND_MAX;

		// Инициализация матрицы весовых коэффициентов между контекстными нейронами и нейронами скрытого слоя случайными значениями в 
		// диапазоне (0, 1), а также вектора весовых коэффициентов от нейронов скрытого слоя к выходному нейрону:
		for(i = 0; i < m; i++)
		{
			(*v)[i] = (double)rand() / RAND_MAX;
			for(j = 0; j < m; j++)
				(*wc)(i, j) = (double)rand() / RAND_MAX;
		}

		trainingStream.open( filenameOfTrainingData );
		numOfTrainingDatum = 0;
		if( !trainingStream.fail() )
		{
	        getline( trainingStream, tempStr );		// считывание 1-ой строки из файла данных.
			do{										// цикл подсчёта кол-ва значений.
				istringstream str( tempStr );
				str >> datum;
				queueOfTrainingData.push( datum );
				//trainingStream >> datum; // возникает проблема с символом новой строки в последней строке данных.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTrainingDatum++;
				getline( trainingStream, tempStr );	// считывание очередной строки из файла данных.
			} while( tempStr.size() || !trainingStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		trainingStream.close();
		numOfTrainingExamples = numOfTrainingDatum - n;
		trainingDatum = new double[numOfTrainingDatum];
		for( i = 0; i < numOfTrainingDatum; i++ )
		{
			trainingDatum[i] = queueOfTrainingData.front();
			queueOfTrainingData.pop();
		}

		testStream.open( filenameOfTestData );
		numOfTestDatum = 0;
		if( !testStream.fail() )
		{
	        getline( testStream, tempStr );			// считывание 1-ой строки из файла данных.
			do{										// цикл подсчёта кол-ва значений.
				istringstream str( tempStr );
				str >> datum;
				queueOfTestData.push( datum );
				//testStream >> datum; // возникает проблема с символом новой строки в последней строке данных.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTestDatum++;
				getline( testStream, tempStr );	// считывание очередной строки из файла данных.
			} while( tempStr.size() || !testStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		testStream.close();
		numOfTestExamples = numOfTestDatum - n;
		testDatum = new double[numOfTestDatum];
		for( i = 0; i < numOfTestDatum; i++ )
		{
			testDatum[i] = queueOfTestData.front();
			queueOfTestData.pop();
		}

		// Следующие 7 строк кода для отладки:
		/*
		cout << "w  =  " << endl; w->Display( cout, 5 ); cout << endl;
		cout << "T  =  "; T->Display( cout, 5 ); cout << endl << endl;
		cout << "wc  =  " << endl; wc->Display( cout, 5 ); cout << endl;
		cout << "p  =  "; p->Display( cout, 5 ); cout << endl << endl;
		cout << "p_prev  =  "; p_prev->Display( cout, 5 ); cout << endl << endl;
		cout << "v  =  "; v->Display( cout, 5 ); cout << endl << endl;
		cout << "TT  =  "; cout << TT << endl;
		*/
	}

	// Конструктор копирования, полностью копирующий всю нейронную сеть вместе с её данными (обучающими и тестовыми) и текущим состоянием.
	RecurrentNeuralNetwork::RecurrentNeuralNetwork( const RecurrentNeuralNetwork& srcRnn )
	{	
		w = new Matrix( *srcRnn.w );
		delta_w_prev = new Matrix( *srcRnn.delta_w_prev );
		wc = new Matrix( *srcRnn.wc );
		delta_wc_prev = new Matrix( *srcRnn.delta_wc_prev );
		T = new Vector( *srcRnn.T );
		delta_T_prev = new Vector( *srcRnn.delta_T_prev );
		p = new Vector( *srcRnn.p );
		p_prev = new Vector( *srcRnn.p_prev );
		v = new Vector( *srcRnn.v );
		delta_v_prev = new Vector( *srcRnn.delta_v_prev );
		n = srcRnn.n;
		m = srcRnn.m;
		numOfTrainingDatum = srcRnn.numOfTrainingDatum;
		numOfTrainingExamples = srcRnn.numOfTrainingExamples;
		numOfTestDatum = srcRnn.numOfTestDatum;
		numOfTestExamples = srcRnn.numOfTestExamples;
		numOfCompletedTrainingCycles = srcRnn.numOfCompletedTrainingCycles;
		a = srcRnn.a;
		mu = srcRnn.mu;
		sumOfErrorsSquares = srcRnn.sumOfErrorsSquares;
		trainingDatum = new double[numOfTrainingDatum];
		memcpy( this->trainingDatum, srcRnn.trainingDatum, numOfTrainingDatum * sizeof( double ) );
		testDatum = new double[numOfTestDatum];
		memcpy( this->testDatum, srcRnn.testDatum, numOfTestDatum * sizeof( double ) );
		//TT = (*srcRnn).TT;	// error
		//TT = *srcRnn.TT;		// error
		TT = srcRnn.TT;
		delta_TT_prev = srcRnn.delta_TT_prev;
		minValue = srcRnn.minValue;
		maxValue = srcRnn.maxValue;
		filenameOfTrainingData = srcRnn.filenameOfTrainingData;
		filenameOfTestData = srcRnn.filenameOfTestData;
	}

	RecurrentNeuralNetwork::~RecurrentNeuralNetwork()	// Деструктор класса.
	{
		delete w;
		delete delta_w_prev;
		delete wc;
		delete delta_wc_prev;
		delete T;
		delete delta_T_prev;
		delete p;
		delete p_prev;
		delete v;
		delete delta_v_prev;
		delete[] trainingDatum;
		delete[] testDatum;
	}

	// Функция копирования, полностью копирующая только состояние исходной рекурентной нейронной сети, являющейся аргументом функции, но не её данные (обучающие и тестовые), которые должны быть одинаковыми для обеих сетей.
	void RecurrentNeuralNetwork::CopyState( const RecurrentNeuralNetwork& srcRnn )
	{
		// Копируем состояние исходной рекурентной нейронной сети, являющейся аргументом функции:
		w = new Matrix( *srcRnn.w );
		delta_w_prev = new Matrix( *srcRnn.delta_w_prev );
		wc = new Matrix( *srcRnn.wc );
		delta_wc_prev = new Matrix( *srcRnn.delta_wc_prev );
		T = new Vector( *srcRnn.T );
		delta_T_prev = new Vector( *srcRnn.delta_T_prev );
		p = new Vector( *srcRnn.p );
		p_prev = new Vector( *srcRnn.p_prev );
		v = new Vector( *srcRnn.v );
		delta_v_prev = new Vector( *srcRnn.delta_v_prev );
		n = srcRnn.n;
		m = srcRnn.m;
		numOfTrainingDatum = srcRnn.numOfTrainingDatum;
		numOfTrainingExamples = srcRnn.numOfTrainingExamples;
		numOfTestDatum = srcRnn.numOfTestDatum;
		numOfTestExamples = srcRnn.numOfTestExamples;
		numOfCompletedTrainingCycles = srcRnn.numOfCompletedTrainingCycles;
		a = srcRnn.a;
		mu = srcRnn.mu;
		sumOfErrorsSquares = srcRnn.sumOfErrorsSquares;
		TT = srcRnn.TT;
		delta_TT_prev = srcRnn.delta_TT_prev;
		minValue = srcRnn.minValue;
		maxValue = srcRnn.maxValue;
		// Данные (обучающие и тестовые) остаются общими для обеих сетей:
		
	}

	// Следующая ф-я реализует 1 шаг алгоритма обратного распространения ошибки и возвращает квадрат ошибки для пары -
	// текущего вектора входных значений 'x' и эталонного значения нейронной сети 'e' соответствующего входному вектору.
	double RecurrentNeuralNetwork::ErrorSquare( Vector x, double e )
	{
		double delta_TT, y, res;
		Vector delta_v( m );		// Вектор изменений синаптических связей от нейронов скрытого слоя к выходному нейрону.
		Vector delta_T( m );		// Вектор изменений порогов активации нейронов скрытого слоя.
		Vector S( m );				// Взвешенные суммы нейронов скрытого слоя.
		Matrix delta_w( n, m );		// Матрица изменений весовых коэффициентов связывающих n нейронов входного и m нейронов скрытого слоёв.
		Matrix delta_wc( m, m );	// Матрица изменений весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя.
		int i, j, l;

		for(i = 0; i < m; i++) // Цикл пробегает по всем нейронам скрытого слоя.
		{
			for(j = 0; j < n; j++) // Цикл пробегает по всем нейронам входного слоя.
				S[i] += (*w)(j, i) * x[j];

			for(l = 0; l < m; l++) // Цикл пробегает по всем контекстным нейронам.
				S[i] += (*wc)(l, i) * (*p_prev)[l];

			S[i] -= (*T)[i];
			(*p)[i] = F( S[i] );
		}
		y = (*v) * (*p) - TT; // Текущее выходное значение нейронной сети.
	
		// Настройка параметров обучения рекурентной сети:
		delta_v = -a * ( y - e ) * (*p) + mu * (*delta_v_prev);
		delta_TT = a * ( y - e ) + mu * delta_TT_prev;
		TT += delta_TT;
		for(i = 0; i < m; i++) // Цикл пробегает по всем нейронам скрытого слоя.
		{
			for(j = 0; j < n; j++) // Цикл пробегает по всем нейронам входного слоя.
			{
				delta_w(j, i) = -a * ( y - e ) * (*v)[i] * Derivative_F( S[i] ) * x[j] + mu * (*delta_w_prev)(j, i);
				(*w)(j, i) += delta_w(j, i);
			}
	
			for(l = 0; l < m; l++) // Цикл пробегает по всем контекстным нейронам.
			{
				delta_wc(l, i) = -a * ( y - e ) * (*v)[i] * Derivative_F( S[i] ) * (*p_prev)[l] + mu * (*delta_wc_prev)(l, i);
				(*wc)(l, i) += delta_wc(l, i);
			}

			delta_T[i] = a * ( y - e ) * (*v)[i] * Derivative_F( S[i] ) + mu * (*delta_T_prev)[i];
			(*T)[i] += delta_T[i];

			(*v)[i] += delta_v[i];
		}

		(*p_prev) = (*p);			// Присваиваем вычисленные значения нейронов скрытого слоя на текущем шаге предыдущим значениям нейронов скрытого слоя.
		(*delta_v_prev) = delta_v;	// Присваиваем вектору изменений синаптических связей от нейронов скрытого слоя к выходному нейрону на предыдущем шаге обучения текущий вектор этих изменений.
		(*delta_T_prev) = delta_T;	// Присваиваем вектору изменений порогов активации нейронов скрытого слоя на предыдущем шаге обучения текущий вектор этих изменений.
		delta_TT_prev = delta_TT;	// Присваиваем значению изменения порога активации выходного нейрона на предыдущем шаге обучения текущее значение этого изменения.
		(*delta_w_prev) = delta_w;	// Присваиваем матрице изменений весовых коэффициентов связывающих n нейронов входного и m нейронов скрытого слоёв на предыдущем шаге обучения текущую матрицу этих изменений.
		(*delta_wc_prev) = delta_wc;// Присваиваем матрице изменений весовых коэффициентов связывающих m контекстных нейронов с m нейронами скрытого слоя на предыдущем шаге обучения текущую матрицу этих изменений.

		res = pow( ( y - e ), 2) / 2;
		return res;
	}

	// Следующая функция выполняет обучение нейронной сети.
	void RecurrentNeuralNetwork::Train( double maxSumOfErrorsSquares, double& sumOfErrorsSquares, int maxNumOfTrainingCycles, int& numOfTrainingCycles )
	{
		int k, j;
		double e;
		Vector x( n ), x0_training( n );
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] );	// x0_training - это самый первый вектор входных значений из обучающего набора данных.

		numOfTrainingCycles = 0;
		do{
			sumOfErrorsSquares = 0.0;

			/*
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				if( k == 0 ) // считываем первый пример.
				{
					for( j = 0; j < n; j++ )
					{
						x[j] = ConvertData( trainingDatum[j] );
					}
					e = ConvertData( trainingDatum[n] );
				}
				else	// if( k != 0 )		// считываем очередной пример.
				{
					for( j = 0; j < n - 1; j++ )
						x[j] = x[j + 1];
					x[n - 1] = e;
					e = ConvertData( trainingDatum[k + n] );
				}

				// Следующие 3 строки кода для отладки:
				//cout << k << ":  "; x.Display( cout, 2 );
				//cout << "  " << e << endl;
				//if( k == 10 ) break;

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.
				sumOfErrorsSquares += ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.

				// Следующая строка кода для отладки:
				//cout << k << ":  " << sumOfErrorsSquares << endl;
			}
			*/
			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.
				sumOfErrorsSquares += ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.

				// Следующая строка кода для отладки:
				//cout << k << ":  " << sumOfErrorsSquares << endl;

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				x[n - 1] = e;
			}

			// Следующая строка кода для отладки:
			//cout << numOfTrainingCycles << ":  " << sumOfErrorsSquares << endl;

			this->sumOfErrorsSquares = sumOfErrorsSquares;
			numOfTrainingCycles++;
			numOfCompletedTrainingCycles++;
		}while( ( sumOfErrorsSquares > maxSumOfErrorsSquares ) && ( numOfTrainingCycles < maxNumOfTrainingCycles ) );
	}

	// Эта функция производит обучение нейронной сети, выполняя ровно numOfTrainingCycles циклов обучения.
	void RecurrentNeuralNetwork::Train( int numOfTrainingCycles )
	{
		int k, j, numOfCycles;
		double e;
		Vector x( n ), x0_training( n );
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] );	// x0_training - это самый первый вектор входных значений из обучающего набора данных.

		numOfCycles = 0;
		do{
			sumOfErrorsSquares = 0.0;

			/*
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				if( k == 0 ) // считываем первый пример.
				{
					for( j = 0; j < n; j++ )
					{
						x[j] = ConvertData( trainingDatum[j] );
					}
					e = ConvertData( trainingDatum[n] );
				}
				else		// считываем очередной пример.
				{
					for( j = 0; j < n - 1; j++ )
						x[j] = x[j + 1];
					x[n - 1] = e;
					e = ConvertData( trainingDatum[k + n] );
				}

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.
				sumOfErrorsSquares += ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.

				// Следующая строка кода для отладки:
				//cout << k << ":  " << sumOfErrorsSquares << endl;
			}
			*/
			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );
				
				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.
				sumOfErrorsSquares += ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.

				// Следующая строка кода для отладки:
				//cout << k << ":  " << sumOfErrorsSquares << endl;

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				x[n - 1] = e;
			}

			// Следующая строка кода для отладки:
			//cout << numOfCycles << ":  " << sumOfErrorsSquares << endl;

			numOfCycles++;
			numOfCompletedTrainingCycles++;
		}while( numOfCycles < numOfTrainingCycles );
	}

	// Следующая функция выполняет обучение нейронной сети до тех пор пока sumOfErrorsSquares на тренировочных данных больше, чем аргумент функции maxSumOfErrorsSquares.
	void RecurrentNeuralNetwork::Train( double maxSumOfErrorsSquares )
	{
		int k, j, numOfTrainingCycles;
		double e;
		Vector x( n ), x0_training( n );
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] );	// x0_training - это самый первый вектор входных значений из обучающего набора данных.

		numOfTrainingCycles = 0;
		do{
			sumOfErrorsSquares = 0.0;

			/*
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				if( k == 0 ) // считываем первый пример.
				{
					for( j = 0; j < n; j++ )
					{
						x[j] = ConvertData( trainingDatum[j] );
					}
					e = ConvertData( trainingDatum[n] );
				}
				else		// считываем очередной пример.
				{
					for( j = 0; j < n - 1; j++ )
						x[j] = x[j + 1];
					x[n - 1] = e;
					e = ConvertData( trainingDatum[k + n] );
				}

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.
				sumOfErrorsSquares += ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.

				// Следующая строка кода для отладки:
				//cout << k << ":  " << sumOfErrorsSquares << endl;
			}
			*/
			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );
				
				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.
				sumOfErrorsSquares += ErrorSquare( x, e ); // выполняем 1 шаг алгоритма обратного распространения ошибки.

				// Следующая строка кода для отладки:
				//cout << k << ":  " << sumOfErrorsSquares << endl;

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				x[n - 1] = e;
			}

			// Следующая строка кода для отладки:
			//cout << numOfTrainingCycles << ":  " << sumOfErrorsSquares << endl;

			numOfTrainingCycles++;
			numOfCompletedTrainingCycles++;
		}while( sumOfErrorsSquares > maxSumOfErrorsSquares );
	}

	double RecurrentNeuralNetwork::y( Vector x )		// Выходное значение обученной нейронной сети.
	{
		double res;
		Vector S( m );	// Взвешенные суммы нейронов скрытого слоя.
		int i, j, l;

		for(i = 0; i < m; i++) // Цикл пробегает по всем нейронам скрытого слоя.
		{
			for(j = 0; j < n; j++) // Цикл пробегает по всем нейронам входного слоя.
				//S[i] += (*w)(j, i) * x[j];
				S[i] += (*w)(j, i) * ConvertData( x[j] );

			for(l = 0; l < m; l++) // Цикл пробегает по всем контекстным нейронам.
				S[i] += (*wc)(l, i) * (*p_prev)[l];

			S[i] -= (*T)[i];
			(*p)[i] = F( S[i] );
		}

		(*p_prev) = (*p);	// Присваиваем вычисленные значения нейронов скрытого слоя на текущем шаге предыдущим значениям нейронов 
		// скрытого слоя.

		//res = (*v) * (*p) - TT;
		res = RecoveryData( (*v) * (*p) - TT );
		return res;
	}

#if( CONVERSION_FUNCTION == LINEAR_1 )

	// Функция преобразования данных:
	double ConvertData( double d )
	{
		return ( d - ::minValue ) / ( ::maxValue - ::minValue );
	}

	// Функция восстановления данных:
	double RecoveryData( double d )
	{
		return ( ::maxValue - ::minValue ) * d + ::minValue;						//  0 <= d <= 1
	}

#endif

#if( CONVERSION_FUNCTION == LINEAR_2 )

	// Функция преобразования данных:
	double ConvertData( double d )
	{
		return 2.0 * ( d - ::minValue ) / ( ::maxValue - ::minValue ) - 1;
	}

	// Функция восстановления данных:
	double RecoveryData( double d )
	{
		return 0.5 * ( ::maxValue - ::minValue ) * ( d + 1) + ::minValue;			//  -1 <= d <= 1
	}

#endif

#if( CONVERSION_FUNCTION == SIGMOID )

	// Функция преобразования данных:
	double ConvertData( double d )
	{
		return 1 / ( 1 + exp( -d ) );
	}

	// Функция восстановления данных:
	double RecoveryData( double d )
	{
		return -log( 1 / d - 1 );
	}

#endif

#if( CONVERSION_FUNCTION == TANH )

	// Функция преобразования данных:
	double ConvertData( double d )
	{
		return tanh( d );
	}

	// Функция восстановления данных:
	double RecoveryData( double d )
	{
		return 0.5 * log( ( 1 + d ) / ( 1 - d ) );									//	|d| < 1
	}

#endif

}

#if( ACTIVATION_FUNCTION == SIGMOID )

// Функция активации нейронов скрытого слоя:
double F( double S )
{
	return 1 / ( 1 + exp( - l * S ) );
}

// Производная функции активации нейронов скрытого слоя:
double Derivative_F( double S )
{
	return l * F( S ) * ( 1 - F( S ) );
}

#endif

#if( ACTIVATION_FUNCTION == TANH )

// Функция активации нейронов скрытого слоя:
double F( double S )
{
	return tanh( l * S );
}

// Производная функции активации нейронов скрытого слоя:
double Derivative_F( double S )
{
	return l * ( 1 - pow( F( S ), 2 ) );
}

#endif

double tanh( double x )
{
	return ( exp( 2 * x ) - 1 ) / ( exp( 2 * x ) + 1 );
}