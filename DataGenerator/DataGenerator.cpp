// ��������� ������ ��� ������������ ������������ ��������� ���� ������.

//#define VARIANT_1		// ������ ���������:									y = A * sin( x );
//#define VARIANT_2		// ��������� � ������������ ����������, 1-�� �������:	y = ( a / ( 2 * Pi ) ) * A * x * sin( x );
//#define VARIANT_3		// ��������� � ������������ ����������, 2-�� �������:	y = ( 1 + a * A * x / ( 2 * Pi ) ) * sin( x );
//#define VARIANT_4		// ��������� � ���������� �����:						y = A * sin( x ) + Random( d * A * sin( x ) );
//#define VARIANT_5		// ��������� � ���������� �����:						y = A * sin( x ) + Random( d * A );
//#define VARIANT_6		// ��������� ��������� �����:							y = A * sin( x ) + A;
//#define VARIANT_7		// ��������� � ���������� ����������:					y = A * ( b / ( x + b ) ) * sin( x );
//#define VARIANT_8		// ����� 3-� ��������:									y = A * ( sin( x ) + sin( x / sqrt( 2.0 ) ) + sin( x / sqrt( 5.0 ) ) );
#define VARIANT_9		// ��� ��������� ���������.
//#define VARIANT_10	// ��� ��������� ��������� � ����������� ���������� ����� ���������������� �������� �������.
//#define VARIANT_11	// ��� ��������� ��������� � ����������� ���������� �����.
//#define VARIANT_12	// ��� ��������� ��������� �� �������� ����������� �����������.
//#define VARIANT_13	// ��� ��������� ��������� �� �������� ����������� ����������� � ��������� (f, g).

#if( !defined( VARIANT_1 ) && !defined( VARIANT_2 ) && !defined( VARIANT_3 ) && !defined( VARIANT_4 ) && !defined( VARIANT_5 ) && !defined( VARIANT_6 ) && !defined( VARIANT_7 ) && !defined( VARIANT_8 ) && !defined( VARIANT_9 ) && !defined( VARIANT_10 ) && !defined( VARIANT_11 ) && !defined( VARIANT_12 ) && !defined( VARIANT_13 ) )
	#error ������ ���� ��������� ���� �� �������� !
#endif

#include <math.h>
//#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

const double Pi = 3.141592653589793;

int		J = 22,			// ����� �������� ������� � ��������������� ������ �� ���������.
		T = 50;			// ����� ����� �� ���� ������ � ��������������� ������ �� ���������.

double	A = 0.5,		// ���������.
		//A = 10.0,		// ���������.
		//A = 200.0,		// ���������.
		//A = 100.0,		// ���������.
		A1,				// �������� ���������� ���������.
		a = 0.05,		// ���������� ��������� �� ����� �������.
		b = 20.0,		// ���������� �������� �������� ��������� ���������.
		c = 1.25,		// ���������� ���������� 2-� ��������� �������� (c > 0.5; ��� c = 1 ��� ��������� ����� �����������).
//		c = 1.0,		// ���������� ���������� 2-� ��������� �������� (c > 0.5; ��� c = 1 ��� ��������� ����� �����������).
		d = 0.1,		// ���������� �������� ����, �������������� �� ���������.
		e = 0.5,		// ���������� ������ ��������� ���������� ��������� 2-� ��������� �������� (��� ������, ����� �������� ������ VARIANT_12).
//		e = 0.25,		// ���������� ������ ��������� ���������� ��������� 2-� ��������� �������� (��� ������, ����� �������� ������ VARIANT_12).
//		f = 79.86,		// ���������� ������ ������� ��������� �������� ���������� ��������� 2-� ��������� �������� (��� ������, ����� �������� ������ VARIANT_13).
//		g = 285.0;		// ���������� ������� ������� ��������� �������� ���������� ��������� 2-� ��������� �������� (��� ������, ����� �������� ������ VARIANT_13).
		f = 150.0,		// ���������� ������ ������� ��������� �������� ���������� ��������� 2-� ��������� �������� (��� ������, ����� �������� ������ VARIANT_13).
		g = 200.0;		// ���������� ������� ������� ��������� �������� ���������� ��������� 2-� ��������� �������� (��� ������, ����� �������� ������ VARIANT_13).

int		precision = 5;	// �������� - ���������� ������ ����� ������� � ������������ ��������� ������ (��� ������, ����� �������� ������ VARIANT_13).

char	*filenameOfGeneratedDataDefault = "GeneratedData.txt";		// ��� ����� �� ��������� � ���������������� �������.

void GenerateData( ofstream& generatedStream );
double Random( double ampl );							// �������, ������������ ��������� �������������� ����� � ��������� �� -|ampl| �� |ampl|.
double Random( double a, double b, int precision );		// �������, ������������ ��������� �������������� ����� � ��������� (a, b) � ��������� ��������� precision (����������� ������ ����� �������).

int main( int argc, char* argv[] )
{
	char *filenameOfGeneratedData;
	ofstream generatedStream;

	if ( argc == 1 )			// ������ ��������� ��� ���������� � ��������� ������. ������ �� ������, �.�. ���������� ���������� ��� ������������������� ���������� �� ���������.
		filenameOfGeneratedData = filenameOfGeneratedDataDefault;
	else if ( argc == 3 )
	{
		J = atoi( argv[1] );	// ����������� ���������� ���������� �������� ���������� ����� ��������� ������.
		T = atoi( argv[2] );	// ����������� ���������� ���������� �������� ���������� ����� ��������� ������.
		filenameOfGeneratedData = filenameOfGeneratedDataDefault;
	}
	else if ( argc == 4 )
	{
		filenameOfGeneratedData = argv[1];
		J = atoi( argv[2] );	// ����������� ���������� ���������� �������� ���������� ����� ��������� ������.
		T = atoi( argv[3] );	// ����������� ���������� ���������� �������� ���������� ����� ��������� ������.
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line!!!" << endl;
		return 1;
	}

	generatedStream.open( filenameOfGeneratedData );
	GenerateData( generatedStream ); // ���������� ������ � ��������� ��� ����.
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

// �������, ������������ ��������� �������������� ����� � ��������� �� -|ampl| �� |ampl|:
double Random( double ampl )
{
    return 2 * ampl * (double)( rand() ) / RAND_MAX - ampl;
}

// �������, ������������ ��������� �������������� ����� � ��������� (a, b) � ��������� ��������� precision (����������� ������ ����� �������):
double Random( double a, double b, int precision )
{
	double res;

	// �������� ��������� ����� ��� ����� ����� � �������� precision:
	res = rand() % (int)pow( (double)10, precision );

	// �������� ������������ ����� � ������ ���������:
	res = a + ( res / pow( (double)10, precision ) ) * ( b - a );

	return res;
}