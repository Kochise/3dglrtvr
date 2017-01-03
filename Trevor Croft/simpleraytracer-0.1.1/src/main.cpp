#include "RayTracer.h"
#include "Sphere.h"
#include "Triangle.h"

#include <stdlib.h>

int main( int argc, char ** argv )
{
	RayTracer* p_ray_tracer = new RayTracer();

	// BEGIN HANDLE COMMAND LINE ARGS
	if ( argc == 1 )
		p_ray_tracer->OutputFilename("default.ppm");
	if ( argc >= 2 )
	{
		p_ray_tracer->OutputFilename(argv[1]);
		p_ray_tracer->EyePoint( Point(0,0,0) );
	}
	if (argc >= 5)
		p_ray_tracer->EyePoint( Point(strtod(argv[2],NULL), strtod(argv[3],NULL), strtod(argv[4],NULL)) );
	if (argc >= 9) // specified eye point and light sources
	{
		Light* dynamic_light;
		// might want to check that (argc - 1) is a multiple of 3
		for(int i=5; i < argc; i+=4)
		{
			// make light sources dynamically
			dynamic_light = new Light(Point(strtod(argv[i],NULL),strtod(argv[i+1],NULL),strtod(argv[i+2],NULL)));
			dynamic_light->Intensity(strtod(argv[i+3],NULL));
			p_ray_tracer->AddLight(dynamic_light);
		}
	}
	// END HANDLE COMMAND LINE ARGS

	
	Light* light = NULL;
	Sphere* sphere = NULL;
	Triangle* triangle = NULL;


	//////////////////////////////////////////////////////
	// begin setup model
	p_ray_tracer->BackgroundColor(Color(0,0,0));

	light = new Light(Point(0,0,0));
	light->Intensity(0.5);
	p_ray_tracer->AddLight(light);


	sphere = new Sphere(Point(-3,0,10),2);
	sphere->ShapeColor(Color(1,1,0.2));
	p_ray_tracer->AddShape(sphere);

	sphere = new Sphere(Point(2.5,0,15),2);
	sphere->ShapeColor(Color(1,0.5,0.2));
	p_ray_tracer->AddShape(sphere);

	triangle = new Triangle(Point(-20,6,20),  Point(20,6,20), Point(0,10,10));	
	triangle->ShapeColor(Color(1,0,0));
	triangle->DiffuseReflectivity(0.9);
	p_ray_tracer->AddShape(triangle);

	triangle = new Triangle(Point(-20,-6,20),  Point(20,-6,20), Point(0,-10,10));	
	triangle->ShapeColor(Color(0,1,0));
	triangle->DiffuseReflectivity(0.9);
	p_ray_tracer->AddShape(triangle);

	// end setup model
	//////////////////////////////////////////////////////
	

	p_ray_tracer->RayTrace();
	delete p_ray_tracer;
	return 0;
}
