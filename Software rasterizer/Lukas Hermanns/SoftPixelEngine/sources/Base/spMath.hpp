/*
 * Math header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_MATH_H__
#define __SP_MATH_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionPoint2D.hpp"
#include "Base/spDimensionRect2D.hpp"
#include "Base/spDimensionLine3D.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionTriangle3D.hpp"
#include "Base/spDimensionPlane3D.hpp"
#include "Base/spMathRadianDegree.hpp"
#include "Base/spMaterialColor.hpp"

#include <math.h>
#include <time.h>
#include <stdlib.h>

#include <vector>


namespace sp
{

namespace dim
{
template <typename T> class matrix4;
typedef matrix4<f32> matrix4f;
}

namespace math
{


/*
 * ======= Some standard S.P. math constants =======
 */

const f64 FLOAT_ROUND64 = 0.00000001;   // 1.0e-8
const f32 FLOAT_ROUND   = 0.000001f;    // 1.0e-6

const f64 PI64      = 3.1415926535897932384626433832795028841971693993751;
const f32 PI        = 3.14159265359;

const f32 DEG       = PI / 180.0f;
const f32 RAD       = 180.0f / PI;
const f64 DEG64     = PI64 / 180.0;
const f64 RAD64     = 180.0 / PI64;

const f64 SQRT2     = sqrt(2.0);

const f64 STDASPECT = 4.0 / 3.0;

#define SIN(x)  sin((x)*M_PI/180.0)
#define COS(x)  cos((x)*M_PI/180.0)
#define TAN(x)  tan((x)*M_PI/180.0)
#define ASIN(x) asin(x)*180.0/M_PI
#define ACOS(x) acos(x)*180.0/M_PI
#define ATAN(x) atan(x)*180.0/M_PI


/*
 * ======= All the mathematical template inline functions which compute fast complexe equations and algorithms =======
 */

//! \return Absolute value. i.e. the return value is always positive.
template <typename T> inline T Abs(const T Value)
{
    return Value < 0 ? -Value : Value;
}

//! \return Largest value of the specified variables.
template <typename T> inline T Max(const T A, const T B)
{
    return A > B ? A : B;
}

//! \return Smallest value of the specified variables.
template <typename T> inline T Min(const T A, const T B)
{
    return A < B ? A : B;
}

//! \return Largest value of the specified variables.
template <typename T> inline T Max(const T A, const T B, const T C)
{
    return ( A >= B && A >= C ? A : ( B >= A && B >= C ? B : C ) );
}

//! \return Smallest value of the specified variables.
template <typename T> inline T Min(const T A, const T B, const T C)
{
    return ( A <= B && A <= C ? A : ( B <= A && B <= C ? B : C ) );
}

//! \return Input value if it is inside the range "Min" and "Max. Otherwise the clamped range.
template <typename T> inline T MinMax(const T Value, const T Min, const T Max)
{
    if (Value > Max)
        return Max;
    else if (Value < Min)
        return Min;
    return Value;
}

//! Clamps the variable "Value" to the range "Min" and "Max".
template <typename T> inline void Clamp(T &Value, T Min, T Max)
{
    if (Value > Max)
        Value = Max;
    else if (Value < Min)
        Value = Min;
}

//! Exchanges (or rather swaps) the content of the specified variables A and B.
template <typename T> inline void Swap(T &A, T &B)
{
    T tmp = A; A = B; B = tmp;
}

//! \return Signed value. Resulting values can only be 1, -1 or 0.
template <typename T> inline T Sgn(const T Value)
{
    return (Value > 0) ? 1 : (Value < 0) ? -1 : 0; // Signum
}

//! \return Rounded value to the specified precision.
template <typename T> inline T Round(T Value, s32 Precision)
{
    s32 exp = static_cast<s32>(pow(10, Precision));
    return (T)((s32)(Value*exp)) / exp;
}

//! \return Square of the specified value (Value * Value).
template <typename T> inline T Pow2(const T Value)
{
    return Value*Value;
}

//! \return Sine of the specified value.
template <typename T> inline T Sin(const T Value)
{
    return sin(DEG64*Value);
}

//! \return Cosine of the specified value.
template <typename T> inline T Cos(const T Value)
{
    return cos(DEG64*Value);
}

//! \return Arcus sine of the specified value.
template <typename T> inline T ASin(const T Value)
{
    return asin(RAD64*Value);
}

//! \return Arcus cosine of the specified value.
template <typename T> inline T ACos(const T Value)
{
    return acos(RAD64*Value);
}

//! \return Logarithm with the specified base.
template <typename T> inline T Log(const T Value, const T Base = T(10))
{
    return log(Value) / log(Base);
}

