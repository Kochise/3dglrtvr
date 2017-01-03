#ifndef ROW3D_H
#define ROW3D_H

#include "printable.h"

class Vector3d;
class Matrix3d;

class Row3d : public Printable
{
    public:
        double dx, dy, dz;
    
        Row3d( );
        Row3d( double _dx, double _dy, double _dz);
        virtual ~Row3d();

        void normalize();
        Row3d normalized() const;
		double length() const;
        
        Row3d operator - () const;
        Row3d operator * (double lambda) const;
        Row3d operator * (const Matrix3d& m) const;
        double operator * (const Vector3d& v) const;
        Row3d operator / (double lambda) const;
        Row3d operator - (const Row3d& v) const;
        Row3d operator + (const Row3d& v) const;

		void operator += (const Row3d& v);
		void operator -= (const Row3d& v);
		void operator *= (double lambda);
		void operator /= (double lambda);

        Vector3d transpose () const;

        virtual std::string toString() const;

        static Row3d instanciateFromStream(const std::istream& is);
};

#endif

