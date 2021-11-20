#include "Matrixes3.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <assert.h>

namespace Matrixes3
{
	// Description of class Vector:
	Vector::Vector(int dimension)
    {
		assert(dimension > 0);
		this->arr = new double[dimension];
		this->dim = dimension;
		//for(int i = 0; i < dimension; i++) arr[i] = 0;
		memset(arr, 0, dim * sizeof(double));
    }

	Vector::Vector(double* arr, int len)
    {
		dim = len;
		this->arr = new double[len];
		//for(int i = 0; i < len; i++) this->arr[i] = arr[i];
		memcpy(this->arr, arr, len * sizeof(double));
    }

	Vector::Vector(const Vector& v)															// Copy constructor
    {
		dim = v.dim;
		this->arr = new double[dim];
		memcpy(this->arr, v.arr, dim * sizeof(double));
    }

	Vector::~Vector()
	{
		delete [] arr;
		// Временная строка, потом НЕ забыть закомментировать:
		//std::cout << "\nThe destructor was called!\n";
	}

	int Vector::Dim() const
	{
		return dim;
	}

	double& Vector::operator[](int index)
	{
		assert( ( index >= 0 ) && ( index < dim ) );
		return arr[index];
	}

	void Vector::operator =(const Vector& right_v)											// Assignment operator
	{
		int new_dim = right_v.dim;
		if( this->dim != new_dim )
		{
			delete [] this->arr;
			this->dim = new_dim;
			this->arr = new double[new_dim];
		}
		//for(int i = 0; i < dim; i++) arr[i] = right_v.arr[i];
		memcpy(this->arr, right_v.arr, new_dim * sizeof(double));
	}

	Vector& operator +(Vector& v1, Vector& v2)
	{
		assert(v1.Dim() == v2.Dim());
		Vector *res = new Vector( v1.Dim() );
		for(int i = 0; i < v2.Dim(); i++) (*res)[i] = v1[i] + v2[i];

		return *res;
	}

	Vector& operator -(Vector& v1, Vector& v2)
	{
		assert(v1.Dim() == v2.Dim());
		Vector *res = new Vector( v1.Dim() );
		for(int i = 0; i < v2.Dim(); i++) (*res)[i] = v1[i] - v2[i];

		return *res;
	}

	Vector& operator -(Vector& v)
	{
		Vector *res = new Vector( v.Dim() );
		for(int i = 0; i < v.Dim(); i++) (*res)[i] = - v[i];

		return *res;
	}

	double operator *(Vector& v1, Vector& v2)
	{
		assert(v1.Dim() == v2.Dim());
		double res = 0;
		for(int i = 0; i < v2.Dim(); i++) res += v1[i] * v2[i];
		
		return res;
	}

	Vector& operator *(double factor, Vector& v)
	{
		Vector *res = new Vector( v );
		for(int i = 0; i < v.Dim(); i++) (*res)[i] *= factor;

		return *res;
	}

	Vector& operator *(Vector& v, double factor)											// New
	{
		Vector *res = new Vector( v );
		for(int i = 0; i < v.Dim(); i++) (*res)[i] *= factor;

		return *res;
	}

	Vector& operator /(Vector& v, double divider)											// New
	{
		Vector *res = new Vector( v );
		for(int i = 0; i < v.Dim(); i++) (*res)[i] /= divider;

		return *res;
	}

	void Vector::operator +=(Vector& v)														// New
	{
		assert( dim == v.dim );
		for(int i = 0; i < dim; i++) arr[i] += v.arr[i];
	}

	void Vector::operator -=(Vector& v)														// New
	{
		assert( dim == v.dim );
		for(int i = 0; i < dim; i++) arr[i] -= v.arr[i];
	}

	void Vector::operator *=(double factor)
	{
		for(int i = 0; i < dim; i++) arr[i] *= factor;
	}

	void Vector::operator /=(double divider)												// New
	{
		for(int i = 0; i < dim; i++) arr[i] /= divider;
	}

