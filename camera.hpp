#include <iostream>
#include <limits>
#include "shapes.hpp"
using namespace std;

#ifndef CAMERA_HPP
#define CAMERA_HPP

const float square_side = 0.1f;

class Camera{
    vec3 eye;
    vec3 u, v, w;

public:
    Camera (vec3& eye, vec3& target, vec3& up) : eye(eye) {
        w = unit_vector(eye - target);
        u = unit_vector(cross(up,w));
        v = cross(w,u);
    }

    // void render(float f, int vres, int hres, vector<Shape*> &objectList);
    void render(float f, int vres, int hres);

    void applyMatrix (const Matrix& m);
};

// void Camera::render(float f, int vres, int hres, vector<Shape*> &objectList) {
//     vec3 topleft = eye - w*f + (v*(vres - 1) - u*(hres - 1))*square_side/2.0;
//     std::cout << "P3" << std::endl;
//     std::cout << hres << ' ' << vres << std::endl;
//     std::cout << 255 << std::endl;
//     for(int i=0; i<vres; i++){
//         for(int j=0; j<hres; j++){
//             try {
//                 vec3 pixelPosition = topleft + (u * j - v * i) * square_side;
//                 vec3 pixelColor = rayCast(Ray(eye, unit_vector(pixelPosition - eye)),objectList);
//                 std::cout << pixelColor << std::endl;
//             }catch (const std::exception& e) {
//                 std::cerr << "Error in i=" << i << " j=" << j << e.what() << std::endl;
//             }
//         }
//     }
// }

void Camera::render(float f, int vres, int hres) {
    vec3 topleft = eye - w*f + (v*(vres - 1) - u*(hres - 1))*square_side/2.0;
    std::cout << "P3" << std::endl;
    std::cout << hres << ' ' << vres << std::endl;
    std::cout << 255 << std::endl;
    for(int i=0; i<vres; i++){
        for(int j=0; j<hres; j++){
            try {
                vec3 pixelPosition = topleft + (u * j - v * i) * square_side;
                vec3 pixelColor = rayCast(Ray(eye, unit_vector(pixelPosition - eye)));
                std::cout << pixelColor << std::endl;
            }catch (const std::exception& e) {
                std::cerr << "Error in i=" << i << " j=" << j << e.what() << std::endl;
            }
        }
    }
}

void Camera::applyMatrix(const Matrix &m) {
    eye = unit_vector(m *eye);
    u = m*u; v = m*v; w = m*w;
}

#endif