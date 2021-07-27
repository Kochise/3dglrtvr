
#include "object.h"
#include "matrix3d.h"

Object::Object(const Point3d& _point, const Base3d& _base, const std::string& _matname)
: Movable(_point), localbase(_base), materialname(_matname)
{
}

Object::~Object()
{
}

Ray Object::convertRayToLocal(const Ray& ray) const
{
	// convert ray origin from global to local
	Point3d localpoint = convertPoint3dToLocal( ray.point );
	// convert ray direction from global to local
	Vector3d localdirection = convertVector3dToLocal( ray.direction );
	// returns the converted ray
	return Ray (localpoint, localdirection);
}

Ray Object::convertRayToGlobal(const Ray& ray) const
{
	// convert ray origin from local to global
	Point3d globalpoint = convertPoint3dToGlobal( ray.point );
	// convert ray direction from local to global
	Vector3d globaldirection = convertVector3dToGlobal( ray.direction );
	// returns the converted ray
	return Ray (globalpoint, globaldirection);
}

Vector3d Object::convertVector3dToLocal(const Vector3d& v) const
{
	// compute base Local->Global convertion matrix
	Matrix3d m (localbase.getV1(), localbase.getV2(), localbase.getV3());
	// Invert it so it converts from global to local
	m.invert();
	// returns the converted ray
	return m * v;
}

Vector3d Object::convertVector3dToGlobal(const Vector3d& v) const
{
	// compute base Local->Global convertion matrix
	Matrix3d m (localbase.getV1(), localbase.getV2(), localbase.getV3());
	// returns the converted ray
	return m * v;
}

Point3d Object::convertPoint3dToLocal(const Point3d& p) const
{
    // create the local to point vector
    Vector3d v(point, p);
	// compute base Local->Global convertion matrix
	Matrix3d m (localbase.getV1(), localbase.getV2(), localbase.getV3());
	// Invert it so it converts from global to local
	m.invert();
	// Computes the converted vector
	Vector3d vu ( m * v );
	// returns the converted ray
	return Point3d( vu.dx, vu.dy, vu.dz );
}

Point3d Object::convertPoint3dToGlobal(const Point3d& p) const
{
    // create the local to point vector
    Vector3d v(p.x, p.y, p.z);
	// compute base Local->Global convertion matrix
	Matrix3d m (localbase.getV1(), localbase.getV2(), localbase.getV3());
	// Computes the converted vector
	Vector3d vu ( m * v );
	// returns the converted ray
	return point + vu;
}

