// ���������� ������������ ��������� ���� ������, ������� 1 ������ � �������� �������� ��������� � �������� ����.
// ����������� ��� ������� ����� ��������������� ��������� �����.

#define LINEAR_1	0
#define LINEAR_2	1
#define SIGMOID		2
#define TANH		3

// ����� ���� ������� ��������� �������� �������� ����:
//#define ACTIVATION_FUNCTION SIGMOID		// ���������� ������� � �������� ������� ��������� �������� �������� ����.
#define ACTIVATION_FUNCTION TANH			// ��������������� ������� � �������� ������� ��������� �������� �������� ����.

// ����� ���� ������� �������������� ������ ��� ������������ ���������� ���������� ������ ��������:
//#define CONVERSION_FUNCTION LINEAR_1		// �������� ������� � �������� ������� �������������� ������ �� ������� [0, 1].
#define CONVERSION_FUNCTION LINEAR_2		// �������� ������� � �������� ������� �������������� ������ �� ������� [-1, 1].
//#define CONVERSION_FUNCTION SIGMOID		// ���������� ������� � �������� ������� �������������� ������.
//#define CONVERSION_FUNCTION TANH			// ��������������� ������� � �������� ������� �������������� ������.

/*
���������� ������� � �������� ������� ��������� �������� �������� ���� ����������� � �������� �������� �������������� ������ 
�� ������� [0, 1].
��������������� ������� � �������� ������� ��������� �������� �������� ���� ����������� � �������� �������� �������������� ������ 
�� ������� [-1, 1].
������� ��������� SIGMOID ����������� � ���������� LINEAR_1, � ��������� TANH ����������� � ���������� LINEAR_2.
*/

#if( !defined( ACTIVATION_FUNCTION ) ) 
	#error ������ ���� ��������� ������ ACTIVATION_FUNCTION !
#endif
#if( !defined( CONVERSION_FUNCTION ) ) 
	#error ������ ���� ��������� ������ CONVERSION_FUNCTION !
#endif

#include <math.h>							// ��� ������� pow()
//#include <cmath>							// ��� ������� tanh()
#include <string>
#include <sstream>
#include <queue>							// ��� ������������� ��������
#include "Matrixes3.h"
#include "RecurrentNeuralNetwork.h"
using namespace std;
using namespace Matrixes3;

static double minValue, maxValue;

// ***** ��������� ������������ ��������� ����: *****

double	l = 1.0;				// ����������� ��������������� ������ ������� ��������� �������� �������� ����.
		//a = 0.01,				// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
		//a = 0.025,			// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
		//a = 0.029,			// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
		//a = 0.030,			// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
		//mu	= 0;			// ������ (�������) �������� ��������� ���� ( 0 <= mu <= 1.0 ). ��� �������� ��������� �������� �� ��������� ��������� ���������.
		//mu	= 0.125;		// ������ (�������) �������� ��������� ���� ( 0 <= mu <= 1.0 ). ��� �������� ��������� �������� �� ��������� ��������� ���������.
		//mu	= 0.275;		// ������ (�������) �������� ��������� ���� ( 0 <= mu <= 1.0 ). ��� �������� ��������� �������� �� ��������� ��������� ���������.

double F( double S );				// ������� ��������� �������� �������� ����.
double Derivative_F( double S );	// ����������� ������� ��������� �������� �������� ����.

double ConvertData( double d );		// ������� �������������� ������ ��� ������������ ���������� ���������� ������ ��������.
double RecoveryData( double d );	// ������� �������������� ������ �������� �������� �� ��������� � ������� �������������� ������.

namespace NeuralNetwork
{

	RecurrentNeuralNetwork::RecurrentNeuralNetwork()
	{	// ����������� �� ���������.
		filenameOfTrainingData = nullptr;
		filenameOfTestData = nullptr;
		w = nullptr;
		delta_w_prev = nullptr;
		wc = nullptr;
		delta_wc_prev = nullptr;
		T = nullptr;
		delta_T_prev = nullptr;
		p = nullptr;
		p_prev = nullptr;
		v = nullptr;
		delta_v_prev = nullptr;
		trainingDatum = nullptr;
		testDatum = nullptr;
	}

