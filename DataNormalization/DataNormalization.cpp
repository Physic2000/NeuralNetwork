// Программа выполняет нормирование данных из файла имя которого передаётся программе через командную строку первым аргументом
// и записывает отнормированые данные в файл, имя которого передаётся программе вторым аргументом.
// Третьим аргументом программе надо передать имя файла, в который будут записаны максимумы числа солнечной активности внутри каждого Солнечного цикла - цикла Швабе-Вольфа (11 лет).

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

//const int NumOfRepetitionsOfFulfilledCondition = 2; // Кол-во подряд идущих повторов выполненного условия для фиксации цикла Вольфа.

//const double MaxToMinRatio = 7.7;
const double MaxToMinRatio = 7.0;
const double FactorOfIntensificationCondition = 0.7;
//const double MinimumMaximum = 35.0;
const double MinimumMaximum = 0;
const double MinOfDouble = numeric_limits<double>::lowest();
const double MaxOfDouble = numeric_limits<double>::max();

// Программе НАДО передать в командной строке: 
// 1) имя исходного файла с данными, которые будем нормировать													-	char* dataFile;
// 2) имя результирующего файла, в который будут записаны отнормированые данные									-	char* resFile;
// 3) имя файла, в который будут записаны максимумы числа солнечной активности внутри каждого Солнечного цикла	-	char* resFile_2;
int main( int argc, char* argv[] )
{
	char* dataFile;
	char* resFile;
	char* resFile_2;
	int numOfDatum, numOfPrevMin, numOfLastMin, numOfLastMax, numOfPeriods; //, numOfFulfilledCondition;
	double value, prevValue, prevPrevValue, lastMin, lastMax, currentMax, averageMaximum, averagePeriodSize;
	string tempStr, str;
	ifstream dataStream, dataStream_2;
	ofstream resStream, resStream_2;

	if ( argc == 4 )
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != 4 )!!!" << endl;
		return 1;
	}

	// Программа будет 2 раза полностью считывать данные из файла dataFile.
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		resStream_2.open( resFile_2 );

		// Считываем из потока данных первые 2 значения типа double:
		dataStream >> prevPrevValue >> prevValue;

		cout << endl;
		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 0;
		numOfDatum = 1;		// Т.к. нумерация данных начинается с 0 и 2 значения мы уже считали.
		averageMaximum = 0.0;
		averagePeriodSize = 0.0;
		do{	
			// Считываем построчно файл до конца.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// Определение текущего минимума:
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;

					cout << "lastMin  =  " << lastMin << endl	// Для отладки
						<< "numOfLastMin  =  " << numOfLastMin << endl;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// Определение текущего максимума:
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;

					cout << "lastMax  =  " << lastMax << endl	// Для отладки
						<< "numOfLastMax  =  " << numOfLastMax << endl;
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
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				/*if( ( lastMax / lastMin >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && 
					( lastMax / prevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && ( lastMax / value < MaxToMinRatio * FactorOfIntensificationCondition ) )*/
				{
					// ПЕРЕХОД К СЛЕДУЮЩЕМУ ЦИКЛУ:

					cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin 
						<< endl << endl << endl << endl; // Для отладки.

					//resStream_2 << lastMax << endl;
					resStream_2 << numOfLastMax << "	" << lastMax << endl; // Альтернативный формат вывода в файл, когда перед выводом значения текущего максимума выводится ещё и номер узла на котором достигается максимум.

					/*
					if( numOfPeriods == 0 ) numOfPrevMin = numOfLastMin;
					else
					{
						averagePeriodSize += numOfLastMin - numOfPrevMin;
						numOfPrevMin = numOfLastMin;
					}
					*/
					if( numOfPeriods > 0 )
						averagePeriodSize += numOfLastMin - numOfPrevMin;
					numOfPrevMin = numOfLastMin;

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
			// Фиксируем последний цикл.
			cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value
				<< endl << endl << endl << endl; // Для отладки.

			//resStream_2 << lastMax << endl;
			resStream_2 << numOfLastMax << "	" << lastMax << endl; // Альтернативный формат вывода в файл, когда перед выводом значения текущего максимума выводится ещё и номер узла на котором достигается максимум.

			averagePeriodSize /= numOfPeriods;
			numOfPeriods++;
			averageMaximum += lastMax;
		}
		else averagePeriodSize /= numOfPeriods;

		resStream_2.close();
	}
	dataStream.close();
	averageMaximum /= numOfPeriods;

	dataStream.open( dataFile );
	dataStream_2.open( resFile_2 );
	if( !dataStream.fail() && !dataStream_2.fail() )
	{
		resStream.open( resFile );

		// Снова считываем из потока данных первые 2 значения типа double:
		dataStream >> prevPrevValue >> prevValue;
		// Считываем значение первого максимума:
		//dataStream_2 >> currentMax;
		dataStream_2 >> currentMax >> currentMax; // Пропускаем в файле, содержащем значения максимумов, номер узла на котором достигается максимум.
		// Записываем в результирующий файл 2 отнормированных значения на первый максимум:
		resStream << ( prevPrevValue / currentMax ) << endl << ( prevValue / currentMax ) << endl;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 0;
		numOfDatum = 1;		// Т.к. нумерация данных начинается с 0 и 2 значения мы уже считали.
		do{	
			// Считываем построчно файл до конца.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;
				// Записываем в результирующий файл отнормированное значение на текущий максимум:
				resStream << ( value / currentMax ) << endl;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// определение текущего минимума;
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// определение текущего максимума;
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;
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
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				/*if( ( lastMax / lastMin >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) && ( lastMax / prevPrevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && 
					( lastMax / prevValue < MaxToMinRatio * FactorOfIntensificationCondition ) && ( lastMax / value < MaxToMinRatio * FactorOfIntensificationCondition ) )*/
				{
					// Переход к следующему циклу.
					//dataStream_2 >> currentMax;
					dataStream_2 >> currentMax >> currentMax; // Пропускаем в файле, содержащем значения максимумов, номер узла на котором достигается максимум.
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
			// Фиксируем последний цикл.
			numOfPeriods++;
		}

		resStream.close();
	}
	dataStream.close();
	dataStream_2.close();

	numOfDatum++; // Т.к. нумерация данных начиналась с 0, то общее число данных на 1 больше, чем номер последнего данного.
	cout << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numberOfPeriods  =  " << numOfPeriods << endl
		 << "averageMaximum  =  " << averageMaximum << endl
		 << "averagePeriodSize  =  " << averagePeriodSize << endl;
		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;

	return 0;
}