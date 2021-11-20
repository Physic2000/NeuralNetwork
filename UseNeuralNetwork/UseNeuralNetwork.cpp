// Использование рекуррентной нейронной сети Элмана, имеющей 1 нейрон с линейной функцией активации в выходном слое.
// Применяется для решения задач прогнозирования временных рядов.
/*
UseNeuralNetwork - программа запускает процесс обучения ИНС и формирует на основе уже обученной ИНС результирующий файл, содержащий аппроксимацию данных, т.е. данные смоделированные ИНС.
UseNeuralNetwork.exe в качестве 1-го параметра принимает имя файла c тренировочными данными,
в качестве 2-го параметра - имя файла c тестовыми данными,
в качестве 3-го параметра - имя результирующего файла в который будут записаны результаты работы ИНС Элмана, т.е. данные смоделированные ИНС,
в качестве 4-го параметра - cкорость обучения нейронной сети, т.е. число типа double ( 0.001 <= a <= 0.5 ),
в качестве 5-го параметра - момент (инерция) обучения нейронной сети, т.е. число типа double ( 0 <= mu <= 1.0 ),
в качестве 6-го параметра - максимально допустимое число циклов обучения, т.е. число типа int ( 100, 200, 500 ),
в качестве 7-го параметра - максимально допустимая сумма квадратов ошибок на тестовых данных, т.е. число типа double,
в качестве 8-го необязательного параметра программа может принять размерность входного вектора, т.е. число нейронов входного слоя,
в качестве 9-го необязательного параметра программа может принять число нейронов скрытого слоя,
в качестве 10-го необязательного параметра программа может принять требуемое от ИНС число смоделированных данных.
Если exe-файл вызывается без необязательных параметров, то по умолчанию число нейронов входного слоя равно 6, число нейронов скрытого слоя равно 10, 
а требуемое от ИНС число смоделированных данных равно числу примеров в тестовых данных, т.е. равно числу тестовых данных уменьшенному на количество нейронов входного слоя.
*/

#include <string>
#include <sstream>
#include <ctime>
#include "RecurrentNeuralNetwork.h"
using namespace std;
using namespace NeuralNetwork;

// ***** СТРУКТУРА РЕКУРЕНТНОЙ НЕЙРОННОЙ СЕТИ ПО УМОЛЧАНИЮ: *****

int		n = 6,					// Размерность входного вектора, т.е. число нейронов входного слоя или число входов ПО УМОЛЧАНИЮ.
		m = 10;					// Число нейронов скрытого слоя ПО УМОЛЧАНИЮ.

// ***** ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ИНИЦИАЛИЗИРУЮЩИЕ ПРОЦЕСС ОБУЧЕНИЯ НЕЙРОННОЙ СЕТИ: *****

int maxNumOfTrainingCycles;		// = 100;	// Максимально допустимое число циклов обучения.
//int maxNumOfTrainingCycles;	// = 200;	// Максимально допустимое число циклов обучения.
//int maxNumOfTrainingCycles;	// = 500;	// Максимально допустимое число циклов обучения.
//double maxSumOfErrorsSquares; // = 0.1;	// Максимально допустимая сумма квадратов ошибок на тестовых данных.
double maxSumOfErrorsSquares; // = 0.001;	// Максимально допустимая сумма квадратов ошибок на тестовых данных.

