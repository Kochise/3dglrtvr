
#include "simplestrenderer.h"

SimplestRenderer::SimplestRenderer()
{
}

SimplestRenderer::~SimplestRenderer()
{
}
        
std::string SimplestRenderer::toString() const
{
    return "BasicRenderer { }";
}

Color SimplestRenderer::renderPixel(const Scene& scene, const RayTracer& tracer, const Ray& currentray) const
{
	return tracer.trace(currentray,scene);
}