//! \return Distance between two 2D points.
template <typename T> inline T getDistance(const dim::point2d<T> &PosA, const dim::point2d<T> &PosB)
{
    const T d = Pow2(PosB.X - PosA.X) + Pow2(PosB.Y - PosA.Y);
    return d > 0.0f ? (T)sqrt(d) : d;
}
template <typename T> inline T getDistance(const T X1, const T Y1, const T X2, const T Y2)
{
    const T d = Pow2(X2 - X1) + Pow2(Y2 - Y1);
    return d > 0.0f ? (T)sqrt(d) : d;
}
//! \return Distance between two 3D points.
template <typename T> inline T getDistance(const dim::vector3d<T> &PosA, const dim::vector3d<T> &PosB)
{
    const T d = Pow2(PosB.X - PosA.X) + Pow2(PosB.Y - PosA.Y) + Pow2(PosB.Z - PosA.Z);
    return d > 0.0f ? (T)sqrt(d) : d;
}
template <typename T> inline T getDistance(const T X1, const T Y1, const T Z1, const T X2, const T Y2, const T Z2)
{
    const T d = Pow2(X2 - X1) + Pow2(Y2 - Y1) + Pow2(Z2 - Z1);
    return d > 0.0f ? (T)sqrt(d) : d;
}

//! \return Suqare distance between two 2D points (used for fast distance comparisions).
template <typename T> inline T getDistanceSq(const dim::point2d<T> &PosA, const dim::point2d<T> &PosB)
{
    return Pow2(PosB.X - PosA.X) + Pow2(PosB.Y - PosA.Y);
}
template <typename T> inline T getDistanceSq(const T X1, const T Y1, const T X2, const T Y2)
{
    return Pow2(X2 - X1) + Pow2(Y2 - Y1);
}
//! \return Suqare distance between two 3D points (used for fast distance comparisions).
template <typename T> inline T getDistanceSq(const dim::vector3d<T> &PosA, const dim::vector3d<T> &PosB)
{
    return Pow2(PosB.X - PosA.X) + Pow2(PosB.Y - PosA.Y) + Pow2(PosB.Z - PosA.Z);
}
template <typename T> inline T getDistanceSq(const T X1, const T Y1, const T Z1, const T X2, const T Y2, const T Z3)
{
    return Pow2(X2 - X1) + Pow2(Y2 - Y1) + Pow2(Y2 - Y1);
}

//! \return Degree (or rather angle) between two 2D vectors.
template <typename T> inline T getDegree(const dim::point2d<T> &VecA, const dim::point2d<T> &VecB)
{
    T Degree;
    
    if (VecA != VecB)
    {
        Degree = ASIN( (VecB.X - VecA.X) / getDistance(VecA, VecB) );
        Degree = VecA.Y > VecB.Y ? (T)180 - Degree : Degree;
    }
    else
        Degree = 0;
    
    return Degree;
}
template <typename T> inline T getDegree(const T X1, const T Y1, const T X2, const T Y2)
{
    T Degree;
    
    if (X1 != X2 || Y1 != Y2)
    {
        Degree = ASIN( (X2 - X1) / getDistance(X1, Y1, X2, Y2) );
        Degree = Y1 > Y2 ? (T)180 - Degree : Degree;
    }
    else
        Degree = 0;
    
    return Degree;
}

//! \return Bezier value used for bezier curves.
template <typename T> inline T getBezierValue(const f32 t, const T &Pos1, const T &Pos2, const T &Radial1, const T &Radial2)
{
    const f32 invt    = 1.0f - t;
    const f32 invt2   = invt*invt;
    const f32 invt3   = invt2*invt;
    const f32 t2      = t*t;
    const f32 t3      = t*t*t;
    
    return Pos1*invt3 + Radial1*3*t*invt2 + Radial2*3*t2*invt + Pos2*t3;
}

//! \return Berstein value used for bezier patch generation.
template <typename T> inline T getBernsteinValue(const f32 t, const T Points[4])
{
    const f32 invt = 1.0f - t;
    
    return
        Points[0] * pow(t, 3) +
        Points[1] * (3.0 * pow(t, 2) * invt) +
        Points[2] * (3.0 * pow(invt, 2) * t) +
        Points[3] * pow(invt, 3);
}

//! \return Normal vector computed by the three specified coordinates.
template <typename T> inline dim::vector3d<T> getNormalVectorSQ(
    const dim::vector3d<T> &PosA, const dim::vector3d<T> &PosB, const dim::vector3d<T> &PosC)
{
    return (PosA - PosB).cross(PosB - PosC);
}

