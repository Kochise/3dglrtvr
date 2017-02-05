#ifndef SURFACEDATA_H
#define SURFACEDATA_H

#include "vector3d.h"
#include "point3d.h"
#include "color.h"

class SurfaceData : public Printable
{
    public:
        Vector3d  globalnormal;
        double    u;
        double    v;
        
        SurfaceData(const Vector3d& _globalnormal, double _u, double _v);
        virtual ~SurfaceData();
        
        virtual std::string toString() const;
};

#endif
