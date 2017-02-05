
#include "solidcolor.h"

SolidColor::SolidColor(const std::string& _materialname, const Color& _color)
: Material(_materialname), color(_color)
{
}
SolidColor::~SolidColor()
{
}

Color SolidColor::getColorAt(double u, double v) const
{
    // arguments are not used !
    return color;
}

std::string SolidColor::toString() const
{
    return "SolidColor { " + Utils::value2string("Color",color) + " }";
}

SolidColor SolidColor::instanciateFromStream(const std::istream& is)
{
    


	return SolidColor("solid",Color(1,1,1));
}

