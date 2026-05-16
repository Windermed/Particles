#include "Matrices.h"


namespace Matrices
{

	Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols)
	{
		a.resize(rows, vector<double>(cols, 0.0));
	}


	Matrix operator+(const Matrix& a, const Matrix& b)
	{
		if (a.GetRows() != b.GetRows() || a.GetCols() != b.GetCols())
		{
			throw runtime_error("Matrix dimensions do NOT match for addition."); // just in case.
		}

		Matrix result(a.GetRows(), a.GetCols());

		for (int i = 0; i < a.GetRows(); i++)
		{
			for (int j = 0; j < a.GetCols(); j++)
			{
				result(i, j) = a(i, j) + b(i, j);
			}

			return result;
		}
	}

	Matrix operator*(const Matrix& a, const Matrix& b)
	{
		if (a.GetCols() != b.GetRows())
		{
			throw runtime_error("Matrix dimensions do NOT match for multiplication.");
		}

		Matrix result(a.GetRows(), b.GetCols());
		for (int i = 0; i < a.GetRows(); i++)
		{
			for (int j = 0; j < b.GetCols(); j++)
			{
				for (int k = 0; k < a.GetCols(); k++)
				{
					result(i, j) += a(i, k) * b(k, j);
				}
			}
			return result;
		}
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