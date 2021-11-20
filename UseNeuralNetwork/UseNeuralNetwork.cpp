// ������������� ������������ ��������� ���� ������, ������� 1 ������ � �������� �������� ��������� � �������� ����.
// ����������� ��� ������� ����� ��������������� ��������� �����.
/*
UseNeuralNetwork - ��������� ��������� ������� �������� ��� � ��������� �� ������ ��� ��������� ��� �������������� ����, ���������� ������������� ������, �.�. ������ ��������������� ���.
UseNeuralNetwork.exe � �������� 1-�� ��������� ��������� ��� ����� c �������������� �������,
� �������� 2-�� ��������� - ��� ����� c ��������� �������,
� �������� 3-�� ��������� - ��� ��������������� ����� � ������� ����� �������� ���������� ������ ��� ������, �.�. ������ ��������������� ���,
� �������� 4-�� ��������� - c������� �������� ��������� ����, �.�. ����� ���� double ( 0.001 <= a <= 0.5 ),
� �������� 5-�� ��������� - ������ (�������) �������� ��������� ����, �.�. ����� ���� double ( 0 <= mu <= 1.0 ),
� �������� 6-�� ��������� - ����������� ���������� ����� ������ ��������, �.�. ����� ���� int ( 100, 200, 500 ),
� �������� 7-�� ��������� - ����������� ���������� ����� ��������� ������ �� �������� ������, �.�. ����� ���� double,
� �������� 8-�� ��������������� ��������� ��������� ����� ������� ����������� �������� �������, �.�. ����� �������� �������� ����,
� �������� 9-�� ��������������� ��������� ��������� ����� ������� ����� �������� �������� ����,
� �������� 10-�� ��������������� ��������� ��������� ����� ������� ��������� �� ��� ����� ��������������� ������.
���� exe-���� ���������� ��� �������������� ����������, �� �� ��������� ����� �������� �������� ���� ����� 6, ����� �������� �������� ���� ����� 10, 
� ��������� �� ��� ����� ��������������� ������ ����� ����� �������� � �������� ������, �.�. ����� ����� �������� ������ ������������ �� ���������� �������� �������� ����.
*/

#include <string>
#include <sstream>
#include <ctime>
#include "RecurrentNeuralNetwork.h"
using namespace std;
using namespace NeuralNetwork;

// ***** ��������� ����������� ��������� ���� �� ���������: *****

int		n = 6,					// ����������� �������� �������, �.�. ����� �������� �������� ���� ��� ����� ������ �� ���������.
		m = 10;					// ����� �������� �������� ���� �� ���������.

// ***** ���������� ���������� ���������������� ������� �������� ��������� ����: *****

int maxNumOfTrainingCycles;		// = 100;	// ����������� ���������� ����� ������ ��������.
//int maxNumOfTrainingCycles;	// = 200;	// ����������� ���������� ����� ������ ��������.
//int maxNumOfTrainingCycles;	// = 500;	// ����������� ���������� ����� ������ ��������.
//double maxSumOfErrorsSquares; // = 0.1;	// ����������� ���������� ����� ��������� ������ �� �������� ������.
double maxSumOfErrorsSquares; // = 0.001;	// ����������� ���������� ����� ��������� ������ �� �������� ������.

