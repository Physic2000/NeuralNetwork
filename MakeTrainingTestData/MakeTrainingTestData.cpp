// ��������� ��������� ������ �� ������������� � �������� � ������� �� �� ���-�� ������� ������ � ��������� ���� ������.
// �������� ������ ��� ��� ������ ��������, ��� ���� ���� ����� ���-�� ������ n, �� ��������� n �������� � ������������� ����
// ������ ��������� � ������� n ���������� � �������� ����.

#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#define FIRST_VARIANT

#if( !defined( FIRST_VARIANT ) )
	#define SECOND_VARIANT

//const int NumOfTestValues = 33;
//const int NumOfTestValues = 31;
//const int NumOfTestValues = 10;
const int NumOfTestValues = 9;
//const int NumOfTestValues = 6;
#endif

//const int NumOfRepetitionsOfFulfilledCondition = 2; // ���-�� ������ ������ �������� ������������ ������� ��� �������� ����� ������.

//const double MaxToMinRatio = 7.7;
const double MaxToMinRatio = 7.0;
const double FactorOfIntensificationCondition = 0.7;
//const double MinimumMaximum = 35.0;
const double MinimumMaximum = 0;
const double MinOfDouble = numeric_limits<double>::lowest();
const double MaxOfDouble = numeric_limits<double>::max();

