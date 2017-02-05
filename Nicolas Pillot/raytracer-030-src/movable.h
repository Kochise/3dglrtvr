#ifndef MOVABLE_H
#define MOVABLE_H

#include "point3d.h"
#include "base3d.h"

class Movable : public Printable
{
    public :
        Point3d point;
        
        Movable(const Point3d& _point);
        virtual ~Movable();
        
        void MoveTo(double x, double y, double z);
        void MoveBy(double dx, double dy, double dz);
};

#endif

