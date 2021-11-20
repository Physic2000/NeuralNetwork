// Эта программа считывает txt файл, содержащий весь массив данных (чисел солнечных пятен записанных помесячно), выполняет разбиение данных по циклам и упаковку их в специальную структуру данных, которая содержит в
// себе полную информацию об 11-ти летнем цикле Вольфа. Затем программа формирует последовательность объектов специальной структуры данных (каждый объект - один Солнечный цикл) и записывает эту последовательность
// объектов в бинарный файл. Первым аргументом программе надо передать имя txt файла, содержащего весь массив данных. Этот файл может содержать или исходные данные, или данные подвергнутые пораболической предобработке.
// Вторым аргументом программе надо передать имя txt файла в который будут записаны преобразованные данные. Суть преобразования: каждый 11-ти летний цикл содержит ровно STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE (= 132) 
// отнормированных значения. Третьим аргументом программе надо передать имя результирующего бинарного файла. Четвёртым аргументом программе надо передать имя txt файла для записи расширенной информации о солнечных циклах.
// Пятым аргументом программе надо передать кол-во полных солнечных циклов, которые будут записаны в txt файл, имя которого программе передаётся вторым аргументом.

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

// Программе НАДО передать в командной строке:
// 1) имя исходного файла, содержащего весь массив данных (чисел солнечных пятен записанных помесячно), которые будем обрабатывать		-	char* dataFile;
// 2) имя результирующего файла, в который будут записаны преобразованные данные в	текстовом формате									-	char* resFile;
// 3) имя результирующего файла, в который будут записаны обработанные и структурированные данные в	бинарном формате					-	char* resFile_2;
// 4) имя результирующего txt файла для записи расширенной информации о солнечных циклах												-	char* resFile_3;
// Дополнительно программе можно передать в командной строке:
// 5) кол-во полных солнечных циклов, которые будут записаны в txt файл, имя которого программе передаётся вторым аргументом			-	numOfSolarCycles;
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
		// Присваиваем локальным переменным значения переданные через командную строку:
		dataFile = argv[1];		// Файл содержащий исходные данные в текстовом формате.		
		resFile = argv[2];		// Файл для записи преобразованных данных в текстовом формате.
		resFile_2 = argv[3];	// Файл для записи обработанных и структурированных данных в бинарном формате.
		resFile_3 = argv[4];	// Файл для записи расширенной информации о солнечных циклах.
		numOfSolarCycles = -1;
	}
	else if ( argc == 6 )
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		dataFile = argv[1];		// Файл содержащий исходные данные в текстовом формате.		
		resFile = argv[2];		// Файл для записи преобразованных данных в текстовом формате.
		resFile_2 = argv[3];	// Файл для записи обработанных и структурированных данных в бинарном формате.
		resFile_3 = argv[4];	// Файл для записи расширенной информации о солнечных циклах.
		numOfSolarCycles = atoi( argv[5] );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: argc != 5, 6 !!!" << endl;
		return 1;
	}

	// Программа будет полностью считывать данные из файла dataFile.
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		// Считываем из потока данных первые 2 значения типа double:
		dataStream >> prevPrevValue >> prevValue;

		//resStream_2.open( "TEMP.txt" );

		cout << endl;
		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriod = 0;
		numOfDatum = 1;						// Т.к. нумерация данных начинается с 0 и 2 значения мы уже считали.
		numOfDatumInCurrentSolarCycle = 1;	// Т.к. нумерация данных в структуре данных солнечного цикла начинается с 0 и 2 значения мы уже считали.
		averageMaximum = 0.0;
		averagePeriodSize = 0.0;

		//pCurrentSolarCycle = new DataOfSolarCycle();
		pCurrentSolarCycle = new DataOfSolarCycle;
		pCurrentSolarCycle->indexOfFirstDatumOfSolarCycle = 0;
		pCurrentSolarCycle->pointBasic[0] = prevPrevValue;
		pCurrentSolarCycle->pointBasic[1] = prevValue;
		do{	
			// Считываем построчно файл до конца.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;
				numOfDatumInCurrentSolarCycle++;
				pCurrentSolarCycle->pointBasic[numOfDatumInCurrentSolarCycle] = value;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// определение текущего минимума:
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;	// Потенциально этот номер может оказаться номером последнего значения в текущем солнечном цикле.
					numOfLastMinInCurrentSolarCycle = numOfDatumInCurrentSolarCycle - 1; // Потенциально этот номер может оказаться номером последнего значения в текущем солнечном цикле.
					/*
					// Для отладки:
					cout << "lastMin  =  " << lastMin << endl
						<< "numOfLastMin  =  " << numOfLastMin << endl;
					*/
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// определение текущего максимума:
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;
					/* 
					// Для отладки:
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
					// ПЕРЕХОД К СЛЕДУЮЩЕМУ ЦИКЛУ:

					cout << endl << "numOfPeriod  =  " << numOfPeriod << endl << "lastMax  =  " << lastMax << endl << "lastMax / lastMin  =  " << lastMax / lastMin << endl 
						<< "numOfValues = " << numOfLastMinInCurrentSolarCycle + 1 << endl << endl; // Для отладки.

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
					pCurrentSolarCycle->numOfValues = numOfLastMinInCurrentSolarCycle + 1; // Поскольку число значений в текущем цикле солнечной активности на 1-цу больше, чем номер последнего значения в цикле, т.к. нумерация этих значений начинается с 0.
					pCurrentSolarCycle->solarCycleCompleted = true;
					//pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )numOfDatumInCurrentSolarCycle / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;	// В будущем удалить эту строку, как ошибочную!!!
					pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )pCurrentSolarCycle->numOfValues / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;
					pCurrentSolarCycle->maxOfValue = lastMax;

					//resStream_2 << "numOfSolarCycle = " << pCurrentSolarCycle->numOfSolarCycle << endl;		// Для отладки.

					//for( int i = 0; i < numOfDatumInCurrentSolarCycle; i++ )	// В будущем удалить эту строку, как ошибочную!!!
					for( int i = 0; i < pCurrentSolarCycle->numOfValues; i++ )
					{
						pCurrentSolarCycle->pointNormalized[i] = pCurrentSolarCycle->pointBasic[i] / lastMax;

						//resStream_2 << i << ":	" << pCurrentSolarCycle->pointBasic[i] << endl;		// Для отладки.
					}
					//resStream_2 << endl;	// Для отладки.
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
								pCurrentSolarCycle->pointTransformed[i] = pCurrentSolarCycle->pointNormalized[( int )x1];  // т.е. в этом случае: pCurrentSolarCycle->pointTransformed[i] = y1;

							// Для теста:
							assert( ( int )x1 < pCurrentSolarCycle->numOfValues );	// Это событие в правильно работающем алгоритме НЕ ДОЛЖНО НИКОГДА НАСТУПАТЬ!!
							//cout << i << ":	" << pCurrentSolarCycle->pointTransformed[i] << endl;
						}
					}
					doubleList.push_back( *pCurrentSolarCycle );
					cout << endl << endl;

					//numOfDatumInCurrentSolarCycle = 0;	// ЭТО НЕ ВЕРНО, т.к. ( numOfDatumInCurrentSolarCycle - numOfLastMinInCurrentSolarCycle ) значений нового цикла мы уже считали.
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
			// Фиксируем последний ЗАКОНЧЕННЫЙ цикл, который в будущем возможно будет ещё дополнен данными:
			cout << endl << "------------------------------------------------------------------------------------------------" << endl;
			cout << "numOfPeriod  =  " << numOfPeriod << endl << "lastMax  =  " << lastMax << endl << "lastMax / lastMin  =  " << lastMax / lastMin << endl
				<< "numOfValues = " << numOfLastMinInCurrentSolarCycle + 1 << endl << endl; // Для отладки.

			averagePeriodSize += numOfLastMin - numOfPrevMin;
			averagePeriodSize /= numOfPeriod;
			numOfPrevMin = numOfLastMin;

			pCurrentSolarCycle->numOfSolarCycle = numOfPeriod;
			pCurrentSolarCycle->numOfValues = numOfLastMinInCurrentSolarCycle + 1; // Поскольку число значений в текущем цикле солнечной активности на 1-цу больше, чем номер последнего значения в цикле, т.к. нумерация этих значений начинается с 0.
			pCurrentSolarCycle->solarCycleCompleted = true;
			//pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )numOfDatumInCurrentSolarCycle / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;	// В будущем удалить эту строку, как ошибочную!!!
			pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )pCurrentSolarCycle->numOfValues / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE;
			pCurrentSolarCycle->maxOfValue = lastMax;
			/*
			cout << "pCurrentSolarCycle->timeStepTranformed = " << pCurrentSolarCycle->timeStepTranformed << endl 
				<< "pCurrentSolarCycle->maxOfValue = " << pCurrentSolarCycle->maxOfValue << endl << endl;
			*/
			//for( int i = 0; i < numOfDatumInCurrentSolarCycle; i++ )	// В будущем удалить эту строку, как ошибочную!!!
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
						pCurrentSolarCycle->pointTransformed[i] = pCurrentSolarCycle->pointNormalized[( int )x1];  // т.е. в этом случае: pCurrentSolarCycle->pointTransformed[i] = y1;

					// Для теста:
					assert( ( int )x1 < pCurrentSolarCycle->numOfValues );	// Это событие в правильно работающем алгоритме НЕ ДОЛЖНО НИКОГДА НАСТУПАТЬ!!
					//cout << i << ":	" << pCurrentSolarCycle->pointTransformed[i] << endl;
				}
			}
			doubleList.push_back( *pCurrentSolarCycle );

			// Для отладки:
			cout << endl << "doubleList.size()  =  " << doubleList.size() << endl << endl; // Для отладки.

			// Сохраняем НОВЫЙ цикл, который является текущим:
			//numOfDatumInCurrentSolarCycle = 0;	// ЭТО НЕ ВЕРНО, т.к. ( numOfDatumInCurrentSolarCycle - numOfLastMinInCurrentSolarCycle ) значений нового цикла мы уже считали.
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
			// Добавляем в список начало нового последнего цикла:
			doubleList.push_back( *pCurrentSolarCycle );
			cout << "doubleList.size()  =  " << doubleList.size() << endl << endl; // Для отладки.

			cout << endl << endl;
		}
		else
		{
			// Фиксируем последний цикл, который ещё НЕ ЗАКОНЧЕН:
			cout << endl << "------------------------------------------------------------------------------------------------" << endl;
			cout << endl << "numOfPeriod  =  " << numOfPeriod << endl << "lastMax / value  =  " << lastMax / value << endl
				<< "lastMax = " << lastMax << endl << "numOfValues = " << numOfDatumInCurrentSolarCycle + 1 << endl << endl << endl << endl; // Для отладки.

			pCurrentSolarCycle->numOfSolarCycle = numOfPeriod;
			//pCurrentSolarCycle->numOfValues = numOfLastMinInCurrentSolarCycle + 1; // Именно в этом месте этот оператор ОШИБОЧЕН!!!
			pCurrentSolarCycle->numOfValues = numOfDatumInCurrentSolarCycle + 1;
			//pCurrentSolarCycle->solarCycleCompleted = false; //	Лишний оператор, т.к. это значение присваивается по умолчанию в конструкторе DataOfSolarCycle().
			if( lastMax > MinimumMaximum )
			{
				pCurrentSolarCycle->maxOfValue = lastMax;
				for( int i = 0; i < pCurrentSolarCycle->numOfValues; i++ )
					pCurrentSolarCycle->pointNormalized[i] = pCurrentSolarCycle->pointBasic[i] / lastMax;
				/*
				if( pCurrentSolarCycle->numOfValues > MINIMUM_NUM_OF_DATUM_IN_SOLAR_CYCLE )
				{
					pCurrentSolarCycle->timeStepTranformed = timeStepTranformed = ( double )pCurrentSolarCycle->numOfValues / STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; // ЭТО СКОРЕЕ ВСЕГО НЕ ПРАВИЛЬНО, т.к. цикл ещё НЕ ЗАКОНЧЕН!!!!!
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
								pCurrentSolarCycle->pointTransformed[i] = pCurrentSolarCycle->pointNormalized[( int )x1];  // т.е. в этом случае: pCurrentSolarCycle->pointTransformed[i] = y1;

							// Для теста:
							//cout << "i  =  " << i << "	" << "x1  =  " << x1 << "	" << "x2  =  " << x2 << endl;
							//cout << "pCurrentSolarCycle->numOfValues  =  " << pCurrentSolarCycle->numOfValues << endl;
							assert( ( int )x1 < pCurrentSolarCycle->numOfValues );	// Это событие в правильно работающем алгоритме НЕ ДОЛЖНО НИКОГДА НАСТУПАТЬ!!
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

	resStream.open( resFile );		// Открываем файл для записи в него преобразованных данных.
	resStream_3.open( resFile_3 );	// Открываем файл для записи в него проиндексированных исходных данных.
	resStream_4.open( resFile_4 );	// Открываем файл для записи в него проиндексированных преобразованных данных.
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

		// **********	СЕЙЧАС ЭТО ОСНОВНОЙ ФРАГМЕНТ КОДА ДЛЯ ФОРМИРОВАНИЯ УРЕЗАННОЙ ПОСЛЕДОВАТЕЛЬНОСТИ ЦИКЛОВ:		**********
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

		// **********	СЕЙЧАС ЭТО ОСНОВНОЙ ФРАГМЕНТ КОДА ДЛЯ ФОРМИРОВАНИЯ ПОЛНОЙ ПОСЛЕДОВАТЕЛЬНОСТИ ЦИКЛОВ:		**********
		else
		{
			// Сохраняем ВСЕ данные за исключением самого первого Солнечного цикла (обрезанного по кол-ву данных):
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

	// Запись списка объектов в файл в двоичном формате:
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

	// Сохранение усреднённого цикла в txt файле:
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

	// Запись в текстовые файлы многократно повторенных усреднённого цикла и стандартного отклонения для каждой точки усреднённого цикла. При этом кол-во их повторений равно кол-ву солнечных циклов записанных в
	// текстовый файл resFile.
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

	numOfDatum++; // Т.к. нумерация данных начиналась с 0, то общее число данных на 1 больше, чем номер последнего данного.
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