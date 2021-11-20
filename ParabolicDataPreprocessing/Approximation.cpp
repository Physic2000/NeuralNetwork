#include "Approximation.h"
#include "Matrixes3.h"
#include "SystemsOfLinearEquations.h"

#include <math.h>
#include <assert.h>
#include <stddef.h>

using namespace Matrixes3;
using namespace SystemsOfLinearEquations;

namespace Approximation
{
	// Description of class LinearApproximation:

	LinearApproximation::LinearApproximation()
	{
		x_array = NULL;
		y_array = NULL;
		numOfPoints = 0;
		standardDeviation = -1.0;	// Ќамерено присвоено не допустимое значение!
		//afterUpdate_a_b_terms = false;
		//afterUpdateStandardDeviation = false;
	}

	LinearApproximation::LinearApproximation( double x_array[], double y_array[], int numOfPoints )
	{
		assert( numOfPoints >= 2 );

		this->x_array = new double[numOfPoints];
		this->y_array = new double[numOfPoints];
		for( int i = 0; i < numOfPoints; i++ )
		{
			this->x_array[i] = x_array[i];
			this->y_array[i] = y_array[i];
		}
		this->numOfPoints = numOfPoints;

		afterUpdate_a_b_terms = false;
		afterUpdateStandardDeviation = false;
	}

	LinearApproximation::~LinearApproximation()
	{
		delete( this->x_array );
		delete( this->y_array );
		this->x_array = NULL;
		this->y_array = NULL;
	}

	void LinearApproximation::Init( double x_array[], double y_array[], int numOfPoints )
	{
		assert( numOfPoints >= 2 );

		if( this->numOfPoints != numOfPoints )
		{
			if( this->x_array != NULL )
				delete( this->x_array );
			if( this->y_array != NULL )
				delete( this->y_array );
			this->x_array = new double[numOfPoints];
			this->y_array = new double[numOfPoints];
			this->numOfPoints = numOfPoints;
		}

		for( int i = 0; i < numOfPoints; i++ )
		{
			this->x_array[i] = x_array[i];
			this->y_array[i] = y_array[i];
		}
		
		afterUpdate_a_b_terms = false;
		afterUpdateStandardDeviation = false;
	}

	void LinearApproximation::Get_a_b_terms2( double& a, double& b )
	{
		if( !afterUpdate_a_b_terms ) this->Update_a_b_terms();
		a = a_term; b = b_term;
	}

	double LinearApproximation::StandardDeviation2( void )
	{
		if( !afterUpdateStandardDeviation ) this->UpdateStandardDeviation();
		return this->standardDeviation;
	}

	double LinearApproximation::PredictedY2( double x )
	{
		if( !afterUpdate_a_b_terms ) this->Update_a_b_terms();
		return a_term * x + b_term;
	}

	void LinearApproximation::Update_a_b_terms( void )
	{
		assert( numOfPoints >= 2 );

		double s1, s2, s3, s4;

		s1 = s2 = s3 = s4 = 0;
		for( int i = 0; i < numOfPoints; i++ )
		{
			s1 += x_array[i];
			s2 += pow( x_array[i], 2 );
			s3 += y_array[i];
			s4 += x_array[i] * y_array[i];
		}

		a_term = ( numOfPoints * s4 - s1 * s3 ) / ( numOfPoints * s2 - s1 * s1 );
		b_term = ( s3 - s1 * a_term ) / numOfPoints;

		afterUpdate_a_b_terms = true;
	}

	void LinearApproximation::UpdateStandardDeviation( void )
	{
		if( !afterUpdate_a_b_terms ) this->Update_a_b_terms();
		double sum = 0;

		if( this->numOfPoints > 2 )
		{
			for( int i = 0; i < numOfPoints; i++ )
				sum += pow( y_array[i] - PredictedY( x_array[i] ), 2 );
			sum /= numOfPoints;
		}
		//this->standardDeviation = pow( sum, 0.5 );
		this->standardDeviation = sqrt( sum );

		afterUpdateStandardDeviation = true;
	}
	// End of description of class LinearApproximation.

	// Description of class ParabolicApproximation:

	ParabolicApproximation::ParabolicApproximation()
	{
		x_array = NULL;
		y_array = NULL;
		numOfPoints = 0;
		standardDeviation = -1.0;	// Ќамерено присвоено не допустимое значение!
		//afterUpdate_a_b_c_terms = false;
		//afterUpdateStandardDeviation = false;
	}

