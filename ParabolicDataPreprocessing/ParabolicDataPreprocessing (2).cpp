// ��������� ��������� �������������� ������������� ������ �� ����� ��� �������� ��������� ��������� ����� ��������� ������ ������ ����������
// � ���������� ������������ ������ � ����, ��� �������� ��������� ��������� ������ ����������.
// ������� ���������� ��������� ���� �������� ��� �����, � ������� ����� �������� ��������� ����� ��������� ���������� ������ 
// ������� ���������� ����� - ����� �����-������ (11 ���). �������� ���������� ��������� ����� �������� ���-�� ����� �� ������� ����� ��������� �������� � ������������ �����������.

#include "Approximation.h"
/*
��������� 2 ������������������ ������������ ����� � ��������������� �� "*.cpp" ����� ����������� ������ ���������� � ��� �� ����������, 
��� � cpp ���� ���������� ������������ ���� "Approximation.h"! ����� ��������� ������ �� ����� ������ ���������:
"1>c1xx : fatal error C1083: Cannot open source file: 'SystemsOfLinearEquations.cpp': No such file or directory
1>  Matrixes3.cpp
1>c1xx : fatal error C1083: Cannot open source file: 'Matrixes3.cpp': No such file or directory
1>  Approximation.cpp
1>...\approximation.cpp(2): fatal error C1083: Cannot open include file: 'Matrixes3.h': No such file or directory"
*/
//#include "Matrixes3.h"
//#include "SystemsOfLinearEquations.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Approximation;

const int NumOfDatumForPorabola = 13;	// ���-�� ����� �� ������� ����� ��������� �������� � ������������ ����������� �� ���������, ���� �������� �������� ��� ������ ��������� ������������.

const double MaxToMinRatio = 7.7;
//const double MinimumMaximum = 35.0;
const double MinimumMaximum = 0;
const double MinOfDouble = numeric_limits<double>::lowest();
const double MaxOfDouble = numeric_limits<double>::max();

template<typename Type> Type abs( Type a );

