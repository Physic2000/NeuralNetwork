// SolarDataStructures.h
// Этот заголовочный файл определяет общие структуры данных, описывающих циклы солнечной активности.

#pragma once

#define MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE 224
#define STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE 132
#define MINIMUM_NUM_OF_DATUM_IN_SOLAR_CYCLE 96

#include <list>
#include <string.h>

using namespace std;

namespace SolarDataStructures 
{
	
	struct DataOfSolarCycle
	{
		DataOfSolarCycle();											// Конструктор по умолчанию.
		DataOfSolarCycle( const DataOfSolarCycle &source );			// Конструктор копирования.
		//DataOfSolarCycle( int timeStep );
		int numOfSolarCycle, indexOfFirstDatumOfSolarCycle, indexOfFirstFilteredDatum, numOfValues;
		bool solarCycleCompleted;
		double timeStepBasic, timeStepTranformed, maxOfValue;
		double pointBasic[MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE];
		double pointNormalized[MAX_NUM_OF_DATUM_IN_SOLAR_CYCLE];
		double pointTransformed[STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE];
	};
	
	struct SequenceOfSolarCycles
	{
		SequenceOfSolarCycles();	// Конструктор по умолчанию.
		SequenceOfSolarCycles( int numOfSolarCycles );
		//SequenceOfSolarCycles( list<DataOfSolarCycle> doubleList ); // Нет необходимости этот список doubleList копировать. Поэтому лучше параметр doubleList передать по ссылке.
		SequenceOfSolarCycles( list<DataOfSolarCycle> &doubleList );
	private:
		int numOfSolarCycles;		// Общее число циклов солнечной активности.
	public:
		int NumOfSolarCycles() { return numOfSolarCycles; };		// Возвращает общее число циклов солнечной активности, хранящихся в последовательности.
		double pointOfAverageCurve[STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE], standardDeviation[STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE]; // standardDeviation - стандартное отклонение для точек усреднённой кривой.
		DataOfSolarCycle* pSolarCycle;
		DataOfSolarCycle& operator []( int index );
		DataOfSolarCycle* PtrToFirstSolarCycle(){ return pFirstSolarCycle; };
	private:
		DataOfSolarCycle* pFirstSolarCycle;
	};

}