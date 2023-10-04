#include "point and vector.hpp"
#include <vector>

#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP


class Matrix {
private:
    //vec3* rows;
    int n_rows;
    int n_cols;
    std::vector<std::vector<double>> data;

public:
    // inicializa a matriz com 0s

    Matrix(int rows, int cols, double init_value = 0.0) : n_rows(rows), n_cols(cols) {
        data.resize(rows, std::vector<double>(cols, init_value));
    }

    double& operator()(int row, int col) {
        return data[row][col];
    }

    const double& operator()(int row, int col) const {
        return data[row][col];
    }

    // multiplica a matriz por vetor

    vec3 operator*(const vec3& v) const {
        double x = data[0][0] * v.x() + data[0][1] * v.y() + data[0][2] * v.z() + data[0][3];
        double y = data[1][0] * v.x() + data[1][1] * v.y() + data[1][2] * v.z() + data[1][3];
        double z = data[2][0] * v.x() + data[2][1] * v.y() + data[2][2] * v.z() + data[2][3];
        return vec3(x, y, z);
    }

    // translada a matriz

    static Matrix translation(double tx, double ty, double tz) {
        Matrix result(4, 4);
        result(0, 0) = 1;
        result(1, 1) = 1;
        result(2, 2) = 1;
        result(3, 3) = 1;
        result(0, 3) = tx;
        result(1, 3) = ty;
        result(2, 3) = tz;
        return result;
    }

    // rotaciona a matriz em torno do eixo x

    static Matrix rotation_x(double angle, bool clockwise = true) {
        Matrix result(4, 4);
        result(0, 0) = 1;
        result(3, 3) = 1;
        result(1, 1) = cos(angle);
        result(1, 2) = clockwise ? sin(angle) : -sin(angle);
        result(2, 1) = clockwise ? -sin(angle) : sin(angle);
        result(2, 2) = cos(angle);
        return result;
    }

    // rotaciona a matriz em torno do eixo y

    static Matrix rotation_y(double angle, bool clockwise = true) {
        Matrix result(4, 4); 
        result(1, 1) = 1;
        result(3, 3) = 1;
        result(0, 0) = cos(angle);
        result(0, 2) = clockwise ? -sin(angle) : sin(angle);
        result(2, 0) = clockwise ? sin(angle) : -sin(angle);
        result(2, 2) = cos(angle);
        return result;
    }

    // rotaciona a matriz em torno do eixo z

    static Matrix rotation_z(double angle, bool clockwise = true) {
        Matrix result(4, 4);
        result(2, 2) = 1;
        result(3, 3) = 1;
        result(0, 0) = cos(angle);
        result(0, 1) = clockwise ? sin(angle) : -sin(angle);
        result(1, 0) = clockwise ? -sin(angle) : sin(angle);
        result(1, 1) = cos(angle);
        return result;
    }

};

vec3 afTransformation(const Matrix& m, const vec3& v){
    vec3 nv = m*vec3(v[0], v[1], v[2]);
    return {nv[0], nv[1], nv[2]};
}
#endif