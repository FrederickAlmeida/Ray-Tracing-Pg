#include "transformations.hpp"

const float EPS = 1e-6f;

int signum(float value) {
    if (std::abs(value) < EPS) {
        return 0; // The value is considered to be zero
    } else if (value < 0) {
        return -1; // The value is negative
    } else {
        return 1; // The value is positive
    }
}


struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction) {}

    vec3 point_at_parameter(float t) const {
        return origin + t*direction;
    }
};


struct Shape {
    vec3 color;
    
    Shape(const vec3& color) : color(color) {}

    virtual bool intersect (const Ray& ray, float& t) {
        return {};
    }

    virtual void applyMatrix (const Matrix& matrix) {
        return;
    }

    virtual vec3 getNormal (const vec3& point) {
        return {};
    }
};


struct Sphere : public Shape {
    vec3 center;
    float radius;

    Sphere(const vec3& color, const vec3& center, float radius) : Shape(color), center(center), radius(radius) {}

    bool intersect (const Ray& ray, float& t) {
        vec3 v = center - ray.origin;
        float tca = dot(v, ray.direction);
        float d2 = dot(v, v) - tca*tca;
        float r2 = radius*radius;

        if (signum(d2 - r2) > 0) {
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

    void applyMatrix (const Matrix& matrix) {
        center = matrix * center;
    }

    vec3 getNormal (const vec3& point) {
        return unit_vector(point - center);
    }

};

struct Plane : public Shape {
    vec3 pp;
    vec3 normal;

    Plane(const vec3& color, const vec3& pp, const vec3& normal) : Shape(color), pp(pp), normal(unit_vector(normal)) {}

    bool intersect (const Ray& ray, float& t) {
        float aux = dot(normal, ray.direction);

        if(std::abs(aux) < EPS) {
            return false;
        }
        else{
            t = dot(normal, pp - ray.origin) / aux;
            return t > EPS;
        }
    }

    vec3 getNormal (const vec3& point) {
        return normal;
    }
};