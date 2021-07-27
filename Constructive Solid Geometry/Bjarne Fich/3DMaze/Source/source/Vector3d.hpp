#ifndef __VECTOR3D_HPP_
#define __VECTOR3D_HPP_

typedef class VECTOR3D {
	public:
		union {
			float v[3];
			struct { float x, y, z; };
		};

		VECTOR3D();
		VECTOR3D(float xx, float yy, float zz);
		VECTOR3D(const float *vv);
		VECTOR3D(const VECTOR3D &vv);

		VECTOR3D &operator=(const VECTOR3D &vv);
		VECTOR3D &operator=(const float *vv);

		bool operator==(const VECTOR3D &vv);
		bool operator!=(const VECTOR3D vv);

		VECTOR3D operator-();
		VECTOR3D &operator+=(const VECTOR3D vv);
		VECTOR3D operator+(const VECTOR3D vv);
		VECTOR3D &operator-=(const VECTOR3D vv);
		VECTOR3D operator-(const VECTOR3D vv);
		VECTOR3D &operator*=(const VECTOR3D vv);
		VECTOR3D operator*(const VECTOR3D vv);
		VECTOR3D &operator/=(const VECTOR3D vv);
		VECTOR3D operator/(const VECTOR3D vv);

		VECTOR3D &operator*=(float f);
		VECTOR3D operator*(float f);
		VECTOR3D &operator/=(float f);
		VECTOR3D operator/(float f);

		bool IsNull();
		float GetLen();
		void SetLen(float f);
		VECTOR3D &Normalize();
		VECTOR3D CrossProduct(const VECTOR3D vv);
		float AngleBetween(VECTOR3D vv);
		float DotProduct(const VECTOR3D vv);

		void RotateX(float decangle);
		void RotateY(float decangle);
		void RotateZ(float decangle);
		void Rotate(float x, float y, float z);
};

#endif // __VECTOR3D_HPP_
