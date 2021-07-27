#ifndef RENDERER_H
#define RENDERER_H

#include "printable.h"
#include "scene.h"
#include "view.h"
#include "raytracer.h"
#include "image.h"

class Renderer : public Printable
{
    private:
        Vector3d h,v;
        
    public:
        Renderer();
        virtual ~Renderer();
        
        void render(const Scene& scene, const View& view, const RayTracer& tracer, Image& image);
        virtual Color renderPixel(const Scene& scene, const RayTracer& tracer, const Ray& currentray) const = 0;

    protected:
        void ComputeVectors(const View& view, const Image& image);
        Vector3d getHorizontalVector() const;
        Vector3d getVerticalVector() const;
};

#endif
