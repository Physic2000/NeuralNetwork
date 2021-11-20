// Генератор данных для тестирования рекуррентной нейронной сети Элмана.

//#define VARIANT_1		// Просто синусоида:									y = A * sin( x );
//#define VARIANT_2		// Синусоида с возрастающей амплитудой, 1-ый вариант:	y = ( a / ( 2 * Pi ) ) * A * x * sin( x );
//#define VARIANT_3		// Синусоида с возрастающей амплитудой, 2-ой вариант:	y = ( 1 + a * A * x / ( 2 * Pi ) ) * sin( x );
//#define VARIANT_4		// Синусоида с переменным шумом:						y = A * sin( x ) + Random( d * A * sin( x ) );
//#define VARIANT_5		// Синусоида с постоянным шумом:						y = A * sin( x ) + Random( d * A );
//#define VARIANT_6		// Синусоида смещённая вверх:							y = A * sin( x ) + A;
//#define VARIANT_7		// Синусоида с затухающей амплитудой:					y = A * ( b / ( x + b ) ) * sin( x );
//#define VARIANT_8		// Сумма 3-х синусоид:									y = A * ( sin( x ) + sin( x / sqrt( 2.0 ) ) + sin( x / sqrt( 5.0 ) ) );
#define VARIANT_9		// Две склеенные синусоиды.
//#define VARIANT_10	// Две склеенные синусоиды с добавленным переменным шумом пропорциональным значению функции.
//#define VARIANT_11	// Две склеенные синусоиды с добавленным постоянным шумом.
//#define VARIANT_12	// Две склеенные синусоиды со случайно меняющимися амплитудами.
//#define VARIANT_13	// Две склеенные синусоиды со случайно меняющимися амплитудами в интервале (f, g).

#if( !defined( VARIANT_1 ) && !defined( VARIANT_2 ) && !defined( VARIANT_3 ) && !defined( VARIANT_4 ) && !defined( VARIANT_5 ) && !defined( VARIANT_6 ) && !defined( VARIANT_7 ) && !defined( VARIANT_8 ) && !defined( VARIANT_9 ) && !defined( VARIANT_10 ) && !defined( VARIANT_11 ) && !defined( VARIANT_12 ) && !defined( VARIANT_13 ) )
	#error Должен быть определен один из макросов !
#endif

#include <math.h>
//#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

const double Pi = 3.141592653589793;

int		J = 22,			// Число периодов функции в сгенерированных данных по умолчанию.
		T = 50;			// Число точек на один период в сгенерированных данных по умолчанию.

double	A = 0.5,		// Амплитуда.
		//A = 10.0,		// Амплитуда.
		//A = 200.0,		// Амплитуда.
		//A = 100.0,		// Амплитуда.
		A1,				// Случайно меняющаяся амплитуда.
		a = 0.05,		// Приращение амплитуды на одном периоде.
		b = 20.0,		// Определяет скорость спадания амплитуды синусоиды.
		c = 1.25,		// Определяет асимметрию 2-х склеенных синусоид (c > 0.5; при c = 1 две синусоиды будут одинаковыми).
//		c = 1.0,		// Определяет асимметрию 2-х склеенных синусоид (c > 0.5; при c = 1 две синусоиды будут одинаковыми).
		d = 0.1,		// Определяет диапазон шума, накладываемого на синусоиду.
		e = 0.5,		// Определяет размах случайных флуктуаций амплитуды 2-х склеенных синусоид (для случая, когда определён макрос VARIANT_12).
//		e = 0.25,		// Определяет размах случайных флуктуаций амплитуды 2-х склеенных синусоид (для случая, когда определён макрос VARIANT_12).
//		f = 79.86,		// Определяет нижнюю границу интервала случайно меняющейся амплитуды 2-х склеенных синусоид (для случая, когда определён макрос VARIANT_13).
//		g = 285.0;		// Определяет верхнюю границу интервала случайно меняющейся амплитуды 2-х склеенных синусоид (для случая, когда определён макрос VARIANT_13).
		f = 150.0,		// Определяет нижнюю границу интервала случайно меняющейся амплитуды 2-х склеенных синусоид (для случая, когда определён макрос VARIANT_13).
		g = 200.0;		// Определяет верхнюю границу интервала случайно меняющейся амплитуды 2-х склеенных синусоид (для случая, когда определён макрос VARIANT_13).

int		precision = 5;	// Точность - количество знаков после запятой в генерируемых случайных числах (для случая, когда определён макрос VARIANT_13).

char	*filenameOfGeneratedDataDefault = "GeneratedData.txt";		// Имя файла по умолчанию с сгенерированными данными.

void GenerateData( ofstream& generatedStream );
double Random( double ampl );							// Функция, генерирующая случайное действительное число в интервале от -|ampl| до |ampl|.
double Random( double a, double b, int precision );		// Функция, генерирующая случайное действительное число в интервале (a, b) и указанной точностью precision (количеством знаков после запятой).

