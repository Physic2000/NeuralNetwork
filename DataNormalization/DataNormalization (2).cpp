// ��������� ��������� ������������ ������ �� ����� ��� �������� ��������� ��������� ����� ��������� ������ ������ ����������
// � ���������� �������������� ������ � ����, ��� �������� ��������� ��������� ������ ����������.
// ������� ���������� ��������� ���� �������� ��� �����, � ������� ����� �������� ��������� ����� ��������� ���������� ������ 
// ������� ���������� ����� - ����� �����-������ (11 ���).

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

// ��������� ���� �������� � ��������� ������: 
// 1) ��� ��������� ����� � �������, ������� ����� �����������													-	char* dataFile;
// 2) ��� ��������������� �����, � ������� ����� �������� �������������� ������									-	char* resFile;
// 3) ��� �����, � ������� ����� �������� ��������� ����� ��������� ���������� ������ ������� ���������� �����	-	char* resFile_2;
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
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		dataFile = argv[1];						
		resFile = argv[2];
		resFile_2 = argv[3];
	}
	else
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line: ( argc != 4 )!!!" << endl;
		return 1;
	}

	// ��������� ����� 2 ���� ��������� ��������� ������ �� ����� dataFile.
	dataStream.open( dataFile );
	if( !dataStream.fail() )
	{
		resStream_2.open( resFile_2 );

		// ��������� �� ������ ������ ������ 2 �������� ���� double:
		dataStream >> prevPrevValue >> prevValue;

		cout << endl;
		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 0;
		numOfDatum = 1;		// �.�. ��������� ������ ���������� � 0.
		averageMaximum = 0.0;
		do{	
			// ��������� ��������� ���� �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// ����������� �������� ��������;
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;

					cout << "lastMin  =  " << lastMin << endl	// ��� �������
						<< "numOfLastMin  =  " << numOfLastMin << endl;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// ����������� �������� ���������;
				{
					lastMax = prevValue;
					lastMin = MaxOfDouble;
					numOfLastMax = numOfDatum - 1;

					cout << "lastMax  =  " << lastMax << endl	// ��� �������
						<< "numOfLastMax  =  " << numOfLastMax << endl;
				}

				//if( ( lastMin != MaxOfDouble ) && ( lastMax / lastMin >= MaxToMinRatio ) )
				//if( lastMax / lastMin >= MaxToMinRatio )
				//if( abs( lastMax / lastMin ) >= MaxToMinRatio )
				if( ( abs( lastMax / lastMin ) >= MaxToMinRatio ) && ( lastMax != MinOfDouble ) )
				{
					// ������� � ���������� �����.
					cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / lastMin  =  " << lastMax / lastMin 
						<< endl << endl << endl << endl; // ��� �������.

					//resStream_2 << lastMax << endl;
					resStream_2 << numOfLastMax << "	" << lastMax << endl; // �������������� ������ ������ � ����, ����� ����� ������� �������� �������� ��������� ��������� ��� � ����� ���� �� ������� ����������� ��������.

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
			// ��������� ��������� ����.
			cout << endl << "numOfPeriod  =  " << numOfPeriods << endl << "lastMax / value  =  " << lastMax / value
				<< endl << endl << endl << endl; // ��� �������.

			//resStream_2 << lastMax << endl;
			resStream_2 << numOfLastMax << "	" << lastMax << endl; // �������������� ������ ������ � ����, ����� ����� ������� �������� �������� ��������� ��������� ��� � ����� ���� �� ������� ����������� ��������.

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

		// ����� ��������� �� ������ ������ ������ 2 �������� ���� double:
		dataStream >> prevPrevValue >> prevValue;
		// ��������� �������� ������� ���������:
		//dataStream_2 >> currentMax;
		dataStream_2 >> currentMax >> currentMax; // ���������� � �����, ���������� �������� ����������, ����� ���� �� ������� ����������� ��������.
		// ���������� � �������������� ���� 2 ��������������� �������� �� ������ ��������:
		resStream << ( prevPrevValue / currentMax ) << endl << ( prevValue / currentMax ) << endl;

		lastMin = MaxOfDouble;
		lastMax = MinOfDouble;
		numOfLastMin = 0;
		numOfLastMax = 0;
		numOfPeriods = 1;
		numOfDatum = 1;		// �.�. ��������� ������ ���������� � 0.
		do{	
			// ��������� ��������� ���� �� �����.
            getline( dataStream, tempStr );
			if( tempStr.size() )						// if( tempStr.size() > 0 )
			{
				istringstream str( tempStr );
				str >> value;
				numOfDatum++;
				// ���������� � �������������� ���� ��������������� �������� �� ������� ��������:
				resStream << ( value / currentMax ) << endl;

				if( ( prevPrevValue >= prevValue ) && ( prevValue <= value ) && ( prevValue < lastMin ) )
				// ����������� �������� ��������;
				{
					lastMin = prevValue;
					numOfLastMin = numOfDatum - 1;
				}

				if( ( prevPrevValue <= prevValue ) && ( prevValue >= value ) && ( prevValue > lastMax ) && ( prevValue > MinimumMaximum ) )
				// ����������� �������� ���������;
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
					// ������� � ���������� �����.
					//dataStream_2 >> currentMax;
					dataStream_2 >> currentMax >> currentMax; // ���������� � �����, ���������� �������� ����������, ����� ���� �� ������� ����������� ��������.
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