//! \return Normalized normal vector computed by the three specified coordinates.
template <typename T> inline dim::vector3d<T> getNormalVector(
    const dim::vector3d<T> &PosA, const dim::vector3d<T> &PosB, const dim::vector3d<T> &PosC)
{
    dim::vector3d<T> Normal = (PosA - PosB).cross(PosB - PosC);
    return Normal / dim::vector3d<T>(Normal.getLength());
}

template <typename T> void sortContainerConst(
    std::vector<T> &ObjectList, bool (*lpFuncCmp)(const T &obj1, const T &obj2))
{
    s32 changed, i;
    
    do
    {
        changed = 0;
        
        for (i = 0; i < ObjectList.size() - 1; ++i)
        {
            if (!lpFuncCmp(ObjectList[i], ObjectList[i + 1]))
            {
                changed = 1;
                Swap(ObjectList[i], ObjectList[i + 1]);
            }
        }
    }
    while (changed);
}

template <typename T> void sortContainer(
    std::vector<T> &ObjectList, bool (*lpFuncCmp)(T &obj1, T &obj2))
{
    s32 changed, i;
    
    do
    {
        changed = 0;
        
        for (i = 0; i < ObjectList.size() - 1; ++i)
        {
            if (!lpFuncCmp(ObjectList[i], ObjectList[i + 1]))
            {
                changed = 1;
                Swap(ObjectList[i], ObjectList[i + 1]);
            }
        }
    }
    while (changed);
}


/* === Vertex functions === */

SP_EXPORT void getVertexInterpolation(
    const dim::triangle3df &Triangle, const dim::vector3df &Pos, f32 &Vert, f32 &Horz
);

template <class T> T getVertexInterpolation(
    const T &VertexA, const T &VertexB, const T &VertexC, const f32 Vert, const f32 Horz)
{
    return VertexA + (VertexB - VertexA) * Vert + (VertexC - VertexA) * Horz;
}


/* === Other distance and vector functions === */

SP_EXPORT dim::matrix4f getTangentSpace(
    const dim::vector3df PosA, const dim::vector3df PosB, const dim::vector3df PosC,
    const dim::point2df MapA, const dim::point2df MapB, const dim::point2df MapC,
    dim::vector3df &Tangent, dim::vector3df &Binormal, dim::vector3df &Normal
);


/* === Other math functions === */

//! Rounds the float value to the nearest integer value (e.g. 3.4 to 3 and 3.5 to 4).
inline s32 Round(const f32 Value)
{
    return static_cast<s32>(Value + 0.5f);
}

//! Rounds the given value to the nearest power of two value (e.g. 34 to 32 and 120 to 128).
inline s32 RoundPow2(const s32 Value)
{
    s32 i;
    
    for (i = 1; i < Value; i <<= 1);
    
    if (i - Value <= Value - i/2)
        return i;
    
    return i/2;
}

//! \return True if A and B are equal with the specified tolerance.
inline bool Equal(const f32 A, const f32 B, const f32 Tolerance = FLOAT_ROUND)
{
    return (A + Tolerance > B && A - Tolerance < B);
}

//! \return Color in form of a 4D vector. The color components R, G, B, and A are transformed to the range [0.0, 1.0].
inline dim::vector4df Convert(const video::color &Color)
{
    return dim::vector4df(
        static_cast<f32>(Color.Red  ) / 255,
        static_cast<f32>(Color.Green) / 255,
        static_cast<f32>(Color.Blue ) / 255,
        static_cast<f32>(Color.Alpha) / 255
    );
}

//! \return Color in typical form. The color components R, G, B, and A are transformed to the range [0, 255].
inline video::color Convert(const dim::vector4df &Color)
{
    return video::color(
        static_cast<u8>(Color.X * 255),
        static_cast<u8>(Color.Y * 255),
        static_cast<u8>(Color.Z * 255),
        static_cast<u8>(Color.W * 255)
    );
}

inline bool getBit(u32 &Integer, s32 Pos)
{
    return (Integer >> Pos) & 0x00000001;
}
inline void setBit(u32 &Integer, s32 Pos, bool Enable)
{
    if (Enable)
        Integer |= (0x00000001 << Pos);
    else
        Integer &= ((0xFFFFFFFE << Pos) + (0x7FFFFFFF << (Pos - 31)));
}

inline bool getBit(u8 &Integer, s32 Pos)
{
    return (Integer >> Pos) & 0x01;
}
inline void setBit(u8 &Integer, s32 Pos, bool Enable)
{
    if (Enable)
        Integer |= (0x01 << Pos);
    else
        Integer &= ((0xFE << Pos) + (0x7F << (Pos - 31)));
}


} // /namespace math

} // /namespace sp


#endif



// ================================================================================
