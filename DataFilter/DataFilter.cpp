// ��������� ��������� ������ �� �����, ��� �������� ��������� ��������� ����� ��������� ������ ������ ����������
// � ���������� ��������������� ������ � ����, ��� �������� ��������� ��������� ������ ����������. 
// ������� ���������� ��������� ���� �������� ��� ��������� ����� � ������� ����� �������� ��� ������� ���������� ����� 
// �������� ������� ������� ���������������� �������� ������ ���������������� �������� �� ������� pointTransformed.
// ������������ ����������� ��������� ���������� ��������� ����������� ����������.

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

char* resFile_3 = "FilteredData_Index_Ri.txt";	// ���� � ������� ���������� ��������������� ����� ������ � ��������.

double TransformData( double d );

// ��������� ���� �������� � ��������� ������: 
// 1) ��� ��������� ����� � �������, ������� ����� �����������																																			-	char* dataFile;
// 2) ��� ��������������� �����, � ������� ����� �������� ��������������� ������																														-	char* resFile;
// 3) ��� ��������� ����� � ������� ����� �������� ��� ������� ���������� ����� �������� ������� ������� �������� � ��������������� ������ ���������������� ������� �������� � ������� pointTransformed	-	char* resFile_2;
// 4) ����� ������� �������������. ����� ������ ���� �� ������ � ������ ��� ������ 3																													-	int numOfWeights;
// 5) ��������� ���������� - numOfWeights ����� � ��������� �������, ������� ������������ � ������																										-	double weight[numOfWeights];
int main( int argc, char* argv[] )
{
	bool binaryFileIsPresent;
	char *dataFile, *resFile, *resFile_2;
	//double newValue, weight[5], value[5], arr[3];
	double newValue, *weight, *value;
	int k, i, numOfDatum, numOfWeights, indexOfNextFilteredDatum;
	string tempStr, str;
	ifstream dataStream;
	ofstream resStream, resStream_3;
	PtrToDataOfSolarCycle pCurrentSolarCycle;
	list<DataOfSolarCycle> doubleList;
	list<DataOfSolarCycle>::iterator currentIterator;
	FILE *pResFile_2;

	cout << endl;

	if ( argc >= 7 )
	{
		if ( argc % 2 == 0 ) binaryFileIsPresent = true;
		else binaryFileIsPresent = false;
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		if ( binaryFileIsPresent )
		{
			dataFile = argv[1];						
			resFile = argv[2];
			resFile_2 = argv[3];
			numOfWeights = atoi( argv[4] );
			if ( !( ( numOfWeights >= 3 ) && ( numOfWeights % 2 == 1 ) ) )
			{
				cout << "ERROR: starting the program with an unexpected numOfPoints on the command line!!!" << endl;
				return 1;
			}

			if ( argc != ( 5 + numOfWeights ) )
			{
				cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != ( 5 + numOfPoints ) )!!!" << endl;
				return 1;
			}

			weight = new double[numOfWeights];
			value = new double[numOfWeights];
			newValue = 0.0;
			for( i = 0; i < numOfWeights; i++ )
			{
				weight[i] = atof( argv[5 + i] );	// �������������� ������ ���������� ����������� ����� ��������� ������.
				newValue += weight[i];
			}
		}
		else	// if ( !binaryFileIsPresent )
		{
			dataFile = argv[1];						
			resFile = argv[2];
			resFile_2 = "";
			numOfWeights = atoi( argv[3] );
			if ( !( ( numOfWeights >= 3 ) && ( numOfWeights % 2 == 1 ) ) )
			{
				cout << "ERROR: starting the program with an unexpected numOfPoints on the command line!!!" << endl;
				return 1;
			}

			if ( argc != ( 4 + numOfWeights ) )
			{
				cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != ( 4 + numOfPoints ) )!!!" << endl;
				return 1;
			}

			weight = new double[numOfWeights];
			value = new double[numOfWeights];
			newValue = 0.0;
			for( i = 0; i < numOfWeights; i++ )
			{
				weight[i] = atof( argv[4 + i] );	// �������������� ������ ���������� ����������� ����� ��������� ������.
				newValue += weight[i];
			}
		}

		// ��������� ������� ������������ ���, ��� �� �� ����� ��������� 1:
		for( i = 0; i < numOfWeights; i++ )
			weight[i] /= newValue;
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc < 7 )!!!" << endl;
		return 1;
	}

	if( binaryFileIsPresent )
	{
		// ��������������� ���������� ���������� doubleList �� ��������� ����� resFile_2:
		if( ( pResFile_2 = fopen( resFile_2, "rb" ) ) == NULL )
		{
			printf( "%s %s", "Cannot open file: ", resFile_2 );
			return 1;
		}
		for( int i = 0; !feof( pResFile_2 ); i++ )
		{
			pCurrentSolarCycle = new DataOfSolarCycle;
			//fread( pCurrentSolarCycle, sizeof( struct DataOfSolarCycle ), 1, pResFile_2 );
			fread( pCurrentSolarCycle, sizeof( DataOfSolarCycle ), 1, pResFile_2 );
			if( !feof( pResFile_2 ) )
			{
				doubleList.push_back( *pCurrentSolarCycle );
				cout << ( *pCurrentSolarCycle ).numOfSolarCycle << ":	pCurrentSolarCycle->maxOfValue = " << pCurrentSolarCycle->maxOfValue << endl;
			}
		}
		fclose( pResFile_2 );
		cout << "doubleList.size() = " << doubleList.size() << endl << endl;
	}
	
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		// ��������� �� ������ ������ ������ numOfWeights �������� ���� double:
		//dataStream >> value[0] >> value[1] >> value[2] >> value[3] >> value[4];
		newValue = 0.0;
		for( i = 0; i < numOfWeights; i++ )
		{
			dataStream >> value[i];
			newValue += value[i] * weight[i];
		}
		// � ���������� newValue ���������� ���������������� ������ numOfWeights ��������, ��������� �� ������.

		resStream.open( resFile );
		resStream << newValue;									// ������ ������� �������� ����������� � ���������� ���������� � ����.

		if( binaryFileIsPresent )
		{
			indexOfNextFilteredDatum = numOfWeights / 2;
			currentIterator = doubleList.begin();
			( *currentIterator ).indexOfFirstFilteredDatum = indexOfNextFilteredDatum;
		}

		resStream_3.open( resFile_3 );									// ������ ����� ���� ��� ������ � ���� ��������������� ������ � �� ��������.
		numOfDatum = 0;		// �.�. ��������� ������ ���������� � 0.
		resStream_3 << numOfDatum << "	" << newValue;			// ������ ������� �������� ����������� � ���������� ���������� � ���� � �������� ������.
		k = 0;
		do{	
			// ��������� ��������� ���� dataFile �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value[k];
				if( ( k + 1 ) % numOfWeights == 0 )
				{
					if( binaryFileIsPresent )
					{
						indexOfNextFilteredDatum += numOfWeights;
						if( indexOfNextFilteredDatum >= STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE )
						{
							indexOfNextFilteredDatum -= STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;
							++currentIterator;
							assert( currentIterator != doubleList.end() );	// ��� ������� ������� �� ������ ���������!!
							( *currentIterator ).indexOfFirstFilteredDatum = indexOfNextFilteredDatum;
						}
					}

					// ������ ��������, ����������� � ���������� ����������, � �������������� ����.
					newValue = 0.0;
					for( i = 0; i < numOfWeights; i++ )
						newValue += value[i] * weight[i];

					// ����������������� �������������� ������ (�� �������� ������������):
					//newValue = TransformData( newValue ); // ������ ��� ������ ������ ���� ����������������.

					resStream << endl << newValue;		// ������ ���������� �������� ����������� � ���������� ���������� � ����.
					numOfDatum++;
					resStream_3 << endl << numOfDatum << "	" << newValue;			// ������ ���������� �������� ����������� � ���������� ���������� � ���� � �������� ������.
					k = 0;
					continue;
				}
				k++;
			}
		} while( !dataStream.eof() );

		resStream.close();
		resStream_3.close();
		dataStream.close();
	}
	else
	{
		cout << "ERROR: the program "<< argv[0] << " cannot open the file \"" << dataFile << "\"" << endl;
		return 1;
	}

	if( binaryFileIsPresent )
	{
		// �������� ����������� �������� ������� ������� �������� � ��������������� ������ ���������������� ������� �������� � ������� pointTransformed ��� ������� ���������� �����.
		cout << endl;
		for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
			cout << ( *currentIterator ).numOfSolarCycle 
			//<< ":	CycleCompleted = " << ( *currentIterator ).solarCycleCompleted 
			//<< ";  timeStepTranformed = " << ( *currentIterator ).timeStepTranformed
			<< ";  numOfValues = " << ( *currentIterator ).numOfValues
			<< ";  indexOfFirstFilteredDatum = " << ( *currentIterator ).indexOfFirstFilteredDatum
			<< ";  indexOfFirstDatumOfSolarCycle = " << ( *currentIterator ).indexOfFirstDatumOfSolarCycle
			//<< ";  maxOfValue = " << ( *currentIterator ).maxOfValue 
			<< endl;

		// ���������� ��������� ����� resFile_2. 
		// ������ � �������� ������ doubleList ���������� �������� ������� ������� �������� � ��������������� ������ ���������������� ������� �������� � ������� pointTransformed ��� ������� ���������� �����.
		if( ( pResFile_2 = fopen( resFile_2, "wb" ) ) == NULL )
		{
			printf( "%s %s", "Cannot open file: ", resFile_2 );
			return 1;
		}
		for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
			//fwrite( &( *currentIterator ), sizeof( struct DataOfSolarCycle ), 1, pResFile_2 );
			fwrite( &( *currentIterator ), sizeof( DataOfSolarCycle ), 1, pResFile_2 );
		fclose( pResFile_2 );
	}

	numOfDatum++; // �.�. ��������� ������ ���������� � 0, �� ����� ����� ������ �� 1 ������, ��� ����� ���������� �������.
	cout << "numOfDatum  =  " << numOfDatum << endl;
/*
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
*/
	delete[] weight; weight = nullptr;
	delete[] value; value = nullptr;

	return 0;
}

double TransformData( double d )
{
	double res;

	if( d >= 1 )
		res = 10 * log10( d );
		//res = 10 * log( d );
		//res = log( d );
	else if( ( 0 < d ) && ( d < 1 ) )
		res = 0;

	return res;
}