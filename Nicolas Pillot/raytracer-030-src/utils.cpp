
#include "utils.h"
#include <cmath>

LogConsole Utils::log(Log::LOG_ALL);

const double Utils::PI   = 3.14159265358979323846 ;
const double Utils::PI_2 = 1.57079632679489661923 ;

double Utils::deg2rad(const double& deg)
{
    return deg * 0.017453292519943295769236907684886;
}

double Utils::rad2deg(const double& rad)
{
	return rad * 57.295779513082320876798154814105;
}

double Utils::epsilon = 0.001;

void Utils::setEpsilon(double _epsilon)
{
	epsilon = fabs(_epsilon);
}