	ParabolicApproximation::ParabolicApproximation( double x_array[], double y_array[], int numOfPoints )
	{
		assert( numOfPoints >= 3 );

		this->x_array = new double[numOfPoints];
		this->y_array = new double[numOfPoints];
		for( int i = 0; i < numOfPoints; i++ )
		{
			this->x_array[i] = x_array[i];
			this->y_array[i] = y_array[i];
		}
		this->numOfPoints = numOfPoints;

		afterUpdate_a_b_c_terms = false;
		afterUpdateStandardDeviation = false;
	}

	ParabolicApproximation::~ParabolicApproximation()
	{
		delete( this->x_array );
		delete( this->y_array );
		this->x_array = NULL;
		this->y_array = NULL;
	}

	void ParabolicApproximation::Init( double x_array[], double y_array[], int numOfPoints )
	{
		assert( numOfPoints >= 3 );

		if( this->numOfPoints != numOfPoints )
		{
			if( this->x_array != NULL )
				delete( this->x_array );
			if( this->y_array != NULL )
				delete( this->y_array );
			this->x_array = new double[numOfPoints];
			this->y_array = new double[numOfPoints];
			this->numOfPoints = numOfPoints;
		}

		for( int i = 0; i < numOfPoints; i++ )
		{
			this->x_array[i] = x_array[i];
			this->y_array[i] = y_array[i];
		}
		
		afterUpdate_a_b_c_terms = false;
		afterUpdateStandardDeviation = false;
	}

	void ParabolicApproximation::Get_a_b_c_terms2( double& a, double& b, double& c )
	{
		if( !afterUpdate_a_b_c_terms ) this->Update_a_b_c_terms();
		a = a_term; b = b_term; c = c_term;
	}

	void ParabolicApproximation::GetPeakParams2( double& peak_x, double& peak_y )
	{
		if( !afterUpdate_a_b_c_terms ) this->Update_a_b_c_terms();
		peak_x = this->peak_x; 
		peak_y = this->peak_y;
	}

	double ParabolicApproximation::StandardDeviation2( void )
	{
		if( !afterUpdateStandardDeviation ) this->UpdateStandardDeviation();
		return this->standardDeviation;
	}

	double ParabolicApproximation::PredictedY2( double x )
	{
		if( !afterUpdate_a_b_c_terms ) this->Update_a_b_c_terms();
		return a_term * x * x + b_term * x + c_term;
	}

	void ParabolicApproximation::Update_a_b_c_terms( void )
	{
		assert( numOfPoints >= 3 );

		double s1, s2, s3, s4, s5, s6, s7;
		Matrix S( 3 );
		Vector z, t( 3 );

		s1 = s2 = s3 = s4 = s5 = s6 = s7 = 0;
		for( int i = 0; i < numOfPoints; i++ )
		{
			s1 += x_array[i];
			s2 += pow( x_array[i], 2 );
			s3 += pow( x_array[i], 3 );
			s4 += pow( x_array[i], 4 );
			s5 += y_array[i];
			s6 += x_array[i] * y_array[i];
			s7 += pow( x_array[i], 2 ) * y_array[i];
		}

		S( 0, 0 ) = numOfPoints;	S( 0, 1 ) = s1;		S( 0, 2 ) = s2;
		S( 1, 0 ) = s1;				S( 1, 1 ) = s2;		S( 1, 2 ) = s3;
		S( 2, 0 ) = s2;				S( 2, 1 ) = s3;		S( 2, 2 ) = s4;
		t[0] = s5;					t[1] = s6;			t[2] = s7;
		// –ешаем систему линейных уравнений: S * z = t.
		z = SystemOfLinearAlgebraicEquations::Solution( S, t );
		a_term = z[2]; 
		b_term = z[1]; 
		c_term = z[0];

		peak_x = -0.5 * b_term / a_term;
		peak_y = a_term * peak_x * peak_x + b_term * peak_x + c_term;

		afterUpdate_a_b_c_terms = true;
	}

	void ParabolicApproximation::UpdateStandardDeviation( void )
	{
		if( !afterUpdate_a_b_c_terms ) this->Update_a_b_c_terms();
		double sum = 0;

		if( this->numOfPoints > 3 )
		{
			for( int i = 0; i < numOfPoints; i++ )
				sum += pow( y_array[i] - PredictedY( x_array[i] ), 2 );
			sum /= numOfPoints;
		}
		//this->standardDeviation = pow( sum, 0.5 );
		this->standardDeviation = sqrt( sum );

		afterUpdateStandardDeviation = true;
	}
	// End of description of class ParabolicApproximation.

	// Description of class PolynomicApproximation:

