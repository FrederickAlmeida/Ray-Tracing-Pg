#include <iostream>
#include <limits>
#include "render.hpp"
#include <cmath>
using namespace std;

#ifndef CAMERA_HPP
#define CAMERA_HPP

const float square_side = 0.5f;

class Camera{
    vec3 eye;
    vec3 u, v, w;

public:
    Camera (vec3& eye, vec3& target, vec3& up) : eye(eye) {
        w = unit_vector(eye - target);
        u = unit_vector(cross(up,w));
        v = cross(w,u);
    }

    void render(float f, int vres, int hres, int max_depth) {
        vec3 topleft = eye - w*f + (v*(vres - 1) - u*(hres - 1))*square_side/2.0;
        std::cout << "P3" << std::endl;
        std::cout << hres << ' ' << vres << std::endl;
        std::cout << 255 << std::endl;
        for(int i=0; i<vres; i++){
            for(int j=0; j<hres; j++){
                vec3 pixelPosition = topleft + (u * j - v * i) * square_side;
                vec3 pixelColor = ray_trace(Ray(eye, unit_vector(pixelPosition - eye)), max_depth);
                std::cout << pixelColor << std::endl;
            }
        }
    }



    void applyMatrix (const Matrix& m){
        eye = unit_vector(m *eye);
        u = m*u; v = m*v; w = m*w;
    }


};



#endif