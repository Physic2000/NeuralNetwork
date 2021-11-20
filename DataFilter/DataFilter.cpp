// Программа фильтрует данные из файла, имя которого передаётся программе через командную строку первым аргументом
// и записывает отфильтрованные данные в файл, имя которого передаётся программе вторым аргументом. 
// Третьим аргументом программе надо передать имя двоичного файла в который будет дописаны для каждого Солнечного цикла 
// значения индекса первого отфильтрованного элемента данных соответствующего элементу из массива pointTransformed.
// Последующими аргументами программе передаются параметры применяемой фильтрации.

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

char* resFile_3 = "FilteredData_Index_Ri.txt";	// Файл в который записываем отфильтрованный набор данных с индексом.

double TransformData( double d );

// Программе НАДО передать в командной строке: 
// 1) имя исходного файла с данными, которые будем фильтровать																																			-	char* dataFile;
// 2) имя результирующего файла, в который будут записаны отфильтрованные данные																														-	char* resFile;
// 3) имя двоичного файла в который будут дописаны для каждого Солнечного цикла значение индекса первого элемента в отфильтрованных данных соответствующего первому элементу в массиве pointTransformed	-	char* resFile_2;
// 4) число весовых коэффициентов. Число должно быть не чётное и больше или равное 3																													-	int numOfWeights;
// 5) параметры фильтрации - numOfWeights чисел с плавающей запятой, которые записываются в массив																										-	double weight[numOfWeights];
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
		// Присваиваем локальным переменным значения переданные через командную строку:
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
				weight[i] = atof( argv[5 + i] );	// Инициализируем массив значениями переданными через командную строку.
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
				weight[i] = atof( argv[4 + i] );	// Инициализируем массив значениями переданными через командную строку.
				newValue += weight[i];
			}
		}

		// Нормируем весовые коэффициенты так, что бы их сумма равнялась 1:
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
		// Восстанавливаем содержимое контейнера doubleList из двоичного файла resFile_2:
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
		// Считываем из потока данных первые numOfWeights значений типа double:
		//dataStream >> value[0] >> value[1] >> value[2] >> value[3] >> value[4];
		newValue = 0.0;
		for( i = 0; i < numOfWeights; i++ )
		{
			dataStream >> value[i];
			newValue += value[i] * weight[i];
		}
		// В переменной newValue содержится средневзвешенное первых numOfWeights значений, считанных из потока.

		resStream.open( resFile );
		resStream << newValue;									// Запись первого значения полученного в результате фильтрации в файл.

		if( binaryFileIsPresent )
		{
			indexOfNextFilteredDatum = numOfWeights / 2;
			currentIterator = doubleList.begin();
			( *currentIterator ).indexOfFirstFilteredDatum = indexOfNextFilteredDatum;
		}

		resStream_3.open( resFile_3 );									// Создаём новый файл для записи в него отфильтрованных данных с их индексом.
		numOfDatum = 0;		// Т.к. нумерация данных начинается с 0.
		resStream_3 << numOfDatum << "	" << newValue;			// Запись первого значения полученного в результате фильтрации в файл с индексом данных.
		k = 0;
		do{	
			// Считываем построчно файл dataFile до конца.
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
							assert( currentIterator != doubleList.end() );	// Это событие НИКОГДА НЕ должно наступать!!
							( *currentIterator ).indexOfFirstFilteredDatum = indexOfNextFilteredDatum;
						}
					}

					// Запись значения, получаемого в результате фильтрации, в результирующий файл.
					newValue = 0.0;
					for( i = 0; i < numOfWeights; i++ )
						newValue += value[i] * weight[i];

					// Экспериментальное преобразование данных (не является обязательным):
					//newValue = TransformData( newValue ); // обычно эта строка должна быть закомментирована.

					resStream << endl << newValue;		// Запись очередного значения полученного в результате фильтрации в файл.
					numOfDatum++;
					resStream_3 << endl << numOfDatum << "	" << newValue;			// Запись очередного значения полученного в результате фильтрации в файл с индексом данных.
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
		// Проверка записанного значения индекса первого элемента в отфильтрованных данных соответствующего первому элементу в массиве pointTransformed для каждого Солнечного цикла.
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

		// Перезапись двоичного файла resFile_2. 
		// Теперь в объектах списка doubleList содержится значение индекса первого элемента в отфильтрованных данных соответствующего первому элементу в массиве pointTransformed для каждого Солнечного цикла.
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

	numOfDatum++; // Т.к. нумерация данных начиналась с 0, то общее число данных на 1 больше, чем номер последнего данного.
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