	PolynomicApproximation::PolynomicApproximation()
	{
		x_array = NULL;
		y_array = NULL;
		term = NULL;
		numOfPoints = 0;
		k = 0;
		standardDeviation = -1.0;	// Ќамерено присвоено не допустимое значение!
		//afterUpdate_terms = false;
		//afterUpdateStandardDeviation = false;
	}

	PolynomicApproximation::PolynomicApproximation( double x_array[], double y_array[], int numOfPoints, int k )
	{
		assert( k >= 1 );
		assert( numOfPoints >= k + 1 );

		this->x_array = new double[numOfPoints];
		this->y_array = new double[numOfPoints];
		for( int i = 0; i < numOfPoints; i++ )
		{
			this->x_array[i] = x_array[i];
			this->y_array[i] = y_array[i];
		}
		this->numOfPoints = numOfPoints;

		this->term = new double[k + 1];
		this->k = k;

		afterUpdate_terms = false;
		afterUpdateStandardDeviation = false;
	}

	PolynomicApproximation::~PolynomicApproximation()
	{
		delete( this->x_array );
		delete( this->y_array );
		delete( this->term );
		this->x_array = NULL;
		this->y_array = NULL;
		this->term = NULL;
	}

	void PolynomicApproximation::Init( double x_array[], double y_array[], int numOfPoints, int k )
	{
		assert( k >= 1 );
		assert( numOfPoints >= k + 1 );

		if( this->numOfPoints != numOfPoints )
		{
			if( this->x_array != NULL )
				delete( this->x_array );
			if( this->y_array != NULL )
				delete( this->y_array );
			this->x_array = new double[numOfPoints];
			this->y_array = new double[numOfPoints];
			this->numOfPoints = numOfPoints;
		}

		for( int i = 0; i < numOfPoints; i++ )
		{
			this->x_array[i] = x_array[i];
			this->y_array[i] = y_array[i];
		}

		if( this->k != k )
		{
			if( this->term != NULL )
				delete( this->term );
			this->term = new double[k + 1];
			this->k = k;
		}
		
		afterUpdate_terms = false;
		afterUpdateStandardDeviation = false;
	}

	void PolynomicApproximation::Get_terms( double*& term )
	{
		term = new double[k + 1];
		for( int i = 0; i < k + 1; i++ )
			term[i] = this->term[i];
	}

	void PolynomicApproximation::Get_terms2( double*& term )
	{
		if( !afterUpdate_terms ) this->Update_terms();
		term = new double[k + 1];
		for( int i = 0; i < k + 1; i++ )
			term[i] = this->term[i];
	}

	double PolynomicApproximation::StandardDeviation2( void )
	{
		if( !afterUpdateStandardDeviation ) this->UpdateStandardDeviation();
		return this->standardDeviation;
	}

	double PolynomicApproximation::PredictedY( double x )
	{
		double res = 0.0;
		for( int i = 0; i < k + 1; i++ ) res += term[i] * pow( x, i );
		return res;
	}
		
	double PolynomicApproximation::PredictedY2( double x )
	{
		double res = 0.0;
		if( !afterUpdate_terms ) this->Update_terms();
		for( int i = 0; i < k + 1; i++ ) res += term[i] * pow( x, i );
		return res;
	}

	void PolynomicApproximation::Update_terms( void )
	{
		assert( numOfPoints >= k + 1 );

		int i, j, l;
		Matrix S( k + 1 );
		Vector z, t( k + 1 );

		for( i = 0; i < k + 1; i++ )
		{
			for( j = 0; j < k + 1; j++ )
				for( l = 0; l < numOfPoints; l++ )
					S( i, j ) += pow( x_array[l], i + j );
			
			for( l = 0; l < numOfPoints; l++ )
				t[i] += pow( x_array[l], i ) * y_array[l];
		}

		// –ешаем систему линейных уравнений: S * z = t.
		z = SystemOfLinearAlgebraicEquations::Solution( S, t );
		for( i = 0; i < k + 1; i++ )
			term[i] = z[i];

		afterUpdate_terms = true;
	}

	void PolynomicApproximation::UpdateStandardDeviation( void )
	{
		if( !afterUpdate_terms ) this->Update_terms();
		double sum = 0;

		if( this->numOfPoints > k + 1 )
		{
			for( int i = 0; i < numOfPoints; i++ )
				sum += pow( y_array[i] - PredictedY( x_array[i] ), 2 );
			sum /= numOfPoints;
		}
		//this->standardDeviation = pow( sum, 0.5 );
		this->standardDeviation = sqrt( sum );

		afterUpdateStandardDeviation = true;
	}
	// End of description of class PolynomicApproximation.

} // End namespace Approximation.