// Matrixes3.h
#pragma once

#include <iostream>
#include <fstream>

namespace Matrixes3 
{
	// Declaration of class Vector:
	class Vector
	{
	private:
		double* arr;
        int dim;
	public:
		Vector(int dim);
		Vector(double* arr, int len);
		Vector(const Vector& v);													// Copy constructor
		~Vector();
		int Dim() const;
		double& operator[](int index);
		void operator =(const Vector& right_v);										// Assignment operator
		friend Vector& operator +(Vector& v1, Vector& v2);
		friend Vector& operator -(Vector& v1, Vector& v2);
		friend Vector& operator -(Vector& v);
		friend double operator *(Vector& v1, Vector& v2);
		friend Vector& operator *(double f, Vector& v);
		friend Vector& operator *(Vector& v, double f);								// New
		friend Vector& operator /(Vector& v, double d);								// New
		void operator +=(Vector& v);												// New
		void operator -=(Vector& v);												// New
		void operator *=(double factor);
		void operator /=(double divider);											// New
		//friend std::ostream& operator <<(std::ostream& outs, Vector& v);
		void Display(std::ostream& outs, int posForInterval) const;
	};

	// Declaration of class Matrix:
	class Matrix
	{
	private:
		double* arr;
        int numOfRows, numOfColumns;
	public:
		Matrix(); 
		Matrix(int size);
		Matrix(int numOfRows, int numOfColumns);
		Matrix(const Matrix& M);													// Copy constructor
		~Matrix();
		int NumOfRows() const;
		int NumOfColumns() const;
		double& operator() (int r, int c);
		double operator() (int r, int c) const; 
		Matrix& operator= (const Matrix& M);										// Assignment operator
		friend Matrix& operator +(const Matrix& A, const Matrix& B);
		friend Matrix& operator -(const Matrix& A, const Matrix& B);
		friend Matrix& operator -(const Matrix& A);
		friend Matrix& operator *(const Matrix& A, const Matrix& B);
		friend Vector& operator *(Vector& a, const Matrix& B);
		friend Vector& operator *(const Matrix& A, Vector& b);
		friend Matrix& operator *(double f, const Matrix& B);
		friend Matrix& operator *(const Matrix& B, double f);						// New
		friend Matrix& operator /(const Matrix& B, double d);						// New
		void operator +=(const Matrix& B);											// New
		void operator -=(const Matrix& B);											// New
		void operator *=(double factor);
		void operator /=(double divider);											// New
		void InitUnity();
		void InitZero();
		void InitArray(double** arr, int rows, int cols);
		void InitArray(double* arr, int cols);
		Matrix& Transpose() const;
		void SwapColumns(int i0, int i1);
		void SwapRows(int i0, int i1);
		double Det() const;
		void DecompositionOnLUP( Matrix& L, Matrix& U, Matrix& P ) const;
		void DecompositionOnLUP( Matrix* &L, Matrix* &U, Matrix* &P ) const;
		Matrix& Inverse() const;
		void Display(std::ostream& outs, int posForInterval) const;

		friend Matrix& Mult(const Matrix& A, const Matrix& B);
		friend Matrix& Sum(const Matrix& A, const Matrix& B);
		friend Matrix& Dif(const Matrix& A, const Matrix& B);
	};

	void swap(double& a, double& b);
	double abs( double x );
}