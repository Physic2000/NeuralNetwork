// ��������� ��������� �������������� ������������� ������ �� ����� ��� �������� ��������� ��������� ����� ��������� ������ ������ ����������
// � ���������� ������������ ������ � ����, ��� �������� ��������� ��������� ������ ����������.
// ������� ���������� ��������� ���� �������� ��� �����, � ������� ����� �������� ���������� ����� ��������� ���������� ������ ������� ���������� ����� - ����� �����-������ (11 ���).
// �������� ���������� ��������� ����� �������� ���-�� ����� �� ������� ����� ��������� �������� � ������������ �������� (�� ��������� ��� ����� 13).
// ����� ���������� ��������� ����� �������� ���-�� ����� �� ������� ����� ��������� �������� � ������������ ��������� (�� ��������� ��� ����� 7).

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

const int NumOfDatumForPorabolaForMin = 13;	// ���-�� ����� �� ������� ����� ��������� �������� � ������������ �������� �� ���������, ���� �������� �������� ��� ������ ��������� ������������.
const int NumOfDatumForPorabolaForMax = 7;		// ���-�� ����� �� ������� ����� ��������� �������� � ������������ ��������� �� ���������, ���� �������� �������� ��� ������ ��������� ������������.
//const int NumOfRepetitionsOfFulfilledCondition = 2; // ���-�� ������ ������ �������� ������������ ������� ��� �������� ����� ������.

//const double MaxToMinRatio = 7.7;
const double MaxToMinRatio = 7.0;
const double FactorOfIntensificationCondition = 0.7;
//const double MinimumMaximum = 35.0;
const double MinimumMaximum = 0;
const double MinOfDouble = numeric_limits<double>::lowest();
const double MaxOfDouble = numeric_limits<double>::max();

template<typename Type> Type abs( Type a );