// Программе НАДО передать в командной строке:
// 1) имя файла c тренировочными данными;
// 2) имя файла c тестовыми данными;
// 3) имя результирующего файла в который будут записаны результаты работы ИНС Элмана, т.е. данные смоделированные ИНС;
// 4) cкорость обучения нейронной сети, т.е. число типа double ( 0.001 <= a <= 0.5 );
// 5) момент (инерция) обучения нейронной сети, т.е. число типа double ( 0 <= mu <= 1.0 );
// 6) максимально допустимое число циклов обучения, т.е. число типа int ( 100, 200, 500 );
// 7) максимально допустимую сумму квадратов ошибок на тестовых данных, т.е. число типа double.
// Дополнительно программе можно передать в командной строке:
// 8) размерность входного вектора, т.е. число нейронов входного слоя;
// 9) число нейронов скрытого слоя;
// 10) требуемое от ИНС число смоделированных данных.
int main( int argc, char* argv[] )
{
	char* trainingData;										// Тренировочные данные.
	char* testData;											// Тестовые данные.
	char* resultData;										// Файл для записи результата работы нейронной сети.
	double	a,					// Скорость обучения нейронной сети ( 0.001 <= a <= 0.5 ).
			mu,					// Момент (инерция) обучения нейронной сети ( 0 <= mu <= 1.0 ). Его введение позволяет выходить из небольших локальных минимумов.
			datum, *testDatum, sumOfErrorsSquares, y;
	ifstream trainingStream, testStream;
	ofstream resultStream;
	int numOfTrainingDatum, numOfTrainingExamples, numOfTestDatum, numOfTestExamples, numOfTrainingCycles, j, k, n, m, numOfSimulatedDatum;
	
	if ( argc == 8 )			// Запуск программы с 7-мью аргументами в командной строке.
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		// присваиваем локальным переменным значения одноимённых глобальных переменных (значения по умолчанию):
		n = ::n;				// присваиваем локальной переменной значение глобальной переменной.
		m = ::m;				// присваиваем локальной переменной значение глобальной переменной.
		numOfSimulatedDatum = 0;
	}
	else if ( argc == 9 )	// Запуск программы с 8-мью аргументами в командной строке.
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		numOfSimulatedDatum = atoi( argv[8] );
		// присваиваем локальным переменным значения одноимённых глобальных переменных (значения по умолчанию):
		n = ::n;				// присваиваем локальной переменной значение глобальной переменной.
		m = ::m;				// присваиваем локальной переменной значение глобальной переменной.
	}
	else if ( argc == 10 )	// Запуск программы с 9-тью аргументами в командной строке.
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		n = atoi( argv[8] );
		m = atoi( argv[9] );
		numOfSimulatedDatum = 0;
	}
	else if ( argc == 11 )	// Запуск программы с 10-тью аргументами в командной строке.
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		n = atoi( argv[8] );
		m = atoi( argv[9] );
		numOfSimulatedDatum = atoi( argv[10] );
	}
	else					// Запуск программы с непредусмотренным кол-ом аргументов в командной строке.
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line!!!" << endl;
		return 1;
	}
	cout << endl;

	//RecurrentNeuralNetwork rnn( trainingData, testData, a, mu, n, m );	// Инициализация рекуррентной нейронной сети.
	RecurrentNeuralNetwork rnn( trainingData, testData, a, mu );	// Инициализация рекуррентной нейронной сети.
	//cout << endl << "After calling constructor RecurrentNeuralNetwork(...)" << endl;
	unsigned int start_time =  clock(); // начальное время
	// 1-ЫЙ ВАРИАНТ ОБУЧЕНИЯ НЕЙРОННОЙ СЕТИ:
	rnn.Train( maxSumOfErrorsSquares, sumOfErrorsSquares, maxNumOfTrainingCycles, numOfTrainingCycles ); // Обучение нейронной сети.
	/*
	// 2-ОЙ ВАРИАНТ ОБУЧЕНИЯ НЕЙРОННОЙ СЕТИ:
	rnn.Train( maxNumOfTrainingCycles ); // Производим обучение нейронной сети, выполняя ровно maxNumOfTrainingCycles циклов обучения.
	numOfTrainingCycles = rnn.NumOfCompletedTrainingCycles();
	sumOfErrorsSquares = rnn.SumOfErrorsSquares();
	*/
	/*
	// 3-ИЙ ВАРИАНТ ОБУЧЕНИЯ НЕЙРОННОЙ СЕТИ:
	//maxSumOfErrorsSquares = 3.3;
	rnn.Train( maxSumOfErrorsSquares ); // Производим обучение нейронной сети до тех пор пока sumOfErrorsSquares на тренировочных данных больше, чем аргумент функции - maxSumOfErrorsSquares.
	numOfTrainingCycles = rnn.NumOfCompletedTrainingCycles();
	sumOfErrorsSquares = rnn.SumOfErrorsSquares();
	*/
	unsigned int end_time = clock(); // конечное время
	unsigned int timeOfTraining = end_time - start_time; // искомое время обучения ИНС в миллисекундах
	
	//cout << endl << "After calling member function Train()" << endl;

	numOfTrainingDatum = rnn.NumOfTrainingDatum();
	numOfTrainingExamples = rnn.NumOfTrainingExamples();
	numOfTestDatum = rnn.NumOfTestDatum();
	numOfTestExamples = rnn.NumOfTestExamples();

	cout << "\nn  =  " << n << endl
		<< "m  =  " << m << endl << endl

		<< "a  =  " << a << endl
		<< "mu  =  " << mu << endl << endl

		<< "maxSumOfErrorsSquares  =  " << maxSumOfErrorsSquares << endl
		<< "maxNumOfTrainingCycles  =  " << maxNumOfTrainingCycles << endl << endl << endl;

	cout << "numOfTrainingDatum  =  " << numOfTrainingDatum << endl
		<< "numOfTrainingExamples  =  " << numOfTrainingExamples << endl
		<< "numOfTestDatum  =  " << numOfTestDatum << endl
		<< "numOfTestExamples  =  " << numOfTestExamples << endl
		<< "sumOfErrorsSquares  =  " << sumOfErrorsSquares << endl
		<< "numOfTrainingCycles  =  " << numOfTrainingCycles << endl << endl

		<< "timeOfTraining  =  " << timeOfTraining << " msec" << endl 
		<< "runtime  =  " << clock() / 1000.0 << " sec" << endl << endl;

	if( numOfSimulatedDatum <= 0 ) numOfSimulatedDatum = numOfTestExamples - numOfSimulatedDatum;	// Т.е. если numOfSimulatedDatum <= 0, то numOfSimulatedDatum = numOfTestExamples + abs( numOfSimulatedDatum );
	if( numOfSimulatedDatum == numOfTestDatum - n )
		cout << "	Test data:		Simulated datum:		Delta:" << endl;
	else
		cout << "	Simulated datum:" << endl;

	Vector x( n ), x0_test( n );	// где x( n ) - это текущий вектор входных данных для ИНС, а x0_test( n ) - это самый первый вектор входных данных из тестового набора данных;
	testDatum = rnn.testDatum;	// Это указатель на самое первое значение в ряду тестовых данных.
	for( j = 0; j < n; j++ ) x0_test[j] = testDatum[j]; // Это самый первый вектор входных значений из тестового набора данных.
	
	testStream.open( testData );
	resultStream.open( resultData );
	for( j = 0; j < n; j++ ) testStream >> datum;	// Считываем из потока тестовых данных уже считанные n этих же значений из массива testDatum.

	x = x0_test;	// Инициализация вектора x первыми n значениями из тестового набора данных.
	for( k = 0; k < numOfSimulatedDatum; k++ )
	{
		y = rnn.y( x );

		if( numOfSimulatedDatum == numOfTestDatum - n )
		{
			testStream >> datum;

			cout << k << ":		" << datum << "		" << y << "		" << datum - y << endl;
		}
		else
			cout << k << ":		" << y << endl;

		resultStream << y << endl; // Вывод смоделированного ИНС значения в txt файл.

		for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
		x[n - 1] = y;
	}
	testStream.close();
	resultStream.close();

/*
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
*/
	return 0;

}// End main( ... )