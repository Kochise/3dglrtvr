#ifndef __QUATERNION_HPP_
#define __QUATERNION_HPP_

#include "Vector3d.hpp"
#include "Matrix4x4.hpp"

typedef class QUATERNION {
	public:
		union {
			float q[4];
			struct {
				float w, x, y, z;
			};
		};

		QUATERNION(void);
		QUATERNION(const QUATERNION &);
		QUATERNION(float, float, float);
		QUATERNION(float, float, float, float);

		QUATERNION &Reset(void);
		QUATERNION &Set(const QUATERNION &);
		QUATERNION &Set(float, float, float);
		QUATERNION &Set(float, float, float, float);
		QUATERNION &PostMult(const QUATERNION &);
		QUATERNION &MultAndSet(const QUATERNION &, const QUATERNION &);
		QUATERNION &Normalize(void);
		QUATERNION Inversed(void);
		float DotProduct(void);
		float DotProduct(QUATERNION);
		float GetLength(void);
		void GetMatrix(MATRIX4X4 *);
		void GetInvertedMatrix(MATRIX4X4 *);
		void SetMatrix(MATRIX4X4);
		void GetAxisAngle(float &, float &, float &, float &);
		void GetDirectionVector(VECTOR3D *);
		void GetUpVector(VECTOR3D *);
		void GetRightVector(VECTOR3D *);
		void EulerToQuat(const float, const float, const float);
		void GetEulerAngles(float &, float &, float &);
		void Slerp(const QUATERNION, const QUATERNION, const float);
		void SquadSlerp(QUATERNION, QUATERNION, QUATERNION, QUATERNION, float);
		void Invert(void);
		void Mult(QUATERNION, QUATERNION);
		void Ln(void);
		void LnDif(QUATERNION, QUATERNION);
		void Exp(void);
		void InnerPoint(QUATERNION, QUATERNION, QUATERNION);
};
	
#endif // __QUATERNION_HPP_

