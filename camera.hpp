#include <iostream>
#include <limits>
#include "render.hpp"
#include <cmath>
using namespace std;

#ifndef CAMERA_HPP
#define CAMERA_HPP

const double square_side = 0.5;

class Camera{
    vec3 eye;
    vec3 u, v, w;

public:
    Camera (vec3& eye, vec3& target, vec3& up) : eye(eye) {
        w = unit_vector(eye - target);
        u = unit_vector(cross(up,w));
        v = cross(w,u);
    }

    void render(double f, int vres, int hres, int max_depth) {
        vec3 topleft = eye - w*f + (v*(vres - 1) - u*(hres - 1))*square_side/2.0;
        std::cout << "P3" << std::endl;
        std::cout << hres << ' ' << vres << std::endl;
        std::cout << 255 << std::endl;
        int k =0;
        for(int i=0; i<vres; i++){
            for(int j=0; j<hres; j++){
                vec3 pixelPosition = topleft + (u * j - v * i) * square_side;
                vec3 pixelColor = ray_trace(Ray(eye, unit_vector(pixelPosition - eye)), max_depth);
                double r=pixelColor[0], g= pixelColor[1], b=pixelColor[2];
                std::cout << pixelColor << "\n";
            }
        }
    }

    void applyTransformation(const Matrix& transformationMatrix) {
        eye = transformationMatrix * eye;
        u = unit_vector(transformationMatrix * u);
        v = unit_vector(transformationMatrix * v);
        w = unit_vector(transformationMatrix * w);
    }


};



#endif