	void Vector::Display(std::ostream& outs, int posForInterval) const
	{
		int i;
		char* interval;
		
		interval = new char[posForInterval + 1];
		for (i = 0; i < posForInterval; i++) interval[i] = ' ';
		interval[posForInterval] = '\0';
		outs << "{ ";
		for(i = 0; i < this->dim - 1; i++) outs << this->arr[i] << ";" << interval;
		outs << this->arr[i] << " }";
	}
	// End of description of class Vector

	// Description of class Matrix:
	Matrix::Matrix()
	{
		numOfRows = 0;
		numOfColumns = 0;
		arr = NULL;
	}

	Matrix::Matrix(int size)
	{
		assert( size > 0 );
		numOfRows = numOfColumns = size;
		arr = new double[size * size];
		for(int r = 0; r < numOfRows; r++)
			for(int c = 0; c < numOfColumns; c++)
				arr[r * numOfColumns + c] = 0;
	}
		
	Matrix::Matrix(int numOfRows, int numOfColumns)
	{
		assert( ( numOfRows > 0 ) && ( numOfColumns > 0 ) );
		
		this->numOfRows = numOfRows;
		this->numOfColumns = numOfColumns;
		arr = new double[numOfRows * numOfColumns];
		/*
		for(int r = 0; r < numOfRows; r++)
			for(int c = 0; c < numOfColumns; c++)
				arr[r * numOfColumns + c] = 0;
		*/
		memset(arr, 0, numOfRows * numOfColumns * sizeof(double));
	}

	Matrix::Matrix(const Matrix& M)															// Copy constructor
	{
		this->numOfRows = M.numOfRows; 
		this->numOfColumns = M.numOfColumns;
		//this->arr = new double[this->numOfRows * this->numOfColumns];
		this->arr = new double[numOfRows * numOfColumns];
		memcpy(this->arr, M.arr, numOfRows * numOfColumns * sizeof(double));
	}

	Matrix::~Matrix()
	{
		delete [] arr;
		// Временная строка, потом НЕ забыть закомментировать:
		//std::cout << "\nThe destructor was called!\n";
	}

	int Matrix::NumOfRows() const
	{
		return numOfRows;
	};
	
	int Matrix::NumOfColumns() const
	{
		return numOfColumns;
	};

	inline double& Matrix::operator() (int r, int c)
	{
		assert( ( r >= 0 ) && ( r < numOfRows ) );
		assert( ( c >= 0 ) && ( c < numOfColumns ) );

		return arr[numOfColumns * r + c];
	}

	inline double Matrix::operator() (int r, int c) const
	{
		assert( ( r >= 0 ) && ( r < numOfRows ) );
		assert( ( c >= 0 ) && ( c < numOfColumns ) );

		return arr[numOfColumns * r + c];
	}

	Matrix& Matrix::operator= (const Matrix& M)												// Assignment operator
	{
		this->numOfRows = M.numOfRows;
		this->numOfColumns = M.numOfColumns;
		this->arr = new double[numOfRows * numOfColumns];
		memcpy(this->arr, M.arr, numOfRows * numOfColumns * sizeof(double));
		return *this;
	}

	Matrix& operator +(const Matrix& A, const Matrix& B)
	{
		assert( ( A.numOfRows == B.numOfRows ) && ( A.numOfColumns == B.numOfColumns ) );
		int numOfRows, numOfColumns, r, c;
		numOfRows = A.NumOfRows();
		numOfColumns = A.NumOfColumns();
		Matrix *ResMx = new Matrix( numOfRows, numOfColumns );
		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				(*ResMx)(r, c) = A(r, c) + B(r, c);

		return *ResMx;
	}

	Matrix& operator -(const Matrix& A, const Matrix& B)
	{
		assert( ( A.numOfRows == B.numOfRows ) && ( A.numOfColumns == B.numOfColumns ) );
		int numOfRows, numOfColumns, r, c;
		numOfRows = A.NumOfRows();
		numOfColumns = A.NumOfColumns();
		Matrix *ResMx = new Matrix( numOfRows, numOfColumns );
		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				(*ResMx)(r, c) = A(r, c) - B(r, c);

		return *ResMx;
	}

	Matrix& operator -(const Matrix& A)
	{
		int numOfRows, numOfColumns, r, c;
		numOfRows = A.NumOfRows();
		numOfColumns = A.NumOfColumns();
		Matrix *ResMx = new Matrix( numOfRows, numOfColumns );
		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				(*ResMx)(r, c) = -A(r, c);

		return *ResMx;
	}

