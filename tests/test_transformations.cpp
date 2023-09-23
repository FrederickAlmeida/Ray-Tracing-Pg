#include "../transformations.hpp"
#include "../scene.hpp"
#include <iostream>
#define M_PI       3.14159265358979323846

// oi bea bea 😎

int main() {
    // Cria um vetor
    vec3 vetor(1, 2, 3);

    // Cria uma matriz de rotação em torno do eixo x
    double angle = M_PI / 2; // 90 graus em radianos
    Matrix rotation_Matrix = Matrix::rotation_x(angle);

    // Aplica a matriz de rotação ao vetor
    vec3 rotated_vector = rotation_Matrix * vetor;

    // Imprime o vetor rotacionado
    std::cout << rotated_vector << std::endl;

    // Cria uma matriz de translação
    Matrix translation_Matrix = Matrix::translation(1, 2, 3);


    // Aplica a matriz de translação ao vetor
    vec3 translated_vector = translation_Matrix * vetor;

    // Imprime o vetor transladado
    std::cout << translated_vector << std::endl;
    std::vector<Light> v;
    // Instanciando Luz e printando
    Light light(translated_vector, rotated_vector);
    Light light1(rotated_vector, translated_vector);
    v.push_back(light);
    v.push_back(light);
    Scene scene(rotated_vector, v);
    std::cout << "Luzes e cena" << std::endl;
    std::cout << light.color << std::endl;
    std::cout << light.position << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << scene.ambient_Color << std::endl;
    std::cout << scene.light_Sources.at(1).color << std::endl;
    
    
    return 0;
}
