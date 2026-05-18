#include "Matrices.h"


namespace Matrices
{

	// constructs a matrix with the set rows and colums.
	Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols)
	{
		a.resize(rows, vector<double>(cols, 0.0));
	}

	// adds two matrices element by element.
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
				// add corresponding elements from both matrices.
				for (int k = 0; k < a.GetCols(); k++)
				{
					result(i, j) = a(i, j) + b(i, j);
				}
			}
		}
		return result;
	}


	// Multiplies two matrices together.
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

	// compare both matrices to see if they are equal. will return false if the dimensions are different.
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

	// will return true if two matrices are NOT equal.
	bool operator!=(const Matrix& a, const Matrix& b)
	{
		return !(a == b);
	}

	// outputs the matrix to an ostream via a formatted grid.
	// rows will be seperated using newlines.
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

	// constructs a 2x2 rotation matrix.
	RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
	{
		a[0][0] = cos(theta);
		a[0][1] = -sin(theta);
		a[1][0] = sin(theta);
		a[1][1] = cos(theta);
	}

	// constructs a 2x2 uniform scaling matrix
	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
	{
		a[0][0] = scale;
		a[0][1] = 0;
		a[1][0] = 0;
		a[1][1] = scale;
	}

	// constructs a 2cN translation matrix where N is the number of points to translate.
	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
	{
		for (int i = 0; i < nCols; i++)
		{
			a[0][i] = xShift;
			a[1][i] = yShift;
		}
	}

	
}