	Matrix& operator *(const Matrix& A, const Matrix& B)
	{
		assert( A.numOfColumns == B.numOfRows );
		int numOfRows, numOfColumns, r, c, size, i;
		size = A.numOfColumns;
        numOfRows = A.numOfRows;
        numOfColumns = B.numOfColumns;
		Matrix *ResMx = new Matrix(numOfRows, numOfColumns);
		for (r = 0; r < numOfRows; r++)
			for (c = 0; c < numOfColumns; c++)
				for (i = 0; i < size; i++)
					(*ResMx)(r, c) += A(r, i) * B(i, c);

		return *ResMx;
	}

	Vector& operator *(Vector& a, const Matrix& B)
	{
		assert( a.Dim() == B.numOfRows );
		int size, numOfColumns, r, c;
		numOfColumns = B.numOfColumns;
		size = a.Dim();

		Vector *res = new Vector( numOfColumns );
		for (c = 0; c < numOfColumns; c++)
			for (r = 0; r < size; r++)
				(*res)[c] += a[r] * B.arr[r * size + c];

		return *res;
	}

	Vector& operator *(const Matrix& A, Vector& b)
	{
		assert( b.Dim() == A.numOfColumns );
		int size, numOfRows, r, c;
		numOfRows = A.numOfRows;
        size = b.Dim();

		Vector *res = new Vector( numOfRows );
		for (r = 0; r < numOfRows; r++)
			for (c = 0; c < size; c++)
				(*res)[r] += A.arr[r * size + c] * b[c];

		return *res;
	}

	Matrix& operator *(double f, const Matrix& B)
	{
		assert( B.arr != NULL );
		int numOfRows, numOfColumns, r, c;
		numOfRows = B.numOfRows;
		numOfColumns = B.numOfColumns;
		Matrix *ResMx = new Matrix( B );
		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				(*ResMx).arr[r * numOfColumns + c] *= f;

		return *ResMx;
	}

	Matrix& operator *(const Matrix& B, double f)											// New
	{
		assert( B.arr != NULL );
		int numOfRows, numOfColumns, r, c;
		numOfRows = B.numOfRows;
		numOfColumns = B.numOfColumns;
		Matrix *ResMx = new Matrix( B );
		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				(*ResMx).arr[r * numOfColumns + c] *= f;

		return *ResMx;
	}

	Matrix& operator /(const Matrix& B, double d)											// New
	{
		assert( B.arr != NULL );
		int numOfRows, numOfColumns, r, c;
		numOfRows = B.numOfRows;
		numOfColumns = B.numOfColumns;
		Matrix *ResMx = new Matrix( B );
		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				ResMx->arr[r * numOfColumns + c] /= d;

		return *ResMx;
	}

	void Matrix::operator +=(const Matrix& B)												// New
	{
		assert( this->arr != NULL );
		assert( this->numOfRows == B.numOfRows );
		assert( this->numOfColumns == B.numOfColumns );
		for(int r = 0; r < this->numOfRows; r++)
			for(int c = 0; c < this->numOfColumns; c++)
				this->arr[r * numOfColumns + c] += B.arr[r * numOfColumns + c];
	}

	void Matrix::operator -=(const Matrix& B)												// New
	{
		assert( this->arr != NULL );
		assert( this->numOfRows == B.numOfRows );
		assert( this->numOfColumns == B.numOfColumns );
		for(int r = 0; r < this->numOfRows; r++)
			for(int c = 0; c < this->numOfColumns; c++)
				this->arr[r * numOfColumns + c] -= B.arr[r * numOfColumns + c];
	}

	void Matrix::operator *=(double factor)
	{
		assert( this->arr != NULL );
		for(int r = 0; r < this->numOfRows; r++)
			for(int c = 0; c < this->numOfColumns; c++)
				this->arr[r * numOfColumns + c] *= factor;
	}

	void Matrix::operator /=(double divider)												// New
	{
		assert( this->arr != NULL );
		for(int r = 0; r < this->numOfRows; r++)
			for(int c = 0; c < this->numOfColumns; c++)
				this->arr[r * numOfColumns + c] /= divider;
	}

