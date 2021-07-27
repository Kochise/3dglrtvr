#ifndef SOLIDCOLOR_H
#define SOLIDCOLOR_H

#include "material.h"

class SolidColor : public Material
{
    public:
        Color color;
    
        SolidColor(const std::string& _materialname, const Color& _color);
        virtual ~SolidColor();
        
        virtual Color getColorAt(double u, double v) const;
        virtual std::string toString() const;

        static SolidColor instanciateFromStream(const std::istream& is);
};

#endif
