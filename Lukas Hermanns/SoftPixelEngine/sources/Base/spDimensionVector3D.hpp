/*
 * Vector 3D header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_VECTOR3D_H__
#define __SP_DIMENSION_VECTOR3D_H__


#include "Base/spStandard.hpp"

#include <math.h>


namespace sp
{
namespace dim
{


template <typename T> class vector4d;
template <typename T> class point2d;

/*
 * vector3d class
 */

template <typename T> class vector3d
{
    
    public:
        
        vector3d() : X(0), Y(0), Z(0)
        {
        }
        vector3d(T Size) : X(Size), Y(Size), Z(Size)
        {
        }
        vector3d(T VecX, T VecY, T VecZ) : X(VecX), Y(VecY), Z(VecZ)
        {
        }
        vector3d(T VecX, T VecY, T VecZ, T VecW) : X(VecX*VecW), Y(VecY*VecW), Z(VecZ*VecW)
        {
        }
        vector3d(const vector3d<T> &other) : X(other.X), Y(other.Y), Z(other.Z)
        {
        }
        vector3d(const vector4d<T> &other);
        vector3d(const point2d<T> &other);
        ~vector3d() // must not be virtual!
        {
        }
        
        /* === Operators - comparisions === */
        
        inline bool operator == (const vector3d<T> &other) const
        {
            return X == other.X && Y == other.Y && Z == other.Z;
        }
        inline bool operator != (const vector3d<T> &other) const
        {
            return X != other.X || Y != other.Y || Z != other.Z;
        }
        
        inline bool operator < (const vector3d<T> &other) const
        {
            return (X == other.X) ? ((Y == other.Y) ? Z < other.Z : Y < other.Y) : X < other.X;
        }
        inline bool operator > (const vector3d<T> &other) const
        {
            return (X == other.X) ? ((Y == other.Y) ? Z > other.Z : Y > other.Y) : X > other.X;
        }
        
        inline bool operator <= (const vector3d<T> &other) const
        {
            return (X == other.X) ? ((Y == other.Y) ? Z <= other.Z : Y <= other.Y) : X <= other.X;
        }
        inline bool operator >= (const vector3d<T> &other) const
        {
            return (X == other.X) ? ((Y == other.Y) ? Z >= other.Z : Y >= other.Y) : X >= other.X;
        }
        
        /* === Operators - addition, subtraction, division, multiplication === */
        
        inline vector3d<T>& operator ++ ()
        {
            ++X; ++Y, ++Z; return *this;
        }
        inline vector3d<T>& operator -- ()
        {
            --X; --Y, --Z; return *this;
        }
        
        inline vector3d<T> operator + (const vector3d<T> &other) const
        {
            return vector3d<T>(X + other.X, Y + other.Y, Z + other.Z);
        }
        inline vector3d<T>& operator += (const vector3d<T> &other)
        {
            X += other.X; Y += other.Y; Z += other.Z; return *this;
        }
        
        inline vector3d<T> operator - (const vector3d<T> &other) const
        {
            return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z);
        }
        inline vector3d<T>& operator -= (const vector3d<T> &other)
        {
            X -= other.X; Y -= other.Y; Z -= other.Z; return *this;
        }
        
        inline vector3d<T> operator / (const vector3d<T> &other) const
        {
            return vector3d<T>(X / other.X, Y / other.Y, Z / other.Z);
        }
        inline vector3d<T>& operator /= (const vector3d<T> &other)
        {
            X /= other.X; Y /= other.Y; Z /= other.Z; return *this;
        }
        
        inline vector3d<T> operator / (T Size) const
        {
            return *this * (1.0 / Size);
        }
        inline vector3d<T>& operator /= (T Size)
        {
            return *this *= (1.0 / Size);
        }
        
        inline vector3d<T> operator * (const vector3d<T> &other) const
        {
            return vector3d<T>(X * other.X, Y * other.Y, Z * other.Z);
        }
        inline vector3d<T>& operator *= (const vector3d<T> &other)
        {
            X *= other.X; Y *= other.Y; Z *= other.Z; return *this;
        }
        
        inline vector3d<T> operator * (T Size) const
        {
            return vector3d<T>(X * Size, Y * Size, Z * Size);
        }
        inline vector3d<T>& operator *= (T Size)
        {
            X *= Size; Y *= Size; Z *= Size; return *this;
        }
        
        inline vector3d<T> operator - () const
        {
            return vector3d<T>(-X, -Y, -Z);
        }
        
        /* === Additional operators === */
        
        inline const T operator [] (u32 i) const
        {
            return i < 3 ? *(&X + i) : static_cast<T>(0.0);
        }
        
        inline T& operator [] (u32 i)
        {
            return *(&X + i);
        }
        
        /* === Extra functions === */
        
        //! Returns the dot (or rather scalar) product between this and the given vector.
        inline T dot(const vector3d<T> &other) const
        {
            return X*other.X + Y*other.Y + Z*other.Z;
        }

        //! Returns the cross (or rather vector) product between this and the given vector.
        inline vector3d<T> cross(const vector3d<T> &other) const
        {
            return vector3d<T>(
                Y*other.Z - other.Y*Z,
                Z*other.X - other.Z*X,
                X*other.Y - other.X*Y
            );
        }
        
        //! Returns the vector's length.
        inline T getLength() const
        {
            return static_cast<T>( sqrt(X*X + Y*Y + Z*Z) );
        }
        
        //! Returns the square of the vector's length (Can be used for faster comparision between two distances).
        inline T getLengthSq() const
        {
            return X*X + Y*Y + Z*Z;
        }
        
        //! Returns the angle (in degrees) between this and the given vector.
        inline T getAngle(const vector3d<T> &other) const
        {
            return static_cast<T>(acos( dot(other) / (getLength()*other.getLength()) )*180.0/M_PI);
        }
        
        inline vector3d<T>& setInvert()
        {
            X = -X; Y = -Y; Z = -Z; return *this;
        }
        inline vector3d<T> getInvert() const
        {
            return vector3d<T>(-X, -Y, -Z);
        }
        
        inline vector3d<T>& setRound(s32 Precision)
        {
            s32 presicion = static_cast<s32>(pow(10, Precision));
            X = static_cast<T>(static_cast<s32>(X*presicion)) / presicion;
            Y = static_cast<T>(static_cast<s32>(Y*presicion)) / presicion;
            Z = static_cast<T>(static_cast<s32>(Z*presicion)) / presicion;
            return *this;
        }
        inline vector3d<T> getRound(s32 Precision) const
        {
            s32 presicion = static_cast<s32>(pow(10, Precision));
            return vector3d<T>(
                static_cast<T>(static_cast<s32>(X*presicion)) / presicion,
                static_cast<T>(static_cast<s32>(Y*presicion)) / presicion,
                static_cast<T>(static_cast<s32>(Z*presicion)) / presicion
            );
        }
        
        inline bool equal(const vector3d<T> &other, f32 Tolerance = 1.0e-6) const
        {
            return
                (X + Tolerance > other.X) && (X - Tolerance < other.X) &&
                (Y + Tolerance > other.Y) && (Y - Tolerance < other.Y) &&
                (Z + Tolerance > other.Z) && (Z - Tolerance < other.Z);
        }
        inline bool empty() const
        {
            return equal(0);
        }
        
        inline void make2DProjection()
        {
            X =   X * f32(__Screen_Width /2) + __Screen_Width /2;
            Y = - Y * f32(__Screen_Height/2) + __Screen_Height/2;
            Z = 0;
        }
        inline void make2DProjection(f32 FOV)
        {
            X =   X / Z * FOV + __Screen_Width /2;
            Y = - Y / Z * FOV + __Screen_Height/2;
        }
        
        inline vector3d<T>& setAbs()
        {
            X = X > 0 ? X : -X;
            Y = Y > 0 ? Y : -Y;
            Z = Z > 0 ? Z : -Z;
            return *this;
        }
        inline vector3d<T> getAbs() const
        {
            return vector3d<T>(
                X > 0 ? X : -X,
                Y > 0 ? Y : -Y,
                Z > 0 ? Z : -Z
            );
        }
        
        inline vector3d<T>& normalize()
        {
            T n = X*X + Y*Y + Z*Z;
            
            if (n == static_cast<T>(0.0) || n == static_cast<T>(1.0))
                return *this;
            
            n = static_cast<T>(1.0 / sqrt(n));
            
            X *= n;
            Y *= n;
            Z *= n;
            
            return *this;
        }
        inline vector3d<T>& sign()
        {
            if (X > 0) X = 1; else if (X < 0) X = -1;
            if (Y > 0) Y = 1; else if (Y < 0) Y = -1;
            if (Z > 0) Z = 1; else if (Z < 0) Z = -1;
            return *this;
        }
        
        inline void set(T NewX, T NewY, T NewZ)
        {
            X = NewX; Y = NewY; Z = NewZ;
        }
        inline void get(T &NewX, T &NewY, T &NewZ) const
        {
            NewX = X; NewY = Y; NewZ = Z;
        }
        
        inline vector3d<T>& make(T NewX, T NewY, T NewZ)
        {
            X = NewX; Y = NewY; Z = NewZ;
            return *this;
        }
        
        inline void setLength(T Length)
        {
            normalize();
            *this *= Length;
        }
        
        inline T getDistanceFromSq(const vector3d<T> &other) const
        {
            return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z).getLengthSq();
        }
        
        inline bool isBetweenPoints(const vector3d<T> &Begin, const vector3d<T> &End) const
        {
            T tmp = (End - Begin).getLengthSq();
            return getDistanceFromSq(Begin) <= tmp && getDistanceFromSq(End) <= tmp;
        }
        
        inline bool isPointInsideSphere(const vector3d<T> &Center, const f32 Radius) const
        {
            return (X - Center.X)*(X - Center.X) + (Y - Center.Y)*(Y - Center.Y) + (Z - Center.Z)*(Z - Center.Z) < Radius*Radius;
        }
        
        inline vector3d<T> getInterpolatedQuadratic(const vector3d<T> &v2, const vector3d<T> &v3, const T d) const
        {
            const T inv = static_cast<T>(1.0) - d;
            const T mul0 = inv * inv;
            const T mul1 = static_cast<T>(2.0) * d * inv;
            const T mul2 = d * d;
            
            return vector3d<T>(
                X * mul0 + v2.X * mul1 + v3.X * mul2,
                Y * mul0 + v2.Y * mul1 + v3.Y * mul2,
                Z * mul0 + v2.Z * mul1 + v3.Z * mul2
            );
        }
        
        inline vector3d<T> getRotatedAxis(T Angle, vector3d<T> Axis) const
        {
            if (Angle == static_cast<T>(0.0))
                return *this;
            
            Axis.normalize();
            
            vector3d<T> rotMatrixRow1, rotMatrixRow2, rotMatrixRow3;
            
            T sinAngle      = sin(Angle*M_PI/180.0f);
            T cosAngle      = cos(Angle*M_PI/180.0f);
            T cosAngleInv   = 1.0f - cosAngle;
            
            rotMatrixRow1.X = Axis.X*Axis.X + cosAngle*(1.0f - Axis.X*Axis.X);
            rotMatrixRow1.Y = Axis.X*Axis.Y*cosAngleInv - sinAngle*Axis.Z;
            rotMatrixRow1.Z = Axis.X*Axis.Z*cosAngleInv + sinAngle*Axis.Y;
            
            rotMatrixRow2.X = Axis.X*Axis.Y*cosAngleInv + sinAngle*Axis.Z;
            rotMatrixRow2.Y = Axis.Y*Axis.Y + cosAngle*(1.0f - Axis.Y*Axis.Y);
            rotMatrixRow2.Z = Axis.Y*Axis.Z*cosAngleInv - sinAngle*Axis.X;
            
            rotMatrixRow3.X = Axis.X*Axis.Z*cosAngleInv - sinAngle*Axis.Y;
            rotMatrixRow3.Y = Axis.Y*Axis.Z*cosAngleInv + sinAngle*Axis.X;
            rotMatrixRow3.Z = Axis.Z*Axis.Z + cosAngle*(1.0f - Axis.Z*Axis.Z);
            
            return vector3d<T>(
                dot(rotMatrixRow1),
                dot(rotMatrixRow2),
                dot(rotMatrixRow3)
            );
        }
        
        inline vector3d<T> getNormal() const
        {
            if (X > Y && X > Z)
                return vector3d<T>(Y, -X, 0);
            else if (Y > X && Y > Z)
                return vector3d<T>(0, Z, -Y);
            return vector3d<T>(-Z, 0, X);
        }

        inline T getMin() const
        {
            if (X <= Y && X <= Z) return X;
            if (Y <= X && Y <= Z) return Y;
            return Z;
        }
        inline T getMax() const
        {
            if (X >= Y && X >= Z) return X;
            if (Y >= X && Y >= Z) return Y;
            return Z;
        }
        
        template <typename B> inline vector3d<B> cast() const
        {
            return vector3d<B>(static_cast<B>(X), static_cast<B>(Y), static_cast<B>(Z));
        }
        
        /* Members */
        
        T X, Y, Z;
        
};

typedef vector3d<s32> vector3di;
typedef vector3d<f32> vector3df;


/*
 * vector4d class
 */

template <typename T> class vector4d : public vector3d<T>
{
    
    public:
        
        vector4d() : vector3d<T>(), W(1)
        {
        }
        vector4d(T Size) : vector3d<T>(Size), W(1)
        {
        }
        vector4d(T VecX, T VecY, T VecZ, T VecW = static_cast<T>(1)) : vector3d<T>(VecX, VecY, VecZ), W(VecW)
        {
        }
        vector4d(const vector3d<T> &other, T VecW = static_cast<T>(1)) : vector3d<T>(other.X, other.Y, other.Z), W(VecW)
        {
        }
        vector4d(const vector4d<T> &other) : vector3d<T>(other.X, other.Y, other.Z), W(other.W)
        {
        }
        ~vector4d()
        {
        }
        
        /* Members */
        
        T W;
        
};

template <typename T> vector3d<T>::vector3d(const vector4d<T> &other) : X(other.X), Y(other.Y), Z(other.Z)
{
}

typedef vector4d<s32> vector4di;
typedef vector4d<f32> vector4df;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