// ��������� ���� �������� � ��������� ������:
// 1) ��� ����� c �������������� �������;
// 2) ��� ����� c ��������� �������;
// 3) ��� ��������������� ����� � ������� ����� �������� ���������� ������ ��� ������, �.�. ������ ��������������� ���;
// 4) c������� �������� ��������� ����, �.�. ����� ���� double ( 0.001 <= a <= 0.5 );
// 5) ������ (�������) �������� ��������� ����, �.�. ����� ���� double ( 0 <= mu <= 1.0 );
// 6) ����������� ���������� ����� ������ ��������, �.�. ����� ���� int ( 100, 200, 500 );
// 7) ����������� ���������� ����� ��������� ������ �� �������� ������, �.�. ����� ���� double.
// ������������� ��������� ����� �������� � ��������� ������:
// 8) ����������� �������� �������, �.�. ����� �������� �������� ����;
// 9) ����� �������� �������� ����;
// 10) ��������� �� ��� ����� ��������������� ������.
int main( int argc, char* argv[] )
{
	char* trainingData;										// ������������� ������.
	char* testData;											// �������� ������.
	char* resultData;										// ���� ��� ������ ���������� ������ ��������� ����.
	double	a,					// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
			mu,					// ������ (�������) �������� ��������� ���� ( 0 <= mu <= 1.0 ). ��� �������� ��������� �������� �� ��������� ��������� ���������.
			datum, *testDatum, sumOfErrorsSquares, y;
	ifstream trainingStream, testStream;
	ofstream resultStream;
	int numOfTrainingDatum, numOfTrainingExamples, numOfTestDatum, numOfTestExamples, numOfTrainingCycles, j, k, n, m, numOfSimulatedDatum;
	
	if ( argc == 8 )			// ������ ��������� � 7-��� ����������� � ��������� ������.
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		// ����������� ��������� ���������� �������� ���������� ���������� ���������� (�������� �� ���������):
		n = ::n;				// ����������� ��������� ���������� �������� ���������� ����������.
		m = ::m;				// ����������� ��������� ���������� �������� ���������� ����������.
		numOfSimulatedDatum = 0;
	}
	else if ( argc == 9 )	// ������ ��������� � 8-��� ����������� � ��������� ������.
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		numOfSimulatedDatum = atoi( argv[8] );
		// ����������� ��������� ���������� �������� ���������� ���������� ���������� (�������� �� ���������):
		n = ::n;				// ����������� ��������� ���������� �������� ���������� ����������.
		m = ::m;				// ����������� ��������� ���������� �������� ���������� ����������.
	}
	else if ( argc == 10 )	// ������ ��������� � 9-��� ����������� � ��������� ������.
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		n = atoi( argv[8] );
		m = atoi( argv[9] );
		numOfSimulatedDatum = 0;
	}
	else if ( argc == 11 )	// ������ ��������� � 10-��� ����������� � ��������� ������.
	{
		// ����������� ��������� ���������� �������� ���������� ����� ��������� ������:
		trainingData = argv[1];
		testData = argv[2];
		resultData = argv[3];
		a = atof( argv[4] );
		mu = atof( argv[5] );
		maxNumOfTrainingCycles = atoi( argv[6] );
		maxSumOfErrorsSquares = atof( argv[7] );
		n = atoi( argv[8] );
		m = atoi( argv[9] );
		numOfSimulatedDatum = atoi( argv[10] );
	}
	else					// ������ ��������� � ����������������� ���-�� ���������� � ��������� ������.
	{
		cout << "ERROR: starting the program with an unexpected number of arguments on the command line!!!" << endl;
		return 1;
	}
	cout << endl;

	//RecurrentNeuralNetwork rnn( trainingData, testData, a, mu, n, m );	// ������������� ������������ ��������� ����.
	RecurrentNeuralNetwork rnn( trainingData, testData, a, mu );	// ������������� ������������ ��������� ����.
	//cout << endl << "After calling constructor RecurrentNeuralNetwork(...)" << endl;
	unsigned int start_time =  clock(); // ��������� �����
	// 1-�� ������� �������� ��������� ����:
	rnn.Train( maxSumOfErrorsSquares, sumOfErrorsSquares, maxNumOfTrainingCycles, numOfTrainingCycles ); // �������� ��������� ����.
	/*
	// 2-�� ������� �������� ��������� ����:
	rnn.Train( maxNumOfTrainingCycles ); // ���������� �������� ��������� ����, �������� ����� maxNumOfTrainingCycles ������ ��������.
	numOfTrainingCycles = rnn.NumOfCompletedTrainingCycles();
	sumOfErrorsSquares = rnn.SumOfErrorsSquares();
	*/
	/*
	// 3-�� ������� �������� ��������� ����:
	//maxSumOfErrorsSquares = 3.3;
	rnn.Train( maxSumOfErrorsSquares ); // ���������� �������� ��������� ���� �� ��� ��� ���� sumOfErrorsSquares �� ������������� ������ ������, ��� �������� ������� - maxSumOfErrorsSquares.
	numOfTrainingCycles = rnn.NumOfCompletedTrainingCycles();
	sumOfErrorsSquares = rnn.SumOfErrorsSquares();
	*/
	unsigned int end_time = clock(); // �������� �����
	unsigned int timeOfTraining = end_time - start_time; // ������� ����� �������� ��� � �������������
	
	//cout << endl << "After calling member function Train()" << endl;

	numOfTrainingDatum = rnn.NumOfTrainingDatum();
	numOfTrainingExamples = rnn.NumOfTrainingExamples();
	numOfTestDatum = rnn.NumOfTestDatum();
	numOfTestExamples = rnn.NumOfTestExamples();

	cout << "\nn  =  " << n << endl
		<< "m  =  " << m << endl << endl

		<< "a  =  " << a << endl
		<< "mu  =  " << mu << endl << endl

		<< "maxSumOfErrorsSquares  =  " << maxSumOfErrorsSquares << endl
		<< "maxNumOfTrainingCycles  =  " << maxNumOfTrainingCycles << endl << endl << endl;

	cout << "numOfTrainingDatum  =  " << numOfTrainingDatum << endl
		<< "numOfTrainingExamples  =  " << numOfTrainingExamples << endl
		<< "numOfTestDatum  =  " << numOfTestDatum << endl
		<< "numOfTestExamples  =  " << numOfTestExamples << endl
		<< "sumOfErrorsSquares  =  " << sumOfErrorsSquares << endl
		<< "numOfTrainingCycles  =  " << numOfTrainingCycles << endl << endl

		<< "timeOfTraining  =  " << timeOfTraining << " msec" << endl 
		<< "runtime  =  " << clock() / 1000.0 << " sec" << endl << endl;

	if( numOfSimulatedDatum <= 0 ) numOfSimulatedDatum = numOfTestExamples - numOfSimulatedDatum;	// �.�. ���� numOfSimulatedDatum <= 0, �� numOfSimulatedDatum = numOfTestExamples + abs( numOfSimulatedDatum );
	if( numOfSimulatedDatum == numOfTestDatum - n )
		cout << "	Test data:		Simulated datum:		Delta:" << endl;
	else
		cout << "	Simulated datum:" << endl;

	Vector x( n ), x0_test( n );	// ��� x( n ) - ��� ������� ������ ������� ������ ��� ���, � x0_test( n ) - ��� ����� ������ ������ ������� ������ �� ��������� ������ ������;
	testDatum = rnn.testDatum;	// ��� ��������� �� ����� ������ �������� � ���� �������� ������.
	for( j = 0; j < n; j++ ) x0_test[j] = testDatum[j]; // ��� ����� ������ ������ ������� �������� �� ��������� ������ ������.
	
	testStream.open( testData );
	resultStream.open( resultData );
	for( j = 0; j < n; j++ ) testStream >> datum;	// ��������� �� ������ �������� ������ ��� ��������� n ���� �� �������� �� ������� testDatum.

	x = x0_test;	// ������������� ������� x ������� n ���������� �� ��������� ������ ������.
	for( k = 0; k < numOfSimulatedDatum; k++ )
	{
		y = rnn.y( x );

		if( numOfSimulatedDatum == numOfTestDatum - n )
		{
			testStream >> datum;

			cout << k << ":		" << datum << "		" << y << "		" << datum - y << endl;
		}
		else
			cout << k << ":		" << y << endl;

		resultStream << y << endl; // ����� ���������������� ��� �������� � txt ����.

		for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
		x[n - 1] = y;
	}
	testStream.close();
	resultStream.close();

/*
	cout << "\nPress any key and [Enter] ";
	cin >> tempStr;
*/
	return 0;

}// End main( ... )