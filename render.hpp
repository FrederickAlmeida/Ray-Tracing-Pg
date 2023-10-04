#include "shapes.hpp"
#include <iostream>
#include <vector>
#include <limits>

class Material{
    Shape* shape;
    vec3 color;
    float ka, kd, ks;
    int eta;
    float ior;

public:
    float kr, kt;

    Material (Shape* shape, const vec3& color, float ka, float kd, float ks, float kr, float kt, float ior ,int eta) :
            shape(shape), color(color), kd(kd), ks(ks), ka(ka), kr(kr), kt(kt), eta(eta), ior(ior) {}

    Shape* getShape() const {
        return shape;
    }

    vec3 shade (const vec3&  point, const vec3& view, const vec3& n);

    vec3 refract (const vec3& v, const vec3& n){
        float d = dot(n, v);
        vec3 n2 = n;
        float ior2 = ior;
        if (d < 0.0) {
            n2 = n2*-1;
            ior2 = 1.0/ior2;
            d *= -1;
        }
        float delta = 1.0 - (1.0 - d*d)/(ior2*ior2);
        if (delta < 0.0) {
            throw -1;
        }
        return v/(-ior2) - n2*(sqrt(delta) - d/ior2);
    }
};

std::vector<Material> objects;

Material* nearest (const Ray& ray, float& t_min) {
    Material* hit = nullptr;
    for (Material& material: objects){
        float t;
        if (material.getShape()->intersect(ray, t) && (!hit || t < t_min)){
            t_min = t;
            hit = &material;
        }
    }
    return hit;
}

vec3 ray_tracer (const Ray& ray, int samples) {
    vec3 cor (0,0,0);
    float t_min;
    Material* object = nearest(ray, t_min);
    if (object){
        vec3 point = ray.pointAtParameter(t_min);
        vec3 v = ray.direction*-1;
        vec3 n = object->getShape()->getNormal(point);
        cor = object->shade(point, v, n);
        if (samples > 0 ){
            Ray reflection(point, n*(2.0f * dot(n, v)) -v);
            try {
                if (object->kt > 0.0){
                    Ray refract(point, object->refract(v,n));
                    cor = cor + ray_tracer(refract, samples-1)*object->kt;
                }
                if (object->kr > 0.0){
                    cor = cor + ray_tracer(reflection, samples-1)*object->kt;
                }
            } catch (int e){
                cor = cor + ray_tracer(reflection, samples-1);
            }
        }
    }
    return cor;
}

struct Light {
    vec3 position;
    vec3 intensity;

    Light () : position(vec3(0, 0, 0)), intensity(vec3(0, 0, 0)) {}

    Light (const vec3& position, const vec3& intensity) : position(position), intensity(intensity) {}
};

vec3 ambientLight = vec3(0, 0, 0);
std::vector<Light> lights;

vec3 Material::shade (const vec3 &point, const vec3 &view, const vec3& normal) {
    vec3 clr = ambientLight * ka * this->color;
    for (Light light : lights) {
        vec3 lightDirection = unit_vector(light.position - point);
        vec3 r = normal*2.0*(dot(normal, lightDirection)) - lightDirection;

        float t;
        auto shadow = nearest(Ray(point, lightDirection), t);

        if (shadow == nullptr || dot(lightDirection, light.position - point) < t) {
            float dotdiff = dot(lightDirection, normal);
            if (dotdiff > 0) {
                clr = clr + light.intensity * kd * dotdiff * this->color;
            }

            float dotspec = dot(r, view);
            if (dotspec > 0) {
                clr = clr + light.intensity * ks * pow(dotspec, eta);
            }
        }
    }
    return color;
}