// ��������� ���� �������� � ��������� ������: 
// 1) ��� ��������� ����� � �������, ������� ����� ��������� �� ������������� � ��������	-	dataFile;
// 2) ��� �����, � ������� ����� �������� ������������� ������								-	trainingData;
// 3) ��� �����, � ������� ����� �������� �������� ������									-	testData;
// 4) ����������� �������� �������, �.�. ����� �������� �������� ����						-	n;
int main( int argc, char* argv[] )
{
	char *dataFile, *trainingData, *testData, *stdDeviationsFile, *tempDataFile, *resultedStdDeviationsFile;
	int numOfTrainigValues, numOfTestValues, n, k, numOfDatum, numOfLastMin, numOfLastMax, numOfPeriods, numOfFulfilledCondition, numOfPrevMin, numOfCurrentMin, numOfValuesInDataCycle;
	double value, prevValue, prevPrevValue, stdDeviation, prevStdDeviation, prevPrevStdDeviation, lastMin, lastMax, currentValue, *arrayOfValues, *arrayOfStdDeviations;
	string tempStr, tempStr2, str, str2;
	ifstream dataStream, stdDeviationsStream;
	ofstream trainingStream, testStream, tempDataStream, resultedStdDeviationsStream;

	if ( argc == 5 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];
		trainingData = argv[2];
		testData = argv[3];
		n = atoi( argv[4] );
	}
	else if( argc == 8 )
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];
		trainingData = argv[2];
		testData = argv[3];
		stdDeviationsFile = argv[4];
		tempDataFile = argv[5];
		resultedStdDeviationsFile = argv[6];
		n = atoi( argv[7] );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line!!!" << endl;
		return 1;
	}

	if ( argc == 5 )	// if ( argc == 5 )
	{
		// ��������� ����� 2 ���� ��������� ��������� ������ �� ����� dataFile.
		dataStream.open( dataFile );
		if( !dataStream.fail() )	// if( !dataStream.fail() )
		{
			// ��������� �� ������ ������ ������ 2 �������� ���� double:
			dataStream >> prevPrevValue >> prevValue;

			cout << endl;
			lastMin = MaxOfDouble;
			lastMax = MinOfDouble;
			numOfPrevMin = 0;
			numOfCurrentMin = 0;
			numOfLastMin = 0;
			numOfLastMax = 0;
			numOfPeriods = 0;
			numOfDatum = 1;					// �.�. ��������� ������ ���������� � 0 � 2 �������� �� ��� �������.
			do{		// do{
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

						if( numOfPrevMin == 0 )
						{
							numOfPrevMin = numOfLastMin;		// ��������� ������������� ���������� numOfPrevMin.
							numOfCurrentMin = numOfLastMin;		// ��������� ������������� ���������� numOfCurrentMin.
						}

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
				
					//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio ) && ( lastMax / prevValue < MaxToMinRatio ) && ( lastMax / value < MaxToMinRatio ) )
					//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax / prevPrevValue < MaxToMinRatio ) && ( lastMax / prevValue < MaxToMinRatio ) && ( lastMax / value < MaxToMinRatio ) )
					/*
					if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( abs( lastMax / prevPrevValue ) < MaxToMinRatio ) && ( abs( lastMax / prevValue ) < MaxToMinRatio ) && ( abs( lastMax / value ) < MaxToMinRatio ) )
						numOfFulfilledCondition++;
					else
						numOfFulfilledCondition = 0;
					if( numOfFulfilledCondition == NumOfRepetitionsOfFulfilledCondition )
					*/
					//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
					if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( abs( lastMax / prevPrevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) &&
						( abs( lastMax / prevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) && ( abs( lastMax / value ) < MaxToMinRatio * FactorOfIntensificationCondition ) )
					{
						// ������� � ���������� �����:

						numOfPrevMin = numOfCurrentMin;
						numOfCurrentMin = numOfLastMin;
						numOfValuesInDataCycle = numOfCurrentMin - numOfPrevMin;
						if( numOfPeriods == 0 ) numOfValuesInDataCycle = numOfCurrentMin + 1;

						cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin << endl << "numOfValuesInDataCycle  =  " << numOfValuesInDataCycle << endl
							<< endl << endl << endl << endl; // ��� �������.

						numOfPeriods++;
						lastMin = MaxOfDouble;
						lastMax = MinOfDouble;
						/*
						if( ( numOfPrevMin == 0 ) && ( numOfLastMin != 0 ) )
						{
							numOfPrevMin = numOfLastMin;	// ��������� ������������� ���������� numOfPrevMin.
						}
						if( numOfPrevMin != 0 ) numOfPrevMin = numOfCurrentMin;
						if( numOfLastMin != 0 ) numOfCurrentMin = numOfLastMin;
						*/
					}
				
					prevPrevValue = prevValue;
					prevValue = value;
				}								// if( tempStr.size() > 0 )!!!
			} while( !dataStream.eof() );	// do{!!!
			dataStream.close();

			//if( ( lastMax / lastMin < MaxToMinRatio ) && ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			//if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			//if( ( numOfFulfilledCondition < NumOfRepetitionsOfFulfilledCondition ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			if( ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			{
				// ��������� ��������� ����.
				numOfPrevMin = numOfCurrentMin;
				numOfValuesInDataCycle = numOfDatum - numOfPrevMin;

				cout << endl << "------------------------------------------------------------------------------------------------" << endl;
				cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value << endl << "numOfValuesInDataCycle  =  " << numOfValuesInDataCycle << endl
					<< endl << endl << endl << endl; // ��� �������.

				numOfPeriods++;
			}
			numOfDatum++;								// �.�. ��������� ������ ���������� � 0, �� ����� ����� ������ �� 1 ������, ��� ����� ���������� �������.

#if( defined( FIRST_VARIANT ) )
			//numOfTrainigValues = numOfPrevMin + 1;	// ��� ������, ����� ��������� ������ � ��������� ������ ������ �������� ������ ������� ���������� �����.
			numOfTrainigValues = numOfPrevMin;			// ��� ������, ����� �������� � ������ �������� ���������� ����� ������������ � 1-�� ������������ ��� ������.
			//numOfTrainigValues = numOfPrevMin - 1;
			//numOfTrainigValues = numOfPrevMin - 3;
			//numOfTrainigValues = numOfPrevMin - 10;
			//numOfTrainigValues = numOfPrevMin - 13;	// ��� ������, ����� ����� ��������� ������� ���������� 1.5 11-�� ������ �����.
			//numOfTrainigValues = numOfPrevMin - 26;	// ��� ������, ����� ����� ��������� ������� ���������� 2 11-�� ������ �����, ������� ����� 25 ���������� (��� �������������� ���������� ���������� ������: DataFilter Data_Ri.txt FilteredData_Ri.txt 5 0 0 1 0 0).
			//numOfTrainigValues = numOfDatum - 528;
			//numOfTrainigValues = numOfDatum - 132;

			numOfTestValues = numOfDatum - numOfTrainigValues + n;	// ��� ��������� �������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.
#elif( defined( SECOND_VARIANT ) )
			numOfTestValues = NumOfTestValues;
			numOfTrainigValues = numOfDatum - numOfTestValues + n;  // ��� ��������� �������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.
#endif

			dataStream.open( dataFile );
			trainingStream.open( trainingData );
			testStream.open( testData );

			arrayOfValues = new double[n];
			k = 0;
			while( k < numOfTrainigValues )
			{	
				// ������ ������������� ������ � ����.
				// ���������� ������ �� ����� ������:
				getline( dataStream, tempStr );
				istringstream str( tempStr );
				str >> currentValue;

				// ������ ���������� �������� � ���� �������� ������������� ��������:
				trainingStream << currentValue << endl;

				// ���������� ��������� n �������� �� �������������� ���� � ������:
				if ( k >= numOfTrainigValues - n )
					arrayOfValues[k - ( numOfTrainigValues - n )] = currentValue;

				k++;
			}
		
			// ������ ������ n �������� ��������� � ��������������� ����:
			for( k = 0; k < n; k++ )
			{
				testStream << arrayOfValues[k] << endl;
				cout << k + 1 << ":	" << arrayOfValues[k] << endl; // ��� �����.
			}

			k = n;
			// ������ �������� ������ � ��������������� ����.
			do{	
				// ���������� ������ �� ����� ������:
		        getline( dataStream, tempStr );
				if( tempStr.size() )
				{											// if( !dataStream.fail() )!!!
					istringstream str( tempStr );
					str >> currentValue;
					// ������ ���������� �������� � ��������������� ����:
					testStream << currentValue << endl;
					cout << currentValue << endl; // ��� �����.
					k++;
				}											// if( tempStr.size() > 0 )!!!
			} while( !dataStream.eof() );
			assert( k == numOfDatum - numOfTrainigValues + n ); // ( k == numOfTestValues ) <-- �������� ���������� ���������, ����������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.

			trainingStream.close();
			testStream.close();
			dataStream.close();
		}						// if( !dataStream.fail() )!!!
		else
		{
			cout << "ERROR: the program "<< argv[0] << " cannot open the file \"" << dataFile << "\"" << endl;
			return 1;
		}
	}			// if ( argc == 5 )!!!
	else		// if( argc == 8 )
	{
		// ��������� ����� �� 2 ���� ��������� ��������� ������ �� ������ dataFile � stdDeviationsFile.
		dataStream.open( dataFile );					stdDeviationsStream.open( stdDeviationsFile );
		if( !dataStream.fail() && !stdDeviationsStream.fail() )
		{
			// ��������� �� ������� ������ �� 2 ������ �������� ���� double:
			dataStream >> prevPrevValue >> prevValue;	stdDeviationsStream >> prevPrevStdDeviation >> prevStdDeviation;

			cout << endl;
			lastMin = MaxOfDouble;
			lastMax = MinOfDouble;
			numOfPrevMin = 0;
			numOfCurrentMin = 0;
			numOfLastMin = 0;
			numOfLastMax = 0;
			numOfPeriods = 0;
			numOfDatum = 1;					// �.�. ��������� ������ ���������� � 0 � 2 �������� �� ��� �������.
			do{	
				// ��������� ��������� ��� ����� �� �����.
				getline( dataStream, tempStr );		getline( stdDeviationsStream, tempStr2 );
				assert( ( ( tempStr.size() == 0 ) && ( tempStr2.size() == 0 ) ) || ( ( tempStr.size() > 0 ) && ( tempStr2.size() > 0 ) ) );
				if( tempStr.size() )						// if( tempStr.size() > 0 )
				{
					istringstream str( tempStr );	istringstream str2( tempStr2 );
					str >> value;					str2 >> stdDeviation;
					numOfDatum++;

					if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
					// ����������� �������� ��������:
					{
						lastMin = prevValue;
						numOfLastMin = numOfDatum - 1;

						if( numOfPrevMin == 0 )
						{
							numOfPrevMin = numOfLastMin;		// ��������� ������������� ���������� numOfPrevMin.
							numOfCurrentMin = numOfLastMin;		// ��������� ������������� ���������� numOfCurrentMin.
						}

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
			
					//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio ) && ( lastMax / prevValue < MaxToMinRatio ) && ( lastMax / value < MaxToMinRatio ) )
					//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax / prevPrevValue < MaxToMinRatio ) && ( lastMax / prevValue < MaxToMinRatio ) && ( lastMax / value < MaxToMinRatio ) )
					/*
					if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( abs( lastMax / prevPrevValue ) < MaxToMinRatio ) && ( abs( lastMax / prevValue ) < MaxToMinRatio ) && ( abs( lastMax / value ) < MaxToMinRatio ) )
						numOfFulfilledCondition++;
					else
						numOfFulfilledCondition = 0;
					if( numOfFulfilledCondition == NumOfRepetitionsOfFulfilledCondition )
					*/
					//if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
					if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( abs( lastMax / prevPrevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) &&
						( abs( lastMax / prevValue ) < MaxToMinRatio * FactorOfIntensificationCondition ) && ( abs( lastMax / value ) < MaxToMinRatio * FactorOfIntensificationCondition ) )
					{
						// ������� � ���������� �����:

						numOfPrevMin = numOfCurrentMin;
						numOfCurrentMin = numOfLastMin;
						numOfValuesInDataCycle = numOfCurrentMin - numOfPrevMin;
						if( numOfPeriods == 0 ) numOfValuesInDataCycle = numOfCurrentMin + 1;

						cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin << endl << "numOfValuesInDataCycle  =  " << numOfValuesInDataCycle << endl
							<< endl << endl << endl << endl; // ��� �������.

						numOfPeriods++;
						lastMin = MaxOfDouble;
						lastMax = MinOfDouble;
						/*
						if( ( numOfPrevMin == 0 ) && ( numOfLastMin != 0 ) )
						{
							numOfPrevMin = numOfLastMin;	// ��������� ������������� ���������� numOfPrevMin.
						}
						if( numOfPrevMin != 0 ) numOfPrevMin = numOfCurrentMin;
						if( numOfLastMin != 0 ) numOfCurrentMin = numOfLastMin;
						*/
					}
				
					prevPrevValue = prevValue;
					prevValue = value;
				}
			} while( !dataStream.eof() );
			assert( dataStream.eof() && stdDeviationsStream.eof() ); // �.�. ��� ������ ������ ��������� ������ ����� ������ ������������.
			dataStream.close();				stdDeviationsStream.close();

			//if( ( lastMax / lastMin < MaxToMinRatio ) && ( lastMax / value >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			//if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			//if( ( numOfFulfilledCondition < NumOfRepetitionsOfFulfilledCondition ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			if( ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
			{
				// ��������� ��������� ����.
				numOfPrevMin = numOfCurrentMin;
				numOfValuesInDataCycle = numOfDatum - numOfPrevMin;

				cout << endl << "------------------------------------------------------------------------------------------------" << endl;
				cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value << endl << "numOfValuesInDataCycle  =  " << numOfValuesInDataCycle << endl
					<< endl << endl << endl << endl; // ��� �������.

				numOfPeriods++;
			}
			numOfDatum++;								// �.�. ��������� ������ ���������� � 0, �� ����� ����� ������ �� 1 ������, ��� ����� ���������� �������.

#if( defined( FIRST_VARIANT ) )
			//numOfTrainigValues = numOfPrevMin + 1;	// ��� ������, ����� ��������� ������ � ��������� ������ ������ �������� ������ ������� ���������� �����.
			numOfTrainigValues = numOfPrevMin;			// ��� ������, ����� �������� � ������ �������� ���������� ����� ������������ � 1-�� ������������ ��� ������.
			//numOfTrainigValues = numOfPrevMin - 1;
			//numOfTrainigValues = numOfPrevMin - 3;
			//numOfTrainigValues = numOfPrevMin - 10;
			//numOfTrainigValues = numOfPrevMin - 13;	// ��� ������, ����� ����� ��������� ������� ���������� 1.5 11-�� ������ �����.
			//numOfTrainigValues = numOfPrevMin - 26;	// ��� ������, ����� ����� ��������� ������� ���������� 2 11-�� ������ �����, ������� ����� 25 ���������� (��� �������������� ���������� ���������� ������: DataFilter Data_Ri.txt FilteredData_Ri.txt 5 0 0 1 0 0).
			//numOfTrainigValues = numOfDatum - 528;
			//numOfTrainigValues = numOfDatum - 132;

			numOfTestValues = numOfDatum - numOfTrainigValues + n;	// ��� ��������� �������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.
#elif( defined( SECOND_VARIANT ) )
			numOfTestValues = NumOfTestValues;
			numOfTrainigValues = numOfDatum - numOfTestValues + n;  // ��� ��������� �������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.
#endif

			dataStream.open( dataFile );					stdDeviationsStream.open( stdDeviationsFile );
			trainingStream.open( trainingData );			tempDataStream.open( tempDataFile );
			testStream.open( testData );					resultedStdDeviationsStream.open( resultedStdDeviationsFile );

			arrayOfValues = new double[n];					arrayOfStdDeviations = new double[n];
			k = 0;
			while( k < numOfTrainigValues )
			{	
				// ������ ������������� ������ � ����.
				// ���������� ������ �� ����� ������:
				getline( dataStream, tempStr );				getline( stdDeviationsStream, tempStr2 );
				istringstream str( tempStr );				istringstream str2( tempStr2 );
				str >> currentValue;						str2 >> stdDeviation;

				// ������ ���������� �������� � ���� �������� ������������� ��������:
				trainingStream << currentValue << endl;		tempDataStream << stdDeviation << endl;

				// ���������� ��������� n �������� �� �������������� ���� � ������:
				if ( k >= numOfTrainigValues - n )
				{
					arrayOfValues[k - ( numOfTrainigValues - n )] = currentValue;
					arrayOfStdDeviations[k - ( numOfTrainigValues - n )] = stdDeviation;
				}

				k++;
			}

			// ������ ������ n �������� ��������� � ��������������� ����:
			for( k = 0; k < n; k++ )
			{
				testStream << arrayOfValues[k] << endl;		resultedStdDeviationsStream << arrayOfStdDeviations[k] << endl;
				cout << k + 1 << ":	" << arrayOfValues[k] << "			" << arrayOfStdDeviations[k] << endl; // ��� �����.
			}

			k = n;
			// ������ �������� ������ � ��������������� ����.
			do{	
				// ���������� ������ �� ����� ������:
		        getline( dataStream, tempStr );				getline( stdDeviationsStream, tempStr2 );
				assert( ( ( tempStr.size() == 0 ) && ( tempStr2.size() == 0 ) ) || ( ( tempStr.size() > 0 ) && ( tempStr2.size() > 0 ) ) );
				if( tempStr.size() )						// if( tempStr.size() > 0 )
				{
					istringstream str( tempStr );			istringstream str2( tempStr2 );
					str >> currentValue;					str2 >> stdDeviation;
					// ������ ���������� �������� � ��������������� ����:
					testStream << currentValue << endl;		resultedStdDeviationsStream << stdDeviation << endl;
					cout << currentValue << "			" << stdDeviation << endl; // ��� �����.
					k++;
				}
			} while( !dataStream.eof() );
			assert( dataStream.eof() && stdDeviationsStream.eof() ); // �.�. ��� ������ ������ ��������� ������ ����� ������ ������������.
			assert( k == numOfDatum - numOfTrainigValues + n ); // ( k == numOfTestValues ) <-- �������� ���������� ���������, ����������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.

			trainingStream.close();							tempDataStream.close();
			testStream.close();								resultedStdDeviationsStream.close();
			dataStream.close();								stdDeviationsStream.close();
		}	// if( !dataStream.fail() && !stdDeviationsStream.fail() )!!!
		else if( stdDeviationsStream.fail() )
		{
			cout << "ERROR: the program "<< argv[0] << " cannot open the file \"" << stdDeviationsFile << "\"" << endl;
			return 1;
		}
		else
		{
			cout << "ERROR: the program "<< argv[0] << " cannot open the file \"" << dataFile << "\"" << endl;
			return 1;
		}
	}	// if( argc == 8 )!!!

	cout << endl << "================================================================================================" << endl;
	cout << endl
		 << "n  =  " << n << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numOfTrainigValues  =  " << numOfTrainigValues << endl		// numOfTestValues = numOfDatum - numOfTrainigValues + n;
		 << "numOfTestValues  =  " << numOfTestValues << endl // ���������� ����� ��������� �������� ����������� ��������� ��������� ����� ������ ������ �� ��������� � �������� ������ ��� ��� ������, ������� n ������.
		 << "numberOfPeriods  =  " << numOfPeriods << endl;
/*	
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
*/
	return 0;
}