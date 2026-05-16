#include "Matrices.h"


namespace Matrices
{

	Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols)
	{
		a.resize(rows, vector<double>(cols, 0.0));
	}


	Matrix operator+(const Matrix& a, const Matrix& b)
	{
		return Matrix(a.getRows(), a.getCols()); // TEMP.
	}

	Matrix operator*(const Matrix& a, const Matrix& b)
	{
		return Matrix(a.getRows(), a.getCols()); // TEMP.
	}


	bool operator==(const Matrix& a, const Matrix& b)
	{

		return false; // temp.
	}

	bool operator!=(const Matrix& a, const Matrix& b)
	{
		return false; // temp.
	}

	ostream& operator<<(ostream& os, const Matrix& a)
	{ 
		
		return os; // TEMP.
		
	}

	RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
	{

	}

	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
	{

	}

	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
	{

	}

	
}