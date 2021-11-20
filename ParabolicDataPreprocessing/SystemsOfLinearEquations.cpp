#include "SystemsOfLinearEquations.h"
#include "Matrixes3.h"

#include <assert.h>

namespace SystemsOfLinearEquations 
{
	// Description of class SystemOfLinearAlgebraicEquations:

	Vector& SystemOfLinearAlgebraicEquations::Solution( const Matrix& A, Vector& b )
	// Возвращает вектор 'x' координаты которого являются решением системы линейных алгебраических уравнений (СЛАУ): Ax = b.
	{
		int size;
		double sum;
		Matrix *L, *U, *P;
		Vector *x, *y, c;

		size = A.NumOfColumns();
		assert( A.NumOfRows() == size );
		assert( b.Dim() == size );

		x = new Vector( size );
		y = new Vector( size );
		
		A.DecompositionOnLUP( L, U, P );	// P * A = L * U;
		c = (*P) * b;

		// Решаем СЛАУ Ly = c:
		for( int i = 0; i < size; i++ )
		{
			(*y)[i] = c[i];
			sum = 0;
			for( int j = 0; j < i; j++ )
				sum += (*y)[j] * (*L)( i, j );
			(*y)[i] -= sum;
		}

		// Решаем СЛАУ Ux = y:
		for( int i = size - 1; i >= 0; i-- )
		{
			(*x)[i] = (*y)[i];
			sum = 0;
			for( int j = i + 1; j < size; j++ )
				sum += (*x)[j] * (*U)( i, j );
			(*x)[i] -= sum;
			(*x)[i] /= (*U)( i, i );
		}

		return *x; // Возвращаем решение исходной СЛАУ Ax = b.
	}
}