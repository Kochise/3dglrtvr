#ifndef CONICSPOTLIGHT_H
#define CONICSPOTLIGHT_H

#include "vector3d.h"
#include "spotlight.h"

class ConicSpotLight : public SpotLight
{
    private:
        void create();

    public:
        double openingangle;
    
        ConicSpotLight(const Point3d& _point, const Color& _color, const Vector3d& _direction, double _openingangle);
        virtual ~ConicSpotLight();
        
        virtual std::string toString() const;
        virtual double getVisibilityFromPoint(const Point3d& _point, const Vector3d& _normal, const Scene* const scene) const;
        
        static ConicSpotLight instanciateFromStream(const std::istream& is);

};

#endif
