#ifndef VECTOR_3D_H_
#define VECTOR_3D_H_

// include C++ standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Vector3d
{
private:
    double x;
    double y;
    double z;

public:

    // Pore methods:
    // default constructors
    Vector3d();
    Vector3d(double _x, double _y, double _z);

    //copy constructors
    Vector3d(const Vector3d &_otherVec3);

    // default destructor
    virtual ~Vector3d(){}

    // set methods
    void setX(double _x) { this->x = _x; }
    void setY(double _y) { this->y = _y; }
    void setZ(double _z) { this->z = _z; }
    
    bool operator==(const Vector3d other){
        if(this->x!=other.x || this->y!=other.y || this->z!=other.z){
            return false;
        }
        else return true;
    }

    bool operator!=(const Vector3d other){
        if(this->x!=other.x || this->y!=other.y || this->z!=other.z){
            return true;
        }
        else return false;
    }

    // get methods
    double getX() const { return this->x; }
    double getY() const { return this->y; }
    double getZ() const { return this->z; }
    double getNorm()  
    { 
        return sqrt((*this).getX()*(*this).getX() + 
                    (*this).getY()*(*this).getY() + 
                    (*this).getZ()*(*this).getZ());; 
    }

    void printInfo();
    string getInfo();

    // LA methods
    void addVector(Vector3d _vec);
    void subtractVector(Vector3d _vec);
    void multiplyVector(double _a);
    double dotProduct(Vector3d _vec);
    void crossProduct(Vector3d _vec);



};

#endif