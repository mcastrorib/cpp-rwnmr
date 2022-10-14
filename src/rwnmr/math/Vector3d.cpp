// include C++ standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// include header file
#include "Vector3d.h"

using namespace std;

// Vector3d methods
// default constructor
Vector3d::Vector3d() : x(0.0), y(0.0), z(0.0)
{}

Vector3d::Vector3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
{}

// copy constructor
Vector3d::Vector3d(const Vector3d &_otherVec3)
{
    // image attributes
    (*this).setX(_otherVec3.getX());
    (*this).setY(_otherVec3.getY());
    (*this).setZ(_otherVec3.getZ());
}

void Vector3d::printInfo()
{
    cout << "{" << (*this).getX();
    cout << ", " << (*this).getY();
    cout << ", " << (*this).getZ();
    cout << "}" << endl;
}

string Vector3d::getInfo()
{
    string info;
    info = "{";
    info = info + std::to_string(this->x) + ", ";
    info = info + std::to_string(this->y) + ", ";
    info = info + std::to_string(this->z) + "}\n";

    return info;
}

// LA methods

void Vector3d::addVector(Vector3d _vec)
{
    (*this).setX((*this).getX() + _vec.getX());
    (*this).setY((*this).getY() + _vec.getY());
    (*this).setZ((*this).getZ() + _vec.getZ());

    return;
}

void Vector3d::subtractVector(Vector3d _vec)
{

    (*this).setX((*this).getX() - _vec.getX());
    (*this).setY((*this).getY() - _vec.getY());
    (*this).setZ((*this).getZ() - _vec.getZ());
    
    return;
}

void Vector3d::multiplyVector(double _a)
{

    (*this).setX(_a*(*this).getX());
    (*this).setY(_a*(*this).getY());
    (*this).setZ(_a*(*this).getZ());
    
    return;
}

double Vector3d::dotProduct(Vector3d _vec)
{
    return ((*this).getX() * _vec.getX() + 
            (*this).getY() * _vec.getY() + 
            (*this).getZ() * _vec.getZ());
}

void Vector3d::crossProduct(Vector3d _vec)
{
    Vector3d tempVec;
    tempVec.setX((*this).getY()*_vec.getZ() - (*this).getZ()*_vec.getY());
    tempVec.setY((*this).getZ()*_vec.getX() - (*this).getX()*_vec.getZ());
    tempVec.setZ((*this).getX()*_vec.getY() - (*this).getY()*_vec.getX());

    (*this).setX(tempVec.getX());
    (*this).setY(tempVec.getY());
    (*this).setZ(tempVec.getZ());
    
    return;
}
