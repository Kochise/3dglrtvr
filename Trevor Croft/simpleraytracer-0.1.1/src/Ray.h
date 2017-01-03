/** Ray.h
 */

#ifndef RAY_H
#define RAY_H

#include "Vector.h"
#include "Point.h"

class Ray
{
public:
	Ray();
	Ray(const Point& rOrigin, const Vector& rDirection);
	Ray(const Point& rOrigin, const Point& rEndPoint);
	~Ray();

	void Origin(Point vPoint);
	const Point& Origin() const;

	const Vector& Direction() const;
	void Direction(Vector vDirection);

private:
	Vector mDirection;
	Point mOrigin;
};

#endif
