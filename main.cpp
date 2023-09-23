#include<bits/stdc++.h>
#include "shapes.hpp"

using namespace std;

vector<Shape*> objectList;

Shape* nearest (const Ray& ray, float& t_min){
    Shape* hit = nullptr;
    for (Shape* shape: objectList){
        float t;
        if (shape->intersect(ray, t) && (!hit || t < t_min)){
            t_min = t;
            hit = shape;
        }
    }
    return hit;
}

vec3 rayCast (const Ray& ray){
    float t_min = numeric_limits<float>::max();
    Shape* hit = nearest(ray, t_min);
    if (hit) {return hit->color;}
    else {return vec3(0,0,0);}
}

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

    void render(float f, int vres, int hres);

    void applyMatrix (const Matrix& m);
};

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

int main(){
    Camera* camera = nullptr;
    int vres, hres;
    float f;
    while(cin){
        char input;
        cin >> input;

        if (input == 'c'){
            vec3 up, c, m;
            cin >> hres >> vres >> f >> up >> c >> m;
            camera = new Camera(c, m , up);
        }
        else if (input == 's'){
            vec3 color, center;
            float radius;
            cin >> center >> radius >> color;
            objectList.push_back(new Sphere(color, center, radius));
        }
        else if  (input == 'p'){
            vec3 p0, n, o;
            cin >> p0 >> n >> o;
            objectList.push_back(new Plane(o, p0, n));
        }
        else if (input == 't'){
            int qntFaces, qntVertices;
            cin >> qntFaces >> qntVertices;
            vector<vec3> verticesList;
            while (qntVertices--){
                vec3 v;
                cin >> v;
                verticesList.push_back(v);
            }

            vector<tuple<int, int, int>> faces;
            while(qntFaces--){
                int i, j, k;
                cin >> i >> j >> k;
                i--; j--; k--;
                faces.emplace_back(i,j,k);
            }

            vec3 o;
            cin >> o;
            for (auto [i,j,k] : faces){
                objectList.push_back(new Triangle(o, verticesList[i], verticesList[j], verticesList[k]));
            }
        }
        else if (input == 'e'){break;}
    }
    camera->render(f, vres, hres);
    return 0;
}