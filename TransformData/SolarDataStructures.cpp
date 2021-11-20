#include "SolarDataStructures.h"
#include <iterator>
#include <assert.h>
#include <iostream>

using namespace std;

namespace SolarDataStructures 
{

	DataOfSolarCycle::DataOfSolarCycle()								// Конструктор по умолчанию.
	{
		memset( pointBasic, 0, MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( pointNormalized, 0, MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( pointTransformed, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		// Следующим переменным-членам намерено присвоено значение не имеющее смысла, которое сигнализирует о том, что эти переменные ещё не были проинициализированы!
		numOfSolarCycle = indexOfFirstDatumOfSolarCycle = indexOfFirstFilteredDatum = -1;
		numOfValues = 0;
		solarCycleCompleted = false;
		timeStepBasic = 1.0;									
		timeStepTranformed = maxOfValue = -1.0;					// Этим переменным-членам намерено присвоено значение не имеющее смысла, которое сигнализирует о том, что эти переменные ещё не были проинициализированы!
	}

	DataOfSolarCycle::DataOfSolarCycle( const DataOfSolarCycle &source )		// Конструктор копирования.
	{
		memcpy( pointBasic, source.pointBasic, MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memcpy( pointNormalized, source.pointNormalized, MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memcpy( pointTransformed, source.pointTransformed, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		numOfSolarCycle = source.numOfSolarCycle;
		// Следующим переменным-членам намерено присвоено значение не имеющее смысла, которое сигнализирует о том, что эти переменные ещё не были проинициализированы!
		indexOfFirstFilteredDatum = -1;
		indexOfFirstDatumOfSolarCycle = source.indexOfFirstDatumOfSolarCycle;
		numOfValues = source.numOfValues;
		solarCycleCompleted = source.solarCycleCompleted;
		timeStepBasic = source.timeStepBasic;
		timeStepTranformed = source.timeStepTranformed;
		maxOfValue = source.maxOfValue;
	}

	/*
	DataOfSolarCycle::DataOfSolarCycle( int timeStep )
	{
		memset( pointBasic, 0, MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( pointNormalized, 0, MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( pointTransformed, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		timeStepBasic = timeStep;
	}
	*/

	SequenceOfSolarCycles::SequenceOfSolarCycles()
	{
		numOfSolarCycles = 0;
		memset( pointOfAverageCurve, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( standardDeviation, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		pSolarCycle = nullptr;
		pFirstSolarCycle = nullptr;
	}

	SequenceOfSolarCycles::SequenceOfSolarCycles( int numOfSolarCycles )
	{
		this->numOfSolarCycles = numOfSolarCycles;
		memset( pointOfAverageCurve, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( standardDeviation, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		pSolarCycle = new DataOfSolarCycle[numOfSolarCycles];
		//pFirstSolarCycle = pSolarCycle;
		pFirstSolarCycle = &pSolarCycle[0];
	}

	//SequenceOfSolarCycles::SequenceOfSolarCycles( list<DataOfSolarCycle> doubleList ) // Нет необходимости этот список doubleList копировать. Поэтому лучше параметр doubleList передать по ссылке.
	SequenceOfSolarCycles::SequenceOfSolarCycles( list<DataOfSolarCycle> &doubleList )
	{
		list<DataOfSolarCycle>::iterator currentIterator;

		numOfSolarCycles = doubleList.size();
		memset( pointOfAverageCurve, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		memset( standardDeviation, 0, STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE * sizeof( double ) );
		pSolarCycle = new DataOfSolarCycle[numOfSolarCycles];
		//ptrToSolarCycle = pFirstSolarCycle = pSolarCycle;
		//ptrToSolarCycle = pFirstSolarCycle = &pSolarCycle[0];

		// Инициализация массива pSolarCycle:
		int i = 0;
		for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
		{
			for( int k = 0; k < MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
			{
				pSolarCycle[i].pointBasic[k] = ( *currentIterator ).pointBasic[k];
				pSolarCycle[i].pointNormalized[k] = ( *currentIterator ).pointNormalized[k];
			}
			for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
				pSolarCycle[i].pointTransformed[k] = ( *currentIterator ).pointTransformed[k];
			i++;
		}

		// Построение усреднённого цикла:
		//i = 0;
		for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
		{
			for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
				pointOfAverageCurve[k] += ( *currentIterator ).pointTransformed[k];
				//pointOfAverageCurve[k] += pSolarCycle[i].pointTransformed[k];
			//i++;
		}
		
		/*
		for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
			for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
				pointOfAverageCurve[k] += ( *currentIterator ).pointTransformed[k];
		*/
		for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
		{
			pointOfAverageCurve[k] /= numOfSolarCycles;

			// Для отладки:
			//cout << endl << "pointOfAverageCurve[" << k << "]	=	" << pointOfAverageCurve[k];
		}
		//cout << endl;

		// Построение средне квадратичного отклонения каждой точки усреднённого цикла:
		
		for( int i = 0; i < numOfSolarCycles; i++ )
			for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
				//standardDeviation[k] += pow( pSolarCycle[i].pointTransformed[k] - pointOfAverageCurve[k], 2 );
				standardDeviation[k] += ( pSolarCycle[i].pointTransformed[k] - pointOfAverageCurve[k] ) * ( pSolarCycle[i].pointTransformed[k] - pointOfAverageCurve[k] );
		
		/*
		for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
			for( int i = 0; i < numOfSolarCycles; i++ )
				standardDeviation[k] += ( pSolarCycle[i].pointTransformed[k] - pointOfAverageCurve[k] ) * ( pSolarCycle[i].pointTransformed[k] - pointOfAverageCurve[k] );
		*/
		/*
		for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
			for( currentIterator = doubleList.begin(); currentIterator != doubleList.end(); ++currentIterator )
				standardDeviation[k] += ( ( *currentIterator ).pointTransformed[k] - pointOfAverageCurve[k] ) * ( ( *currentIterator ).pointTransformed[k] - pointOfAverageCurve[k] );
		*/
		for( int k = 0; k < STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE; k++ )
		{
			standardDeviation[k] = sqrt( standardDeviation[k] / numOfSolarCycles );

			// Для отладки:
			cout << endl << "pointOfAverageCurve[" << k << "]	=	" << pointOfAverageCurve[k] << "	standardDeviation[" << k << "]	=	" << standardDeviation[k];
		}
	}

	DataOfSolarCycle& SequenceOfSolarCycles::operator []( int index )
	{
		assert( index < this->numOfSolarCycles );
		return this->pSolarCycle[index];
	}

}