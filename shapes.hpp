#include "transformations.hpp"

#ifndef SHAPES_HPP
#define SHAPES_HPP

const float EPS = 1e-6f;

// Função para determinar o sinal de um número real
int signum(float value) {
    if (std::abs(value) < EPS) {
        return 0; // O valor é zero
    } else if (value < 0) {
        return -1; // O valor é negativo
    } else {
        return 1; // O valor é positivo
    }
}

// Definição de um raio com origem e direção
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction) {}

    // Função para calcular um ponto ao longo do raio a partir de um parâmetro t
    vec3 point_at_parameter(float t) const {
        return origin + t * direction;
    }
};

// Classe abstracta para representar formas geométricas
struct Shape {
    vec3 color;

    Shape(const vec3& color) : color(color) {}

    // Funções virtuais puras para testar interseção com um raio, aplicar uma matriz de transformação e obter o vetor normal
    virtual bool intersect(const Ray& ray, float& t) {
        return false; // Implementação default retorna false
    }

    virtual void applyMatrix(const Matrix& matrix) {
        // Implementação default não faz nada
    }

    virtual vec3 getNormal(const vec3& point) {
        return {}; // Implementação default retorna vetor nulo
    }
};

// Definição de uma esfera
struct Sphere : public Shape {
    vec3 center;
    float radius;

    Sphere(const vec3& color, const vec3& center, float radius) : Shape(color), center(center), radius(radius) {}

    // Função para testar interseção entre um raio e a esfera
    bool intersect(const Ray& ray, float& t) {
        vec3 v = center - ray.origin;
        float tca = dot(v, ray.direction);
        float d2 = dot(v, v) - tca * tca;
        float r2 = radius * radius;

        if (d2 - r2 > EPS) { // edit: comparacao com o EPS ao inves de 0
            return false;
        }

        float thc = sqrt(r2 - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;
        t = t0;

        if (t0 < EPS) {
            if (t1 < EPS) {
                return false;
            }
            t = t1;
        }
        return true;
    }

    // Função para aplicar uma matriz de transformação à esfera	
    void applyMatrix(const Matrix& matrix) {
        center = matrix * center;
    }

    // Função para obter o vetor normal à esfera num dado ponto
    vec3 getNormal(const vec3& point) {
        return unit_vector(point - center);
    }
};

// Definição de um plano
struct Plane : public Shape {
    vec3 pp; // Ponto pertencente ao plano
    vec3 normal; // Normal do plano

    Plane(const vec3& color, const vec3& pp, const vec3& normal) : Shape(color), pp(pp), normal(unit_vector(normal)) {}

    // Função para testar interseção entre um raio e o plano
    bool intersect(const Ray& ray, float& t) {
        float aux = dot(normal, ray.direction);

        if (std::abs(aux) < EPS) {
            return false;
        } else {
            t = dot(normal, pp - ray.origin) / aux;
            return t > EPS;
        }
    }

    // Função para obter a normal do plano
    vec3 getNormal(const vec3& point) {
        return normal;
    }

    void applyMatrix(const Matrix& m){
        pp = m * pp;
        normal = unit_vector(m * normal);
    }
};

// Definição de um triângulo
struct Triangle : public Plane {
    vec3 edgeVectorAB, edgeVectorAC;

    Triangle(const vec3& color, const vec3& a, const vec3& b, const vec3& c) : Plane(color, a, unit_vector(cross(b-a, c-a))) {
        //std::ofstream debugLog("debug.log", std::ios_base::app); 



        vec3 u = b - a;
        vec3 v = c - a;
        vec3 projuv = v * (dot(u, v) / dot(v, v));
        vec3 projvu = u * (dot(u, v) / dot(u, u));
        edgeVectorAB = u - projuv;
        edgeVectorAB = edgeVectorAB / dot(edgeVectorAB, edgeVectorAB);
        edgeVectorAC = v - projvu;
        edgeVectorAC = edgeVectorAC / dot(edgeVectorAC, edgeVectorAC);

        //debugLog << "Triangle vertices: " << a << ", " << b << ", " << c << std::endl;
        //debugLog << "Plane point (pp): " << pp << std::endl;
        //debugLog << "Plane normal: " << normal << std::endl;
    }

    bool intersect(const Ray& ray, float& t) {
        //std::ofstream //debugLog("debug.log", std::ios_base::app);

        if (!Plane::intersect(ray, t)) {
            //debugLog << "No intersection with the plane" << std::endl;
            return false;
        }
        
        vec3 p = ray.point_at_parameter(t);
        vec3 ap = p - pp; 
        double beta = dot(ap, edgeVectorAB);
        double gamma = dot(ap, edgeVectorAC);
        double alpha = 1 - (gamma + beta);

        //debugLog << "Intersection point: " << p << std::endl;
        //debugLog << "Barycentric coordinates (alpha, beta, gamma): " << alpha << ", " << beta << ", " << gamma << std::endl;

        if (alpha < -EPS || beta < -EPS || gamma < -EPS) {
            //debugLog << "Intersection point failed barycentric test" << std::endl;
            return false;
        }

        return true;
    }


    void applyMatrix(const Matrix& m){
        Plane::applyMatrix(m);
        edgeVectorAB = m * edgeVectorAB;
        edgeVectorAC = m * edgeVectorAC;
    }
};

#endif