// ��������� ���� �������� � ��������� ������: 
// 1) ��� ��������� ����� � �������, ������� ����� ������������													-	char* dataFile;
// 2) ��� ��������������� �����, � ������� ����� �������� ������������ ������									-	char* resFile;
// 3) ��� �����, � ������� ����� �������� ���������� ����� ��������� ���������� ������ ������� ���������� �����	-	char* resFile_2;
// 4) 4-�� ���������� ����� �������� ���-�� ����� �� ������� ����� ��������� �������� � ������������ ��������	-	int numOfDatumForPorabolaForMin;
// 5) 5-�� ���������� ����� �������� ���-�� ����� �� ������� ����� ��������� �������� � ������������ ���������	-	int numOfDatumForPorabolaForMax;
// ���� ��������� �� �������� 4-�� � 5-�� ���������, �� �������� �� ��������� ����� ��������� �� 13-�� ������ � ����������� �������� � �� 7-�� ������ � ����������� ���������.
int main( int argc, char* argv[] )
{
	char *dataFile, *resFile, *resFile_2, *resFile_3, *appDebugFile = "AppDebug.txt";
	int numOfDatum, numOfLastMin, numOfLastMax, numOfCurrentMin, numOfCurrentMax, numOfPeriods, numOfDatumForPorabolaForMin, numOfDatumForPorabolaForMax, maxNumOfDatumToMinInArray, maxNumOfDatumToMaxInArray, numOfFulfilledCondition;
	double value, prevValue, prevPrevValue, predictedValue, lastMin, lastMax, currentMin, currentMax, averageMaximum, *arrayOfIndexesForMin, *arrayOfIndexesForMax, *arrayOfDatumForMin, *arrayOfDatumForMax;
	string tempStr, str;
	ifstream dataStream, dataStream_2;
	ofstream resStream, resStream_2, resStream_3, debugStream;
	bool itIsNotFirstValueFor_ResStream = false, itIsNotFirstValueFor_ResStream_2 = false, itIsNotFirstValueFor_ResStream_3 = false, itIsNotFirstValueFor_DebugStream = false;
	ParabolicApproximation parabolicAppr;

	cout << endl;

	if ( argc == 5 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		resFile_3 = argv[4];
		numOfDatumForPorabolaForMin = NumOfDatumForPorabolaForMin; // ����������� ���������� �������� �� ���������.
		numOfDatumForPorabolaForMax = NumOfDatumForPorabolaForMax; // ����������� ���������� �������� �� ���������.
	}
	else if ( argc == 6 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		resFile_3 = argv[4];
		numOfDatumForPorabolaForMin = atoi( argv[5] );
		numOfDatumForPorabolaForMax = numOfDatumForPorabolaForMin;
	}
	else if ( argc == 7 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		resFile_3 = argv[4];
		numOfDatumForPorabolaForMin = atoi( argv[5] );
		numOfDatumForPorabolaForMax = atoi( argv[6] );
		assert( ( numOfDatumForPorabolaForMin % 2 == 1 ) && ( numOfDatumForPorabolaForMin >= 3 ) );
		assert( ( numOfDatumForPorabolaForMax % 2 == 1 ) && ( numOfDatumForPorabolaForMax >= 3 ) );
		assert( numOfDatumForPorabolaForMin >= numOfDatumForPorabolaForMax );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != 5, 6, 7 )!!!" << endl;
		return 1;
	}

	arrayOfIndexesForMin = new double[numOfDatumForPorabolaForMin];
	arrayOfDatumForMin = new double[numOfDatumForPorabolaForMin];
	maxNumOfDatumToMinInArray = numOfDatumForPorabolaForMin / 2;
	for( int i = 0; i < numOfDatumForPorabolaForMin; i++ ) arrayOfIndexesForMin[i] = i;

	arrayOfIndexesForMax = new double[numOfDatumForPorabolaForMax];
	arrayOfDatumForMax = new double[numOfDatumForPorabolaForMax];
	maxNumOfDatumToMaxInArray = numOfDatumForPorabolaForMax / 2;
	for( int i = 0; i < numOfDatumForPorabolaForMax; i++ ) arrayOfIndexesForMax[i] = i;

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
		numOfPeriods = 0;
		numOfDatum = 1;		// �.�. ��������� ������ ���������� � 0 � 2 �������� �� ��� �������.
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
				// ����������� �������� ��������:
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;

					cout << "lastMin  =  " << lastMin << endl	// ��� �������
						<< "numOfLastMin  =  " << numOfLastMin << endl;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// ����������� �������� ���������:
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;

					cout << "lastMax  =  " << lastMax << endl	// ��� �������
						<< "numOfLastMax  =  " << numOfLastMax << endl;
				}

				//if( ( lastMin != MaxOfDouble ) && ( lastMax / lastMin >= MaxToMinRatio ) )
				//if( lastMax / lastMin >= MaxToMinRatio )
				//if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / value < MaxToMinRatio ) )
				/*
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio ) && ( lastMax / prevValue < MaxToMinRatio ) && ( lastMax / value < MaxToMinRatio ) )
					numOfFulfilledCondition++;
				else
					numOfFulfilledCondition = 0;
				if( numOfFulfilledCondition == NumOfRepetitionsOfFulfilledCondition )
				*/
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				/*
				if( ( lastMax / lastMin >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && 
					( lastMax / prevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && ( lastMax / value < MaxToMinRatio * FactorOfIntensificationCondition ) )
				*/
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( abs( lastMax / prevPrevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) && 
					( abs( lastMax / prevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) && ( abs( lastMax / value ) < MaxToMinRatio * FactorOfIntensificationCondition ) )
				{
					// ������� � ���������� �����:

					cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin 
						<< endl << endl << endl << endl; // ��� �������.

					// ������ ������ � ����, ����� ����� ������� �������� �������� ���������� ��������� ��� � ����� ���� �� ������� ����������� ���������:
					if( itIsNotFirstValueFor_ResStream_2 )
					{
						resStream_2 << endl << numOfLastMax << "	" << lastMax;
						resStream_2 << endl << numOfLastMin << "	" << lastMin;
					}
					else
					{
						itIsNotFirstValueFor_ResStream_2 = true;
						resStream_2 << numOfLastMax << "	" << lastMax;
						resStream_2 << endl << numOfLastMin << "	" << lastMin;
					}

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
		//if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		//if( ( numOfFulfilledCondition < NumOfRepetitionsOfFulfilledCondition ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		if( lastMax > MinimumMaximum )
		//if( ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		{
			// ��������� ��������� ����.
			cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value
				<< endl << endl << endl << endl; // ��� �������.

			// ��������� � ���� ������ � ��������� ���������:
			if( itIsNotFirstValueFor_ResStream_2 )
				resStream_2 << endl << numOfLastMax << "	" << lastMax;
			else
			{
				itIsNotFirstValueFor_ResStream_2 = true;
				resStream_2 << numOfLastMax << "	" << lastMax;
			}

			numOfPeriods++;
			averageMaximum += lastMax;
		}

		resStream_2.close();
	}
	dataStream.close();
	averageMaximum /= numOfPeriods;

	dataStream.open( dataFile );
	dataStream_2.open( resFile_2 );
	resStream_3.open( resFile_3 );
	debugStream.open( appDebugFile );
	if( !dataStream.fail() && !dataStream_2.fail() )
	{
		resStream.open( resFile );

		// ����� ��������� �� ������ ������ ������ 2 �������� ���� double:
		dataStream >> prevPrevValue >> prevValue;
		// ���������� �� � ����� resStream_3 ������ � �� ���������:
		if( itIsNotFirstValueFor_ResStream_3 )
			resStream_3 << endl << "0	" << prevPrevValue << endl
				<< "1	" << prevValue;
		else
		{
			itIsNotFirstValueFor_ResStream_3 = true;
			resStream_3 << "0	" << prevPrevValue << endl
				<< "1	" << prevValue;
		}
		// ��������� �������� ������ ���� �����������:
		dataStream_2 >> numOfCurrentMax >> currentMax;
		dataStream_2 >> numOfCurrentMin >> currentMin;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 0;
		numOfDatum = 1;		// �.�. ��������� ������ ���������� � 0 � 2 �������� �� ��� �������.

		// ���������� � �������������� ���� 2 ����������� ��������� ��������, ������ ���� ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToMaxInArray ):
		//if( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray )
		if( numOfCurrentMax - numOfDatum > maxNumOfDatumToMaxInArray )
		{
			if( itIsNotFirstValueFor_ResStream )
				resStream << endl << prevPrevValue << endl << prevValue;
			else
			{
				itIsNotFirstValueFor_ResStream = true;
				resStream << prevPrevValue << endl << prevValue;
			}
		}
		else
		{
			arrayOfDatumForMax[0] = prevPrevValue;
			arrayOfDatumForMax[1] = prevValue;
		}

		do{	
			// ��������� ��������� ���� �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;
				// ���������� � ����, ��������� � ������� resStream_3, ������ ���������� �������� � ���� ��� ��������:
				if( itIsNotFirstValueFor_ResStream_3 )
					resStream_3 << endl << numOfDatum << "	" << value;
				else
				{
					itIsNotFirstValueFor_ResStream_3 = true;
					resStream_3 << numOfDatum << "	" << value;
				}

				// ���������� � �������������� ���� ����������� ��������� ��������, ������ 
				// ���� ( ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToMaxInArray ) && ( abs( numOfCurrentMin - numOfDatum ) > maxNumOfDatumToMinInArray ) ):
				if( ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToMaxInArray ) && ( abs( numOfCurrentMin - numOfDatum ) > maxNumOfDatumToMinInArray ) )
				{
					if( itIsNotFirstValueFor_ResStream )
						resStream << endl << value;
					else
					{
						itIsNotFirstValueFor_ResStream = true;
						resStream << value;
					}
				}
				else
				{
				// ����� �� ��������� ��� � ����� ��������� ����� ��� � ����� �������� �����.
					// ���� �� ��������� � ����� ��������� �����:
					if( abs( numOfCurrentMax - numOfDatum ) <= maxNumOfDatumToMaxInArray )
					{
						// ��������� �������� ��������� � ������ arrayOfDatumForMax, ��� ������������ ���������� �������������� �������������:
						arrayOfDatumForMax[maxNumOfDatumToMaxInArray - ( numOfCurrentMax - numOfDatum )] = value;
						if( itIsNotFirstValueFor_DebugStream )
							debugStream << endl << numOfDatum << ":	" << value;
						else
						{
							itIsNotFirstValueFor_DebugStream = true;
							debugStream << numOfDatum << ":	" << value;
						}

						if( maxNumOfDatumToMaxInArray - ( numOfCurrentMax - numOfDatum ) == numOfDatumForPorabolaForMax - 1 ) // �.�. ���� ������ arrayOfDatumForMax ���������� ���������:
						{
							parabolicAppr.Init( arrayOfIndexesForMax, arrayOfDatumForMax, numOfDatumForPorabolaForMax );
							//parabolicAppr.Update_a_b_c_terms();
							//for( int i = 0; i < numOfDatumForPorabolaForMax; i++ ) resStream << parabolicAppr.PredictedY( i ) << endl;
							//for( int i = 0; i < numOfDatumForPorabolaForMax; i++ ) resStream << parabolicAppr.PredictedY2( i ) << endl;
							for( int i = 0; i < numOfDatumForPorabolaForMax; i++ ) 
							{
								predictedValue = parabolicAppr.PredictedY2( i );
								if( itIsNotFirstValueFor_ResStream )
									resStream << endl << predictedValue;
								else
								{
									itIsNotFirstValueFor_ResStream = true;
									resStream << predictedValue;
								}
								if( itIsNotFirstValueFor_DebugStream )
									debugStream << endl << i << ":	" << predictedValue;
								else
								{
									itIsNotFirstValueFor_DebugStream = true;
									debugStream << i << ":	" << predictedValue;
								}
							}
							debugStream << endl;
						}
					}
					// ���� �� ��������� � ����� �������� �����:
					else
					//if( abs( numOfCurrentMin - numOfDatum ) <= maxNumOfDatumToMinInArray )
					{
						// ��������� �������� ��������� � ������ arrayOfDatumForMin, ��� ������������ ���������� �������������� �������������:
						arrayOfDatumForMin[maxNumOfDatumToMinInArray - ( numOfCurrentMin - numOfDatum )] = value;
						if( itIsNotFirstValueFor_DebugStream )
							debugStream << endl << numOfDatum << ":	" << value;
						else
						{
							itIsNotFirstValueFor_DebugStream = true;
							debugStream << numOfDatum << ":	" << value;
						}

						if( ( maxNumOfDatumToMinInArray - ( numOfCurrentMin - numOfDatum ) == numOfDatumForPorabolaForMin - 1 ) ) // �.�. ���� ������ arrayOfDatumForMin ���������� ���������:
						{
							parabolicAppr.Init( arrayOfIndexesForMin, arrayOfDatumForMin, numOfDatumForPorabolaForMin );
							//parabolicAppr.Update_a_b_c_terms();
							//for( int i = 0; i < numOfDatumForPorabolaForMin; i++ ) resStream << parabolicAppr.PredictedY( i ) << endl;
							//for( int i = 0; i < numOfDatumForPorabolaForMin; i++ ) resStream << parabolicAppr.PredictedY2( i ) << endl;
							for( int i = 0; i < numOfDatumForPorabolaForMin; i++ )
							{
								predictedValue = parabolicAppr.PredictedY2( i );
								if( itIsNotFirstValueFor_ResStream )
									resStream << endl << predictedValue;
								else
								{
									itIsNotFirstValueFor_ResStream = true;
									resStream << predictedValue;
								}
								if( itIsNotFirstValueFor_DebugStream )
									debugStream << endl << i << ":	" << predictedValue;
								else
								{
									itIsNotFirstValueFor_DebugStream = true;
									debugStream << i << ":	" << predictedValue;
								}
							}
							debugStream << endl;

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
				//if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / value < MaxToMinRatio ) )
				/*
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio ) && ( lastMax / prevValue < MaxToMinRatio ) && ( lastMax / value < MaxToMinRatio ) )
					numOfFulfilledCondition++;
				else
					numOfFulfilledCondition = 0;
				if( numOfFulfilledCondition == NumOfRepetitionsOfFulfilledCondition )
				*/
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				if( ( lastMax / lastMin >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && 
					( lastMax / prevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && ( lastMax / value < MaxToMinRatio * FactorOfIntensificationCondition ) )
				{
					// ������� � ���������� �����.
					debugStream << endl;
					if( itIsNotFirstValueFor_DebugStream )
						debugStream << endl << "numOfPeriods  =  " << numOfPeriods;
					else
					{
						itIsNotFirstValueFor_DebugStream = true;
						debugStream << "numOfPeriods  =  " << numOfPeriods;
					}
					debugStream << endl << "LocalMax:  " << numOfLastMax << "     " << lastMax;
					debugStream << endl << "LocalMin:  " << numOfLastMin << "     " << lastMin;
					debugStream << endl << "lastMax / lastMin  =  " << lastMax / lastMin;
					debugStream << endl << endl;

					dataStream_2 >> numOfCurrentMax >> currentMax;
					numOfPeriods++;
					lastMin = MaxOfDouble;
					lastMax = MinOfDouble;
				}
				
				prevPrevValue = prevValue;
				prevValue = value;
			}
		} while( !dataStream.eof() );

		//if( ( lastMax / lastMin < MaxToMinRatio ) && ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		//if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		//if( ( numOfFulfilledCondition < NumOfRepetitionsOfFulfilledCondition ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		if( lastMax > MinimumMaximum )
		//if( ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		{
			// ��������� ��������� ����.
			if( itIsNotFirstValueFor_DebugStream )
				debugStream << endl << "numOfPeriods  =  " << numOfPeriods;
			else
			{
				itIsNotFirstValueFor_DebugStream = true;
				debugStream << "numOfPeriods  =  " << numOfPeriods;
			}
			debugStream << endl << "LocalMax:  " << numOfLastMax << "     " << lastMax;
			debugStream << endl << "lastMax / value  =  " << lastMax / value;
			//debugStream << endl << endl;

			numOfPeriods++;
		}

		resStream.close();
	}
	dataStream.close();
	dataStream_2.close();
	resStream_3.close();
	debugStream.close();
	delete( arrayOfDatumForMin );
	delete( arrayOfDatumForMax );

	numOfDatum++; // �.�. ��������� ������ ���������� � 0, �� ����� ����� ������ �� 1 ������, ��� ����� ���������� �������.
	cout << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numberOfPeriods  =  " << numOfPeriods << endl
		 << "averageMaximum  =  " << averageMaximum << endl;
/*		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
*/
	return 0;
}

template<typename Type> Type abs( Type a )
{
	return ( a > 0 )? a: -a;
}