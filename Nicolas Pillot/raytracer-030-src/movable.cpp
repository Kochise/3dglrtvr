
#include "movable.h"

Movable::Movable(const Point3d& _point)
: point(_point)
{
}

Movable::~Movable()
{
}

void Movable::MoveTo(double x, double y, double z)
{
    point.MoveTo(x,y,z);
}

void Movable::MoveBy(double dx, double dy, double dz)
{
    point.MoveBy(dx,dy,dz);
}

