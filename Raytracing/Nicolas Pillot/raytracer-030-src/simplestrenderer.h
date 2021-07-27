#ifndef SIMPLESTRENDERER_H
#define SIMPLESTRENDERER_H

#include "renderer.h"

class SimplestRenderer : public Renderer
{
    public:
        SimplestRenderer();
        virtual ~SimplestRenderer();
        
        virtual Color renderPixel(const Scene& scene, const RayTracer& tracer, const Ray& currentray) const;
        virtual std::string toString() const;
};

#endif
