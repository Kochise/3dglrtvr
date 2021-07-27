#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "printable.h"

class Point3d;
class Matrix3d;
class Row3d;

class Vector3d : public Printable
{
    public:
        double dx, dy, dz;
    
        Vector3d( );
        Vector3d( const Point3d& base, const Point3d& extrem );
        Vector3d( double _dx, double _dy, double _dz);
        virtual ~Vector3d();

        void normalize();
        Vector3d normalized() const;
		double length() const;
		Row3d transpose() const;
        
        Vector3d operator - () const;
        Vector3d operator * (double lambda) const;
        Matrix3d operator * (const Row3d& r) const;
        Vector3d operator / (double lambda) const;
        Vector3d operator ^ (const Vector3d& v) const;
        Vector3d operator - (const Vector3d& v) const;
        Vector3d operator + (const Vector3d& v) const;
        double   operator | (const Vector3d& v) const;
		
		void     operator += (const Vector3d& v);
		void     operator -= (const Vector3d& v);
		void     operator *= (double lambda);
		void     operator /= (double lambda);

		Point3d operator + (const Point3d& p) const;
		Point3d operator - (const Point3d& p) const;

        virtual std::string toString() const;

        static Vector3d instanciateFromStream(const std::istream& is);
};

#endif

