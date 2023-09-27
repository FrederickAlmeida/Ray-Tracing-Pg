#include "transformations.hpp"
#include <iostream>
#include <vector>
#include <limits>

// Definição de um raio com origem e direção
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray (const vec3& origin, const vec3& direction) : origin(origin + direction * 1e-5), direction(direction) {}

    // Função para calcular um ponto ao longo do raio a partir de um parâmetro t
    vec3 pointAtParameter(float t) const {
        return origin + direction * t;
    }
};

struct Light {
    vec3 position;
    vec3 intensity;

    Light () : position(vec3(0, 0, 0)), intensity(vec3(0, 0, 0)) {}

    Light (const vec3& position, const vec3& intensity) : position(position), intensity(intensity) {}
};

vec3 ambientLight = vec3(0, 0, 0);
std::vector<Light> lights;

// Classe abstracta para representar formas geométricas
struct Shape {
    vec3 color;
    float ka, kd, ks;
    int eta;

    Shape (const vec3 &color, float ka, float kd, float ks, int eta) : color(color), ka(ka), kd(kd), ks(ks), eta(eta) {}

    virtual bool intersect(const Ray& ray, float& t) {
        return {};
    }

    virtual void applyMatrix(const Matrix& m) {
        return;
    }

    virtual vec3 normalAt(const vec3& point) {
        return {};
    }

    vec3 shade (const vec3 &point, const vec3 &view, const vec3 &normal);

};

std::vector<Shape*> shapes;

Shape* nearest (const Ray& ray, float& t_min) {
    Shape* hit = nullptr;
    for (Shape* shape: shapes){
        float t;
        if (shape->intersect(ray, t) && (!hit || t < t_min)){
            t_min = t;
            hit = shape;
        }
    }
    return hit;
}

vec3 rayCast (const Ray& ray) {
    // precisa inicializar?
    float t_min;
    Shape* hit = nearest(ray, t_min);

    if (hit) {
        vec3 point = ray.pointAtParameter(t_min);
        return hit->shade(point, ray.direction*-1, hit->normalAt(point));
    } else {
        return vec3(0,0,0);
    }
}

vec3 Shape::shade (const vec3 &point, const vec3 &view, const vec3 &normal) {
    vec3 color = ambientLight * ka * this->color;
    for (Light light : lights) {
        vec3 lightDirection = unit_vector(light.position - point);
        vec3 r = normal*2.0*(dot(normal, lightDirection)) - lightDirection;

        float t;
        auto shadow = nearest(Ray(point, lightDirection), t);

        if (shadow == nullptr || dot(lightDirection, light.position - point) < t) {
            float dotdiff = dot(lightDirection, normal);
            if (dotdiff > 0) {
                color += light.intensity * kd * dotdiff * this->color;
            }

            float dotspec = dot(r, view);
            if (dotspec > 0) {
                color += light.intensity * ks * pow(dotspec, eta);
            }
        }
    }
    return color;
}