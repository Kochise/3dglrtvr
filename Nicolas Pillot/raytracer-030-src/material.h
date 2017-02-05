#ifndef MATERIAL_H
#define MATERIAL_H

#include "printable.h"
#include "color.h"

class Material : public Printable
{
    private:
        std::string materialname;

    public:
        Material(const std::string& _materialname);
        virtual ~Material();
        
        std::string getMaterialName() const;

        virtual Color getColorAt(double u, double v) const = 0;
};

#endif

