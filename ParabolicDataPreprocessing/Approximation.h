// Approximation.h
#pragma once

namespace Approximation
{
	// Declaration of class LinearApproximation:
	class LinearApproximation
	{
	private:
		double *x_array, *y_array;
		int numOfPoints;
		double a_term, b_term, standardDeviation;
		bool afterUpdate_a_b_terms;
		bool afterUpdateStandardDeviation;
	public:
		LinearApproximation();
		LinearApproximation( double x_array[], double y_array[], int numOfPoints );
		~LinearApproximation();
		void Init( double x_array[], double y_array[], int numOfPoints );
		inline void Get_a_b_terms( double& a, double& b ) { a = a_term; b = b_term; };
		inline double StandardDeviation( void ) { return standardDeviation; };
		void Get_a_b_terms2( double& a, double& b );
		double StandardDeviation2( void );
		inline double PredictedY( double x ) { return a_term * x + b_term; };
		double PredictedY2( double x );
		inline int GetNumOfPoints( void ) { return numOfPoints; };
		inline bool AfterUpdate_a_b_terms( void ) { return afterUpdate_a_b_terms; };
		inline bool AfterUpdateStandardDeviation( void ) { return afterUpdateStandardDeviation; };
		void Update_a_b_terms( void );
		void UpdateStandardDeviation( void );
	};

	// Declaration of class ParabolicApproximation:
	class ParabolicApproximation
	{
	private:
		double *x_array, *y_array;
		int numOfPoints;
		double a_term, b_term, c_term, peak_x, peak_y, standardDeviation;
		bool afterUpdate_a_b_c_terms;
		bool afterUpdateStandardDeviation;
	public:
		ParabolicApproximation();
		ParabolicApproximation( double x_array[], double y_array[], int numOfPoints );
		~ParabolicApproximation();
		void Init( double x_array[], double y_array[], int numOfPoints );
		inline void Get_a_b_c_terms( double& a, double& b, double& c ) { a = a_term; b = b_term; c = c_term; };
		inline void GetPeakParams( double& peak_x, double& peak_y ) { peak_x = this->peak_x; peak_y = this->peak_y; };
		inline double StandardDeviation( void ) { return standardDeviation; };
		void Get_a_b_c_terms2( double& a, double& b, double& c );
		void GetPeakParams2( double& peak_x, double& peak_y );
		double StandardDeviation2( void );
		inline double PredictedY( double x ) { return a_term * x * x + b_term * x + c_term; };
		double PredictedY2( double x );
		inline int GetNumOfPoints( void ) { return numOfPoints; };
		inline bool AfterUpdate_a_b_c_terms( void ) { return afterUpdate_a_b_c_terms; };
		inline bool AfterUpdateStandardDeviation( void ) { return afterUpdateStandardDeviation; };
		void Update_a_b_c_terms( void );
		void UpdateStandardDeviation( void );
	};

	// Declaration of class PolynomicApproximation:
	class PolynomicApproximation
	{
	private:
		double *x_array, *y_array;
		int numOfPoints, k;
		double *term, standardDeviation;
		bool afterUpdate_terms;
		bool afterUpdateStandardDeviation;
	public:
		PolynomicApproximation();
		PolynomicApproximation( double x_array[], double y_array[], int numOfPoints, int k );
		~PolynomicApproximation();
		void Init( double x_array[], double y_array[], int numOfPoints, int k );
		void Get_terms( double*& term );
		inline double StandardDeviation( void ) { return standardDeviation; };
		void Get_terms2( double*& term );
		double StandardDeviation2( void );
		double PredictedY( double x );
		double PredictedY2( double x );
		inline int GetNumOfPoints( void ) { return numOfPoints; };
		inline int Get_k( void ) { return k; };
		inline bool AfterUpdate_terms( void ) { return afterUpdate_terms; };
		inline bool AfterUpdateStandardDeviation( void ) { return afterUpdateStandardDeviation; };
		void Update_terms( void );
		void UpdateStandardDeviation( void );
	};

}