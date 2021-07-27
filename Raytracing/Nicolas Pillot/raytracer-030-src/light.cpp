
#include "diffuselight.h"

Light::Light(const Point3d& _point, const Color& _color)
: Movable(_point), color(_color)
{
}

Light::~Light()
{
}

