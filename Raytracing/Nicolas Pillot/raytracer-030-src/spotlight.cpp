
#include "spotlight.h"

SpotLight::SpotLight(const Point3d& _point, const Color& _color, const Vector3d& _direction)
: DiffuseLight(_point,_color), direction(_direction)
{
}

SpotLight::~SpotLight()
{
}
