/*
 * Spline header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_MATH_SPLINE_H__
#define __SP_MATH_SPLINE_H__


#include "Base/spMath.hpp"
#include "Base/spInputOutput.hpp"

#include <vector>


namespace sp
{
namespace math
{


/*
 * Structures
 */

template <typename T> struct SSplinePolynom
{
    SSplinePolynom() : a(0), b(0), c(0), d(0)
    {
    }
    ~SSplinePolynom()
    {
    }
    
    /* Functions */
    inline T calc(const T &t) const
    {
        return a + b*t + c*t*t + d*t*t*t;
    }
    
    /* Members */
    T a, b, c, d;
};


/*
 * Spline class
 */        

template <typename T> class SP_EXPORT Spline
{
    
    public:
        
        Spline(f64 Expansion = 1.0)
            : PointCount_(0), Polynom_(0), Expansion_(Expansion)
        {
        }
        ~Spline()
        {
            clear();
        }
        
        /* Functions */
        
        bool create(const T* PointArray, s32 Count)
        {
            /* Check if the polynoms are already allocated */
            clear();
            
            /* Allocate new polynoms */
            PointCount_ = Count;
            Polynom_    = new SSplinePolynom<T>[PointCount_];
            
            update(PointArray, PointCount_);
            
            /* Exit the function */
            return true;
        }
        
        void update(const T* PointArray, s32 Count)
        {
            for (s32 offset = 0; offset < sizeof(T)/sizeof(f32); ++offset)
                buildPolynom(PointArray, Polynom_, offset, Expansion_);
        }
        
        void clear()
        {
            /* Reset the counter of points */
            PointCount_ = 0;
            
            /* Delete the polynoms */
            MemoryManager::deleteBuffer(Polynom_);
        }
        
        /* Inline functions */
        
        inline bool create(const std::vector<T> &PointList)
        {
            return PointList.size() ? create(&PointList[0], PointList.size()) : false;
        }
        inline void update(const std::vector<T> &PointList)
        {
            if (PointList.size())
                update(&PointList[0], PointList.size());
        }
        
        inline void setExpansion(f64 Expansion)
        {
            Expansion_ = Expansion;
        }
        inline f64 getExpansion() const
        {
            return Expansion_;
        }
        
        inline const s32 getPointCount() const
        {
            return PointCount_;
        }
        
        inline SSplinePolynom<T>& getPolynom(const u32 Index)
        {
            return Polynom_[Index];
        }
        inline SSplinePolynom<T> getPolynom(const u32 Index) const
        {
            if (Index < PointCount_)
                return Polynom_[Index];
            return SSplinePolynom<T>();
        }
        
    private:
        
        /* Functions */
        
        void buildPolynom(const T* Points, SSplinePolynom<T>* f, s32 offset, f64 e)
        {
            f64* s = new f64[PointCount_];
            f64* y = new f64[PointCount_];
            f64* v = new f64[PointCount_];
            f64* q = new f64[PointCount_];
            
            s32 i;
            
            y[0] = 3 * (Points[1][offset] - Points[0][offset]);
            
            for (i = 1; i < PointCount_ - 1; ++i)
                y[i] = 3 * (Points[i+1][offset] - Points[i-1][offset]);
            
            y[PointCount_ - 1] = 3 * (Points[PointCount_ - 1][offset] - Points[PointCount_ - 2][offset]);
            
            v[0] = 0.5;
            q[0] = 0.5 * y[0];
            
            for (i = 1; i < PointCount_ - 1; ++i)
            {
                v[i] = 1.0 / (4 - v[i - 1]);
                q[i] = e * v[i] * (y[i] - q[i - 1]);
            }
            
            q[PointCount_ - 1] = e * (1.0 / (2 - v[PointCount_ - 2])) * (y[PointCount_ - 1] - q[PointCount_ - 2]);
            
            s[PointCount_ - 1] = q[PointCount_ - 1];
            
            for (i = PointCount_ - 2; i >= 0; --i)
                s[i] = q[i] - v[i]*s[i + 1];
            
            for (i = 0; i < PointCount_ - 1; ++i)
            {
                f[i].a[offset] = Points[i][offset];
                f[i].b[offset] = s[i];
                f[i].c[offset] = 3*Points[i+1][offset] - 3*Points[i  ][offset] - 2*s[i] - s[i+1];
                f[i].d[offset] = 2*Points[i  ][offset] - 2*Points[i+1][offset] +   s[i] + s[i+1];
            }
            
            delete [] s;
            delete [] y;
            delete [] v;
            delete [] q;
        }
        
        /* Members */
        
        s32 PointCount_;
        SSplinePolynom<T>* Polynom_;
        f64 Expansion_;
        
};

typedef Spline<dim::point2df> Spline2D;
typedef Spline<dim::vector3df> Spline3D;


} // /namespace math

} // /namespace sp


#endif



// ================================================================================