	RecurrentNeuralNetwork::RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu, int n, int m )
	{	// ��������� ������������� ��������� ����:
		int i, j;
		double datum;
		queue<double> queueOfTrainingData, queueOfTestData;
		ifstream trainingStream, testStream;
		string tempStr, str;

		this->filenameOfTrainingData = filenameOfTrainingData;	// ���� �������� ������������� ������ ��� �������� � ��������� �������.
		this->filenameOfTestData = filenameOfTestData;			// ���� �������� �������� ������ ��� �������� � ��������� �������.
		this->n = n;						// ����� �������� �������� ����.
		this->m = m;						// ����� �������� �������� ����.
		this->a = a;						// �������� �������� ��������� ���� ( 0.001 <= a <= 0.5 ).
		this->mu = mu;						// ������ (�������) �������� ��������� ���� ( 0 <= mu <= 1.0 ). ��� �������� ��������� �������� �� ��������� ��������� ���������.
		this->numOfCompletedTrainingCycles = 0;					// ���������� ����������� ������������� ������ ��������.
		this->sumOfErrorsSquares = -1;							// ����� ��������� ������ �� ������������� ������ ������.
		w = new Matrix( n, m );				// ������� ������� ������������� ����������� ������� �������� � �������� ����.
		delta_w_prev = new Matrix( n, m );	// ������� ��������� ������� ������������� ����������� n �������� �������� � m �������� �������� ���� �� ���������� ���� �������� ���������������� ������� ��������.
		wc = new Matrix( m, m );			// ������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ����. ����� ����������� �������� ��������� � ������ �������� �������������� (��������) ����.
		delta_wc_prev = new Matrix( m, m );	// ������� ��������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ���� �� ���������� ���� �������� ���������������� ������� ��������.
		T = new Vector( m );				// ������-������� ������� ��������� �������� �������� ���� �������������� ����-��������.
		delta_T_prev = new Vector( m );		// ������ ��������� ������� ��������� �������� �������� ���� �� ���������� ���� �������� �������������� ����-��������.
		p = new Vector( m );				// ������ ������� �������� �������� �������� �������� ���� �������������� ������� ��������.
		p_prev = new Vector( m );			// ������ �������� �������� �������� �������� ����, �������� ��� �������� �� ���������� ����, �������������� ����-��������.
		v = new Vector( m );				// ������ ������������� ������ �� �������� �������� ���� � ��������� �������.
		delta_v_prev = new Vector( m );		// ������ ��������� ������������� ������ �� �������� �������� ���� � ��������� ������� �� ���������� ���� �������� �������������� ����-��������.
		TT = 0.0;							// ����� ��������� ��������� ������� �������������� ������� ���������.
		delta_TT_prev = 0.0;				// ��������� ������ ��������� ��������� ������� �� ���������� ���� �������� �������������� ������� ���������.
		minValue = numeric_limits<double>::max();
		maxValue = numeric_limits<double>::lowest();

		// ������������� ������� ������� ������������� ����� ��������� �������� ���� � ��������� �������� ���� ���������� ���������� � 
		// ��������� (0, 1):
		for(i = 0; i < n; i++)
			for(j = 0; j < m; j++)
				(*w)(i, j) = (double)rand() / RAND_MAX;

		// ������������� ������� ������� ������������� ����� ������������ ��������� � ��������� �������� ���� ���������� ���������� � 
		// ��������� (0, 1), � ����� ������� ������� ������������� �� �������� �������� ���� � ��������� �������:
		for(i = 0; i < m; i++)
		{
			(*v)[i] = (double)rand() / RAND_MAX;
			for(j = 0; j < m; j++)
				(*wc)(i, j) = (double)rand() / RAND_MAX;
		}

		trainingStream.open( filenameOfTrainingData );
		numOfTrainingDatum = 0;
		if( !trainingStream.fail() )
		{
	        getline( trainingStream, tempStr );		// ���������� 1-�� ������ �� ����� ������.
			do{										// ���� �������� ���-�� ��������.
				istringstream str( tempStr );
				str >> datum;
				queueOfTrainingData.push( datum );
				//trainingStream >> datum; // ��������� �������� � �������� ����� ������ � ��������� ������ ������.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTrainingDatum++;
				getline( trainingStream, tempStr );	// ���������� ��������� ������ �� ����� ������.
			} while( tempStr.size() || !trainingStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		trainingStream.close();
		numOfTrainingExamples = numOfTrainingDatum - n;
		trainingDatum = new double[numOfTrainingDatum];
		for( i = 0; i < numOfTrainingDatum; i++ )
		{
			trainingDatum[i] = queueOfTrainingData.front();
			queueOfTrainingData.pop();
		}
		/*
		i = 0;
		while( !queueOfTrainingData.empty() )
		{
			trainingDatum[i] = queueOfTrainingData.front();
			queueOfTrainingData.pop();
			i++;
		}
		*/

		testStream.open( filenameOfTestData );
		numOfTestDatum = 0;
		if( !testStream.fail() )
		{
	        getline( testStream, tempStr );			// ���������� 1-�� ������ �� ����� ������.
			do{										// ���� �������� ���-�� ��������.
				istringstream str( tempStr );
				str >> datum;
				queueOfTestData.push( datum );
				//testStream >> datum; // ��������� �������� � �������� ����� ������ � ��������� ������ ������.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTestDatum++;
				getline( testStream, tempStr );	// ���������� ��������� ������ �� ����� ������.
			} while( tempStr.size() || !testStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		testStream.close();
		numOfTestExamples = numOfTestDatum - n;
		testDatum = new double[numOfTestDatum];
		for( i = 0; i < numOfTestDatum; i++ )
		{
			testDatum[i] = queueOfTestData.front();
			queueOfTestData.pop();
		}

		// ��������� 7 ����� ���� ��� �������:
		/*
		cout << "w  =  " << endl; w->Display( cout, 5 ); cout << endl;
		cout << "T  =  "; T->Display( cout, 5 ); cout << endl << endl;
		cout << "wc  =  " << endl; wc->Display( cout, 5 ); cout << endl;
		cout << "p  =  "; p->Display( cout, 5 ); cout << endl << endl;
		cout << "p_prev  =  "; p_prev->Display( cout, 5 ); cout << endl << endl;
		cout << "v  =  "; v->Display( cout, 5 ); cout << endl << endl;
		cout << "TT  =  "; cout << TT << endl;
		*/
	}

	RecurrentNeuralNetwork::RecurrentNeuralNetwork( char* filenameOfTrainingData, char* filenameOfTestData, double a, double mu ):
		filenameOfTrainingData( filenameOfTrainingData ), filenameOfTestData( filenameOfTestData ), a( a ), mu( mu ), n( N ), m( M ), numOfCompletedTrainingCycles( 0 ), sumOfErrorsSquares( -1 )
	{	// ��������� ������������� ��������� ���� (�������� ��� n � m ������� �� ��������� - n = 6, m = 10):
		int i, j;
		double datum;
		queue<double> queueOfTrainingData, queueOfTestData;
		ifstream trainingStream, testStream;
		string tempStr, str;

		/*
		cout << endl;
		cout << "this->filenameOfTrainingData  =  " << this->filenameOfTrainingData << endl
			<< "this->filenameOfTestData  =  " << this->filenameOfTestData << endl
			<< "this->a  =  " << this->a << endl
			<< "this->mu  =  " << this->mu << endl
			<< "this->maxNumOfTrainingCycles  =  " << this->maxNumOfTrainingCycles << endl
			<< "this->maxSumOfErrorsSquares  =  " << this->maxSumOfErrorsSquares << endl
			<< "this->n  =  " << this->n << endl
			<< "this->m  =  " << this->m << endl;
		*/

		w = new Matrix( n, m );				// ������� ������� ������������� ����������� ������� �������� � �������� ����.
		delta_w_prev = new Matrix( n, m );	// ������� ��������� ������� ������������� ����������� n �������� �������� � m �������� �������� ���� �� ���������� ���� �������� ���������������� ������� ��������.
		wc = new Matrix( m, m );			// ������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ����. ����� ����������� �������� ��������� � ������ �������� �������������� (��������) ����.
		delta_wc_prev = new Matrix( m, m );	// ������� ��������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ���� �� ���������� ���� �������� ���������������� ������� ��������.
		T = new Vector( m );				// ������-������� ������� ��������� �������� �������� ���� �������������� ����-��������.
		delta_T_prev = new Vector( m );		// ������ ��������� ������� ��������� �������� �������� ���� �� ���������� ���� �������� �������������� ����-��������.
		p = new Vector( m );				// ������ ������� �������� �������� �������� �������� ���� �������������� ������� ��������.
		p_prev = new Vector( m );			// ������ �������� �������� �������� �������� ����, �������� ��� �������� �� ���������� ����, �������������� ����-��������.
		v = new Vector( m );				// ������ ������������� ������ �� �������� �������� ���� � ��������� �������.
		delta_v_prev = new Vector( m );		// ������ ��������� ������������� ������ �� �������� �������� ���� � ��������� ������� �� ���������� ���� �������� �������������� ����-��������.
		TT = 0.0;							// ����� ��������� ��������� ������� �������������� ������� ���������.
		delta_TT_prev = 0.0;				// ��������� ������ ��������� ��������� ������� �� ���������� ���� �������� �������������� ������� ���������.
		minValue = numeric_limits<double>::max();
		maxValue = numeric_limits<double>::lowest();

		// ������������� ������� ������� ������������� ����� ��������� �������� ���� � ��������� �������� ���� ���������� ���������� � 
		// ��������� (0, 1):
		for(i = 0; i < n; i++)
			for(j = 0; j < m; j++)
				(*w)(i, j) = (double)rand() / RAND_MAX;

		// ������������� ������� ������� ������������� ����� ������������ ��������� � ��������� �������� ���� ���������� ���������� � 
		// ��������� (0, 1), � ����� ������� ������� ������������� �� �������� �������� ���� � ��������� �������:
		for(i = 0; i < m; i++)
		{
			(*v)[i] = (double)rand() / RAND_MAX;
			for(j = 0; j < m; j++)
				(*wc)(i, j) = (double)rand() / RAND_MAX;
		}

		trainingStream.open( filenameOfTrainingData );
		numOfTrainingDatum = 0;
		if( !trainingStream.fail() )
		{
	        getline( trainingStream, tempStr );		// ���������� 1-�� ������ �� ����� ������.
			do{										// ���� �������� ���-�� ��������.
				istringstream str( tempStr );
				str >> datum;
				queueOfTrainingData.push( datum );
				//trainingStream >> datum; // ��������� �������� � �������� ����� ������ � ��������� ������ ������.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTrainingDatum++;
				getline( trainingStream, tempStr );	// ���������� ��������� ������ �� ����� ������.
			} while( tempStr.size() || !trainingStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		trainingStream.close();
		numOfTrainingExamples = numOfTrainingDatum - n;
		trainingDatum = new double[numOfTrainingDatum];
		for( i = 0; i < numOfTrainingDatum; i++ )
		{
			trainingDatum[i] = queueOfTrainingData.front();
			queueOfTrainingData.pop();
		}

		testStream.open( filenameOfTestData );
		numOfTestDatum = 0;
		if( !testStream.fail() )
		{
	        getline( testStream, tempStr );			// ���������� 1-�� ������ �� ����� ������.
			do{										// ���� �������� ���-�� ��������.
				istringstream str( tempStr );
				str >> datum;
				queueOfTestData.push( datum );
				//testStream >> datum; // ��������� �������� � �������� ����� ������ � ��������� ������ ������.

				if( datum < minValue ) minValue = datum;
				if( datum > maxValue ) maxValue = datum;

				numOfTestDatum++;
				getline( testStream, tempStr );	// ���������� ��������� ������ �� ����� ������.
			} while( tempStr.size() || !testStream.eof() );
			::minValue = minValue;
			::maxValue = maxValue;
		}
		testStream.close();
		numOfTestExamples = numOfTestDatum - n;
		testDatum = new double[numOfTestDatum];
		for( i = 0; i < numOfTestDatum; i++ )
		{
			testDatum[i] = queueOfTestData.front();
			queueOfTestData.pop();
		}

		// ��������� 7 ����� ���� ��� �������:
		/*
		cout << "w  =  " << endl; w->Display( cout, 5 ); cout << endl;
		cout << "T  =  "; T->Display( cout, 5 ); cout << endl << endl;
		cout << "wc  =  " << endl; wc->Display( cout, 5 ); cout << endl;
		cout << "p  =  "; p->Display( cout, 5 ); cout << endl << endl;
		cout << "p_prev  =  "; p_prev->Display( cout, 5 ); cout << endl << endl;
		cout << "v  =  "; v->Display( cout, 5 ); cout << endl << endl;
		cout << "TT  =  "; cout << TT << endl;
		*/
	}

	// ����������� �����������, ��������� ���������� ��� ��������� ���� ������ � � ������� (���������� � ���������) � ������� ����������.
	RecurrentNeuralNetwork::RecurrentNeuralNetwork( const RecurrentNeuralNetwork& srcRnn )
	{	
		w = new Matrix( *srcRnn.w );
		delta_w_prev = new Matrix( *srcRnn.delta_w_prev );
		wc = new Matrix( *srcRnn.wc );
		delta_wc_prev = new Matrix( *srcRnn.delta_wc_prev );
		T = new Vector( *srcRnn.T );
		delta_T_prev = new Vector( *srcRnn.delta_T_prev );
		p = new Vector( *srcRnn.p );
		p_prev = new Vector( *srcRnn.p_prev );
		v = new Vector( *srcRnn.v );
		delta_v_prev = new Vector( *srcRnn.delta_v_prev );
		n = srcRnn.n;
		m = srcRnn.m;
		numOfTrainingDatum = srcRnn.numOfTrainingDatum;
		numOfTrainingExamples = srcRnn.numOfTrainingExamples;
		numOfTestDatum = srcRnn.numOfTestDatum;
		numOfTestExamples = srcRnn.numOfTestExamples;
		numOfCompletedTrainingCycles = srcRnn.numOfCompletedTrainingCycles;
		a = srcRnn.a;
		mu = srcRnn.mu;
		sumOfErrorsSquares = srcRnn.sumOfErrorsSquares;
		trainingDatum = new double[numOfTrainingDatum];
		memcpy( this->trainingDatum, srcRnn.trainingDatum, numOfTrainingDatum * sizeof( double ) );
		testDatum = new double[numOfTestDatum];
		memcpy( this->testDatum, srcRnn.testDatum, numOfTestDatum * sizeof( double ) );
		//TT = (*srcRnn).TT;	// error
		//TT = *srcRnn.TT;		// error
		TT = srcRnn.TT;
		delta_TT_prev = srcRnn.delta_TT_prev;
		minValue = srcRnn.minValue;
		maxValue = srcRnn.maxValue;
		filenameOfTrainingData = srcRnn.filenameOfTrainingData;
		filenameOfTestData = srcRnn.filenameOfTestData;
	}

	RecurrentNeuralNetwork::~RecurrentNeuralNetwork()	// ���������� ������.
	{
		delete w;
		delete delta_w_prev;
		delete wc;
		delete delta_wc_prev;
		delete T;
		delete delta_T_prev;
		delete p;
		delete p_prev;
		delete v;
		delete delta_v_prev;
		delete[] trainingDatum;
		delete[] testDatum;
	}

	// ������� �����������, ��������� ���������� ������ ��������� �������� ����������� ��������� ����, ���������� ���������� �������, �� �� � ������ (��������� � ��������), ������� ������ ���� ����������� ��� ����� �����.
	void RecurrentNeuralNetwork::CopyState( const RecurrentNeuralNetwork& srcRnn )
	{
		// �������� ��������� �������� ����������� ��������� ����, ���������� ���������� �������:
		w = new Matrix( *srcRnn.w );
		delta_w_prev = new Matrix( *srcRnn.delta_w_prev );
		wc = new Matrix( *srcRnn.wc );
		delta_wc_prev = new Matrix( *srcRnn.delta_wc_prev );
		T = new Vector( *srcRnn.T );
		delta_T_prev = new Vector( *srcRnn.delta_T_prev );
		p = new Vector( *srcRnn.p );
		p_prev = new Vector( *srcRnn.p_prev );
		v = new Vector( *srcRnn.v );
		delta_v_prev = new Vector( *srcRnn.delta_v_prev );
		n = srcRnn.n;
		m = srcRnn.m;
		numOfTrainingDatum = srcRnn.numOfTrainingDatum;
		numOfTrainingExamples = srcRnn.numOfTrainingExamples;
		numOfTestDatum = srcRnn.numOfTestDatum;
		numOfTestExamples = srcRnn.numOfTestExamples;
		numOfCompletedTrainingCycles = srcRnn.numOfCompletedTrainingCycles;
		a = srcRnn.a;
		mu = srcRnn.mu;
		sumOfErrorsSquares = srcRnn.sumOfErrorsSquares;
		TT = srcRnn.TT;
		delta_TT_prev = srcRnn.delta_TT_prev;
		minValue = srcRnn.minValue;
		maxValue = srcRnn.maxValue;
		// ������ (��������� � ��������) �������� ������ ��� ����� �����:
		
	}

	// ��������� �-� ��������� 1 ��� ��������� ��������� ��������������� ������ � ���������� ������� ������ ��� ���� -
	// �������� ������� ������� �������� 'x' � ���������� �������� ��������� ���� 'e' ���������������� �������� �������.
	double RecurrentNeuralNetwork::ErrorSquare( Vector x, double e )
	{
		double delta_TT, y, res;
		Vector delta_v( m );		// ������ ��������� ������������� ������ �� �������� �������� ���� � ��������� �������.
		Vector delta_T( m );		// ������ ��������� ������� ��������� �������� �������� ����.
		Vector S( m );				// ���������� ����� �������� �������� ����.
		Matrix delta_w( n, m );		// ������� ��������� ������� ������������� ����������� n �������� �������� � m �������� �������� ����.
		Matrix delta_wc( m, m );	// ������� ��������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ����.
		int i, j, l;

		for(i = 0; i < m; i++) // ���� ��������� �� ���� �������� �������� ����.
		{
			for(j = 0; j < n; j++) // ���� ��������� �� ���� �������� �������� ����.
				S[i] += (*w)(j, i) * x[j];

			for(l = 0; l < m; l++) // ���� ��������� �� ���� ����������� ��������.
				S[i] += (*wc)(l, i) * (*p_prev)[l];

			S[i] -= (*T)[i];
			(*p)[i] = F( S[i] );
		}
		y = (*v) * (*p) - TT; // ������� �������� �������� ��������� ����.
	
		// ��������� ���������� �������� ����������� ����:
		delta_v = -a * ( y - e ) * (*p) + mu * (*delta_v_prev);
		delta_TT = a * ( y - e ) + mu * delta_TT_prev;
		TT += delta_TT;
		for(i = 0; i < m; i++) // ���� ��������� �� ���� �������� �������� ����.
		{
			for(j = 0; j < n; j++) // ���� ��������� �� ���� �������� �������� ����.
			{
				delta_w(j, i) = -a * ( y - e ) * (*v)[i] * Derivative_F( S[i] ) * x[j] + mu * (*delta_w_prev)(j, i);
				(*w)(j, i) += delta_w(j, i);
			}
	
			for(l = 0; l < m; l++) // ���� ��������� �� ���� ����������� ��������.
			{
				delta_wc(l, i) = -a * ( y - e ) * (*v)[i] * Derivative_F( S[i] ) * (*p_prev)[l] + mu * (*delta_wc_prev)(l, i);
				(*wc)(l, i) += delta_wc(l, i);
			}

			delta_T[i] = a * ( y - e ) * (*v)[i] * Derivative_F( S[i] ) + mu * (*delta_T_prev)[i];
			(*T)[i] += delta_T[i];

			(*v)[i] += delta_v[i];
		}

		(*p_prev) = (*p);			// ����������� ����������� �������� �������� �������� ���� �� ������� ���� ���������� ��������� �������� �������� ����.
		(*delta_v_prev) = delta_v;	// ����������� ������� ��������� ������������� ������ �� �������� �������� ���� � ��������� ������� �� ���������� ���� �������� ������� ������ ���� ���������.
		(*delta_T_prev) = delta_T;	// ����������� ������� ��������� ������� ��������� �������� �������� ���� �� ���������� ���� �������� ������� ������ ���� ���������.
		delta_TT_prev = delta_TT;	// ����������� �������� ��������� ������ ��������� ��������� ������� �� ���������� ���� �������� ������� �������� ����� ���������.
		(*delta_w_prev) = delta_w;	// ����������� ������� ��������� ������� ������������� ����������� n �������� �������� � m �������� �������� ���� �� ���������� ���� �������� ������� ������� ���� ���������.
		(*delta_wc_prev) = delta_wc;// ����������� ������� ��������� ������� ������������� ����������� m ����������� �������� � m ��������� �������� ���� �� ���������� ���� �������� ������� ������� ���� ���������.

		res = pow( ( y - e ), 2) / 2;
		return res;
	}

	// ��������� ������� ��������� �������� ��������� ����.
	void RecurrentNeuralNetwork::Train( double maxSumOfErrorsSquares, double& sumOfErrorsSquares, int maxNumOfTrainingCycles, int& numOfTrainingCycles )
	{
		int k, j;
		double e;
		Vector x( n ), x0_training( n );
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] );	// x0_training - ��� ����� ������ ������ ������� �������� �� ���������� ������ ������.

		numOfTrainingCycles = 0;
		do{
			sumOfErrorsSquares = 0.0;

			/*
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				if( k == 0 ) // ��������� ������ ������.
				{
					for( j = 0; j < n; j++ )
					{
						x[j] = ConvertData( trainingDatum[j] );
					}
					e = ConvertData( trainingDatum[n] );
				}
				else	// if( k != 0 )		// ��������� ��������� ������.
				{
					for( j = 0; j < n - 1; j++ )
						x[j] = x[j + 1];
					x[n - 1] = e;
					e = ConvertData( trainingDatum[k + n] );
				}

				// ��������� 3 ������ ���� ��� �������:
				//cout << k << ":  "; x.Display( cout, 2 );
				//cout << "  " << e << endl;
				//if( k == 10 ) break;

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				sumOfErrorsSquares += ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.

				// ��������� ������ ���� ��� �������:
				//cout << k << ":  " << sumOfErrorsSquares << endl;
			}
			*/
			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				sumOfErrorsSquares += ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.

				// ��������� ������ ���� ��� �������:
				//cout << k << ":  " << sumOfErrorsSquares << endl;

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				x[n - 1] = e;
			}

			// ��������� ������ ���� ��� �������:
			//cout << numOfTrainingCycles << ":  " << sumOfErrorsSquares << endl;

			this->sumOfErrorsSquares = sumOfErrorsSquares;
			numOfTrainingCycles++;
			numOfCompletedTrainingCycles++;
		}while( ( sumOfErrorsSquares > maxSumOfErrorsSquares ) && ( numOfTrainingCycles < maxNumOfTrainingCycles ) );
	}

	// ��� ������� ���������� �������� ��������� ����, �������� ����� numOfTrainingCycles ������ ��������.
	void RecurrentNeuralNetwork::Train( int numOfTrainingCycles )
	{
		int k, j, numOfCycles;
		double e;
		Vector x( n ), x0_training( n );
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] );	// x0_training - ��� ����� ������ ������ ������� �������� �� ���������� ������ ������.

		numOfCycles = 0;
		do{
			sumOfErrorsSquares = 0.0;

			/*
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				if( k == 0 ) // ��������� ������ ������.
				{
					for( j = 0; j < n; j++ )
					{
						x[j] = ConvertData( trainingDatum[j] );
					}
					e = ConvertData( trainingDatum[n] );
				}
				else		// ��������� ��������� ������.
				{
					for( j = 0; j < n - 1; j++ )
						x[j] = x[j + 1];
					x[n - 1] = e;
					e = ConvertData( trainingDatum[k + n] );
				}

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				sumOfErrorsSquares += ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.

				// ��������� ������ ���� ��� �������:
				//cout << k << ":  " << sumOfErrorsSquares << endl;
			}
			*/
			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );
				
				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				sumOfErrorsSquares += ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.

				// ��������� ������ ���� ��� �������:
				//cout << k << ":  " << sumOfErrorsSquares << endl;

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				x[n - 1] = e;
			}

			// ��������� ������ ���� ��� �������:
			//cout << numOfCycles << ":  " << sumOfErrorsSquares << endl;

			numOfCycles++;
			numOfCompletedTrainingCycles++;
		}while( numOfCycles < numOfTrainingCycles );
	}

	// ��������� ������� ��������� �������� ��������� ���� �� ��� ��� ���� sumOfErrorsSquares �� ������������� ������ ������, ��� �������� ������� maxSumOfErrorsSquares.
	void RecurrentNeuralNetwork::Train( double maxSumOfErrorsSquares )
	{
		int k, j, numOfTrainingCycles;
		double e;
		Vector x( n ), x0_training( n );
		for( j = 0; j < n; j++ ) x0_training[j] = ConvertData( trainingDatum[j] );	// x0_training - ��� ����� ������ ������ ������� �������� �� ���������� ������ ������.

		numOfTrainingCycles = 0;
		do{
			sumOfErrorsSquares = 0.0;

			/*
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				if( k == 0 ) // ��������� ������ ������.
				{
					for( j = 0; j < n; j++ )
					{
						x[j] = ConvertData( trainingDatum[j] );
					}
					e = ConvertData( trainingDatum[n] );
				}
				else		// ��������� ��������� ������.
				{
					for( j = 0; j < n - 1; j++ )
						x[j] = x[j + 1];
					x[n - 1] = e;
					e = ConvertData( trainingDatum[k + n] );
				}

				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				sumOfErrorsSquares += ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.

				// ��������� ������ ���� ��� �������:
				//cout << k << ":  " << sumOfErrorsSquares << endl;
			}
			*/
			x = x0_training;
			for( k = 0; k < numOfTrainingExamples; k++ )
			{
				e = ConvertData( trainingDatum[k + n] );
				
				//sumOfErrorsSquares += this->ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.
				sumOfErrorsSquares += ErrorSquare( x, e ); // ��������� 1 ��� ��������� ��������� ��������������� ������.

				// ��������� ������ ���� ��� �������:
				//cout << k << ":  " << sumOfErrorsSquares << endl;

				for( j = 0; j < n - 1; j++ ) x[j] = x[j + 1];
				x[n - 1] = e;
			}

			// ��������� ������ ���� ��� �������:
			//cout << numOfTrainingCycles << ":  " << sumOfErrorsSquares << endl;

			numOfTrainingCycles++;
			numOfCompletedTrainingCycles++;
		}while( sumOfErrorsSquares > maxSumOfErrorsSquares );
	}

	double RecurrentNeuralNetwork::y( Vector x )		// �������� �������� ��������� ��������� ����.
	{
		double res;
		Vector S( m );	// ���������� ����� �������� �������� ����.
		int i, j, l;

		for(i = 0; i < m; i++) // ���� ��������� �� ���� �������� �������� ����.
		{
			for(j = 0; j < n; j++) // ���� ��������� �� ���� �������� �������� ����.
				//S[i] += (*w)(j, i) * x[j];
				S[i] += (*w)(j, i) * ConvertData( x[j] );

			for(l = 0; l < m; l++) // ���� ��������� �� ���� ����������� ��������.
				S[i] += (*wc)(l, i) * (*p_prev)[l];

			S[i] -= (*T)[i];
			(*p)[i] = F( S[i] );
		}

		(*p_prev) = (*p);	// ����������� ����������� �������� �������� �������� ���� �� ������� ���� ���������� ��������� �������� 
		// �������� ����.

		//res = (*v) * (*p) - TT;
		res = RecoveryData( (*v) * (*p) - TT );
		return res;
	}