// ��������� ���� �������� � ��������� ������: 
// 1) ��� ��������� ����� � �������, ������� ����� ������������													-	char* dataFile;
// 2) ��� ��������������� �����, � ������� ����� �������� ������������ ������									-	char* resFile;
// 3) ��� �����, � ������� ����� �������� ��������� ����� ��������� ���������� ������ ������� ���������� �����	-	char* resFile_2;
// � 4-�� ���������� ����� �������� ���-�� ����� �� ������� ����� ��������� �������� � ������������ ����������� -	int numOfDatumForPorabola;
int main( int argc, char* argv[] )
{
	char* dataFile;
	char* resFile;
	char* resFile_2;
	int numOfDatum, numOfLastMin, numOfLastMax, numOfCurrentMin, numOfCurrentMax, numOfPeriods, numOfDatumForPorabola, maxNumOfDatumToExtremumInArray;
	double value, prevValue, prevPrevValue, lastMin, lastMax, currentMin, currentMax, averageMaximum, *arrayOfIndexes, *arrayOfDatum;
	string tempStr, str;
	ifstream dataStream, dataStream_2;
	ofstream resStream, resStream_2;
	ParabolicApproximation parabolicAppr;

	if ( argc == 4 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		numOfDatumForPorabola = NumOfDatumForPorabola;
	}
	else if ( argc == 5 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		numOfDatumForPorabola = atoi( argv[4] );
		assert( ( numOfDatumForPorabola % 2 == 1 ) && ( numOfDatumForPorabola >= 3 ) );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != 4 OR 5 )!!!" << endl;
		return 1;
	}

	arrayOfIndexes = new double[numOfDatumForPorabola];
	arrayOfDatum = new double[numOfDatumForPorabola];
	maxNumOfDatumToExtremumInArray = numOfDatumForPorabola / 2;
	for( int i = 0; i < numOfDatumForPorabola; i++ ) arrayOfIndexes[i] = i;

	// ��������� ����� 2 ���� ��������� ��������� ������ �� ����� dataFile.
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		resStream_2.open( resFile_2 );

		// ��������� �� ������ ������ ������ 2 �������� ���� double:
		dataStream >> prevPrevValue >> prevValue;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 1;
		numOfDatum = 1;		// �.�. ��������� ������ ���������� � 0.
		averageMaximum = 0.0;
		do{	
			// ��������� ��������� ���� �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// ����������� �������� ��������;
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;

					cout << "lastMin  =  " << lastMin << endl	// ��� �������
						<< "numOfLastMin  =  " << numOfLastMin << endl;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// ����������� �������� ���������;
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;

					cout << "lastMax  =  " << lastMax << endl	// ��� �������
						<< "numOfLastMax  =  " << numOfLastMax << endl;
				}

				//if( ( lastMin != MaxOfDouble ) && ( lastMax / lastMin >= MaxToMinRatio ) )
				//if( lastMax / lastMin >= MaxToMinRatio )
				if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				{
					// ������� � ���������� �����.
					cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin 
						<< endl << endl << endl << endl; // ��� �������.

					// ������ ������ � ����, ����� ����� ������� �������� �������� ���������� ��������� ��� � ����� ���� �� ������� ����������� ���������:
					resStream_2 << numOfLastMax << "	" << lastMax << endl;
					resStream_2 << numOfLastMin << "	" << lastMin << endl;

					numOfPeriods++;
					averageMaximum += lastMax;
					lastMin = MaxOfDouble;
					lastMax = MinOfDouble;
				}
				
				prevPrevValue = prevValue;
				prevValue = value;
			}
		} while( !dataStream.eof() );

		//if( ( lastMax / lastMin < MaxToMinRatio ) && ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		{
			// ��������� ��������� ����.
			cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value
				<< endl << endl << endl << endl; // ��� �������.

			// ��������� � ���� ������ � ��������� ���������:
			resStream_2 << numOfLastMax << "	" << lastMax << endl;

			numOfPeriods++;
			averageMaximum += lastMax;
		}

		resStream_2.close();
	}
	dataStream.close();

	averageMaximum /= numOfPeriods;

	dataStream.open( dataFile );
	dataStream_2.open( resFile_2 );
	if( !dataStream.fail() && !dataStream_2.fail() )
	{
		resStream.open( resFile );

		// ����� ��������� �� ������ ������ ������ 2 �������� ���� double:
		dataStream >> prevPrevValue >> prevValue;
		// ��������� �������� ������ ���� �����������:
		dataStream_2 >> numOfCurrentMax >> currentMax;
		dataStream_2 >> numOfCurrentMin >> currentMin;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 1;
		numOfDatum = 1;		// �.�. ��������� ������ ���������� � 0.

		// ���������� � �������������� ���� 2 ����������� ��������� ��������, ������ ���� ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray ):
		//if( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray )
		if( numOfCurrentMax - numOfDatum > maxNumOfDatumToExtremumInArray )
			resStream << prevPrevValue << endl << prevValue << endl;
		else
		{
			arrayOfDatum[0] = prevPrevValue;
			arrayOfDatum[1] = prevValue;
		}

		do{	
			// ��������� ��������� ���� �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;

				// ���������� � �������������� ���� ����������� ��������� ��������, ������ 
				// ���� ( ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray ) && ( abs( numOfCurrentMin - numOfDatum ) > maxNumOfDatumToExtremumInArray ) ):
				if( ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray ) && ( abs( numOfCurrentMin - numOfDatum ) > maxNumOfDatumToExtremumInArray ) )
					resStream << value << endl;
				else
				{
				// ����� �� ��������� ��� � ����� ��������� ����� ��� � ����� �������� �����.
					// ���� �� ��������� � ����� ��������� �����:
					if( abs( numOfCurrentMax - numOfDatum ) <= maxNumOfDatumToExtremumInArray )
					{
						// ��������� �������� ��������� � ������ arrayOfDatum, ��� ������������ ���������� �������������� �������������:
						arrayOfDatum[maxNumOfDatumToExtremumInArray - ( numOfCurrentMax - numOfDatum )] = value;

						if( maxNumOfDatumToExtremumInArray - ( numOfCurrentMax - numOfDatum ) == numOfDatumForPorabola - 1 ) // �.�. ���� ������ arrayOfDatum ���������� ���������:
						{
							parabolicAppr.Init( arrayOfIndexes, arrayOfDatum, numOfDatumForPorabola );
							//parabolicAppr.Update_a_b_c_terms();
							//for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY( i ) << endl;
							for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY2( i ) << endl;
						}
					}
					// ���� �� ��������� � ����� �������� �����:
					else
					//if( abs( numOfCurrentMin - numOfDatum ) <= maxNumOfDatumToExtremumInArray )
					{
						// ��������� �������� ��������� � ������ arrayOfDatum, ��� ������������ ���������� �������������� �������������:
						arrayOfDatum[maxNumOfDatumToExtremumInArray - ( numOfCurrentMin - numOfDatum )] = value;

						if( ( maxNumOfDatumToExtremumInArray - ( numOfCurrentMin - numOfDatum ) == numOfDatumForPorabola - 1 ) ) // �.�. ���� ������ arrayOfDatum ���������� ���������:
						{
							parabolicAppr.Init( arrayOfIndexes, arrayOfDatum, numOfDatumForPorabola );
							//parabolicAppr.Update_a_b_c_terms();
							//for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY( i ) << endl;
							for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY2( i ) << endl;

							dataStream_2 >> numOfCurrentMin >> currentMin;
						}
					}
				}

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// ����������� �������� ��������;
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// ����������� �������� ���������;
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;
				}

				//if( ( lastMin != MaxOfDouble ) && ( lastMax / lastMin >= MaxToMinRatio ) )
				//if( lastMax / lastMin >= MaxToMinRatio )
				if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				{
					// ������� � ���������� �����.
					numOfPeriods++;
					dataStream_2 >> numOfCurrentMax >> currentMax;
					lastMin = MaxOfDouble;
					lastMax = MinOfDouble;
				}
				
				prevPrevValue = prevValue;
				prevValue = value;
			}
		} while( !dataStream.eof() );

		resStream.close();
	}
	dataStream.close();
	dataStream_2.close();
	delete( arrayOfDatum );

	cout << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numberOfPeriods  =  " << numOfPeriods << endl
		 << "averageMaximum  =  " << averageMaximum << endl;
		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;

	return 0;
}

template<typename Type> Type abs( Type a )
{
	return ( a > 0 )? a: -a;
}