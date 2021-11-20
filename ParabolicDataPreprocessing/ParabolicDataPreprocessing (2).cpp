// Программа выполняет пораболическую предобработку данных из файла имя которого передаётся программе через командную строку первым аргументом
// и записывает обработанные данные в файл, имя которого передаётся программе вторым аргументом.
// Третьим аргументом программе надо передать имя файла, в который будут записаны максимумы числа солнечной активности внутри 
// каждого Солнечного цикла - цикла Швабе-Вольфа (11 лет). Четвёртым аргументом программе можно передать кол-во точек по которым будет строиться порабола в окрестностях экстремумов.

#include "Approximation.h"
/*
Следующие 2 закомментированных заголовочных файла и соответствующие им "*.cpp" файлы ОБЯЗАТЕЛЬНО ДОЛЖНЫ НАХОДИТЬСЯ в той же директории, 
что и cpp файл включающий заголовочный файл "Approximation.h"! Иначе возникают ошибки на этапе сборки программы:
"1>c1xx : fatal error C1083: Cannot open source file: 'SystemsOfLinearEquations.cpp': No such file or directory
1>  Matrixes3.cpp
1>c1xx : fatal error C1083: Cannot open source file: 'Matrixes3.cpp': No such file or directory
1>  Approximation.cpp
1>...\approximation.cpp(2): fatal error C1083: Cannot open include file: 'Matrixes3.h': No such file or directory"
*/
//#include "Matrixes3.h"
//#include "SystemsOfLinearEquations.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Approximation;

const int NumOfDatumForPorabola = 13;	// Кол-во точек по которым будет строиться порабола в окрестностях экстремумов по умолчанию, если четвёртый аргумент при вызове программы отсутствовал.

const double MaxToMinRatio = 7.7;
//const double MinimumMaximum = 35.0;
const double MinimumMaximum = 0;
const double MinOfDouble = numeric_limits<double>::lowest();
const double MaxOfDouble = numeric_limits<double>::max();

template<typename Type> Type abs( Type a );