#if( CONVERSION_FUNCTION == LINEAR_1 )

	// ������� �������������� ������:
	double ConvertData( double d )
	{
		return ( d - ::minValue ) / ( ::maxValue - ::minValue );
	}

	// ������� �������������� ������:
	double RecoveryData( double d )
	{
		return ( ::maxValue - ::minValue ) * d + ::minValue;						//  0 <= d <= 1
	}

#endif

#if( CONVERSION_FUNCTION == LINEAR_2 )

	// ������� �������������� ������:
	double ConvertData( double d )
	{
		return 2.0 * ( d - ::minValue ) / ( ::maxValue - ::minValue ) - 1;
	}

	// ������� �������������� ������:
	double RecoveryData( double d )
	{
		return 0.5 * ( ::maxValue - ::minValue ) * ( d + 1) + ::minValue;			//  -1 <= d <= 1
	}

#endif

#if( CONVERSION_FUNCTION == SIGMOID )

	// ������� �������������� ������:
	double ConvertData( double d )
	{
		return 1 / ( 1 + exp( -d ) );
	}

	// ������� �������������� ������:
	double RecoveryData( double d )
	{
		return -log( 1 / d - 1 );
	}

#endif

#if( CONVERSION_FUNCTION == TANH )

	// ������� �������������� ������:
	double ConvertData( double d )
	{
		return tanh( d );
	}

	// ������� �������������� ������:
	double RecoveryData( double d )
	{
		return 0.5 * log( ( 1 + d ) / ( 1 - d ) );									//	|d| < 1
	}

#endif

}

#if( ACTIVATION_FUNCTION == SIGMOID )

// ������� ��������� �������� �������� ����:
double F( double S )
{
	return 1 / ( 1 + exp( - l * S ) );
}

// ����������� ������� ��������� �������� �������� ����:
double Derivative_F( double S )
{
	return l * F( S ) * ( 1 - F( S ) );
}

#endif

#if( ACTIVATION_FUNCTION == TANH )

// ������� ��������� �������� �������� ����:
double F( double S )
{
	return tanh( l * S );
}

// ����������� ������� ��������� �������� �������� ����:
double Derivative_F( double S )
{
	return l * ( 1 - pow( F( S ), 2 ) );
}

#endif

double tanh( double x )
{
	return ( exp( 2 * x ) - 1 ) / ( exp( 2 * x ) + 1 );
}