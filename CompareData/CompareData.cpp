
// ***** ��������� ����������� ��������� ���� �� ���������: *****

int		n = 6,					// ����������� �������� �������, �.�. ����� �������� �������� ���� ��� ����� ������ �� ���������.
		m = 10;					// ����� �������� �������� ���� �� ���������.

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

double CalculationTheSumOfSquaredDeviations( char* filename_1, char* filename_2 );

int main( int argc, char* argv[] )
{
	char *resultData, *fileForComparison;
	double sumOfSquaredDeviations;

	resultData = argv[1];
	fileForComparison = argv[2];

	try
	{
		sumOfSquaredDeviations = CalculationTheSumOfSquaredDeviations( resultData, fileForComparison );
		cout << endl << "sumOfSquaredDeviations  =  " << sumOfSquaredDeviations << endl;
	}
	catch( const string &str )
	{
		cout << endl << str;
	}

	return 0;
}// End main( ... )

double CalculationTheSumOfSquaredDeviations( char* filename_1, char* filename_2 )
{
	double res, datum_1, datum_2;
	int i;
	ifstream dataStream_1, dataStream_2;
	string strOfException, file_1, file_2;

	/*
	cout << endl 
		<< filename_1 << endl
		<< filename_2 << endl;
	*/

	dataStream_1.open( filename_1 );
	dataStream_2.open( filename_2 );
	if( dataStream_1.fail() )	// �������� �������� �����.
	{
		strOfException = filename_1;
		strOfException = "ERROR: can't open file " + strOfException + "!";
		//strOfException = "ERROR: can't open file " + *filename_1;
		throw strOfException;
	}
	if( dataStream_2.fail() )	// �������� �������� �����.
	{
		strOfException = filename_2;
		strOfException = "ERROR: can't open file " + strOfException + "!";
		//strOfException = "ERROR: can't open file " + *filename_2;
		throw strOfException;
	}

	cout << endl;
	// ��������� � ���������� ������ n �������� �� ����� filename_2:
	for( i = 0; i < n; i++ ) 
	{
		dataStream_2 >> datum_2;
		cout << i + 1 << " skipped value:  " << datum_2 << endl;
	}
	cout << endl;

	res = 0; i = 0;
	do{
		// ��������� ���� filename_2 �� �����, ������������ �������� � �������� �� ����� filename_1:
		dataStream_1 >> datum_1;
		dataStream_2 >> datum_2;

		if( !dataStream_2.eof() )
		{
			res += ( datum_2 - datum_1 ) * ( datum_2 - datum_1 );
			i++;
			cout << i << ":     datum_1  =  " << datum_1 << ";     datum_2  =  " << datum_2 << ";     res  =  " << res << endl;
		}
	}while( !dataStream_2.eof() );

	// �������� ������������ ��������� �������� � ������ filename_1 � filename_2:
	dataStream_1 >> datum_1;	// ����������� �� ����� filename_1 �������� ������ ��������� ������ �����!
	if( dataStream_1.eof() != true )
	{
		file_1 = filename_1;
		file_2 = filename_2;
		strOfException = "ERROR: file \"" + file_1 + "\" does not match file \"" + file_2 + "\" in number of datum!";
		throw strOfException;
	}

	dataStream_1.close();
	dataStream_2.close();

	return res;
}