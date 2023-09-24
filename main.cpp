#include<bits/stdc++.h>
#include "shapes.hpp"
#include "camera.hpp"
using namespace std;

vector<Shape*> objectList;

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
    camera->render(f, vres, hres, objectList);
    return 0;
}