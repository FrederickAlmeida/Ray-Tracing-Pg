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
            float kd, ks, ka;
            int eta;
            cin >> center >> radius >> color >> kd >> ks >> ka >> eta;
            color = color/255.0;
            objects.emplace_back(new Sphere(center, radius), color, ka, kd, ks, eta);
        }
        else if  (input == 'p'){
            vec3 p0, n, o;
            float kd, ks, ka;
            int eta;
            cin >> p0 >> n >> o >> kd >> ks >> ka >> eta;
            o = o/255.0;
            objects.emplace_back(new Plane(p0, n), o, ka, kd, ks, eta);
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
            float kd, ks, ka;
            int eta;
            cin >> o >> kd >> ks >> ka >> eta;
            o = o/255.0;
            for (auto [i,j,k] : faces){
                objects.emplace_back(new Triangle(verticesList[i], verticesList[j], verticesList[k]), o, ka, kd, ks, eta);
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
            //ambientLight = ambientLight/255.0;
        }
        else if (input == 'e'){break;}
    }
    camera->render(f, vres, hres);
    return 0;
}