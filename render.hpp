#include "shapes.hpp"
#include <iostream>
#include <vector>
#include <limits>

class Material{
    Shape* shape;
    vec3 color;
    float ka, kd, ks;
    int eta;

public:
    Material(Shape* shape, vec3 color, float ka, float kd, float ks, int eta) : shape(shape), color(color), ka(ka), kd(kd), ks(ks), eta(eta) {}

    Shape* getShape() const {
        return shape;
    }

    vec3 shade (const vec3&  point, const vec3& view);
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

vec3 ray_cast (const Ray& ray) {
    float t_min;
    Material* hit = nearest(ray, t_min);
    if (hit){
        vec3 point = ray.pointAtParameter(t_min);
        return hit->shade(point, -ray.direction);
    }
    return vec3(0, 0, 0);
}

struct Light {
    vec3 position;
    vec3 intensity;

    Light () : position(vec3(0, 0, 0)), intensity(vec3(0, 0, 0)) {}

    Light (const vec3& position, const vec3& intensity) : position(position), intensity(intensity) {}
};

vec3 ambientLight = vec3(0, 0, 0);
std::vector<Light> lights;

vec3 Material::shade (const vec3 &point, const vec3 &view) {
    vec3 normal = shape->getNormal(point);
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