int main( int argc, char* argv[] )
{
	char *filenameOfGeneratedData;
	ofstream generatedStream;

	if ( argc == 1 )			// Запуск программы без аргументов в командной строке. Ничего не делаем, т.к. глобальные переменные уже проинициализированы значениями по умолчанию.
		filenameOfGeneratedData = filenameOfGeneratedDataDefault;
	else if ( argc == 3 )
	{
		J = atoi( argv[1] );	// Присваиваем глобальной переменной значение переданное через командную строку.
		T = atoi( argv[2] );	// Присваиваем глобальной переменной значение переданное через командную строку.
		filenameOfGeneratedData = filenameOfGeneratedDataDefault;
	}
	else if ( argc == 4 )
	{
		filenameOfGeneratedData = argv[1];
		J = atoi( argv[2] );	// Присваиваем глобальной переменной значение переданное через командную строку.
		T = atoi( argv[3] );	// Присваиваем глобальной переменной значение переданное через командную строку.
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line!!!" << endl;
		return 1;
	}

	generatedStream.open( filenameOfGeneratedData );
	GenerateData( generatedStream ); // Генерируем данные и заполняем ими файл.
	generatedStream.close();

	return 0;
}

void GenerateData( ofstream& generatedStream )
{
	int j, t;
	double x, y;

	for(j = 0; j < J; j++)
	{

#if( defined( VARIANT_12 ) )

		A1 = A + Random( e * A );

#elif( defined( VARIANT_13 ) )

		A1 = Random( f, g, precision );

#endif

		for(t = 0; t < T; t++)
		{
			x = ( j + (double)t / T ) * 2 * Pi;

#if( defined( VARIANT_1 ) )

			y = A * sin( x );

#elif( defined( VARIANT_2 ) )

			y = ( a / ( 2 * Pi ) ) * A * x * sin( x );

#elif( defined( VARIANT_3 ) )

			y = ( 1 + a * A * x / ( 2 * Pi ) ) * sin( x );

#elif( defined( VARIANT_4 ) )

			//y = A * sin( x ) + Random( d * A * sin( x ) );
			y = A * sin( x );
			y += Random( d * y );

#elif( defined( VARIANT_5 ) )

			y = A * sin( x ) + Random( d * A );

#elif( defined( VARIANT_6 ) )

			y = A * sin( x ) + A;

#elif( defined( VARIANT_7 ) )

			y = A * ( b / ( x + b ) ) * sin( x );

#elif( defined( VARIANT_8 ) )

			y = A * ( sin( x ) + sin( x / sqrt( 2.0 ) ) + sin( x / sqrt( 5.0 ) ) );

#elif( defined( VARIANT_9 ) )

			if( x >= 2 * Pi )
				do{
					x -= 2 * Pi;
				}while( x >= 2 * Pi );
			if( ( 0 <= x ) && ( x < Pi / c ) )
				y = A * ( 1 + sin( c * x + 1.5 * Pi ) );
			else //if( ( Pi / c <= x ) && ( x < 2 * Pi ) )
				y = A * ( 1 + sin( ( c / ( 2 * c - 1 ) ) * x + ( ( 2 * c - 3 ) / ( 4 * c - 2 ) ) * Pi ) );

#elif( defined( VARIANT_10 ) )

			if( x >= 2 * Pi )
				do{
					x -= 2 * Pi;
				}while( x >= 2 * Pi );
			if( ( 0 <= x ) && ( x < Pi / c ) )
				y = A * ( 1 + sin( c * x + 1.5 * Pi ) );
			else //if( ( Pi / c <= x ) && ( x < 2 * Pi ) )
				y = A * ( 1 + sin( ( c / ( 2 * c - 1 ) ) * x + ( ( 2 * c - 3 ) / ( 4 * c - 2 ) ) * Pi ) );

			y += Random( d * y );

#elif( defined( VARIANT_11 ) )

			if( x >= 2 * Pi )
				do{
					x -= 2 * Pi;
				}while( x >= 2 * Pi );
			if( ( 0 <= x ) && ( x < Pi / c ) )
				y = A * ( 1 + sin( c * x + 1.5 * Pi ) );
			else //if( ( Pi / c <= x ) && ( x < 2 * Pi ) )
				y = A * ( 1 + sin( ( c / ( 2 * c - 1 ) ) * x + ( ( 2 * c - 3 ) / ( 4 * c - 2 ) ) * Pi ) );

			y += Random( d * A );

#elif( defined( VARIANT_12 ) || defined( VARIANT_13 ) )

			if( x >= 2 * Pi )
				do{
					x -= 2 * Pi;
				}while( x >= 2 * Pi );
			if( ( 0 <= x ) && ( x < Pi / c ) )
				y = A1 * ( 1 + sin( c * x + 1.5 * Pi ) );
			else //if( ( Pi / c <= x ) && ( x < 2 * Pi ) )
				y = A1 * ( 1 + sin( ( c / ( 2 * c - 1 ) ) * x + ( ( 2 * c - 3 ) / ( 4 * c - 2 ) ) * Pi ) );

#endif

			generatedStream << y << endl;
		};
	}
}

// Функция, генерирующая случайное действительное число в интервале от -|ampl| до |ampl|:
double Random( double ampl )
{
    return 2 * ampl * (double)( rand() ) / RAND_MAX - ampl;
}

// Функция, генерирующая случайное действительное число в интервале (a, b) и указанной точностью precision (количеством знаков после запятой):
double Random( double a, double b, int precision )
{
	double res;

	// получить случайное число как целое число с порядком precision:
	res = rand() % (int)pow( (double)10, precision );

	// получить вещественное число в нужном интервале:
	res = a + ( res / pow( (double)10, precision ) ) * ( b - a );

	return res;
}