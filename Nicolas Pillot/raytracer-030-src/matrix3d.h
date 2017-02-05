#ifndef MATRIX3D_H
#define MATRIX3D_H

#include "printable.h"

class Vector3d;
class Row3d;

class Matrix3d : public Printable
{
    public:
        double a11, a12, a13, a21, a22, a23, a31, a32, a33;
    
        Matrix3d( );
        Matrix3d(double _a11, double _a12, double _a13, 
			     double _a21, double _a22, double _a23, 
			     double _a31, double _a32, double _a33);
        Matrix3d(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3);
        Matrix3d(const Row3d& v1, const Row3d& v2, const Row3d& v3);
        virtual ~Matrix3d();

        void transpose();
        Matrix3d transposed() const;
		double det() const;

        void invert();
        Matrix3d inverted() const;

        Matrix3d operator - () const;
        Matrix3d operator * (double lambda) const;
        Matrix3d operator / (double lambda) const;
        Matrix3d operator - (const Matrix3d& m) const;
        Matrix3d operator + (const Matrix3d& m) const;
        Matrix3d operator * (const Matrix3d& m) const;
        Vector3d operator * (const Vector3d& v) const;

		void operator += (const Matrix3d& m);
		void operator -= (const Matrix3d& m);
		void operator *= (double lambda);
		void operator /= (double lambda);

        virtual std::string toString() const;

        static Matrix3d instanciateFromStream(const std::istream& is);

};

#endif

