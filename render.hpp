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

    Material(Shape* shape, vec3 color, float ka, float kd, float ks, float kr, float kt, int eta, float ior) : 
    shape(shape), color(color), ka(ka), kd(kd), ks(ks), kr(kr), kt(kt), eta(eta), ior(ior) {}

    Shape* getShape() const {
        return shape;
    }

    vec3 shade (const vec3&  point, const vec3& view, const vec3& normal);

    vec3 refract(const vec3& view, const vec3& normal) {
        float cosi = dot(normal, view);
        vec3 n2 = normal;
        float ior2 = ior;
        if (cosi < 0) {
            n2 = -n2;
            ior2 = 1/ior2 ;
            cosi *= -1;  
        }
        float delta = 1 - (1 - cosi * cosi) / (ior2 * ior2);
        if (delta < 0) {
            throw -1;
        }
        return view / (-ior2) - n2 * (sqrt(delta) - cosi/ior2);
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

vec3 ray_trace (const Ray& ray, int ttl) {
    vec3 colour = vec3(0, 0, 0);
    float t_min;
    Material* hit = nearest(ray, t_min);
    if (hit){
        vec3 point = ray.pointAtParameter(t_min);
        vec3 view = - ray.direction;
        vec3 normal = hit->getShape()->getNormal(point);
        colour = hit->shade(point, view, normal);
        if (ttl > 0) {
            Ray reflectedRay(point, normal*(2.0*dot(normal, view)) - view);
            try{
                if (hit->kt > 0) {
                Ray refractedRay(point, hit->refract(view, normal));
                colour = colour + ray_trace(refractedRay, ttl - 1) * hit->kt;
                }
                if (hit->kr > 0) {
                colour = colour + ray_trace(reflectedRay, ttl - 1) * hit->kr;
                }
            } catch (int e) {
                colour = colour + ray_trace(reflectedRay, ttl - 1);
            }
        }
    }
    return colour;
}

struct Light {
    vec3 position;
    vec3 intensity;

    Light () : position(vec3(0, 0, 0)), intensity(vec3(0, 0, 0)) {}

    Light (const vec3& position, const vec3& intensity) : position(position), intensity(intensity) {}
};

vec3 ambientLight = vec3(0, 0, 0);
std::vector<Light> lights;

vec3 Material::shade (const vec3 &point, const vec3 &view, const vec3 &normal) {
    vec3 color = ambientLight * ka * this->color;
    for (Light light : lights) {
        vec3 lightDirection = unit_vector(light.position - point);
        vec3 r = normal*2.0*(dot(normal, lightDirection)) - lightDirection;

        float t;
        auto shadow = nearest(Ray(point, lightDirection), t);

        if (shadow == nullptr || dot(lightDirection, light.position - point) < t) {
            float dotdiff = dot(lightDirection, normal);
            if (dotdiff > 0) {
                color = color + light.intensity * kd * dotdiff * this->color;
            }

            float dotspec = dot(r, view);
            if (dotspec > 0) {
                color = color + light.intensity * ks * pow(dotspec, eta);
            }
        }
    }
    return color;
}