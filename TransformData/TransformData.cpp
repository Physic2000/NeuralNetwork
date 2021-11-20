// ��� ��������� ��������� txt ����, ���������� ���� ������ ������ (����� ��������� ����� ���������� ���������), ��������� ��������� ������ �� ������ � �������� �� � ����������� ��������� ������, ������� �������� �
// ���� ������ ���������� �� 11-�� ������ ����� ������. ����� ��������� ��������� ������������������ �������� ����������� ��������� ������ (������ ������ - ���� ��������� ����) � ���������� ��� ������������������
// �������� � �������� ����. ������ ���������� ��������� ���� �������� ��� txt �����, ����������� ���� ������ ������. ���� ���� ����� ��������� ��� �������� ������, ��� ������ ������������ �������������� �������������.
// ������ ���������� ��������� ���� �������� ��� txt ����� � ������� ����� �������� ��������������� ������. ���� ��������������: ������ 11-�� ������ ���� �������� ����� STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE (= 132) 
// ��������������� ��������. ������� ���������� ��������� ���� �������� ��� ��������������� ��������� �����. �������� ���������� ��������� ���� �������� ��� txt ����� ��� ������ ����������� ���������� � ��������� ������.
// ����� ���������� ��������� ���� �������� ���-�� ������ ��������� ������, ������� ����� �������� � txt ����, ��� �������� ��������� ��������� ������ ����������.

#include "SolarDataStructures.h"

#include <assert.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <list>
#include <iterator>

using namespace std;
using namespace SolarDataStructures;

typedef DataOfSolarCycle* PtrToDataOfSolarCycle;

//const double MaxToMinRatio = 7.0;
const double MaxToMinRatio = 6.0;
const double FactorOfIntensificationCondition = 0.7;
//const double MinimumMaximum = 35.0;
const double MinimumMaximum = 0;
const double MinOfDouble = numeric_limits<double>::lowest();
const double MaxOfDouble = numeric_limits<double>::max();

