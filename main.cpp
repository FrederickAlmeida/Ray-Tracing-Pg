#include <iostream>
#include <tuple>
#include "camera.hpp"
using namespace std;

extern vec3 ambientLight;
extern std::vector<Light> lights;
extern std::vector<Material> objects;

int main(){
    Camera* camera = nullptr;
    int vres, hres;
    double f;
    int max_depth;
    while(cin){
        char input;
        cin >> input;

        if (input == 'c'){
            vec3 up, c, m;
            cin >> hres >> vres >> f >> up >> c >> m >> max_depth;
            camera = new Camera(c, m , up);
        }
        else if (input == 's'){
            vec3 o, center;
            double radius;
            double kd, ks, ka, kr, kt, ior;
            int eta;
            cin >> center >> radius >> o >> kd >> ks >> ka >> kr >> kt >> eta >> ior;
            objects.emplace_back(new Sphere(center, radius), o, ka, kd, ks, kr, kt, eta ,ior, false);
        }
        else if  (input == 'p'){
            vec3 p0, n, o;
            double kd, ks, ka, kr, kt, ior;
            int eta;
            cin >> p0 >> n >> o >> kd >> ks >> ka >> kr >> kt >> eta >> ior;
            objects.emplace_back(new Plane(p0, n), o, ka, kd, ks, kr, kt, eta, ior, false);
        }
        else if  (input == 'i'){
            vec3 p0, n, o;
            double kd, ks, ka, kr, kt, ior;
            int eta;
            std::string imagePath;
            cin >> p0 >> n >> o >> kd >> ks >> ka >> kr >> kt >> eta >> ior >> imagePath;
            objects.emplace_back(new Plane(p0, n), o, ka, kd, ks, kr, kt, eta, ior, true, imagePath);
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
            double kd, ks, ka, kr, kt, ior;
            int eta;
            cin >> o >> kd >> ks >> ka >> kr >> kt >> eta >> ior;
            for (auto [i,j,k] : faces){
                objects.emplace_back(new Triangle(verticesList[i], verticesList[j], verticesList[k]), o, ka, kd, ks, kr, kt, eta, ior, false);
            }
        }
        else if (input == 'l') {
            Light light;
            cin >> light.position >> light.intensity;
            light.intensity = light.intensity/255.0;
            lights.push_back(light);
        }
        else if (input == 'a') {
            cin >> ambientLight;
            ambientLight = ambientLight/255.0;
        }
        else if (input == 'e'){break;}
    }
    camera->render(f, vres, hres, max_depth);
    return 0;
}