	void Matrix::InitUnity()
	{
		assert( numOfColumns == numOfRows );
		int i, size, length;
		memset(arr, 0, numOfRows * numOfRows * sizeof(double));
		size = numOfRows;
		length = size * size;
		for (i = 0; i < length; i += size + 1) arr[i] = 1.0;
	}

	void Matrix::InitZero()
	{
		assert( this->arr != NULL );
		memset(arr, 0, numOfRows * numOfColumns * sizeof(double));
	}

	void Matrix::InitArray(double** arr, int rows, int cols)
	{
		int r, c;
		if( ( numOfRows != rows ) || ( numOfColumns != cols ) )
		{
			delete [] this->arr;
			numOfRows = rows;
			numOfColumns = cols;
			this->arr = new double[rows * cols];
		}
		for(r = 0; r < rows; r++)
			for(c = 0; c < cols; c++)
				this->arr[r * cols + c] = arr[r][c];
	}

	void Matrix::InitArray(double* arr, int cols)
	{
		int c;
		if( ( numOfRows != 1 ) || ( numOfColumns != cols ) )
		{
			delete [] this->arr;
			numOfRows = 1;
			numOfColumns = cols;
			this->arr = new double[cols];
		}
		for(c = 0; c < cols; c++)
			this->arr[c] = arr[c];
	}

	Matrix& Matrix::Transpose() const
	{
		Matrix *PtrResMx = new Matrix(numOfColumns, numOfRows);
		int r, c;

		for(r = 0; r < numOfRows; r++)
			for(c = 0; c < numOfColumns; c++)
				PtrResMx->arr[c * numOfRows + r] = arr[r * numOfColumns + c];

		return *PtrResMx;
	}

	void Matrix::SwapColumns(int i0, int i1)
	{
		assert( ( i0 >= 0 ) && ( i0 < numOfColumns ) );
		assert( ( i1 >= 0 ) && ( i1 < numOfColumns ) );
		int j, size;
		size = numOfColumns;
		for (j = 0; j < numOfRows; j++) swap( arr[j * size + i0], arr[j * size + i1]);
	}
	
	void Matrix::SwapRows(int i0, int i1)
	{
		assert( ( i0 >= 0 ) && ( i0 < numOfRows ) );
		assert( ( i1 >= 0 ) && ( i1 < numOfRows ) );
		int j, size;
		size = numOfColumns;
		for (j = 0; j < numOfColumns; j++) swap( arr[i0 * size + j], arr[i1 * size + j]);
	}

	double Matrix::Det() const
	{ // Вычисление определителя методом LUP разложения
		assert( numOfRows == numOfColumns );
		Matrix *C, *P;
		int i, j, k, row, pivot, size;
        double pivotValue, res;

		size = numOfRows;
		P = new Matrix( size ); P->InitUnity();
		C = new Matrix( *this ); // В матрицу *C копируем матрицу для которой вычисляем определитель;
		res = 1;

		for (i = 0; i < size; i++)
		{ // Поиск опорного элемента:
			pivotValue = 0;
            pivot = -1;
            for (row = i; row < size; row++)
            {
				if ( abs( (*C).arr[row * size + i] ) > pivotValue )
                {
					pivotValue = abs( (*C).arr[row * size + i] );
                    pivot = row;
                }
            }
            if(pivotValue == 0) // Матрица вырождена
				return 0;

            // Меняем местами i-ю строку и строку с опорным элементом:
            if(pivot > i)
            {
				(*P).SwapRows(pivot, i);
                (*C).SwapRows(pivot, i);
                res = -res;
            }
            for(j = i + 1; j < size; j++)
            {
				(*C).arr[j * size + i] /= (*C).arr[i * size + i];
                for( k = i + 1; k < size; k++ )
					(*C).arr[j * size + k] += - (*C).arr[j* size + i] * (*C).arr[i * size + k];
            }
		}
        // Теперь матрица C = L + U - E, где E - единичная матрица, P * A = L * U

		for( i = 0; i < size; i++) res *= (*C).arr[i * size + i];
        return res;
	}

