
#include "color.h"

Color::~Color()
{
}

Color::Color()
: red(0), green(0), blue(0)
{
}

Color::Color(double _red, double _green, double _blue)
{
    setRed(_red);
    setGreen(_green);
    setBlue(_blue);
}
 
std::string Color::toString() const
{
    return "Color { "
		+ Utils::value2string("Red",red) + ' '
		+ Utils::value2string("Green", green) + ' '
		+ Utils::value2string("Blue", blue) + " }";
}

double Color::getRed() const
{
    return red;
}

double Color::getGreen() const
{
    return green;
}

double Color::getBlue() const
{
    return blue;
}

void Color::setRed(double _red)
{
    red = _red;
    if (red < 0)
        red = 0;
    if (red > 1)
        red = 1;
}

void Color::setGreen(double _green)
{
    green = _green;
    if (green < 0)
        green = 0;
    if (green > 1)
        green = 1;
}

void Color::setBlue(double _blue)
{
    blue = _blue;
    if (blue < 0)
        blue = 0;
    if (blue > 1)
        blue = 1;
}

Color Color::operator * (const double lambda) const
{
    return Color(red * lambda, green * lambda, blue * lambda);
}

Color Color::operator * (const Color& color) const
{
    return Color(red * color.red, green * color.green, blue * color.blue);
}

Color Color::operator + (const Color& color) const
{
    return Color(red + color.red, green + color.green, blue + color.blue);
}

Color Color::operator - (const Color& color) const
{
    return Color(red - color.red, green - color.green, blue - color.blue);
}

void Color::operator *= (const double lambda)
{
    setRed(red * lambda);
    setGreen(green * lambda);
    setBlue(blue * lambda);
}

void Color::operator *= (const Color& color)
{
    setRed(red * color.red);
    setGreen(green * color.green);
    setBlue(blue * color.blue);
}

void Color::operator += (const Color& color)
{
    setRed(red + color.red);
    setGreen(green + color.green);
    setBlue(blue + color.blue);
}

void Color::operator -= (const Color& color)
{
    setRed(red - color.red);
    setGreen(green - color.green);
    setBlue(blue - color.blue);
}

