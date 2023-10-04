#include "shapes.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

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
        if (cosi < 0.0) {
            n2 = -n2;
            ior2 = 1.0/ior2 ;
            cosi *= -1;  
        }
        float delta = 1.0 - (1.0 - cosi * cosi) / (ior2 * ior2);
        if (delta < 0.0) {
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

float dist (const vec3& a, const vec3& b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
}

vec3 ray_trace (const Ray& ray, int ttl) {
    vec3 colour = vec3(0, 0, 0);
    float t_min;
    vec3 r_origin = ray.origin;
    Material* hit = nearest(ray, t_min);
    if (hit && (dist(r_origin, ray.pointAtParameter(t_min)) > 1e-6f)){
        vec3 point = ray.pointAtParameter(t_min);
        vec3 view = - ray.direction;
        vec3 normal = hit->getShape()->getNormal(point);
        colour = hit->shade(point, view, normal);
        if (ttl > 0) {
            Ray reflectedRay(point, normal*(2.0*dot(normal, view)) - view);
            try{
                if (hit->kt > 0.0) {
                Ray refractedRay(point, hit->refract(view, normal));
                colour = colour + ray_trace(refractedRay, ttl - 1) * hit->kt;
                }
                if (hit->kr > 0.0) {
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

std::ofstream debuglog("debug.log");

vec3 Material::shade (const vec3 &point, const vec3 &view, const vec3 &normal) {
    vec3 res_color = (ambientLight * ka) * color;
    // debuglog << "color: " << color << std::endl;
    // debuglog << "ambient light: " << ambientLight << std::endl;
    // debuglog << "res_color: " << res_color << std::endl;
    for (Light light : lights) {
        debuglog << "light intensity: " << light.intensity << std::endl;
        vec3 lightDirection = unit_vector(light.position - point);
        vec3 r = normal*2.0*(dot(normal, lightDirection)) - lightDirection;

        float t;
        auto shadow = nearest(Ray(point, lightDirection), t);

        if (shadow == nullptr || dot(lightDirection, light.position - point) > t) {
            float dotdiff = dot(lightDirection, normal);
            if (dotdiff > 0) {
                res_color = res_color + light.intensity * kd * dotdiff * color;
            }

            float dotspec = dot(r, view);
            if (dotspec > 0) {
                res_color = res_color + light.intensity * ks * pow(dotspec, eta);
            }
        }
    }
    // debuglog << "res_color final: " << res_color << std::endl;
    return res_color;
}