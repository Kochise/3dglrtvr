#ifndef COLOR_H
#define COLOR_H

#include "printable.h"

class Color : public Printable
{
    private:
        double red, green, blue;
        void clipValues();
    
    public:
        Color( );
        Color( double _red, double _green, double _blue);
        virtual ~Color();

        virtual std::string toString() const;

        double getRed() const;
        double getGreen() const;
        double getBlue() const;
        void  setRed(double _red);
        void  setGreen(double _green);
        void  setBlue(double _blue);
        
        Color operator * (const double lambda) const;
        Color operator * (const Color& color) const;
        Color operator + (const Color& color) const;
        Color operator - (const Color& color) const;

        void operator *= (const double lambda);
        void operator *=(const Color& color);
        void operator += (const Color& color);
        void operator -= (const Color& color);
};

#endif

