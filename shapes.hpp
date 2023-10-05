#include "transformations.hpp"
#include "image.hpp"

#ifndef SHAPES_HPP
#define SHAPES_HPP


const double EPS = 1e-6;

// Definição de um raio com origem e direção
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray (const vec3& origin, const vec3& direction) : origin(origin + direction * 1e-5), direction(direction) {}

    // Função para calcular um ponto ao longo do raio a partir de um parâmetro t
    vec3 pointAtParameter(double t) const {
        return origin + direction * t;
    }
};

class Shape{
public:
    Image* texture = NULL;

     // Funções virtuais puras para testar interseção com um raio, aplicar uma matriz de transformação e obter o vetor normal
    virtual bool intersect(const Ray& ray, double& t) {
        return {}; // Implementação default
    }

    virtual vec3 getNormal(const vec3& point) {
        return {}; // Implementação default retorna vetor nulo
    }

    virtual void applyTransformation(const Matrix& m) {
        return;
    }
};

// Definição de uma esfera
class Sphere : public Shape {
    vec3 center;
    double radius;

public:

    Sphere(const vec3& center, double radius) : center(center), radius(radius) {}

    // Função para testar interseção entre um raio e a esfera
    bool intersect(const Ray& ray, double& t) {
        vec3 v = center - ray.origin;
        double tca = dot(v, ray.direction);
        double d2 = dot(v, v) - tca * tca;
        double r2 = radius * radius;

        if (d2 - r2 > EPS) { // edit: comparacao com o EPS ao inves de 0
            return false;
        }

        double thc = sqrt(r2 - d2);
        double t0 = tca - thc;
        double t1 = tca + thc;
        t = t0;

        if (t0 < EPS) {
            if (t1 < EPS) {
                return false;
            }
            t = t1;
        }
        return true;
    }

    // Função para obter o vetor normal à esfera num dado ponto
    vec3 getNormal(const vec3& point) {
        return unit_vector(point - center);
    }

    void applyTransformation(const Matrix& transformationMatrix) {
        vec3 transformedCenter = transformationMatrix * center;
        center = transformedCenter;
    }
};

// Definição de um plano
class Plane : public Shape {
protected:
    vec3 pp; // Ponto pertencente ao plano
    vec3 normal; // Normal do plano
    vec3 intersection_point; // último ponto de interseção entre o raio e o parâmetro
    Image* texture = NULL; // Textura do plano

public:

    Plane(const vec3& pp, const vec3& normal) : pp(pp), normal(unit_vector(normal)) {}
    Plane(const vec3& pp, const vec3& normal, Image* texture) : pp(pp), normal(unit_vector(normal)), texture(texture) {}

    // Função para testar interseção entre um raio e o plano


     bool intersect(const Ray& ray, double& t) {
        double aux = dot(normal, ray.direction);

        if (std::abs(aux) < EPS) {
            return false;
        } else {
            t = dot(normal, pp - ray.origin) / aux;
            intersection_point = ray.origin + t * ray.direction;
            return t > EPS;
        }
    }


    // Função para obter a normal do plano
    vec3 getNormal(const vec3& point) {
        return normal;
    }

    void applyTransformation(const Matrix& transformationMatrix) {
        vec3 transformedPoint = transformationMatrix * pp;
        pp = transformedPoint;

        vec3 transformedNormal = transformationMatrix * normal;
        normal = unit_vector(transformedNormal);
    }

};

// Definição de um triângulo
class Triangle : public Plane {
    vec3 edgeVectorAB, edgeVectorAC;

public:

    Triangle(const vec3& a, const vec3& b, const vec3& c) : Plane(a, unit_vector(cross(b-a, c-a))) {


        vec3 u = b - a;
        vec3 v = c - a;
        vec3 projuv = v * (dot(u, v) / dot(v, v));
        vec3 projvu = u * (dot(u, v) / dot(u, u));
        edgeVectorAB = u - projuv;
        edgeVectorAB = edgeVectorAB / dot(edgeVectorAB, edgeVectorAB);
        edgeVectorAC = v - projvu;
        edgeVectorAC = edgeVectorAC / dot(edgeVectorAC, edgeVectorAC);

    }

    bool intersect(const Ray& ray, double& t) {

        if (!Plane::intersect(ray, t)) {

            return false;
        }
        
        vec3 p = ray.pointAtParameter(t);
        vec3 ap = p - pp; 
        double beta = dot(ap, edgeVectorAB);
        double gamma = dot(ap, edgeVectorAC);
        double alpha = 1 - (gamma + beta);



        if (alpha < -EPS || beta < -EPS || gamma < -EPS) {
            return false;
        }

        return true;
    }

    void applyTransformation(const Matrix& transformationMatrix) {
        vec3 transformedA = transformationMatrix * pp;
        vec3 transformedB = transformationMatrix * (pp + edgeVectorAB);
        vec3 transformedC = transformationMatrix * (pp + edgeVectorAC);

        pp = transformedA;
        edgeVectorAB = transformedB - transformedA;
        edgeVectorAC = transformedC - transformedA;

        normal = unit_vector(cross(edgeVectorAB, edgeVectorAC));
    }

};

#endif