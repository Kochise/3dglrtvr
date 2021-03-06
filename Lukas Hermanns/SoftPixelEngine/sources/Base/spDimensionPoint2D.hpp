/*
 * Point 2D header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_POINT2D_H__
#define __SP_DIMENSION_POINT2D_H__


#include "Base/spStandard.hpp"

#include <math.h>


namespace sp
{
namespace dim
{


template <typename T> class vector3d;

template <typename T> class point2d
{
    
    public:
        
        point2d() : X(0), Y(0)
        {
        }
        point2d(T Size)
        {
            X = Y = Size;
        }
        point2d(T NewX, T NewY)
        {
            X = NewX; Y = NewY;
        }
        point2d(const point2d<T> &other)
        {
            X = other.X; Y = other.Y;
        }
        point2d(const vector3d<T> &other);
        ~point2d()
        {
        }
        
        /* === Operators - comparisions === */
        
        inline bool operator == (const point2d<T> &other) const
        {
            return X == other.X && Y == other.Y;
        }
        inline bool operator != (const point2d<T> &other) const
        {
            return X != other.X || Y != other.Y;
        }
        
        inline bool operator > (const point2d<T> &other) const
        {
            return (X == other.X) ? Y > other.Y : X > other.X;
        }
        inline bool operator < (const point2d<T> &other) const
        {
            return (X == other.X) ? Y < other.Y : X < other.X;
        }
        
        inline bool operator >= (const point2d<T> &other) const
        {
            return (X == other.X) ? Y >= other.Y : X >= other.X;
        }
        inline bool operator <= (const point2d<T> &other) const
        {
            return (X == other.X) ? Y <= other.Y : X <= other.X;
        }
        
        /* === Operators - addition, subtraction, division, multiplication === */
        
        inline point2d<T> operator + (const point2d<T> &other) const
        {
            return point2d<T>(X + other.X, Y + other.Y);
        }
        inline point2d<T>& operator += (const point2d<T> &other)
        {
            X += other.X; Y += other.Y; return *this;
        }
        
        inline point2d<T> operator - (const point2d<T> &other) const
        {
            return point2d<T>(X - other.X, Y - other.Y);
        }
        inline point2d<T>& operator -= (const point2d<T> &other)
        {
            X -= other.X; Y -= other.Y; return *this;
        }
        
        inline point2d<T> operator / (const point2d<T> &other) const
        {
            return point2d<T>(X / other.X, Y / other.Y);
        }
        inline point2d<T>& operator /= (const point2d<T> &other)
        {
            X /= other.X; Y /= other.Y; return *this;
        }
        
        inline point2d<T> operator * (const point2d<T> &other) const
        {
            return point2d<T>(X * other.X, Y * other.Y);
        }
        inline point2d<T>& operator *= (const point2d<T> &other)
        {
            X *= other.X; Y *= other.Y; return *this;
        }
        
        inline point2d<T> operator / (T Size) const
        {
            return point2d<T>(X / Size, Y / Size);
        }
        inline point2d<T>& operator /= (T Size)
        {
            X /= Size; Y /= Size; return *this;
        }
        
        inline point2d<T> operator * (T Size) const
        {
            return point2d<T>(X * Size, Y * Size);
        }
        inline point2d<T>& operator *= (T Size)
        {
            X *= Size; Y *= Size; return *this;
        }
        
        inline point2d<T> operator - () const
        {
            return point2d<T>(-X, -Y);
        }
        
        /* === Additional operators === */
        
        inline const T operator [] (s32 i) const
        {
            switch (i)
            {
                case 0: return X;
                case 1: return Y;
            }
            return 0;
        }
        
        inline T& operator [] (s32 i)
        {
            return *(&X + i);
        }
        
        /* === Static functions === */
        
        static inline void make3DProjection(T &NewX, T &NewY)
        {
            NewX = static_cast<T>( f32(  NewX - __Screen_Width /2) / (__Screen_Width /2) );
            NewY = static_cast<T>( f32(- NewY + __Screen_Height/2) / (__Screen_Height/2) );
        }
        
        /* === Extra functions === */
        
        inline T dot(const point2d<T> &other) const // Dot/ Scalar product
        {
            return X*other.X + Y*other.Y;
        }

        inline T getLength() const
        {
            return static_cast<T>( sqrt( static_cast<f64>(X*X + Y*Y) ) );
        }
        inline T getLengthSq() const
        {
            return X*X + Y*Y;
        }
        
        inline void make3DProjection() // (for 2D graphics)
        {
            X = static_cast<T>( f32( X - __Screen_Width /2) / (__Screen_Width /2) );
            Y = static_cast<T>( f32(-Y + __Screen_Height/2) / (__Screen_Height/2) );
        }
        
        inline void make3DProjectionOrigin()
        {
            X = static_cast<T>( f32(X) / (__Screen_Width /2) );
            Y = static_cast<T>( f32(Y) / (__Screen_Height/2) );
        }
        
        inline void make3DFrustum(f32 Width, f32 Height) // (for picking)
        {
            const f32 aspect = Width / Height;
            const f32 stdasp = 4.0 / 3.0;
            
            X = static_cast<T>( f32( X - Width /2) / (Width/2) * aspect / stdasp );
            Y = static_cast<T>( f32(-Y + Height/2) / (Width/2) * aspect / stdasp );
        }
        inline void make3DFrustum() // (for picking)
        {
            make3DFrustum((f32)__Screen_Width, (f32)__Screen_Height);
        }
        
        inline point2d<T>& setAbs()
        {
            X = X > 0 ? X : -X;
            Y = Y > 0 ? Y : -Y;
            return *this;
        }
        inline point2d<T> getAbs() const
        {
            return point2d<T>(
                X > 0 ? X : -X,
                Y > 0 ? Y : -Y
            );
        }
        
        inline point2d<T>& normalize()
        {
            T n = X*X + Y*Y;
            
            if (n == 1 || n == 0)
                return *this;
            
            n = static_cast<T>(1.0 / sqrt(n));
            
            X *= n;
            Y *= n;
            
            return *this;
        }
        
        inline void set(T NewX, T NewY)
        {
            X = NewX; Y = NewY;
        }
        inline void get(T &NewX, T &NewY) const
        {
            NewX = X; NewY = Y;
        }
        
        inline void setLength(T Length)
        {
            normalize();
            *this *= Length;
        }
        
        inline point2d<T>& getCircleCollision(f32 ThisRadius, point2d<T> &OtherPoint, f32 OtherRadius)
        {
            f32 Distance = sqrt( (OtherPoint.X - X)*(OtherPoint.X - X) + (OtherPoint.Y - Y)*(OtherPoint.Y - Y) );
            f32 Degree = asin( (OtherPoint.X - X) / Distance )*180.0f/M_PI;
            
            if (Y < OtherPoint.Y)
                Degree = 180 - Degree;
            
            if (Distance < ThisRadius + OtherRadius) {
                OtherPoint.X = X + sin(Degree*M_PI/180) * (ThisRadius + OtherRadius);
                OtherPoint.Y = Y + cos(Degree*M_PI/180) * (ThisRadius + OtherRadius);
            }
            
            return OtherPoint;
        }
        
        inline bool isPointInsideCircle(const point2d<T> &Center, const f32 Radius) const
        {
            return (X - Center.X)*(X - Center.X) + (Y - Center.Y)*(Y - Center.Y) < Radius*Radius;
        }

        inline T getMin() const
        {
            return (X <= Y) ? X : Y;
        }
        inline T getMax() const
        {
            return (X >= Y) ? X : Y;
        }
        
        template <typename B> inline point2d<B> cast() const
        {
            return point2d<B>(static_cast<B>(X), static_cast<B>(Y));
        }
        
        /* Members */
        
        T X, Y;
        
};

typedef point2d<s32> point2di;
typedef point2d<f32> point2df;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
