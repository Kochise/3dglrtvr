
#include "surfacedata.h"

SurfaceData::SurfaceData(const Vector3d& _globalnormal, double _u, double _v)
: globalnormal(_globalnormal), u(_u), v(_v)
{
}

SurfaceData::~SurfaceData()
{
}
        
std::string SurfaceData::toString() const
{
    return "SurfaceData { "
        + Utils::value2string("NormalVector", globalnormal) + ' '
		+ Utils::value2string("U", u) + ' '
        + Utils::value2string("V", v) + " }";
}

