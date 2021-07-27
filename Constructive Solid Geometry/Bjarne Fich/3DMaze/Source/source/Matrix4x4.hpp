#ifndef __MATRIX4X4_HPP_
#define __MATRIX4X4_HPP_

#include "Vector3d.hpp"

typedef class MATRIX4X4 {
	public:
		union {
			float m[16];
			struct {
				float xx, xy, xz, xw;
				float yx, yy, yz, yw;
				float zx, zy, zz, zw;
				float wx, wy, wz, ww;
			};
			struct {
				float m44[4][4];
			};
		};

		MATRIX4X4(void);
		MATRIX4X4(const float *f);
		MATRIX4X4(const MATRIX4X4 &mT);

		MATRIX4X4 &operator=(const MATRIX4X4 mT);
		MATRIX4X4 operator*(const MATRIX4X4 m2);
		MATRIX4X4 &operator*=(const MATRIX4X4 mT);
		
		VECTOR3D operator*(const VECTOR3D vT);
		MATRIX4X4 operator*(const float scalar);
		MATRIX4X4 operator+(const MATRIX4X4 &mm);
		MATRIX4X4 operator-(const MATRIX4X4 &mm);
		MATRIX4X4 operator*(const MATRIX4X4 &mm);

		void LoadIdentity(void);
		void LoadRotateX(const float angle);
		void LoadRotateY(const float angle);
		void LoadRotateZ(const float angle);
		void LoadScale(const float xT, const float yT, const float zT);
		void LoadScale(const VECTOR3D vT);
		void LoadScale(const float *vT);

		inline void Transpose(void);

		void Translate(const float xT, const float yT, const float zT);
		void Translate(const VECTOR3D vT);
		void Translate(const float *vT);

		void GetEulerAngles(float &, float &, float &);
};

#endif // __MATRIX4X4_HPP_
