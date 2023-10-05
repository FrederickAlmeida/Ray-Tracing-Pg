#include "shapes.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

//std::ofstream debuglog("debug.log");

class Material{
    Shape* shape;
    vec3 color;
    double ka, kd, ks;
    int eta;
    double ior;
    bool textured;

public:
    double kr, kt;

    Material(Shape* shape, vec3 color, double ka, double kd, double ks, double kr, double kt, int eta, double ior, bool textured) : 
    shape(shape), color(color/255.0f), ka(ka), kd(kd), ks(ks), kr(kr), kt(kt), eta(eta), ior(ior), textured(textured) {}

    Shape* getShape() const {
        return shape;
    }

    vec3 shade (const vec3&  point, const vec3& view, const vec3& normal);

    vec3 refract(const vec3& view, const vec3& normal) {
        double cosi = dot(normal, view);
        vec3 n2 = normal;
        double ior2 = ior;
        if (cosi < 0.0) {
            n2 = -n2;
            ior2 = 1.0/ior2 ;
            cosi *= -1;  
        }
        double delta = 1.0 - (1.0 - cosi * cosi) / (ior2 * ior2);
        if (delta < 0.0) {
            throw -1;
        }
        return view / (-ior2) - n2 * (sqrt(delta) - cosi/ior2);
    }
};

std::vector<Material> objects;

Material* nearest (const Ray& ray, double& t_min) {
    Material* hit = nullptr;
    for (Material& material: objects){
        double t;
        bool cond1 = material.getShape()->intersect(ray, t);
        bool cond2 = (!hit || t < t_min);
        if (material.getShape()->intersect(ray, t) && (!hit || t < t_min)){
            t_min = t;
            hit = &material;
        }
    }
    return hit;
}

double dist (const vec3& a, const vec3& b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
}

vec3 ray_trace (const Ray& ray, int ttl) {
    vec3 colour = vec3(0, 0, 0);
    double t_min;
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


vec3 Material::shade (const vec3 &point, const vec3 &view, const vec3 &normal) {

    if (textured){
        const char* image_path = "textures/minions.jpg"; 
        Image texture(image_path);
        color = texture.getPixelColor(static_cast<int>(point.x()) % texture.getWidth(), static_cast<int>(point.z()) % texture.getHeight());
    }
    vec3 res_color = (ambientLight * ka) * color;

    for (Light light : lights) {

        vec3 lightDirection = unit_vector(light.position - point);
        vec3 r = normal*2.0*(dot(normal, lightDirection)) - lightDirection;

        double t;
        auto shadow = nearest(Ray(point, lightDirection), t);

        if (shadow == nullptr || dot(lightDirection, light.position - point) > t) {
            double dotdiff = dot(lightDirection, normal);
            if (dotdiff > 0) {
                res_color = res_color + light.intensity * kd * dotdiff * color;
            }

            double dotspec = dot(r, view);
            if (dotspec > 0) {
                res_color = res_color + light.intensity * ks * pow(dotspec, eta);
            }
        }
    }
        return res_color;
} 
    

