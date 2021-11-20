// SolarDataStructures.h
// ���� ������������ ���� ���������� ����� ��������� ������, ����������� ����� ��������� ����������.

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
		DataOfSolarCycle();											// ����������� �� ���������.
		DataOfSolarCycle( const DataOfSolarCycle &source );			// ����������� �����������.
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
		SequenceOfSolarCycles();	// ����������� �� ���������.
		SequenceOfSolarCycles( int numOfSolarCycles );
		//SequenceOfSolarCycles( list<DataOfSolarCycle> doubleList ); // ��� ������������� ���� ������ doubleList ����������. ������� ����� �������� doubleList �������� �� ������.
		SequenceOfSolarCycles( list<DataOfSolarCycle> &doubleList );
	private:
		int numOfSolarCycles;		// ����� ����� ������ ��������� ����������.
	public:
		int NumOfSolarCycles() { return numOfSolarCycles; };		// ���������� ����� ����� ������ ��������� ����������, ���������� � ������������������.
		double pointOfAverageCurve[STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE], standardDeviation[STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE]; // standardDeviation - ����������� ���������� ��� ����� ���������� ������.
		DataOfSolarCycle* pSolarCycle;
		DataOfSolarCycle& operator []( int index );
		DataOfSolarCycle* PtrToFirstSolarCycle(){ return pFirstSolarCycle; };
	private:
		DataOfSolarCycle* pFirstSolarCycle;
	};

}