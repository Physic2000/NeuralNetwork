// Программа выполняет нормирование данных из файла имя которого передаётся программе через командную строку первым аргументом
// и записывает отнормированые данные в файл, имя которого передаётся программе вторым аргументом.
// Третьим аргументом программе надо передать имя файла, в который будут записаны максимумы числа солнечной активности внутри 
// каждого Солнечного цикла - цикла Швабе-Вольфа (11 лет).

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

const double MaxToMinRatio = 7.7;
const double MinimumMaximum = 35.0;
//const double MinimumMaximum = 0;
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
	int numOfDatum, numOfLastMin, numOfLastMax, numOfPeriods;
	double value, prevValue, prevPrevValue, lastMin, lastMax, currentMax, averageMaximum;
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
				//if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				{
					// Переход к следующему циклу.
					cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin 
						<< endl << endl << endl << endl; // Для отладки.

					//resStream_2 << lastMax << endl;
					resStream_2 << numOfLastMax << "	" << lastMax << endl; // Альтернативный формат вывода в файл, когда перед выводом значения текущего максимума выводится ещё и номер узла на котором достигается максимум.

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

			//resStream_2 << lastMax << endl;
			resStream_2 << numOfLastMax << "	" << lastMax << endl; // Альтернативный формат вывода в файл, когда перед выводом значения текущего максимума выводится ещё и номер узла на котором достигается максимум.

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
		// Считываем значение первого максимума:
		//dataStream_2 >> currentMax;
		dataStream_2 >> currentMax >> currentMax; // Пропускаем в файле, содержащем значения максимумов, номер узла на котором достигается максимум.
		// Записываем в результирующий файл 2 отнормированных значения на первый максимум:
		resStream << ( prevPrevValue / currentMax ) << endl << ( prevValue / currentMax ) << endl;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 1;
		numOfDatum = 1;		// Т.к. нумерация данных начинается с 0.
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
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
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

		resStream.close();
	}
	dataStream.close();
	dataStream_2.close();

	cout << endl
		 << "numOfDatum  =  " << numOfDatum << endl
		 << "numberOfPeriods  =  " << numOfPeriods << endl
		 << "averageMaximum  =  " << averageMaximum << endl;
		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;

	return 0;
}