// Программе НАДО передать в командной строке: 
// 1) имя исходного файла с данными, которые будем обрабатывать													-	char* dataFile;
// 2) имя результирующего файла, в который будут записаны обработанные данные									-	char* resFile;
// 3) имя файла, в который будут записаны максимумы числа солнечной активности внутри каждого Солнечного цикла	-	char* resFile_2;
// и 4-ым аргументом можно передать кол-во точек по которым будет строиться порабола в окрестностях экстремумов -	int numOfDatumForPorabola;
int main( int argc, char* argv[] )
{
	char* dataFile;
	char* resFile;
	char* resFile_2;
	int numOfDatum, numOfLastMin, numOfLastMax, numOfCurrentMin, numOfCurrentMax, numOfPeriods, numOfDatumForPorabola, maxNumOfDatumToExtremumInArray;
	double value, prevValue, prevPrevValue, lastMin, lastMax, currentMin, currentMax, averageMaximum, *arrayOfIndexes, *arrayOfDatum;
	string tempStr, str;
	ifstream dataStream, dataStream_2;
	ofstream resStream, resStream_2;
	ParabolicApproximation parabolicAppr;

	if ( argc == 4 )
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		numOfDatumForPorabola = NumOfDatumForPorabola;
	}
	else if ( argc == 5 )
	{
		// Присваиваем локальным переменным значения переданные через командную строку:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
		numOfDatumForPorabola = atoi( argv[4] );
		assert( ( numOfDatumForPorabola % 2 == 1 ) && ( numOfDatumForPorabola >= 3 ) );
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != 4 OR 5 )!!!" << endl;
		return 1;
	}

	arrayOfIndexes = new double[numOfDatumForPorabola];
	arrayOfDatum = new double[numOfDatumForPorabola];
	maxNumOfDatumToExtremumInArray = numOfDatumForPorabola / 2;
	for( int i = 0; i < numOfDatumForPorabola; i++ ) arrayOfIndexes[i] = i;

	// Программа будет 2 раза полностью считывать данные из файла dataFile.
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		resStream_2.open( resFile_2 );

		// Считываем из потока данных первые 2 значения типа double:
		dataStream >> prevPrevValue >> prevValue;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 1;
		numOfDatum = 1;		// Т.к. нумерация данных начинается с 0.
		averageMaximum = 0.0;
		do{	
			// Считываем построчно файл до конца.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// определение текущего минимума;
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;

					cout << "lastMin  =  " << lastMin << endl	// Для отладки
						<< "numOfLastMin  =  " << numOfLastMin << endl;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// определение текущего максимума;
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;

					cout << "lastMax  =  " << lastMax << endl	// Для отладки
						<< "numOfLastMax  =  " << numOfLastMax << endl;
				}

				//if( ( lastMin != MaxOfDouble ) && ( lastMax / lastMin >= MaxToMinRatio ) )
				//if( lastMax / lastMin >= MaxToMinRatio )
				if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				{
					// Переход к следующему циклу.
					cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin 
						<< endl << endl << endl << endl; // Для отладки.

					// Формат вывода в файл, когда перед выводом значения текущего экстремума выводится ещё и номер узла на котором достигается экстремум:
					resStream_2 << numOfLastMax << "	" << lastMax << endl;
					resStream_2 << numOfLastMin << "	" << lastMin << endl;

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
		if( ( abs( lastMax / lastMin ) < MaxToMinRatio ) && ( abs( lastMax / value ) >= MaxToMinRatio ) && ( lastMax > MinimumMaximum ) )
		{
			// Фиксируем последний цикл.
			cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value
				<< endl << endl << endl << endl; // Для отладки.

			// Сохраняем в файл данные о последнем максимуме:
			resStream_2 << numOfLastMax << "	" << lastMax << endl;

			numOfPeriods++;
			averageMaximum += lastMax;
		}

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
		// Считываем значения первых двух экстремумов:
		dataStream_2 >> numOfCurrentMax >> currentMax;
		dataStream_2 >> numOfCurrentMin >> currentMin;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 1;
		numOfDatum = 1;		// Т.к. нумерация данных начинается с 0.

		// Записываем в результирующий файл 2 неизменённых считанных значения, только если ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray ):
		//if( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray )
		if( numOfCurrentMax - numOfDatum > maxNumOfDatumToExtremumInArray )
			resStream << prevPrevValue << endl << prevValue << endl;
		else
		{
			arrayOfDatum[0] = prevPrevValue;
			arrayOfDatum[1] = prevValue;
		}

		do{	
			// Считываем построчно файл до конца.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;

				// Записываем в результирующий файл неизменённое считанное значение, только 
				// если ( ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray ) && ( abs( numOfCurrentMin - numOfDatum ) > maxNumOfDatumToExtremumInArray ) ):
				if( ( abs( numOfCurrentMax - numOfDatum ) > maxNumOfDatumToExtremumInArray ) && ( abs( numOfCurrentMin - numOfDatum ) > maxNumOfDatumToExtremumInArray ) )
					resStream << value << endl;
				else
				{
				// иначе мы находимся или в близи максимума цикла или в близи минимума цикла.
					// Если мы находимся в близи максимума цикла:
					if( abs( numOfCurrentMax - numOfDatum ) <= maxNumOfDatumToExtremumInArray )
					{
						// считанное значение добавляем в массив arrayOfDatum, для последующего применения пораболической аппроксимации:
						arrayOfDatum[maxNumOfDatumToExtremumInArray - ( numOfCurrentMax - numOfDatum )] = value;

						if( maxNumOfDatumToExtremumInArray - ( numOfCurrentMax - numOfDatum ) == numOfDatumForPorabola - 1 ) // т.е. если массив arrayOfDatum заполнился полностью:
						{
							parabolicAppr.Init( arrayOfIndexes, arrayOfDatum, numOfDatumForPorabola );
							//parabolicAppr.Update_a_b_c_terms();
							//for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY( i ) << endl;
							for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY2( i ) << endl;
						}
					}
					// Если мы находимся в близи минимума цикла:
					else
					//if( abs( numOfCurrentMin - numOfDatum ) <= maxNumOfDatumToExtremumInArray )
					{
						// считанное значение добавляем в массив arrayOfDatum, для последующего применения пораболической аппроксимации:
						arrayOfDatum[maxNumOfDatumToExtremumInArray - ( numOfCurrentMin - numOfDatum )] = value;

						if( ( maxNumOfDatumToExtremumInArray - ( numOfCurrentMin - numOfDatum ) == numOfDatumForPorabola - 1 ) ) // т.е. если массив arrayOfDatum заполнился полностью:
						{
							parabolicAppr.Init( arrayOfIndexes, arrayOfDatum, numOfDatumForPorabola );
							//parabolicAppr.Update_a_b_c_terms();
							//for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY( i ) << endl;
							for( int i = 0; i < numOfDatumForPorabola; i++ ) resStream << parabolicAppr.PredictedY2( i ) << endl;

							dataStream_2 >> numOfCurrentMin >> currentMin;
						}
					}
				}

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
				if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				{
					// Переход к следующему циклу.
					numOfPeriods++;
					dataStream_2 >> numOfCurrentMax >> currentMax;
					lastMin = MaxOfDouble;
					lastMax = MinOfDouble;
				}
				
				prevPrevValue = prevValue;
				prevValue = value;
			}
		} while( !dataStream.eof() );

		resStream.close();
	}
	dataStream.close();
	dataStream_2.close();
	delete( arrayOfDatum );

	cout << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numberOfPeriods  =  " << numOfPeriods << endl
		 << "averageMaximum  =  " << averageMaximum << endl;
		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;

	return 0;
}

template<typename Type> Type abs( Type a )
{
	return ( a > 0 )? a: -a;
}