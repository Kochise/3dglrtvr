
#include "simplestrenderer.h"
#include "simplestraytracer.h"
#include "lightraytracer.h"
#include "diffuselight.h"
#include "sphere.h"
#include "solidcolor.h"
#include "exceptions.h"
#include "matrix3d.h"
#include "row3d.h"
#include "vector3d.h"
#include "logconsole.h"
#include <exception>

void test();


int main(int argc, char **argv)
{
    try
    {
      test();
    }
    catch (const MyException& e)
    {
        Utils::log.err(e.what());
    }
    catch (const std::exception& e)
    {
        Utils::log.err("Standard C++ Exception : " + std::string(e.what()));
    }
    catch (...)
    {
        Utils::log.err("Unhandled exception");
    }
    return 0;
}

void test()
{
	View view (Point3d(+5,0,+10), Point3d(0,0,0), Vector3d(0,1,0.1), 4,3);

	Scene scene;

	Object * o;

	Vector3d VX(1,0,0), VY(0,1,0), VZ(0,0,1);

	o = new Ellipsoid (Point3d(-0.5,0,0),
		Base3d(VZ,VX,VY),
		"bleu ciel",1.0,1,0.5,1.5);
	scene.addObject(o);

	o = new Ellipsoid (Point3d(-0.25,0,0.15),
		Base3d(VX-VZ-VY,VY-VZ,VX+VZ),
		"blanc",1.0,2,0.5,0.25);
	scene.addObject(o);

	o = new Sphere (Point3d(0,0.25,0.25), 
		Base3d(VX,VY,VZ),
		"rouge",1.0,0.5);
	scene.addObject(o);

	o = new Sphere (Point3d(-1,0,0),
		Base3d(VX,VY,VZ),
		"jaune",1.0,1);
	scene.addObject(o);

	Light * l;
	l = new DiffuseLight(Point3d(3,0,0), Color(1,1,1));
	scene.addLight(l);
	
	Material * m;
	m = new SolidColor("bleu ciel",Color(0.5,0.5,0.75));
	scene.addMaterial(m);

	m = new SolidColor("jaune", Color(1,1,0));
	scene.addMaterial(m);

	m = new SolidColor("rouge", Color(1,0,0));
	scene.addMaterial(m);

	m = new SolidColor("blanc", Color(1,1,1));
	scene.addMaterial(m);

	LightRayTracer raytracer(Color(0,0,0), Color(0.25,0.25,0.25));
	SimplestRenderer renderer;

	Image image("output.bmp",800,600);
	renderer.render(scene,view,raytracer,image);
}