// ��������� ���� �������� � ��������� ������:
// 1) ��� ��������� �����, ����������� ���� ������ ������ (����� ��������� ����� ���������� ���������), ������� ����� ������������		-	char* dataFile;
// 2) ��� ��������������� �����, � ������� ����� �������� ��������������� ������ �	��������� �������									-	char* resFile;
// 3) ��� ��������������� �����, � ������� ����� �������� ������������ � ����������������� ������ �	�������� �������					-	char* resFile_2;
// 4) ��� ��������������� txt ����� ��� ������ ����������� ���������� � ��������� ������												-	char* resFile_3;
// ������������� ��������� ����� �������� � ��������� ������:
// 5) ���-�� ������ ��������� ������, ������� ����� �������� � txt ����, ��� �������� ��������� ��������� ������ ����������			-	numOfSolarCycles;
int main( int argc, char* argv[] )
{
	char *dataFile, *resFile, *resFile_2, *resFile_3, *resFile_4, *filenameForAverageCurve, *filenameForMultipleRepeatedAveragedCycle, *filenameForMultipleRepeatedStandardDeviations;
	bool itIsNotFirstValueFor_ResStream = false, itIsNotFirstValueFor_ResStream_3 = false, itIsNotFirstValueFor_ResStream_4 = false, itIsNotFirstValueFor_AverageCurve = false, 
	itIsNotFirstValueFor_MultipleRepeatedAveragedCycle = false, itIsNotFirstValueFor_ForMultipleRepeatedStandardDeviations = false;
	int numOfDatum, numOfDatumInCurrentSolarCycle, numOfPrevMin, numOfLastMin, numOfLastMax, numOfLastMinInCurrentSolarCycle, numOfPeriod, numOfSolarCycles, numOfCyclesForAverageCurve;
	double value, prevValue, prevPrevValue, lastMin, lastMax, averageMaximum, averagePeriodSize, timeStepTranformed;
	string tempStr, str;
	ifstream dataStream;
	ofstream resStream, resStream_3, resStream_4, streamForAverageCurve, streamForMultipleRepeatedAveragedCycle, streamForMultipleRepeatedStandardDeviations;
	//DataOfSolarCycle *pCurrentSolarCycle;
	PtrToDataOfSolarCycle pCurrentSolarCycle;
	list<DataOfSolarCycle> doubleList, trimmedList;
	list<DataOfSolarCycle>::iterator currentIterator;
	FILE *pResFile_2;

	cout << endl;
	//resFile = "TransformedData_Ri.txt";
	resFile_4 = "TransformedData_Ri_FullInformation.txt";

	if( argc == 5 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];		// ���� ���������� �������� ������ � ��������� �������.		
		resFile = argv[2];		// ���� ��� ������ ��������������� ������ � ��������� �������.
		resFile_2 = argv[3];	// ���� ��� ������ ������������ � ����������������� ������ � �������� �������.
		resFile_3 = argv[4];	// ���� ��� ������ ����������� ���������� � ��������� ������.
		numOfSolarCycles = -1;
	}
	else if ( argc == 6 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];		// ���� ���������� �������� ������ � ��������� �������.		
		resFile = argv[2];		// ���� ��� ������ ��������������� ������ � ��������� �������.
		resFile_2 = argv[3];	// ���� ��� ������ ������������ � ����������������� ������ � �������� �������.
		resFile_3 = argv[4];	// ���� ��� ������ ����������� ���������� � ��������� ������.
		numOfSolarCycles = atoi( argv[5] );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: argc != 5, 6 !!!" << endl;
		return 1;
	}

	// ��������� ����� ��������� ��������� ������ �� ����� dataFile.
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		// ��������� �� ������ ������ ������ 2 �������� ���� double:
		dataStream >> prevPrevValue >> prevValue;

		//resStream_2.open( "TEMP.txt" );

		cout << endl;
		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriod = 0;
		numOfDatum = 1;						// �.�. ��������� ������ ���������� � 0 � 2 �������� �� ��� �������.
		numOfDatumInCurrentSolarCycle = 1;	// �.�. ��������� ������ � ��������� ������ ���������� ����� ���������� � 0 � 2 �������� �� ��� �������.
		averageMaximum = 0.0;
		averagePeriodSize = 0.0;

		//pCurrentSolarCycle = new DataOfSolarCycle();
		pCurrentSolarCycle = new DataOfSolarCycle;
		pCurrentSolarCycle->indexOfFirstDatumOfSolarCycle = 0;
		pCurrentSolarCycle->pointBasic[0] = prevPrevValue;
		pCurrentSolarCycle->pointBasic[1] = prevValue;
		do{	
			// ��������� ��������� ���� �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;
				numOfDatumInCurrentSolarCycle++;
				pCurrentSolarCycle->pointBasic[numOfDatumInCurrentSolarCycle] = value;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// ����������� �������� ��������:
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;	// ������������ ���� ����� ����� ��������� ������� ���������� �������� � ������� ��������� �����.
					numOfLastMinInCurrentSolarCycle = numOfDatumInCurrentSolarCycle - 1; // ������������ ���� ����� ����� ��������� ������� ���������� �������� � ������� ��������� �����.
					/*
					// ��� �������:
					cout << "lastMin  =  " << lastMin << endl
						<< "numOfLastMin  =  " << numOfLastMin << endl;
					*/
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// ����������� �������� ���������:
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;
					/* 
					// ��� �������:
					cout << "lastMax  =  " << lastMax << endl
						<< "numOfLastMax  =  " << numOfLastMax << endl;
					*/
				}

				//if( ( lastMin != MaxOfDouble ) && ( lastMax / lastMin >= MaxToMinRatio ) )
				//if( lastMax / lastMin >= MaxToMinRatio )
				//if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
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
				/*
				if( ( lastMax / lastMin >= MaxToMinRatio ) && ( lastMax / prevPrevValue < MaxToMinRatio * FactorOfIntensificationCondition ) &&
					( lastMax / prevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && ( lastMax / value < MaxToMinRatio * FactorOfIntensificationCondition ) )
				*/
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( abs( lastMax / prevPrevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) &&
					( abs( lastMax / prevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) && ( abs( lastMax / value ) < MaxToMinRatio * FactorOfIntensificationCondition ) )
				{
					// ������� � ���������� �����:

					cout << endl << "numOfPeriod  =  " << numOfPeriod << endl << "lastMax  =  " << lastMax << endl << "lastMax / lastMin  =  " << lastMax / lastMin << endl 
						<< "numOfValues = " << numOfLastMinInCurrentSolarCycle + 1 << endl << endl; // ��� �������.

					/*
					if( numOfPeriod == 0 ) numOfPrevMin = numOfLastMin;
					else
					{
						averagePeriodSize += numOfLastMin - numOfPrevMin;
						numOfPrevMin = numOfLastMin;
					}
					*/
					if( numOfPeriod > 0 )
						averagePeriodSize += numOfLastMin - numOfPrevMin;
					numOfPrevMin = numOfLastMin;

					pCurrentSolarCycle->numOfSolarCycle = numOfPeriod;
					pCurrentSolarCycle->numOfValues = numOfLastMinInCurrentSolarCycle + 1; // ��������� ����� �������� � ������� ����� ��������� ���������� �� 1-�� ������, ��� ����� ���������� �������� � �����, �.�. ��������� ���� �������� ���������� � 0.
					pCurrentSolarCycle->solarCycleCompleted = true;
					//pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )numOfDatumInCurrentSolarCycle / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;	// � ������� ������� ��� ������, ��� ���������!!!
					pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )pCurrentSolarCycle->numOfValues / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;
					pCurrentSolarCycle->maxOfValue = lastMax;

					//resStream_2 << "numOfSolarCycle = " << pCurrentSolarCycle->numOfSolarCycle << endl;		// ��� �������.

					//for( int i = 0; i < numOfDatumInCurrentSolarCycle; i++ )	// � ������� ������� ��� ������, ��� ���������!!!
					for( int i = 0; i < pCurrentSolarCycle->numOfValues; i++ )
					{
						pCurrentSolarCycle->pointNormalized[i] = pCurrentSolarCycle->pointBasic[i] / lastMax;

						//resStream_2 << i << ":	" << pCurrentSolarCycle->pointBasic[i] << endl;		// ��� �������.
					}
					//resStream_2 << endl;	// ��� �������.
					for( int i = 0; i < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; i++ )
					{
						double y1, y2, x1, x2, x;
						if( i == 0 ) pCurrentSolarCycle->pointTransformed[0] = pCurrentSolarCycle->pointNormalized[0];
						else	// if( i > 0 )
						{
							x = i * timeStepTranformed;
							x1 = floor( x );
							x2 = ceil( x );
							if( x1 != x2 )
							{
								//y1 = pCurrentSolarCycle->pointNormalized[int( x1 )];
								//y2 = pCurrentSolarCycle->pointNormalized[int( x2 )];
								y1 = pCurrentSolarCycle->pointNormalized[( int )x1];
								y2 = pCurrentSolarCycle->pointNormalized[( int )x2];
								pCurrentSolarCycle->pointTransformed[i] = y1 + ( x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
							}
							else
								pCurrentSolarCycle->pointTransformed[i] = pCurrentSolarCycle->pointNormalized[( int )x1];  // �.�. � ���� ������: pCurrentSolarCycle->pointTransformed[i] = y1;

							// ��� �����:
							assert( ( int )x1 < pCurrentSolarCycle->numOfValues );	// ��� ������� � ��������� ���������� ��������� �� ������ ������� ���������!!
							//cout << i << ":	" << pCurrentSolarCycle->pointTransformed[i] << endl;
						}
					}
					doubleList.push_back( *pCurrentSolarCycle );
					cout << endl << endl;

					//numOfDatumInCurrentSolarCycle = 0;	// ��� �� �����, �.�. ( numOfDatumInCurrentSolarCycle - numOfLastMinInCurrentSolarCycle ) �������� ������ ����� �� ��� �������.
					numOfDatumInCurrentSolarCycle = numOfDatumInCurrentSolarCycle - numOfLastMinInCurrentSolarCycle;
					numOfPeriod++;
					averageMaximum += lastMax;
					lastMin = MaxOfDouble;
					lastMax = MinOfDouble;
					//pCurrentSolarCycle = new DataOfSolarCycle();
					pCurrentSolarCycle = new DataOfSolarCycle;
					pCurrentSolarCycle->indexOfFirstDatumOfSolarCycle = numOfLastMin + 1;
					memcpy( pCurrentSolarCycle->pointBasic, doubleList.back().pointBasic + doubleList.back().numOfValues, numOfDatumInCurrentSolarCycle * sizeof( double ) );
					memset( doubleList.back().pointBasic + doubleList.back().numOfValues, 0, numOfDatumInCurrentSolarCycle * sizeof( double ) );
					numOfDatumInCurrentSolarCycle--;
				}

				prevPrevValue = prevValue;
				prevValue = value;
			}
		} while( !dataStream.eof() );

		//if( ( lastMax / lastMin < MaxToMinRatio ) && ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		//if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		//if( ( numOfFulfilledCondition < NumOfRepetitionsOfFulfilledCondition ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		//if( lastMax / lastMin >= MaxToMinRatio )
		if( abs( lastMax / lastMin ) >= MaxToMinRatio )
		{
			// ��������� ��������� ����������� ����, ������� � ������� �������� ����� ��� �������� �������:
			cout << endl << "------------------------------------------------------------------------------------------------" << endl;
			cout << "numOfPeriod  =  " << numOfPeriod << endl << "lastMax  =  " << lastMax << endl << "lastMax / lastMin  =  " << lastMax / lastMin << endl
				<< "numOfValues = " << numOfLastMinInCurrentSolarCycle + 1 << endl << endl; // ��� �������.

			averagePeriodSize += numOfLastMin - numOfPrevMin;
			averagePeriodSize /= numOfPeriod;
			numOfPrevMin = numOfLastMin;

			pCurrentSolarCycle->numOfSolarCycle = numOfPeriod;
			pCurrentSolarCycle->numOfValues = numOfLastMinInCurrentSolarCycle + 1; // ��������� ����� �������� � ������� ����� ��������� ���������� �� 1-�� ������, ��� ����� ���������� �������� � �����, �.�. ��������� ���� �������� ���������� � 0.
			pCurrentSolarCycle->solarCycleCompleted = true;
			//pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )numOfDatumInCurrentSolarCycle / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;	// � ������� ������� ��� ������, ��� ���������!!!
			pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )pCurrentSolarCycle->numOfValues / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;
			pCurrentSolarCycle->maxOfValue = lastMax;
			/*
			cout << "pCurrentSolarCycle->timeStepTranformed = " << pCurrentSolarCycle->timeStepTranformed << endl 
				<< "pCurrentSolarCycle->maxOfValue = " << pCurrentSolarCycle->maxOfValue << endl << endl;
			*/
			//for( int i = 0; i < numOfDatumInCurrentSolarCycle; i++ )	// � ������� ������� ��� ������, ��� ���������!!!
			for( int i = 0; i < pCurrentSolarCycle->numOfValues; i++ )
				pCurrentSolarCycle->pointNormalized[i] = pCurrentSolarCycle->pointBasic[i] / lastMax;
			for( int i = 0; i < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; i++ )
			{
				double y1, y2, x1, x2, x;
				if( i == 0 ) pCurrentSolarCycle->pointTransformed[0] = pCurrentSolarCycle->pointNormalized[0];
				else	// if( i > 0 )
				{
					x = i * timeStepTranformed;
					x1 = floor( x );
					x2 = ceil( x );
					if( x1 != x2 )
					{
						//y1 = pCurrentSolarCycle->pointNormalized[int( x1 )];
						//y2 = pCurrentSolarCycle->pointNormalized[int( x2 )];
						y1 = pCurrentSolarCycle->pointNormalized[( int )x1];
						y2 = pCurrentSolarCycle->pointNormalized[( int )x2];
						pCurrentSolarCycle->pointTransformed[i] = y1 + ( x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
					}
					else
						pCurrentSolarCycle->pointTransformed[i] = pCurrentSolarCycle->pointNormalized[( int )x1];  // �.�. � ���� ������: pCurrentSolarCycle->pointTransformed[i] = y1;

					// ��� �����:
					assert( ( int )x1 < pCurrentSolarCycle->numOfValues );	// ��� ������� � ��������� ���������� ��������� �� ������ ������� ���������!!
					//cout << i << ":	" << pCurrentSolarCycle->pointTransformed[i] << endl;
				}
			}
			doubleList.push_back( *pCurrentSolarCycle );

			// ��� �������:
			cout << endl << "doubleList.size()  =  " << doubleList.size() << endl << endl; // ��� �������.

			// ��������� ����� ����, ������� �������� �������:
			//numOfDatumInCurrentSolarCycle = 0;	// ��� �� �����, �.�. ( numOfDatumInCurrentSolarCycle - numOfLastMinInCurrentSolarCycle ) �������� ������ ����� �� ��� �������.
			numOfDatumInCurrentSolarCycle = numOfDatumInCurrentSolarCycle - numOfLastMinInCurrentSolarCycle;
			numOfPeriod++;
			averageMaximum += lastMax;
			//lastMin = MaxOfDouble;
			//lastMax = MinOfDouble;
			//pCurrentSolarCycle = new DataOfSolarCycle();
			pCurrentSolarCycle = new DataOfSolarCycle;
			pCurrentSolarCycle->numOfSolarCycle = numOfPeriod;
			pCurrentSolarCycle->indexOfFirstDatumOfSolarCycle = numOfLastMin + 1;
			pCurrentSolarCycle->numOfValues = numOfDatumInCurrentSolarCycle;
			memcpy( pCurrentSolarCycle->pointBasic, doubleList.back().pointBasic + doubleList.back().numOfValues, numOfDatumInCurrentSolarCycle * sizeof( double ) );
			memset( doubleList.back().pointBasic + doubleList.back().numOfValues, 0, numOfDatumInCurrentSolarCycle * sizeof( double ) );
			numOfDatumInCurrentSolarCycle--;
			// ��������� � ������ ������ ������ ���������� �����:
			doubleList.push_back( *pCurrentSolarCycle );
			cout << "doubleList.size()  =  " << doubleList.size() << endl << endl; // ��� �������.

			cout << endl << endl;
		}
		else
		{
			// ��������� ��������� ����, ������� ��� �� ��������:
			cout << endl << "------------------------------------------------------------------------------------------------" << endl;
			cout << endl << "numOfPeriod  =  " << numOfPeriod << endl << "lastMax / value  =  " << lastMax / value << endl
				<< "lastMax = " << lastMax << endl << "numOfValues = " << numOfDatumInCurrentSolarCycle + 1 << endl << endl << endl << endl; // ��� �������.

			pCurrentSolarCycle->numOfSolarCycle = numOfPeriod;
			//pCurrentSolarCycle->numOfValues = numOfLastMinInCurrentSolarCycle + 1; // ������ � ���� ����� ���� �������� ��������!!!
			pCurrentSolarCycle->numOfValues = numOfDatumInCurrentSolarCycle + 1;
			//pCurrentSolarCycle->solarCycleCompleted = false; //	������ ��������, �.�. ��� �������� ������������� �� ��������� � ������������ DataOfSolarCycle().
			if( lastMax > MinimumMaximum )
			{
				pCurrentSolarCycle->maxOfValue = lastMax;
				for( int i = 0; i < pCurrentSolarCycle->numOfValues; i++ )
					pCurrentSolarCycle->pointNormalized[i] = pCurrentSolarCycle->pointBasic[i] / lastMax;
				/*
				if( pCurrentSolarCycle->numOfValues > MINIMUM_NUM_OF_DATUM_IN_SOLAR_CYCLE )
				{
					pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )pCurrentSolarCycle->numOfValues / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; // ��� ������ ����� �� ���������, �.�. ���� ��� �� ��������!!!!!
					for( int i = 0; i < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; i++ )
					{
						double y1, y2, x1, x2, x;
						if( i == 0 ) pCurrentSolarCycle->pointTransformed[0] = pCurrentSolarCycle->pointNormalized[0];
						else	// if( i > 0 )
						{
							x = i * timeStepTranformed;
							x1 = floor( x );
							x2 = ceil( x );
							if( x1 != x2 )
							{
								//y1 = pCurrentSolarCycle->pointNormalized[int( x1 )];
								//y2 = pCurrentSolarCycle->pointNormalized[int( x2 )];
								y1 = pCurrentSolarCycle->pointNormalized[( int )x1];
								y2 = pCurrentSolarCycle->pointNormalized[( int )x2];
								pCurrentSolarCycle->pointTransformed[i] = y1 + ( x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
							}
							else
								pCurrentSolarCycle->pointTransformed[i] = pCurrentSolarCycle->pointNormalized[( int )x1];  // �.�. � ���� ������: pCurrentSolarCycle->pointTransformed[i] = y1;

							// ��� �����:
							//cout << "i  =  " << i << "	" << "x1  =  " << x1 << "	" << "x2  =  " << x2 << endl;
							//cout << "pCurrentSolarCycle->numOfValues  =  " << pCurrentSolarCycle->numOfValues << endl;
							assert( ( int )x1 < pCurrentSolarCycle->numOfValues );	// ��� ������� � ��������� ���������� ��������� �� ������ ������� ���������!!
							//cout << i << ":	" << pCurrentSolarCycle->pointTransformed[i] << endl;
						}
					}
				}
				*/
				pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = 1.0;

			}
			doubleList.push_back( *pCurrentSolarCycle );

			averagePeriodSize /= numOfPeriod;
			numOfPeriod++;
			averageMaximum += lastMax;
		}
	}
	else
	{
		cout << "ERROR: can't open file " << dataFile << endl;
		return 1;
	}
	dataStream.close();
	//resStream_2.close();
	averageMaximum /= numOfPeriod;

	resStream.open( resFile );		// ��������� ���� ��� ������ � ���� ��������������� ������.
	resStream_3.open( resFile_3 );	// ��������� ���� ��� ������ � ���� ������������������ �������� ������.
	resStream_4.open( resFile_4 );	// ��������� ���� ��� ������ � ���� ������������������ ��������������� ������.
	int globalIndex = 0;
	int globalIndexForTransformedData = 0;
	numOfCyclesForAverageCurve = 0;
	int i = 0;
	for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
	{
		//if( currentIterator == doubleList.begin() ) cout << "timeStepTranformed:" << endl << endl;
		if( i == 0 ) cout << "timeStepTranformed:" << endl << endl;

		cout << i << ":	CycleCompleted = " << ( *currentIterator ).solarCycleCompleted 
		//<< ";  timeStepTranformed = " << ( *currentIterator ).timeStepTranformed
		<< ";  numOfValues = " << ( *currentIterator ).numOfValues
		<< ";  indexOfFirstFilteredDatum = " << ( *currentIterator ).indexOfFirstFilteredDatum
		<< ";  indexOfFirstDatumOfSolarCycle = " << ( *currentIterator ).indexOfFirstDatumOfSolarCycle
		<< ";  maxOfValue = " << ( *currentIterator ).maxOfValue 
		<< endl;

		// **********	������ ��� �������� �������� ���� ��� ������������ ��������� ������������������ ������:		**********
		if( numOfSolarCycles != -1 )
		{
			if( ( ( *currentIterator ).numOfValues > MINIMUM_NUM_OF_DATUM_IN_SOLAR_CYCLE ) && ( i <= numOfSolarCycles ) )
			{
				numOfCyclesForAverageCurve++;
				pCurrentSolarCycle = new DataOfSolarCycle( *currentIterator );
				trimmedList.push_back( *pCurrentSolarCycle );
				for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
				{
					if( itIsNotFirstValueFor_ResStream )
						resStream << endl << ( *currentIterator ).pointTransformed[k];
					else
					{
						itIsNotFirstValueFor_ResStream = true;
						resStream << ( *currentIterator ).pointTransformed[k];
					}
					if( itIsNotFirstValueFor_ResStream_4 )
						resStream_4 << endl << globalIndexForTransformedData << "	" << ( *currentIterator ).numOfSolarCycle << "	" << k << "	" << ( *currentIterator ).pointTransformed[k];
					else
					{
						itIsNotFirstValueFor_ResStream_4 = true;
						resStream_4 << globalIndexForTransformedData << "	" << ( *currentIterator ).numOfSolarCycle << "	" << k << "	" << ( *currentIterator ).pointTransformed[k];
					}
					globalIndexForTransformedData++;
				}
			}
		}

		// **********	������ ��� �������� �������� ���� ��� ������������ ������ ������������������ ������:		**********
		else
		{
			// ��������� ��� ������ �� ����������� ������ ������� ���������� ����� (����������� �� ���-�� ������):
			if( ( *currentIterator ).numOfValues > MINIMUM_NUM_OF_DATUM_IN_SOLAR_CYCLE )
			{
				numOfCyclesForAverageCurve++;
				pCurrentSolarCycle = new DataOfSolarCycle( *currentIterator );
				trimmedList.push_back( *pCurrentSolarCycle );
				for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
				{
					if( itIsNotFirstValueFor_ResStream )
						resStream << endl << ( *currentIterator ).pointTransformed[k];
					else
					{
						itIsNotFirstValueFor_ResStream = true;
						resStream << ( *currentIterator ).pointTransformed[k];
					}
					if( itIsNotFirstValueFor_ResStream_4 )
						resStream_4 << endl << globalIndexForTransformedData << "	" << ( *currentIterator ).numOfSolarCycle << "	" << k << "	" << ( *currentIterator ).pointTransformed[k];
					else
					{
						itIsNotFirstValueFor_ResStream_4 = true;
						resStream_4 << globalIndexForTransformedData << "	" << ( *currentIterator ).numOfSolarCycle << "	" << k << "	" << ( *currentIterator ).pointTransformed[k];
					}
					globalIndexForTransformedData++;
				}
			}
		}

		for( int k = 0; k < ( *currentIterator ).numOfValues; k++ )
		{
			if( itIsNotFirstValueFor_ResStream_3 )
				resStream_3 << endl << globalIndex << "	" << ( *currentIterator ).numOfSolarCycle << "	" << k << "	" << ( *currentIterator ).pointBasic[k];
			else
			{
				itIsNotFirstValueFor_ResStream_3 = true;
				resStream_3 << globalIndex << "	" << ( *currentIterator ).numOfSolarCycle << "	" << k << "	" << ( *currentIterator ).pointBasic[k];
			}
			globalIndex++;
		}

		i++;
	}
	resStream.close();
	resStream_3.close();
	resStream_4.close();
	assert( numOfCyclesForAverageCurve == trimmedList.size() );

	// ������ ������ �������� � ���� � �������� �������:
	if( ( pResFile_2 = fopen( resFile_2, "wb" ) ) == NULL )
	{
		printf( "%s %s", "Cannot open file: ", resFile_2 );
		return 1;
	}
	for( currentIterator = trimmedList.begin(); currentIterator != trimmedList.end(); ++currentIterator )
		//fwrite( &( *currentIterator ), sizeof( struct DataOfSolarCycle ), 1, pResFile_2 );
		fwrite( &( *currentIterator ), sizeof( DataOfSolarCycle ), 1, pResFile_2 );
	fclose( pResFile_2 );

	SequenceOfSolarCycles seq( trimmedList );

	// ���������� ����������� ����� � txt �����:
	filenameForAverageCurve = "AverageCurve.txt";
	streamForAverageCurve.open( filenameForAverageCurve );
	for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
	{
		if( itIsNotFirstValueFor_AverageCurve )
			streamForAverageCurve << endl << seq.pointOfAverageCurve[k];
		else
		{
			itIsNotFirstValueFor_AverageCurve = true;
			streamForAverageCurve << seq.pointOfAverageCurve[k];
		}
	}
	streamForAverageCurve.close();

	// ������ � ��������� ����� ����������� ����������� ����������� ����� � ������������ ���������� ��� ������ ����� ����������� �����. ��� ���� ���-�� �� ���������� ����� ���-�� ��������� ������ ���������� �
	// ��������� ���� resFile.
	filenameForMultipleRepeatedAveragedCycle = "MultipleRepeatedAveragedCycle.txt";
	filenameForMultipleRepeatedStandardDeviations = "MultipleRepeatedStdDeviations.txt";
	streamForMultipleRepeatedAveragedCycle.open( filenameForMultipleRepeatedAveragedCycle );
	streamForMultipleRepeatedStandardDeviations.open( filenameForMultipleRepeatedStandardDeviations );
	i = 0;
	do{
		for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
		{
			if( itIsNotFirstValueFor_MultipleRepeatedAveragedCycle )
				streamForMultipleRepeatedAveragedCycle << endl << seq.pointOfAverageCurve[k];
			else
			{
				itIsNotFirstValueFor_MultipleRepeatedAveragedCycle = true;
				streamForMultipleRepeatedAveragedCycle << seq.pointOfAverageCurve[k];
			}
			if( itIsNotFirstValueFor_ForMultipleRepeatedStandardDeviations )
				streamForMultipleRepeatedStandardDeviations << endl << seq.standardDeviation[k];
			else
			{
				itIsNotFirstValueFor_ForMultipleRepeatedStandardDeviations = true;
				streamForMultipleRepeatedStandardDeviations << seq.standardDeviation[k];
			}
		}

		i++;
	}while( i < numOfCyclesForAverageCurve );
	streamForMultipleRepeatedAveragedCycle.close();
	streamForMultipleRepeatedStandardDeviations.close();

	numOfDatum++; // �.�. ��������� ������ ���������� � 0, �� ����� ����� ������ �� 1 ������, ��� ����� ���������� �������.
	cout << endl << "================================================================================================" << endl;
	cout << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numberOfPeriods  =  " << numOfPeriod << endl
		 << "averageMaximum  =  " << averageMaximum << endl
		 << "averagePeriodSize  =  " << averagePeriodSize << endl << endl
		 << "numOfCyclesForAverageCurve  =  " << numOfCyclesForAverageCurve << endl;
/*		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
*/
	return 0;
}