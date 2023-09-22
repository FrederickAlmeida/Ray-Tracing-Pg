#ifndef POINT_AND_VECTOR_HPP
#define POINT_AND_VECTOR_HPP

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
public:
    // constructor para quando não é dado as coordenadas
    vec3() {}

    // constructor para quando é dada as coordenadas
    vec3(float x, float y, float z) {
        vetor[0] = x; 
        vetor[1] = y; 
        vetor[2] = z;
    }


    // metodos para retornar as coordenadas do vetor
    inline float x() const {return vetor[0];}
    inline float y() const {return vetor[1];}
    inline float z() const {return vetor[2];}
    inline float r() const {return vetor[0];}
    inline float g() const {return vetor[1];}
    inline float b() const {return vetor[2];}


    // retorna uma referencia constante para o vetor
    inline const vec3& operator+() const {return *this;}

    // retorna o vetor com coordenadas invertidas
    // ok
    inline vec3 operator-() const {
        return vec3(-vetor[0], -vetor[1], -vetor[2]);
    }

    // acessa o index "i" do vetor e retorna uma cópia
    // ok
    inline float operator[](int i) const {
        return vetor[i];
    }

    // acessa o index "i" do vetor e retorna uma referencia
    // ok
    inline float& operator[](int i) {
        return vetor[i];
    }


    // operacoes 
    // ok
    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    // retorna a norma do vetor
    // ok
    inline float length() const {
        return sqrt(vetor[0]*vetor[0] + vetor[1]*vetor[1] + vetor[2]*vetor[2]);
    }

    // retorna o quadrado da norma
    // ok
    inline float squared_length() const {
        return vetor[0]*vetor[0] + vetor[1]*vetor[1] + vetor[2]*vetor[2];
    }

    // normaliza o vetor
    // ok
    inline void make_unit_vector();

    float vetor[3];
};

// ok
inline std::istream& operator>>(std::istream &is, vec3 &t) {
    is >> t.vetor[0] >> t.vetor[1] >> t.vetor[2];
    return is;
}

// ok
inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    os << t.vetor[0] << " " << t.vetor[1] << " " << t.vetor[2];
    return os;
}

// ok
inline void vec3::make_unit_vector() {
    float k = (1.0 / sqrt(vetor[0]*vetor[0] + vetor[1]*vetor[1] + vetor[2]*vetor[2]));
    vetor[0] *= k;
    vetor[1] *= k;
    vetor[2] *= k;
}

// ok
inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.vetor[0]+v2.vetor[0], v1.vetor[1]+v2.vetor[1], v1.vetor[2]+v2.vetor[2]);
}

// ok
inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.vetor[0]-v2.vetor[0], v1.vetor[1]-v2.vetor[1], v1.vetor[2]-v2.vetor[2]);
}

// ok
inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.vetor[0]*v2.vetor[0], v1.vetor[1]*v2.vetor[1], v1.vetor[2]*v2.vetor[2]);
}

// ok
inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.vetor[0]/v2.vetor[0], v1.vetor[1]/v2.vetor[1], v1.vetor[2]/v2.vetor[2]);
}

// ok
inline vec3 operator*(const vec3 &v, float t) {
    return vec3(v.vetor[0]*t, v.vetor[1]*t, v.vetor[2]*t);
}

// ok
inline vec3 operator*(float t, const vec3 &v) {
    return vec3(v.vetor[0]*t, v.vetor[1]*t, v.vetor[2]*t);
}

// ok
inline vec3 operator/(float t, const vec3 &v) {
    return vec3(v.vetor[0]/t, v.vetor[1]/t, v.vetor[2]/t);
}

// ok
inline vec3 operator/(const vec3 &v, float t) {
    return vec3(v.vetor[0]/t, v.vetor[1]/t, v.vetor[2]/t);
}

// ok
inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.vetor[0]*v2.vetor[0] + v1.vetor[1]*v2.vetor[1] + v1.vetor[2]*v2.vetor[2];
}

// ok
inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3((v1.vetor[1]*v2.vetor[2] - v1.vetor[2]*v2.vetor[1]), (-(v1.vetor[0]*v2.vetor[2] - v1.vetor[2]*v2.vetor[0])), (v1.vetor[0]*v2.vetor[1] - v1.vetor[1]*v2.vetor[0]));
}

// ok
inline vec3& vec3::operator+=(const vec3 &v) {
    vetor[0] += v.vetor[0];
    vetor[1] += v.vetor[1];
    vetor[2] += v.vetor[2];
    return *this;
}

// ok
inline vec3& vec3::operator*=(const vec3 &v) {
    vetor[0] *= v.vetor[0];
    vetor[1] *= v.vetor[1];
    vetor[2] *= v.vetor[2];
    return *this;
}

// ok
inline vec3& vec3::operator-=(const vec3 &v) {
    vetor[0] -= v.vetor[0];
    vetor[1] -= v.vetor[1];
    vetor[2] -= v.vetor[2];
    return *this;
}

// ok
inline vec3& vec3::operator/=(const vec3 &v) {
    vetor[0] /= v.vetor[0];
    vetor[1] /= v.vetor[1];
    vetor[2] /= v.vetor[2];
    return *this;
}

// ok
inline vec3& vec3::operator*=(const float t) {
    vetor[0] *= t;
    vetor[1] *= t;
    vetor[2] *= t;
    return *this;
}

// ok
inline vec3& vec3::operator/=(const float t) {
    vetor[0] /= t;
    vetor[1] /= t;
    vetor[2] /= t;
    return *this;
}

// ok
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

#endif