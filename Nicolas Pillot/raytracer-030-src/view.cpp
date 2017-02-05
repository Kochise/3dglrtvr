
#include "view.h"

View::View(const Point3d& _point, const Point3d& _target, const Vector3d& _upperdirection, double _width, double _height)
: Movable(_point), target(_target), upperdirection(_upperdirection), width(_width), height(_height)
{
}

View::~View()
{
}

std::string View::toString () const
{
    return "View { "
        + Utils::value2string("Point", point) + ' '
		+ Utils::value2string("Target", target) + ' '
        + Utils::value2string("UpperDirection", upperdirection) + ' '
        + Utils::value2string("Width", width) + ' '
        + Utils::value2string("Height", height) + " }";
}

View View::instanciateFromStream(const std::istream& is)
{




	return View(Point3d(0,0,0), Point3d(1,1,1), Vector3d(1,0,0), 1,1);
}


