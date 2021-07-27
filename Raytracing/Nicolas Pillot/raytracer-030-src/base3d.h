#ifndef BASE3D_H
#define BASE3D_H

#include "printable.h"
#include "vector3d.h"

class Base3d : public Printable
{
    private:
        Vector3d v1,v2,v3;
		void checkValidBase() const;
		void orthoNormalizeBase();

    public:
        Base3d(const Vector3d& _v1, const Vector3d& _v2, const Vector3d& _v3);
        virtual ~Base3d();
        
        void setVectors(const Vector3d& _v1, const Vector3d& _v2, const Vector3d& _v3);
        virtual std::string toString() const;

		void rotateV1(double deg);
		void rotateV2(double deg);
		void rotateV3(double deg);

		Vector3d getV1() const;
		Vector3d getV2() const;
		Vector3d getV3() const;

        static Base3d instanciateFromStream(const std::istream& is);
};

#endif