	void Matrix::DecompositionOnLUP( Matrix& L, Matrix& U, Matrix& P ) const
	{
		assert( numOfRows == numOfColumns );
		assert( arr != NULL );

		Matrix C( *this );
        int size, i, j, k, row, pivot;
        double pivotValue;

		size = numOfRows;
		L = *new Matrix(size);
        U = *new Matrix(size);
        P = *new Matrix(size); P.InitUnity();

		for (i = 0; i < size; i++)
        { 
			// Поиск опорного элемента:
            pivotValue = 0;
            pivot = -1;
            for (row = i; row < size; row++)
            {
				if ( abs(C.arr[row * size + i]) > pivotValue )
                {
					pivotValue = abs( C.arr[row * size + i] );
                    pivot = row;
                }
            }
            assert( pivotValue != 0 ); // Матрица НЕ вырождена

            // Меняем местами i-ю строку и строку с опорным элементом:
            if( pivot > i )
            {
				P.SwapRows(pivot, i);
                C.SwapRows(pivot, i);
            }
            for(j = i + 1; j < size; j++)
            {
				C.arr[j * size + i] /= C.arr[i * size + i];
                for( k = i + 1; k < size; k++ )
					C.arr[j * size + k] += - C.arr[j* size + i] * C.arr[i * size + k];
            }
		}
        // Теперь матрица C = L + U - E, где E - единичная матрица, P * A = L * U

        for (i = 0; i < size; i++)
        {
			for (j = 0; j < size; j++)
            {
				if (i < j)
                {
					U.arr[i * size + j] = C.arr[i * size + j];
                }
                else if (i > j)
                {
					L.arr[i * size + j] = C.arr[i * size + j];
                }
                else // (i == j)
                {
					L.arr[i * size + i] = 1;
                    U.arr[i * size + i] = C.arr[i * size + i];
                }
            }
        }
	}

	void Matrix::DecompositionOnLUP( Matrix* &L, Matrix* &U, Matrix* &P ) const
	{
		assert( numOfRows == numOfColumns );
		assert( arr != NULL );

		Matrix C( *this );
        int size, i, j, k, row, pivot;
        double pivotValue;

		size = numOfRows;
		L = new Matrix(size);
        U = new Matrix(size);
        P = new Matrix(size); P->InitUnity();

		for (i = 0; i < size; i++)
        { 
			// Поиск опорного элемента:
            pivotValue = 0;
            pivot = -1;
            for (row = i; row < size; row++)
            {
				if ( abs(C.arr[row * size + i]) > pivotValue )
                {
					pivotValue = abs( C.arr[row * size + i] );
                    pivot = row;
                }
            }
            assert( pivotValue != 0 ); // Матрица НЕ вырождена

            // Меняем местами i-ю строку и строку с опорным элементом:
            if( pivot > i )
            {
				P->SwapRows(pivot, i);
                C.SwapRows(pivot, i);
            }
            for(j = i + 1; j < size; j++)
            {
				C.arr[j * size + i] /= C.arr[i * size + i];
                for( k = i + 1; k < size; k++ )
					C.arr[j * size + k] += - C.arr[j* size + i] * C.arr[i * size + k];
            }
		}
        // Теперь матрица C = L + U - E, где E - единичная матрица, P * A = L * U

        for (i = 0; i < size; i++)
        {
			for (j = 0; j < size; j++)
            {
				if (i < j)
                {
					U->arr[i * size + j] = C.arr[i * size + j];
                }
                else if (i > j)
                {
					L->arr[i * size + j] = C.arr[i * size + j];
                }
                else // (i == j)
                {
					L->arr[i * size + i] = 1;
                    U->arr[i * size + i] = C.arr[i * size + i];
                }
            }
        }
	}

