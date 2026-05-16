#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

namespace Matrices
{
    class Matrix
    {
        public:
            
            /* Construct a matrix of the specified size. */
            /* Initialize each element to 0. */
            Matrix(int _rows, int _cols);

            ///************************************
            ///inline accessors / mutators, these are done:

            /* Read element at row i, column j */
            /* @usage:  double x = a(i, j) */
            const double& operator()(int i, int j) const
            {
                return a.at(i).at(j);
            }

            /* Assign element at row i, column j */
            /* @usage:  a(i, j) = x; */
            double& operator()(int i, int j)
            {
                return a.at(i).at(j);
            }

            /* Gets rows. */
            int GetRows() const{return rows;}

            /* Gets cols. */
            int GetCols() const{return cols;}

            
        protected:
            /* adjusted to protected so subclasses may modify. */
            vector<vector<double>> a;
        private:
            int rows;
            int cols;
    };

    /* Adds each corresponding element. */
    /* @usage:  c = a + b; */
    Matrix operator+(const Matrix& a, const Matrix& b);

    /* Matrix multiply. */
    /* @usage:  c = a * b; */
    Matrix operator*(const Matrix& a, const Matrix& b);

    /* Matrix comparison.See description. */
    /* @usage:  a == b */
    bool operator==(const Matrix& a, const Matrix& b);

    /* Matrix comparison.See description. */
    /* @usage:  a != b */
    bool operator!=(const Matrix& a, const Matrix& b);

    /* Output Matrix. */
    /* Separate columns by ' ' and rows by '\n' */
    ostream& operator<<(ostream& os, const Matrix& a);

    /*******************************************************************************/

    // first attempt at writing comments like this.

    /**
    * @brief A 2D rotation matrix.
    *
    * @code
    * A = R * a;
    * @endcode
    * rotates A theta radians counter - clockwise
    */
    class RotationMatrix : public Matrix
    {
        public:
            /**
            * @brief Constructs a 2x2 counter-clocwise rotation matrix.
            * Then assign each element as follows:
            * @code
            * [cos(theta) - sin(theta)]
            * [sin(theta) cos(theta) 
            * @endcode
            *
            * @param theta represents the angle of rotation in radians, counter-clockwise
            */
            RotationMatrix(double theta);
    };

    /**
    * @brief A 2D rotation matrix.
    * 
    * @code
    * A = S * A; // Expands or contracts A by the specified scaling factor.
    * @endcode
    */
    class ScalingMatrix : public Matrix
    {
        public:
            /**
            /* @brief Call the parent constructor to create a 2x2 matrix
            /* Then assign each element as follows:
            /* @code
            /* [scale 0]
            /* [0       scale]
            * @endcode
            * 
            * @param scale the size multipler.
            */
            ScalingMatrix(double scale);
    };

    /**
    * @brief A 2D Translation matrix.
    *
    * @code
    * A = T + A; // will shift all coordinates of A by (xShift, yShift)
    * @endcode
    */
    class TranslationMatrix : public Matrix
    {
        public:
            /**
            * @brief Call the parent constructor to create a 2xn matrix
            * Then assign each element as follows:
            *
            * [xShift  xShift  xShift  ...]
            * [yShift  yShift  yShift  ...]
            * @endcode
            *
            * paramaters are xShift, yShift, and nCols
            * nCols represents the number of columns in the matrix
            * where each column contains one (x,y) coordinate pair;e
            */
            TranslationMatrix(double xShift, double yShift, int nCols);
    };
}