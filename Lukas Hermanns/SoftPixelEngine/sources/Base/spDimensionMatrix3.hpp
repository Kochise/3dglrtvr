/*
 * 3x3 Matrix header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_MATRIX3_H__
#define __SP_DIMENSION_MATRIX3_H__


#include "Base/spStandard.hpp"
#include "Base/spMath.hpp"

#include <string.h>


namespace sp
{
namespace dim
{


template <typename T> class matrix3
{
    
    public:
        
        /* === Constructors === */
        
        matrix3()
        {
            reset();
        }
        matrix3(const T (&other)[9])
        {
            *this = other;
        }
        matrix3(const matrix3<T> &other)
        {
            *this = other;
        }
        matrix3(
            T m1n1, T m2n1, T m3n1,
            T m1n2, T m2n2, T m3n2,
            T m1n3, T m2n3, T m3n3)
        {
            M[0] = m1n1; M[3] = m2n1; M[6] = m3n1;
            M[1] = m1n2; M[4] = m2n2; M[7] = m3n2;
            M[2] = m1n3; M[5] = m2n3; M[8] = m3n3;
        }
        ~matrix3()
        {
        }
        
        /* === Operators === */
        
        inline const T& operator () (s32 row, s32 col) const
        {
            return M[ (row * 3) + col ];
        }
        inline T& operator () (s32 row, s32 col)
        {
            return M[ (row * 3) + col ];
        }
        
        inline const T operator [] (u32 i) const
        {
            return i < 9 ? M[i] : (T)0;
        }
        inline T& operator [] (u32 i)
        {
            return M[i];
        }
        
        inline bool operator == (const matrix3<T> &other)
        {
            for (s32 i = 0; i < 9; ++i)
            {
                if (M[i] != other.M[i])
                    return false;
            }
            return true;
        }
        inline bool operator != (const matrix3<T> &other)
        {
            for (s32 i = 0; i < 9; ++i)
            {
                if (M[i] != other.M[i])
                    return true;
            }
            return false;
        }
        
        inline matrix3<T>& operator = (const T (&other)[9])
        {
            M[0] = other[0]; M[3] = other[3]; M[6] = other[6];
            M[1] = other[1]; M[4] = other[4]; M[7] = other[7];
            M[2] = other[2]; M[5] = other[5]; M[8] = other[8];
            return *this;
        }
        inline matrix3<T>& operator = (const matrix3<T> &other)
        {
            M[0] = other[0]; M[3] = other[3]; M[6] = other[6];
            M[1] = other[1]; M[4] = other[4]; M[7] = other[7];
            M[2] = other[2]; M[5] = other[5]; M[8] = other[8];
            return *this;
        }
        inline matrix3<T>& operator = (T Scalar)
        {
            memset(M, Scalar, sizeof(M));
            return *this;
        }
        
        inline matrix3<T> operator + (const matrix3<T> &mltMatrix) const
        {
            matrix3<T> other;
            
            other[0] = M[0] + mltMatrix[0]; other[3] = M[3] + mltMatrix[3]; other[6] = M[6] + mltMatrix[6];
            other[1] = M[1] + mltMatrix[1]; other[4] = M[4] + mltMatrix[4]; other[7] = M[7] + mltMatrix[7];
            other[2] = M[2] + mltMatrix[2]; other[5] = M[5] + mltMatrix[5]; other[8] = M[8] + mltMatrix[8];
            
            return other;
        }
        
        inline matrix3<T>& operator += (const matrix3<T> &mltMatrix)
        {
            M[0] += mltMatrix[0]; M[3] += mltMatrix[3]; M[6] += mltMatrix[6];
            M[1] += mltMatrix[1]; M[4] += mltMatrix[4]; M[7] += mltMatrix[7];
            M[2] += mltMatrix[2]; M[5] += mltMatrix[5]; M[8] += mltMatrix[8];
            return *this;
        }
        
        inline matrix3<T> operator - (const matrix3<T> &mltMatrix) const
        {
            matrix3<T> other;
            other[0] = M[0] - mltMatrix[0]; other[3] = M[3] - mltMatrix[3]; other[6] = M[6] - mltMatrix[6];
            other[1] = M[1] - mltMatrix[1]; other[4] = M[4] - mltMatrix[4]; other[7] = M[7] - mltMatrix[7];
            other[2] = M[2] - mltMatrix[2]; other[5] = M[5] - mltMatrix[5]; other[8] = M[8] - mltMatrix[8];
            return other;
        }
        inline matrix3<T>& operator -= (const matrix3<T> &mltMatrix)
        {
            M[0] -= mltMatrix[0]; M[3] -= mltMatrix[3]; M[6] -= mltMatrix[6];
            M[1] -= mltMatrix[1]; M[4] -= mltMatrix[4]; M[7] -= mltMatrix[7];
            M[2] -= mltMatrix[2]; M[5] -= mltMatrix[5]; M[8] -= mltMatrix[8];
            return *this;
        }
        
        inline matrix3<T> operator * (const matrix3<T> &mltMatrix) const
        {
            matrix3<T> m3;
            const T* m1 = M;
            const T* m2 = mltMatrix.M;
            
            m3[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
            m3[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
            m3[2] = m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];
            
            m3[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];
            m3[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
            m3[5] = m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];
            
            m3[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
            m3[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];
            m3[8] = m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];
            
            return m3;
        }
        
        inline matrix3<T> operator * (T Scalar) const
        {
            matrix3<T> other;
            
            other[0] = M[0]*Scalar; other[3] = M[3]*Scalar; other[6] = M[6]*Scalar;
            other[1] = M[1]*Scalar; other[4] = M[4]*Scalar; other[7] = M[7]*Scalar;
            other[2] = M[2]*Scalar; other[5] = M[5]*Scalar; other[8] = M[8]*Scalar;
            
            return other;
        }
        
        inline matrix3<T>& operator *= (const matrix3<T> &mltMatrix)
        {
            T m1[16];
            memcpy(m1, M, sizeof(M));
            const T* m2 = mltMatrix.M;
            
            M[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
            M[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
            M[2] = m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];
            
            M[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];
            M[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
            M[5] = m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];
            
            M[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
            M[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];
            M[8] = m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];
            
            return *this;
        }
        
        inline matrix3<T>& operator *= (T Scalar)
        {
            M[0] *= Scalar; M[3] *= Scalar; M[6] *= Scalar;
            M[1] *= Scalar; M[4] *= Scalar; M[7] *= Scalar;
            M[2] *= Scalar; M[5] *= Scalar; M[8] *= Scalar;
            return *this;
        }
        
        /*
        (  1-1  2-1  3-1  )     ( x )
        |  1-2  2-2  3-2  |  *  | y |
        (  1-3  2-3  3-3  )     ( z )
        */
        inline vector3d<T> operator * (const vector3d<T> &Vector) const
        {
            return vector3d<T>(
                Vector.X*M[0] + Vector.Y*M[3] + Vector.Z*M[6],
                Vector.X*M[1] + Vector.Y*M[4] + Vector.Z*M[7],
                Vector.X*M[2] + Vector.Y*M[5] + Vector.Z*M[8]
            );
        }
        
        inline point2d<T> operator * (const point2d<T> &Vector) const
        {
            return point2d<T>(
                Vector.X*M[0] + Vector.Y*M[3],
                Vector.X*M[1] + Vector.Y*M[4]
            );
        }
        
        inline triangle3d<T> operator * (const triangle3d<T> &Triangle) const
        {
            return triangle3d<T>(
                *this * Triangle.PointA,
                *this * Triangle.PointB,
                *this * Triangle.PointC
            );
        }
        
        inline triangle3d<T> operator * (const triangle3d<T, vector3d<T>*> &Triangle) const
        {
            return triangle3d<T>(
                *this * (*Triangle.PointA),
                *this * (*Triangle.PointB),
                *this * (*Triangle.PointC)
            );
        }
        
        inline plane3d<T> operator * (const plane3d<T> &Plane) const
        {
            plane3d<T> NewPlane(Plane);
            
            vector3d<T> Member( *this * ( - NewPlane.Normal * NewPlane.Distance ) );
            
            vector3d<T> Origin;
            NewPlane.Normal = *this * NewPlane.Normal;
            Origin = *this * Origin;
            
            NewPlane.Normal -= Origin;
            NewPlane.Distance = - Member.dot(NewPlane.Normal);
            
            return NewPlane;
        }
        
        /* === Transformation functions === */
        
        inline vector3d<T> vecRotate(const vector3d<T> &Vector) const
        {
            return vector3d<T>(
                Vector.X*M[0] + Vector.Y*M[3] + Vector.Z*M[6],
                Vector.X*M[1] + Vector.Y*M[4] + Vector.Z*M[7],
                Vector.X*M[2] + Vector.Y*M[5] + Vector.Z*M[8]
            );
        }
        
        inline vector3d<T> vecRotateInverse(const vector3d<T> &Vector) const
        {
            return vector3d<T>(
                Vector.X*M[0] + Vector.Y*M[1] + Vector.Z*M[2],
                Vector.X*M[3] + Vector.Y*M[4] + Vector.Z*M[5],
                Vector.X*M[6] + Vector.Y*M[7] + Vector.Z*M[8]
            );
        }
        
        inline void clear()
        {
            memset(M, 0, sizeof(M));
        }
        
        /*
        (  1  0  0  )
        |  0  1  0  |
        (  0  0  1  )
        */
        inline matrix3<T>& reset() // Loads identity
        {
            M[0] = 1; M[3] = 0; M[6] = 0;
            M[1] = 0; M[4] = 1; M[7] = 0;
            M[2] = 0; M[5] = 0; M[8] = 1;
            return *this;
        }
        
        inline void multiplySingleMatrix(const T (&other)[3]) // Multiplies this matrix with a ( 1 x 3 ) matrix
        {
            T Result[3];
            
            Result[0] = M[0]*other[0] + M[3]*other[1] + M[6]*other[2];
            Result[1] = M[1]*other[0] + M[4]*other[1] + M[7]*other[2];
            Result[2] = M[2]*other[0] + M[5]*other[1] + M[8]*other[2];
            
            other[0] = Result[0]; other[1] = Result[1]; other[2] = Result[2];
        }
        
        inline void matrixLookAt(const vector3d<T> &Position, const vector3d<T> &LookAt, const vector3d<T> &upVector)
        {
            vector3d<T> ZAxis = LookAt - Position;
            ZAxis.normalize();
            
            vector3d<T> XAxis = upVector.cross(ZAxis);
            XAxis.normalize();
            
            vector3d<T> YAxis = ZAxis.cross(XAxis);
            
            M[0] = XAxis.X; M[3] = XAxis.Y; M[6] = XAxis.Z;
            M[1] = YAxis.X; M[4] = YAxis.Y; M[7] = YAxis.Z;
            M[2] = ZAxis.X; M[5] = ZAxis.Y; M[8] = ZAxis.Z;
        }
        
        inline bool getInverse(matrix3<T> &InverseMat) const
        {
            const matrix3<T> &m = *this;
            
            f32 d = ( m(0, 0) * m(1, 1) * m(2, 2) ) + ( m(0, 1) * m(1, 2) * m(2, 0) ) + ( m(0, 2) * m(1, 0) * m(2, 1) ) -
                    ( m(2, 0) * m(1, 1) * m(0, 2) ) - ( m(2, 0) * m(1, 2) * m(0, 0) ) - ( m(2, 2) * m(1, 0) * m(0, 1) );
            
            if (fabs(d) < math::FLOAT_ROUND)
                return false;
            
            d = 1.0f / d;
            
            InverseMat(0, 0) = d * ( m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1) );
            InverseMat(0, 1) = d * ( m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2) );
            InverseMat(0, 2) = d * ( m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1) );
            InverseMat(1, 0) = d * ( m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2) );
            InverseMat(1, 1) = d * ( m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0) );
            InverseMat(1, 2) = d * ( m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0) );
            InverseMat(2, 0) = d * ( m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0) );
            InverseMat(2, 1) = d * ( m(0, 1) * m(2, 0) - m(0, 0) * m(2, 1) );
            InverseMat(2, 2) = d * ( m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0) );
            
            return true;
        }
        
        inline bool setInverse()
        {
            matrix3<T> Matrix;
            
            if (getInverse(Matrix))
            {
                *this = Matrix;
                return true;
            }
            
            return false;
        }
        
        inline matrix3<T> getInverse() const
        {
            matrix3<T> Mat;
            getInverse(Mat);
            return Mat;
        }
        
        /*
        (  x  0  0  )
        |  0  y  0  |
        (  0  0  z  )
        */
        inline matrix3<T>& scale(const vector3d<T> &Vector)
        {
            matrix3<T> other;
            
            other[0] = Vector.X;
            other[4] = Vector.Y;
            other[8] = Vector.Z;
            
            return *this *= other;
        }
        
        /*
        ( xx(1-c)+c   xy(1-c)-zs  xz(1-c)+ys )
        | yx(1-c)+zs  yy(1-c)+c   yz(1-c)-xs |
        ( xz(1-c)-ys  yz(1-c)+xs  zz(1-c)+c  )
        */
        inline matrix3<T>& rotate(const T Angle, vector3df Rotation)
        {
            matrix3<T> other;
            
            /* Normalize the vector */
            Rotation.normalize();
            
            /* Parameters normalization */
            f64 x   = (f64)Rotation.X;
            f64 y   = (f64)Rotation.Y;
            f64 z   = (f64)Rotation.Z;
            f64 c   = COS(Angle);
            f64 s   = SIN(Angle);
            f64 cc  = 1.0 - c;
            
            /* Rotation */
            other[0] = x*x*cc+c;   other[3] = x*y*cc-z*s; other[6] = x*z*cc+y*s;
            other[1] = y*x*cc+z*s; other[4] = y*y*cc+c;   other[7] = y*z*cc-x*s;
            other[2] = x*z*cc-y*s; other[5] = y*z*cc+x*s; other[8] = z*z*cc+c;
            
            return *this *= other;
        }
        
        inline matrix3<T>& rotateX(const T Angle)
        {
            matrix3<T> other;
            f64 c = COS(Angle);
            f64 s = SIN(Angle);
            
            /* Rotation */
            other[0] = 1; other[4] = 0; other[ 8] =  0;
            other[1] = 0; other[5] = c; other[ 9] = -s;
            other[2] = 0; other[6] = s; other[10] =  c;
            
            return *this *= other;
        }
        
        inline matrix3<T>& rotateY(const T Angle)
        {
            matrix3<T> other;
            f64 c = COS(Angle);
            f64 s = SIN(Angle);
            
            /* Rotation */
            other[0] =  c; other[3] = 0; other[6] = s;
            other[1] =  0; other[4] = 1; other[7] = 0;
            other[2] = -s; other[5] = 0; other[8] = c;
            
            return *this *= other;
        }
        
        inline matrix3<T>& rotateZ(const T Angle)
        {
            matrix3<T> other;
            f64 c = COS(Angle);
            f64 s = SIN(Angle);
            
            /* Rotation */
            other[0] = c; other[3] = -s; other[6] = 0;
            other[1] = s; other[4] =  c; other[7] = 0;
            other[2] = 0; other[5] =  0; other[8] = 1;
            
            return *this *= other;
        }
        
        inline void rotateYXZ(const vector3df &Rotation)
        {
            rotateY(Rotation.Y);
            rotateX(Rotation.X);
            rotateZ(Rotation.Z);
        }
        
        inline void rotateZXY(const vector3df &Rotation)
        {
            rotateZ(Rotation.Z);
            rotateX(Rotation.X);
            rotateY(Rotation.Y);
        }
        
        inline void setRotation(vector3df Rotation, bool UseDegrees = true)
        {
            if (UseDegrees)
                Rotation = Rotation * M_PI / 180.0;
            
            f64 cx = cos(Rotation.X);
            f64 sx = sin(Rotation.X);
            f64 cy = cos(Rotation.Y);
            f64 sy = sin(Rotation.Y);
            f64 cz = cos(Rotation.Z);
            f64 sz = sin(Rotation.Z);
            
            f64 sxy = sx*sy;
            f64 cxy = cx*sy;
            
            M[0] = (T)(cy*cz);
            M[1] = (T)(cy*sz);
            M[2] = (T)(-sy);
            
            M[3] = (T)(sxy*cz - cx*sz);
            M[4] = (T)(sxy*sz + cx*cz);
            M[5] = (T)(sx*cy);
            
            M[6] = (T)(cxy*cz + sx*sz);
            M[7] = (T)(cxy*sz - sx*cz);
            M[8] = (T)(cx*cy);
        }
        
        inline void setInverseRotation(vector3df Rotation, bool UseDegrees = true)
        {
            if (UseDegrees)
                Rotation = Rotation * M_PI / 180.0;
            
            f64 cx = cos(Rotation.X);
            f64 sx = sin(Rotation.X);
            f64 cy = cos(Rotation.Y);
            f64 sy = sin(Rotation.Y);
            f64 cz = cos(Rotation.Z);
            f64 sz = sin(Rotation.Z);
            
            f64 sxy = sx*sy;
            f64 cxy = cx*sy;
            
            M[0] = (T)(cy*cz);
            M[3] = (T)(cy*sz);
            M[6] = (T)(-sy);
            
            M[1] = (T)(sxy*cz - cx*sz);
            M[4] = (T)(sxy*sz + cx*cz);
            M[7] = (T)(sx*cy);
            
            M[2] = (T)(cxy*cz + sx*sz);
            M[5] = (T)(cxy*sz - sx*cz);
            M[8] = (T)(cx*cy);
        }
        
        inline void setTextureRotation(const f32 Degree)
        {
            f64 c = COS(Degree);
            f64 s = SIN(Degree);
            
            M[0] = (T)c;
            M[1] = (T)s;
            M[2] = (T)( -0.5 * ( c + s) + 0.5 );
            
            M[3] = (T)(-s);
            M[4] = (T)c;
            M[5] = (T)( -0.5 * (-s + c) + 0.5 );
        }
        
        /* === Row & columns === */
        
        inline vector3d<T> getRow(s32 Position) const
        {
            switch (Position) {
                case 0:
                    return vector3d<T>(M[0], M[3], M[6]);
                case 1:
                    return vector3d<T>(M[1], M[4], M[7]);
                case 2:
                    return vector3d<T>(M[2], M[5], M[8]);
            }
            return vector3d<T>();
        }
        
        inline vector3d<T> getColumn(s32 Position) const
        {
            switch (Position) {
                case 0:
                    return vector3d<T>(M[0], M[1], M[2]);
                case 1:
                    return vector3d<T>(M[3], M[4], M[5]);
                case 2:
                    return vector3d<T>(M[6], M[7], M[8]);
            }
            return vector3d<T>();
        }
        
        inline void setScale(const vector3d<T> &Scale)
        {
            M[0] = Scale.X; M[4] = Scale.Y; M[8] = Scale.Z;
        }
        inline vector3d<T> getScale() const
        {
            return vector3d<T>(M[0], M[5], M[10]);
        }
        
        inline vector3d<T> getRotation() const
        {
            const matrix3<T> &Mat = *this;
            
            T Y = -(T)asin(Mat(0, 2));
            T C = (T)cos(Y);
            T rotx, roty, X, Z;
            
            Y *= (T)math::RAD64;
            
            if (fabs(C) > math::FLOAT_ROUND)
            {
                C       = (T)(1.0 / C);
                rotx    = Mat(2, 2) * C;
                roty    = Mat(1, 2) * C;
                X       = (T)(atan2(roty, rotx) * math::RAD64);
                rotx    = Mat(0, 0) * C;
                roty    = Mat(0, 1) * C;
                Z       = (T)(atan2(roty, rotx) * math::RAD64);
            }
            else
            {
                X       = 0;
                rotx    =  Mat(1, 1);
                roty    = -Mat(1, 0);
                Z       = atan2(roty, rotx) * math::RAD64;
            }
            
            if (X < 0) X += 360;
            if (Y < 0) Y += 360;
            if (Z < 0) Z += 360;
            
            return vector3d<T>(X, Y, Z);
        }
        
        inline matrix3<T> getTransposed() const
        {
            matrix3<T> Mat;
            getTransposed(Mat);
            return Mat;
        }
        
        inline void getTransposed(matrix3<T> &other) const
        {
            other[0] = M[0]; other[3] = M[1]; other[6] = M[2];
            other[1] = M[3]; other[4] = M[4]; other[7] = M[5];
            other[2] = M[6]; other[5] = M[7]; other[8] = M[8];
        }
        
        inline matrix3<T> interpolate(const matrix3<T> &other, f32 seek) const
        {
            matrix3<T> Mat;
            
            for (s32 i = 0; i < 9; ++i)
                Mat.M[i] = M[i] + (other.M[i] - M[i]) * seek;
            
            return Mat;
        }
        
        inline bool isIdentity() const
        {
            for (s32 i = 0, j; i < 3; ++i)
            {
                for (j = 0; j < 3; ++j)
                {
                    if ( ( i != j && !math::Equal((*this)(i, j), 0.0f) ) ||
                         ( i == j && !math::Equal((*this)(i, j), 1.0f) ) )
                    {
                        return false;
                    }
                }
            }
            
            return true;
        }
        
        inline const T* getArray() const
        {
            return M;
        }
        
        inline T* getArray()
        {
            return M;
        }
        
        template <typename B> inline matrix3<B> cast() const
        {
            B other[9];
            
            for (s32 i = 0; i < 9; ++i)
                other[i] = static_cast<B>(M[i]);
            
            return matrix3<B>(other);
        }
        
    private:
        
        /* === Members === */
        
        /*
         * The matrix memory
         * (all 3x3 matrices allocate 9 elements)
         */
        T M[9];
        
};

typedef matrix3<f32> matrix3f;
typedef matrix3<f64> matrix3d;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
