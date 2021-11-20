// SystemsOfLinearEquations.h
#pragma once

#include "Matrixes3.h"
using namespace Matrixes3;

namespace SystemsOfLinearEquations 
{
	// Declaration of class SystemOfLinearAlgebraicEquations:
	class SystemOfLinearAlgebraicEquations
	{
	public:
		static Vector& Solution( const Matrix& A, Vector& b );
		// ���������� ������ 'x' ���������� �������� �������� �������� ������� �������� �������������� ���������: Ax = b.
	};

}