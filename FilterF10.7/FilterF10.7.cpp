// ������ ���������� �� ����� "RecentIndices.txt" ���������� �������� F10.7 � ������������ �� � ��������� �����.

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

char* dataFile = "RecentIndices.txt";		// �������� ����, �������� �������� �������.
char* resFile_1 = "Data_Time_F10.7.txt";	// ���� � ������� ���������� ��������� ����� ������ � �����.
char* resFile_2 = "Data_F10.7.txt";			// ���� � ������� ���������� ��������� ����� ������ ��� ����.
char* resFile_3 = "Data_Index_F10.7.txt";	// ���� � ������� ���������� ��������� ����� ������ � ��������.

int main( int argc, char* argv[] )
{
	double currentValue;
	char buffer[20] = {};
	int i, indexOfValues, numOfValues, year, month;
	ifstream in_stream;
	ofstream out_stream_1, out_stream_2, out_stream_3;
	string tempStr, str;

	in_stream.open( dataFile );
	if( !in_stream.fail() )
	{
		numOfValues = 0;
		do{	
			// ���� �������� ���-�� ��������:
			// ���������� ������ �� ����� ������:
            getline( in_stream, tempStr );
			if ( tempStr.size() && ( ( tempStr[0] == '#' ) || ( tempStr[0] == ':' ) ) )
			    continue;
			//cout << tempStr;

			istringstream str( tempStr );
			for(i = 0; i <= 8; i++) str >> currentValue;
			if ( currentValue > 0 ) numOfValues++;
		} while( !in_stream.eof() );
		in_stream.close();

		in_stream.open( dataFile );
		//out_stream_1.open( resFile_1 );
		out_stream_2.open( resFile_2 );
		out_stream_3.open( resFile_3 );

		indexOfValues = 0;
		while( indexOfValues < numOfValues )
		{
			// ���������� ������ �� ����� ������:
            getline( in_stream, tempStr );
			if ( tempStr.size() && ( ( tempStr[0] == '#' ) || ( tempStr[0] == ':' ) ) )
			    continue;
			//cout << tempStr;

			istringstream str( tempStr );

			str >> year >> month;
			for(i = 0; i <= 6; i++) str >> currentValue;

			// ������ ��������� �������� � ��������������� �����:
			/*
			out_stream_1 << year << ".";
			if ( month < 10 )
				out_stream_1 << "0" << month << "	" << currentValue << endl;
			else
				out_stream_1 << month << "	" << currentValue << endl;
			*/
			out_stream_2 << currentValue << endl;
			out_stream_3 << indexOfValues << "	" << currentValue << endl;
			//cout << year << "." << month << "	" << currentValue << endl;

			indexOfValues++;
		}

		//out_stream_1.close();
		out_stream_2.close();
		out_stream_3.close();
	}
	in_stream.close();

	cout << endl
		 << "numOfValues  =  " << numOfValues << endl;
		
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
	return 0;
}