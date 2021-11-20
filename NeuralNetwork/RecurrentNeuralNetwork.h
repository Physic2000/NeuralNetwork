// RecurrentNeuralNetwork.h
#pragma once

#include "Matrixes3.h"
using namespace Matrixes3;

namespace NeuralNetwork 
{
	// ***** ��������� ����������� ��������� ���� �� ���������: *****

	const int	N = 6,					// ����������� �������� �������, �.�. ����� �������� �������� ���� ��� ����� ������ �� ���������.
				M = 10;					// ����� �������� �������� ���� �� ���������.

	double tanh( double );

	double ConvertData( double d );		// ������� �������������� ������.
	double RecoveryData( double d );	// ������� �������������� ������.

	class RecurrentNeuralNetwork
	{
	private:
		Matrix	*w,								// ������� ������� ������������� ����������� ������� �������� � �������������� ����.
				*delta_w_prev,					// ������� ��������� ������� ������������� ����������� n �������� �������� � m �������� �������� ���� �� ���������� ���� ��������.
				*wc,							// ������� ������� ������������� ����������� ����������� ������� � ��������� �������������� ����.
				*delta_wc_prev;					// ������� ��������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ���� �� ���������� ���� ��������.
		Vector	*T,								// ������-������� ������� ��������� �������� �������� ����.
				*delta_T_prev,					// ������ ��������� ������� ��������� �������� �������� ���� �� ���������� ���� ��������.
				*p,								// ������ ������� �������� �������� �������� �������� ����.
				*p_prev,						// ������ �������� �������� �������� �������� ����, �������� ��� �������� �� ���������� ����.
				*v,								// ������ ������������� ������ �� �������� �������� ���� � ��������� �������.
				*delta_v_prev;					// ������ ��������� ������������� ������ �� �������� �������� ���� � ��������� ������� �� ���������� ���� ��������.
		int		n,								// ����� ������ ��������� ����.
				m,								// ����� �������� �������� ����.
				numOfTrainingDatum,				// ����� ������������� ������.
				numOfTrainingExamples,			// ����� ������������� ��������.
				numOfTestDatum,					// ����� �������� ������.
				numOfTestExamples,				// ����� �������� ��������.
				numOfCompletedTrainingCycles;	// ���������� ����������� ������������� ������ ��������.
		double	a,								// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
				mu,								// ������ (�������) �������� ��������� ���� ( 0 <= mu <= 1.0 ). ��� �������� ��������� �������� �� ��������� ��������� ���������.
				sumOfErrorsSquares,				// ����� ��������� ������ �� ������������� ������.
				TT,								// ����� ��������� ��������� �������.
				delta_TT_prev,					// ��������� ������ ��������� ��������� ������� �� ���������� ���� ��������.
				minValue,						// ����������� �������� �� �������������� ���� (������ ������).
				maxValue;						// ������������ �������� �� �������������� ���� (������ ������).
	public:
		double	*trainingDatum,					// ������������� ������.
				*testDatum;						// �������� ������.
	private:
		char	*filenameOfTrainingData,		// ��������� ���� �������� ������������� ������.
				*filenameOfTestData;			// ��������� ���� �������� �������� ������.

	public:
		RecurrentNeuralNetwork(); // ����������� �� ���������.
		RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int n, int m );	// ������������� ��������� ����,
		// ��� a - ��� �������� ��������, mu - ������ ������� ��������, n - ����� �������� �������� ����, m - ����� �������� �������� ����.
		RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu );
		RecurrentNeuralNetwork( const RecurrentNeuralNetwork& srcRnn );	// ����������� �����������, ��������� ���������� ��������� �������� ����������� ��������� ����, ���������� ����������.
		~RecurrentNeuralNetwork();										// ���������� ������.
		void CopyState( const RecurrentNeuralNetwork& srcRnn );			// ������� �����������, ��������� ���������� ������ ��������� �������� ����������� ��������� ����, �� �� � ������ (��������� � ��������), ������� ������ ���� ����������� ��� ����� �����.
		int NumOfTrainingDatum() { return numOfTrainingDatum; };		// ���������� ���������� ������ � ������������� ������.
		int NumOfTrainingExamples() { return numOfTrainingExamples; };	// ���������� ���������� �������� ������������ � ������������� ������.
		int NumOfTestDatum() { return numOfTestDatum; };				// ���������� ���������� ������ � �������� ������.
		int NumOfTestExamples() { return numOfTestExamples; };			// ���������� ���������� �������� ������������ � �������� ������.
		int NumOfCompletedTrainingCycles() { return numOfCompletedTrainingCycles; };	// ���������� ���������� ����������� ������������� ������ ��������.
		double SumOfErrorsSquares() { return sumOfErrorsSquares; };		// ���������� ����� ��������� ������ �� ������������� ������.
		double ErrorSquare( Vector x, double e ); // ������� ����������� 1 ��� ��������� ��������� ��������������� ������ � ������������ ������ 
		// ��� ���� - �������� ������� ������� �������� � ���������� �������� ��������� ���� ���������������� �������� �������.
		void Train( double maxSumOfErrorsSquares, double& sumOfErrorsSquares, int maxNumOfTrainingCycles, int& numOfTrainingCycles ); // ��� ������� ��������� �������� ��������� ����.
		void Train( int numOfTrainingCycles ); // ��� ������� ���������� �������� ��������� ����, �������� ����� numOfTrainingCycles ������ ��������.
		void Train( double maxSumOfErrorsSquares ); // ��� ������� ��������� �������� ��������� ���� �� ��� ��� ���� sumOfErrorsSquares �� ������������� ������ ������, ��� �������� ������� maxSumOfErrorsSquares.
		double y( Vector x );		// �������� �������� ��������� ����. ��� ������� ����������� ��� ��� ��������� ��������� ����.
	};

}