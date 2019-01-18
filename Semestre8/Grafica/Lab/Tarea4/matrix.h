#ifndef MATRIX_H
#define MATRIX_H

#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Window{
    int width;
    int height;
};


class Matrix{
public:
    Matrix(){};
    Matrix(Window window);
    Matrix operator+(Matrix b);

    bool ** matrix;
    Window size;
    GLint yMax;
    GLint yMin;
    GLint xMax;
    GLint xMin;
};


Matrix::Matrix(Window window){
    size = window;
    matrix = new bool*[window.height];
    for(int i = 0; i < window.height; i++){
        matrix[i] = new bool[window.width];
        for(int j = 0; j < window.width; j++){
            matrix[i][j] = 0;
        }
    }
}

Matrix Matrix::operator+(Matrix b){
    for(int i = this->yMax; i >= this->yMin; i--){
        int y = abs(i - this->size.height - 1);
        for(int j = this->xMin; j <= this->xMax; j++){
            if(this->matrix[y][j] == 1) b.matrix[y][j] = 1;
        }
    }
    b.xMax = max(this->xMax,b.xMax);
    b.xMin = min(this->xMin,b.xMin);
    b.yMax = max(this->yMax,b.yMax);
    b.yMin = min(this->yMin,b.yMin);
    return b;
}

#endif