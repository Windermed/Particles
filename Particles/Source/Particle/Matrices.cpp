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
				for (int k = 0; k < a.GetCols(); k++)
				{
					result(i, j) = a(i, j) + b(i, j);
				}
			}
		}
		return result;
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
			
		}
		return result;
	}


	bool operator==(const Matrix& a, const Matrix& b)
	{
		if (a.GetRows() != b.GetRows() || a.GetCols() != b.GetCols())
		{
			return false;
		}

		for (int i = 0; i < a.GetRows(); i++)
		{
			for (int j = 0; j < a.GetCols(); j++)
			{
				if (fabs(a(i, j) - b(i, j)) > 0.0001)
				{
					return false;
				}
			}
		}

		return true;

	}

	bool operator!=(const Matrix& a, const Matrix& b)
	{
		return !(a == b);
	}

	ostream& operator<<(ostream& os, const Matrix& a)
	{ 
		
		for (int i = 0; i < a.GetRows(); i++)
		{
			for (int j = 0; j < a.GetCols(); j++)
			{
				os << setw(10) << a(i, j);
				if (j < a.GetCols() - 1) os << ' ';
			}

			os << '\n';
		}
		return os;
		
	}

	RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
	{
		a[0][0] = cos(theta);
		a[0][1] = -sin(theta);
		a[1][0] = sin(theta);
		a[1][1] = cos(theta);
	}

	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
	{
		a[0][0] = scale;
		a[0][1] = 0;
		a[1][0] = 0;
		a[1][1] = scale;
	}

	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
	{
		for (int i = 0; i < nCols; i++)
		{
			a[0][i] = xShift;
			a[1][i] = yShift;
		}
	}

	
}