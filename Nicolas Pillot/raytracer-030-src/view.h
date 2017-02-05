#ifndef VIEW_H
#define VIEW_H

#include "movable.h"
#include "vector3d.h"

class View : public Movable
{
    public:
        Point3d target;
        Vector3d upperdirection;
		double width, height;
    
        View(const Point3d& _point, const Point3d& _target, const Vector3d& _upperdirection, double _width, double _height);
        virtual ~View();

		std::string toString () const;

        static View instanciateFromStream(const std::istream& is);
};


#endif
