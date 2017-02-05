#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include <map>
#include "printable.h"
#include "intersectiondata.h"

class Ray;
class Scene;

class Intersections : public Printable
{
    private:
		typedef std::map<double,IntersectionData> IntersectionMap;
        IntersectionMap intersections;

    public:
        Intersections(const Ray * const ray, const Scene * const scene);
        virtual ~Intersections();

		void addIntersectionData(const IntersectionData& id);
		// returns a pointer to the first visible intersection
		// or NULL if nothing has been met by the current Ray
        const IntersectionData * const getFirstVisible() const;
       
        std::string toString () const;
};

#endif
