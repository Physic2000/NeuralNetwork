// Программа анализирует данные из двух, переданных ей, через командную строку, файлов, выполняет сдвиг в право данных из 1-го файла относительно данных из 2-го файла и записывает результат сдвига в файл имя которого
// передано программе 3-м аргументом. При этом сдвиг подбирается т.о., что бы сумма квадратов разностей соответствующих значений из результирующего и 2-го файлов была бы минимальна. 4-ым аргументом программе надо
// передать кол-во нейронов входного слоя ИНС Элмана. 5-ым аргументом число сдвигов.

#include <math.h>
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
//#include <algorithm>
//#include <iterator>
//#include <cstdio>
using namespace std;

// Программе НАДО передать в командной строке: 
// 1) имя файла с данными, которые будем сдвигать										-	simulatedData;
// 2) имя файла с данными, относительно которого будем сдвигать данные из 1-го файла	-	testData;
// 3) имя файла, в который будут записаны сдвинутые данные								-	shiftedData;
// 4) кол-во нейронов входного слоя ИНС	Элмана											-	n;
// 5) число сдвигов																		-	numOfShiftings;
int main( int argc, char* argv[] )
{
	char *simulatedData, *testData, *shiftedData;
	double *arrayOfSimulatedData, *arrayOfTestData, **arrayOfShiftedData, *arrayOfErrorsSums, simulatedValue, testValue, sumErrorWithoutShift, minOfSumError;
	int numOfDatum, i , j, n, numOfShiftings, indexOfMin;
	//char *strOfSimulatedDatum_2 = new char[100];
	//FILE *ptrSimulatedDataFile;
	//fstream fs;
	string strOfSimulatedDatum, strOfTestDatum, tempStr;
	ifstream streamOfSimulatedData, streamOfTestData;
	ofstream streamOfShiftedData;

	if ( argc == 6 )
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		simulatedData = argv[1];
		testData = argv[2];
		shiftedData = argv[3];
		n = atoi( argv[4] );
		numOfShiftings = atoi( argv[5] );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line!!!" << endl;
		return 1;
	}

	// ЗАПЛАТКА:
	/*
	streamOfSimulatedData.open( simulatedData );
	string file_content( ( std::istream_iterator<char>( streamOfSimulatedData ) ), istream_iterator<char>() );
	streamOfSimulatedData.close();
	file_content = "\n" + file_content;
	ofstream out( simulatedData );
    out << file_content;
    out.close();
	*/

	// Программа будет 2 раза полностью считывать данные из файлов simulatedData и testData.
	streamOfSimulatedData.open( simulatedData );
	//ptrSimulatedDataFile = fopen( simulatedData , "r" );
	streamOfTestData.open( testData );
	if( !streamOfSimulatedData.fail() )
	{
		if( !streamOfTestData.fail() )
		//if( ptrSimulatedDataFile != NULL )
		{
			for( i = 0; i < n; i++ )
				streamOfTestData >> testValue;

			sumErrorWithoutShift = 0;
			numOfDatum = 0;
			while( ( streamOfSimulatedData >> simulatedValue ) && ( streamOfTestData >> testValue ) )
			{
			// Считываем построчно оба файла до конца:
				//getline( streamOfSimulatedData, strOfSimulatedDatum );
				//streamOfSimulatedData >> simulatedValue;
				//streamOfSimulatedData.getline( strOfSimulatedDatum_2, 100 );
				//fgets( strOfSimulatedDatum_2, 100, ptrSimulatedDataFile );
				//getline( streamOfTestData, strOfTestDatum );
				//streamOfTestData >> testValue;
				//if( strOfSimulatedDatum.size() && strOfTestDatum.size() )						// if( strOfSimulatedDatum.size() > 0 && strOfTestDatum.size() > 0 )
				//if( strOfTestDatum.size() )														// if( strOfTestDatum.size() > 0 )
				//if( strlen( strOfSimulatedDatum_2 ) && strOfTestDatum.size() )
				//istringstream str( strOfSimulatedDatum );
				//istringstream str( strOfSimulatedDatum_2 );
				//str >> simulatedValue;
				//istringstream str2( strOfTestDatum );
				//str2 >> testValue;
				sumErrorWithoutShift += pow( simulatedValue - testValue, 2 );
				numOfDatum++;	// При выходе из цикла do{ ... } while( ... ) переменная numOfDatum будет хранить кол-во данных в файле simulatedData.

				// Для отладки:
				//cout << numOfDatum << ":	" << simulatedValue << endl;
				//cout << numOfDatum << ":	" << testValue << endl;
				assert( ( !streamOfSimulatedData.eof() && !streamOfTestData.eof() ) || ( streamOfSimulatedData.eof() && streamOfTestData.eof() ) );
			}
			//} while( !streamOfTestData.eof() );
			streamOfSimulatedData.close();
			streamOfTestData.close();
			sumErrorWithoutShift /= numOfDatum;
			
			arrayOfErrorsSums = new double[numOfShiftings + 1];
			arrayOfShiftedData = new double*[numOfShiftings + 1];
			arrayOfSimulatedData = new double[numOfDatum];
			arrayOfTestData = new double[numOfDatum];
			for( i = 0; i < numOfShiftings + 1; i++ )
				arrayOfShiftedData[i] = new double[numOfDatum];

			streamOfSimulatedData.open( simulatedData );
			streamOfTestData.open( testData );
			for( i = 0; i < n; i++ )
				streamOfTestData >> testValue;

			for( j = 0; j < numOfDatum; j++ )
			{
				streamOfSimulatedData >> arrayOfSimulatedData[j];
				streamOfTestData >> arrayOfTestData[j];
			}
			streamOfSimulatedData.close();
			streamOfTestData.close();

			arrayOfErrorsSums[0] = sumErrorWithoutShift;
			minOfSumError = sumErrorWithoutShift;
			indexOfMin = 0;
			for( i = 0; i < numOfShiftings + 1; i++ )
			{
				copy( arrayOfSimulatedData, arrayOfSimulatedData + numOfDatum - i, arrayOfShiftedData[i] + i );
				for( j = 0; j < i; j++ )
					arrayOfShiftedData[i][j] = arrayOfTestData[j];

				// Для отладки:
				/*
				for( j = 0; j < numOfDatum; j++ )
					cout << "arrayOfShiftedData[" << i << " ][ " << j << " ]  =  " << arrayOfShiftedData[i][j] << endl;
				*/
				if( i > 0 )
				{
					arrayOfErrorsSums[i] = 0;
					for( j = 0; j < numOfDatum; j++ )
						arrayOfErrorsSums[i] += pow( arrayOfShiftedData[i][j] - arrayOfTestData[j], 2 );
					arrayOfErrorsSums[i] /= numOfDatum;
	
					if( arrayOfErrorsSums[i] < minOfSumError )
					{
						minOfSumError = arrayOfErrorsSums[i];
						indexOfMin = i;
					}
				}
			}

			streamOfShiftedData.open( shiftedData );
			for( j = 0; j < numOfDatum; j++ )
				streamOfShiftedData << arrayOfShiftedData[indexOfMin][j] << endl;
			streamOfShiftedData.close();
		}
	}

	cout << endl
		 << "n  =  " << n << endl
		 << "numOfShiftings  =  " << numOfShiftings << endl
		 << "numOfDatum  =  " << numOfDatum << endl;
	for( i = 0; i < numOfShiftings + 1; i++ )
		cout << "arrayOfErrorsSums[" << i << "]  =  " << arrayOfErrorsSums[i] << endl;
	cout << endl << "indexOfMin  =  " << indexOfMin << endl
		 << "minOfSumError  =  " << minOfSumError << endl;

	// Освобождение, выделенных ранее, блоков памяти:
	/*
	for( i = 0; i < NumOfShiftings - 1; i++ )
		delete( arrayOfShiftedData[i] );
	delete( arrayOfShiftedData );
	delete( arrayOfErrorsSums );
	delete( arrayOfSimulatedData );
	delete( arrayOfTestData );
	*/
	for( i = 0; i < numOfShiftings + 1; i++ )
		delete [] arrayOfShiftedData[i];
	delete [] arrayOfShiftedData;
	delete [] arrayOfErrorsSums;
	delete [] arrayOfSimulatedData;
	delete [] arrayOfTestData;
	arrayOfShiftedData = nullptr;
	arrayOfErrorsSums = arrayOfSimulatedData = arrayOfTestData = nullptr;
	/*
	arrayOfErrorsSums = nullptr;
	arrayOfSimulatedData = nullptr;
	arrayOfTestData = nullptr;
	*/

	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
	return 0;
}