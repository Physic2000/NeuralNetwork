// NeuralNetworkDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
//#include <math.h>
//#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Matrixes3.h"
#include "RecurrentNeuralNetwork.h"

using namespace std;
using namespace NeuralNetwork;

typedef double* PtrDouble;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

extern "C"
{
	__declspec(dllexport) void __stdcall Training( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
	{
		// ������������ �����������: �� ������� ���������, ���������� ������� Training(...), �� ������ res ������ ���� �������� ����� �� ���� ��� ���������� �������� Training(...) ->
		// ( numOfTrainingCycles + 1 ) * numOfSimulatedDatum ����� double.
		// filenameOfTrainingData	- ��� �����, ����������� ������������� ������;
		// filenameOfTestData		- ��� ����� c ��������� �������, ������� ������ ���� ����������� � �������������� �������;	
		// numOfTrainingCycles		- ����� ������ ��������;
		// numOfSimulatedDatum		- ����� ��������� ����, ������������� ��������� ����� (���);
		// res						- �������������� ������ ����� double ���������� ���������������� ����������� numOfSimulatedDatum ��������, ������������ ���, ���������� ����� ����������� ������� ����� �������� ���.

		int n = N, k, i, j, p, numOfTrainingExamples;
		double e, x_simulated, sumOfErrorsSquares, *trainingDatum, *testDatum;
		Vector	x0_training( n ),	// - ����� ������ ������ ������� ������ �� �������������� ������ ������;
				x( n ),				// - ������� ������ ������� ������ �� �������������� ������ ������;
				x0_test( n ),		// - ����� ������ ������ ������� ������ �� ��������� ������ ������;
				x2( n );			// - ������� ������ ������� ������ ��� ������������� ��� ������;
		
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu ), rnn_copy( rnn );	// ������������� ������������ ��������� ����.
		numOfTrainingExamples = rnn.NumOfTrainingExamples();

		trainingDatum = rnn.trainingDatum;	// ��������� �� ����� ������ �������� � ���� ������������� ������.
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] ); // ����� ������ ������ ������� ������ �� �������������� ������ ������;

		testDatum = rnn.testDatum;	// ��������� �� ����� ������ �������� � ���� �������� ������.
		for( j = 0; j < n; j++ ) x0_test[j] = testDatum[j]; // ����� ������ ������ ������� �������� �� ��������� ������ ������.

		for( p = 0; p < numOfTrainingCycles; p++ )
		{
			//sumOfErrorsSquares = 0.0;

			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );

				//sumOfErrorsSquares += rnn.ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				rnn.ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������. ���� ������ x � �������� e ���������� ���� ������ ��� �������� ���.

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				//x[n - 1] = ConvertData( trainingDatum[k + n - 1] );
				x[n - 1] = e;
			}

			// ��������� ��������� ���� numOfSimulatedDatum ��������, ������������ ���, �.�. ��������� ��������������� ������ res:
			rnn_copy.CopyState( rnn ); // �������� ��������� ��������� ��� ��� ��������� ���������� ����������� ������� ����������� ������������ ����� ���� ��� ����������� ��������� ������.
			x2 = x0_test;	// ��� ����� ������ ������ ������� �������� �� ��������� ������ ������, � �������� ���������� ������������� ������ ���.
			for( i = 0; i < numOfSimulatedDatum; i++ )
			{
				//x_simulated = rnn.y( x2 );	// ���� ��������� �������� ������ ���� ��������������, � ��������� ���������� �������� ���������������!
				x_simulated = rnn_copy.y( x2 );	// ������� ������������� ��������� ����� ��������� ��� ��� ������������� ������ �������� �� ��������� ��������������� ������� ������� ����������� � ������������ ���.
				res[p * numOfSimulatedDatum + i] = x_simulated;	// ���������� ����������� ������� ����������� � ������������ ��� ��� ������������� ������ �������� ���� ����������� � ������� Training_2(...).
				for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
				x2[n - 1] = x_simulated;
			}
		}
		// �� ������ �� ����� �������� ���������� ����� �� p, k � i ����������� ������������������� numOfTrainingCycles * numOfSimulatedDatum ��������� ������� res � ��� ������� ������������������� ������,
		// ���������� ������������������������, numOfSimulatedDatum ��������� ������� res, ��������������� ������������ ���-�� ��������������� ��� ������ - numOfSimulatedDatum.

		// ��������� ��������� ���� numOfSimulatedDatum ��������, ������������ ���, �.�. ��������� ��������� ��������������� ������ res:
		x2 = x0_test;		// ��� ����� ������ ������ ������� �������� �� ��������� ������ ������ � �������� ���������� ������������� ������ ���.
		for( i = 0; i < numOfSimulatedDatum; i++ ) 
		{
			x_simulated = rnn.y( x2 );
			res[numOfTrainingCycles * numOfSimulatedDatum + i] = x_simulated;
			for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
			x2[n - 1] = x_simulated;
		}

	}// End Training( ... )

	__declspec(dllexport) void __stdcall Training_2( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
	{
		// ������������ �����������: �� ������� ���������, ���������� ������� Training_2(...), �� ������ res ������ ���� �������� ����� �� ���� ��� ���������� �������� Training_2(...) ->
		// ( numOfTrainingCycles * numOfTrainingExamples + 1 ) * numOfSimulatedDatum ����� double, ��� ���������� numOfTrainingExamples ���������������� ����� ������������� ���.
		// filenameOfTrainingData	- ��� �����, ����������� ������������� ������;
		// filenameOfTestData		- ��� ����� c ��������� �������, ������� ������ ���� ����������� � �������������� �������;	
		// numOfTrainingCycles		- ����� ������ ��������;
		// numOfSimulatedDatum		- ����� ��������� ����, ������������� ��������� ����� (���);
		// res						- �������������� ������ ����� double ���������� ���������������� ����������� numOfSimulatedDatum ��������, ������������ ���, ���������� ����� ���������� ������� ���� ��������� 
		// ��������� ��������������� ������;

		int n = N, // ����� �������� �������� ����.
			k, i, j, p, numOfTrainingExamples;
		double e, x_simulated, sumOfErrorsSquares, *trainingDatum, *testDatum;
		Vector	x0_training( n ),	// - ����� ������ ������ ������� ������ �� �������������� ������ ������;
				x( n ),				// - ������� ������ ������� ������ �� �������������� ������ ������;
				x0_test( n ),		// - ����� ������ ������ ������� ������ �� ��������� ������ ������;
				x2( n );			// - ������� ������ ������� ������ ��� ������������� ��� ������;
		
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu ), rnn_copy( rnn );	// ������������� ������������ ��������� ����.
		numOfTrainingExamples = rnn.NumOfTrainingExamples();

		trainingDatum = rnn.trainingDatum;	// ��������� �� ����� ������ �������� � ���� ������������� ������.
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] ); // ����� ������ ������ ������� ������ �� �������������� ������ ������;

		testDatum = rnn.testDatum;	// ��������� �� ����� ������ �������� � ���� �������� ������.
		for( j = 0; j < n; j++ ) x0_test[j] = testDatum[j]; // ����� ������ ������ ������� �������� �� ��������� ������ ������.

		for( p = 0; p < numOfTrainingCycles; p++ )
		{
			//sumOfErrorsSquares = 0.0;

			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );
			
				//sumOfErrorsSquares += rnn.ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				rnn.ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������. ���� ������ x � �������� e ���������� ���� ������ ��� �������� ���.

				// ��������� ��������� ���� numOfSimulatedDatum ��������, ������������ ���, �.�. ��������� ��������������� ������ res:
				rnn_copy.CopyState( rnn );	// �������� ��������� ��������� ��� ��� ��������� ���������� ����������� ������� ����������� ������������ ����� ���� ��� ����������� ��������� ������.
				x2 = x0_test;					// ��� ����� ������ ������ ������� �������� �� ��������� ������ ������, � �������� ���������� ������������� ������ ���.
				for( i = 0; i < numOfSimulatedDatum; i++ ) 
				{
					//x_simulated = rnn.y( x2 );	// ���� ��������� �������� ������ ���� ��������������, � ��������� ���������� �������� ���������������!
					x_simulated = rnn_copy.y( x2 );	// ������� ������������� ��������� ����� ��������� ��� ��� ������������� ������ �������� �� ��������� ��������������� ������� ������� ����������� � ������������ ���.
					res[(p * numOfTrainingExamples + k) * numOfSimulatedDatum + i] = x_simulated;	// ���������� ����������� ������� ����������� � ������������ ��� ��� ������������� ������ �������� ���� ����������� � ������� Training_2(...).
					for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
					x2[n - 1] = x_simulated;
				}

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				//x[n - 1] = ConvertData( trainingDatum[k + n - 1] );
				x[n - 1] = e;
			}
		}
		// �� ������ �� ����� �������� ���������� ����� �� p, k, i ����������� ������������������� numOfTrainingCycles * numOfTrainingExamples * numOfSimulatedDatum ��������� ������� res � ��� ������� 
		// ������������������� ������ ���������� ������������������������ numOfSimulatedDatum ��������� ������� res, ��������������� ������������ ���-�� ��������������� ��� ������ - numOfSimulatedDatum.

		// ��������� ��������� ���� numOfSimulatedDatum ��������, ������������ ���, �.�. ��������� ��������� ��������������� ������ res:
		p = numOfTrainingCycles - 1;	// ����������� �������� ���������� p � ���� ����� ��������� �������� � ���������.
		k = numOfTrainingExamples;		// ����������� �������� ���������� k � ���� ����� ��������� �������� � ���������.
		x2 = x0_test;					// ��� ����� ������ ������ ������� �������� �� ��������� ������ ������ � �������� ���������� ������������� ������ ���.
		for( i = 0; i < numOfSimulatedDatum; i++ ) 
		{
			x_simulated = rnn.y( x2 );
			res[(p * numOfTrainingExamples + k) * numOfSimulatedDatum + i] = x_simulated;
			for( j = 0; j < n - 1; j++ ) x2[j] = x2[j + 1];
			x2[n - 1] = x_simulated;
		}

	}// End Training_2( ... )

	__declspec(dllexport) void __stdcall Apply( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, double maxSumOfErrorsSquares, double& sumOfErrorsSquares, 
		int maxNumOfTrainingCycles, int& numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
		// ������������ �����������: �� ������� ���������, ���������� ������� Apply(...), �� ������ res ������ ���� �������� ����� �� ���� ��� ���������� �������� Apply(...) numOfSimulatedDatum ����� double,
		// ������������ ���.
	{
		int n = N, j, k;		// N - ����� ������ ��������� ����.
		Vector x( n );			// ������ ������� ������ ��� ��������� ����.
		double y;
		string tempStr, str;
		ifstream testStream;
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu );						// ������������� ������������ ��������� ����.
		rnn.Train( maxSumOfErrorsSquares, sumOfErrorsSquares, maxNumOfTrainingCycles, numOfTrainingCycles );	// �������� ��������� ����.

		// ������������� ������� x ������� n ���������� �� ��������� ������ ������:
		testStream.open( filenameOfTestData );
		for( j = 0; j < n; j++ )
		{
			getline( testStream, tempStr );
			istringstream str( tempStr );
			str >> x[j];
		}
		testStream.close();

		// ��������� ��������������� ������ res ������������� ��� �������:
		for( k = 0; k < numOfSimulatedDatum; k++ )
		{
			y = rnn.y( x );
			res[k] = y;
			for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
			x[n - 1] = y;
		}
	}

	__declspec(dllexport) void __stdcall Apply_2( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int numOfTrainingCycles, int numOfSimulatedDatum, PtrDouble res )
		// ������������ �����������: �� ������� ���������, ���������� ������� Apply(...), �� ������ res ������ ���� �������� ����� �� ���� ��� ���������� �������� Apply_2(...) numOfSimulatedDatum ����� double,
		// ������������ ���.
	{
		int n = N, j, k;		// N - ����� ������ ��������� ����.
		Vector x( n );			// ������ ������� ������ ��� ��������� ����.
		double y;
		string tempStr, str;
		ifstream testStream;
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu );						// ������������� ������������ ��������� ����.
		rnn.Train( numOfTrainingCycles );																		// �������� ��������� ����.

		// ������������� ������� x ������� n ���������� �� ��������� ������ ������:
		testStream.open( filenameOfTestData );
		for( j = 0; j < n; j++ )
		{
			getline( testStream, tempStr );
			istringstream str( tempStr );
			str >> x[j];
		}
		testStream.close();

		// ��������� ��������������� ������ res ������������� ��� �������:
		for( k = 0; k < numOfSimulatedDatum; k++ )
		{
			y = rnn.y( x );
			res[k] = y;
			for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
			x[n - 1] = y;
		}
	}

	__declspec(dllexport) void __stdcall Apply_3( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, double maxSumOfErrorsSquares, int numOfSimulatedDatum, PtrDouble res )
		// ������������ �����������: �� ������� ���������, ���������� ������� Apply(...), �� ������ res ������ ���� �������� ����� �� ���� ��� ���������� �������� Apply_2(...) numOfSimulatedDatum ����� double,
		// ������������ ���.
	{
		int n = N, j, k;		// N - ����� ������ ��������� ����.
		Vector x( n );			// ������ ������� ������ ��� ��������� ����.
		double y;
		string tempStr, str;
		ifstream testStream;
		RecurrentNeuralNetwork rnn( filenameOfTrainingData, filenameOfTestData, a, mu );						// ������������� ������������ ��������� ����.
		rnn.Train( maxSumOfErrorsSquares );																		// �������� ��������� ����.

		// ������������� ������� x ������� n ���������� �� ��������� ������ ������:
		testStream.open( filenameOfTestData );
		for( j = 0; j < n; j++ )
		{
			getline( testStream, tempStr );
			istringstream str( tempStr );
			str >> x[j];
		}
		testStream.close();

		// ��������� ��������������� ������ res ������������� ��� �������:
		for( k = 0; k < numOfSimulatedDatum; k++ )
		{
			y = rnn.y( x );
			res[k] = y;
			for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
			x[n - 1] = y;
		}
	}

}// End extern "C"