	Matrix& Matrix::Inverse() const
	/* Матричное уравнение AX = In для обратной матрицы X можно рассматривать, как совокупность n систем вида Ax = b с одинаковыми левыми частями. 
     * Обозначим i-й столбец матрицы X через Xi; тогда AXi = ei, i = 1, ..., n, поскольку i-м столбцом матрицы In является единичный вектор ei. 
     * Совокупность систем линейных уравнений решается с помощью LUP разложения исходной матрицы. Трудоёмкость алгоритма O(n*n*n). */
	{
		assert( numOfRows == numOfColumns );
		assert( arr != NULL );

		Matrix *L, *U, *P, *X;
        int i, k, j, size;
        double sum;
        double *y;

		size = numOfRows;
		y = new double[size];
        X = new Matrix( size );
        this->DecompositionOnLUP( L, U, P );

		// Решаем n систем вида AXj = ej
        for (j = 0; j < size; j++)
        {
			// Решаем систему Ly = Pj
            y[0] = P->arr[j];
            for (i = 1; i < size; i++)
            {
				sum = 0;
                for (k = 0; k < i; k++)
					sum += L->arr[i * size + k] * y[k];
                y[i] = P->arr[i * size + j] - sum;
            }
                
            // Решаем систему UXj = y
            X->arr[(size - 1) * size + j] = y[size - 1] / U->arr[(size - 1) * size + size - 1];
            for (i = size - 2; i >= 0; i--)
            {
				sum = 0;
                for(k = i + 1; k < size; k++)
					sum += U->arr[i * size + k] * X->arr[k * size + j];
                X->arr[i * size + j] = (y[i] - sum) / U->arr[i * size + i];
            }
		}

        return *X;
	}

	void Matrix::Display(std::ostream& outs, int posForInterval) const
	{
		assert( arr != NULL );
		int r, c, i;
		char* interval;
		
		interval = new char[posForInterval + 1];
		for (i = 0; i < posForInterval; i++) interval[i] = ' ';
		interval[posForInterval] = '\0';
		for (r = 0; r < numOfRows; r++)
        {
			for (c = 0; c < numOfColumns; c++)
				outs << this->arr[r * numOfColumns + c] << interval;
            outs << std::endl;
        }
	}

	Matrix& Mult(const Matrix& A, const Matrix& B)
	{
		assert( A.numOfColumns == B.numOfRows );
		Matrix* PtrResMx;
		int numOfRows, numOfColumns, r, c, size, i;
		size = A.numOfColumns;
        numOfRows = A.numOfRows;
        numOfColumns = B.numOfColumns;
		PtrResMx = new Matrix(numOfRows, numOfColumns);
		for (r = 0; r < numOfRows; r++)
			for (c = 0; c < numOfColumns; c++)
				for (i = 0; i < size; i++)
					PtrResMx->arr[r * numOfColumns + c] += A.arr[r * size + i] * B.arr[i * numOfColumns + c];

		return *PtrResMx;
	}

	Matrix& Sum(const Matrix& A, const Matrix& B)
	{
		assert( ( A.numOfRows == B.numOfRows ) && ( A.numOfColumns == B.numOfColumns ) );
		int numOfRows, numOfColumns, r, c;
		numOfRows = A.numOfRows;
		numOfColumns = A.numOfColumns;
		Matrix* PtrResMx = new Matrix( numOfRows, numOfColumns );
		for (r = 0; r < numOfRows; r++)
			for (c = 0; c < numOfColumns; c++)
				PtrResMx->arr[r * numOfColumns + c] = A.arr[r * numOfColumns + c] + B.arr[r * numOfColumns + c];

		return *PtrResMx;
	}

	Matrix& Dif(const Matrix& A, const Matrix& B)
	{
		assert( ( A.numOfRows == B.numOfRows ) && ( A.numOfColumns == B.numOfColumns ) );
		int numOfRows, numOfColumns, r, c;
		numOfRows = A.numOfRows;
		numOfColumns = A.numOfColumns;
		Matrix* PtrResMx = new Matrix( numOfRows, numOfColumns );
		for (r = 0; r < numOfRows; r++)
			for (c = 0; c < numOfColumns; c++)
				PtrResMx->arr[r * numOfColumns + c] = A.arr[r * numOfColumns + c] - B.arr[r * numOfColumns + c];

		return *PtrResMx;
	}
	// End of description of class Matrix

	void swap(double& a, double& b)
	{
		double temp;

		temp = a;
        a = b;
        b = temp;
	}

	double abs( double x )
	{
		return ( x >= 0 )? x: -x;
	}
}

