#ifndef INTERNAL_FIELD_H_
#define INTERNAL_FIELD_H_

#include "Defs.h"
#include "BitBlock.h"

class InternalField
{
private:
	double *data;
	int dimX;
    int dimY;
    int dimZ;
    uint rowScale;
	uint depthScale;

public:	
	InternalField(BitBlock &_bitblock, string _file);
	InternalField(BitBlock &_bitblock, double _resolution, double _gradient, int _direction);
	InternalField(const InternalField &_other);
	virtual ~InternalField()
	{
		if(this->data != NULL)
        {
            delete[] this->data;
            this->data = NULL;
        }
	}

	void setData(double *_data){ this->data = _data; };
	void setDimX(int _x){if(_x > 0)	this->dimX = _x;};
	void setDimY(int _y){if(_y > 0)	this->dimY = _y;};
	void setDimZ(int _z){if(_z > 0)	this->dimZ = _z;};
	void setLinearRowScale();
	void setLinearDepthScale();
	void setDims(int _x, int _y, int _z);
	
	
	int getDimX() { return this->dimX; }
	int getDimY() { return this->dimY; }
	int getDimZ() { return this->dimZ; }
	long getSize() { return (*this).getDimX() * (*this).getDimY() * (*this).getDimZ(); }
	double *getData() { return this->data; }
	double getData(int x, int y, int z);
	uint getRowScale() { return this->rowScale; }
	uint getDepthScale() { return this->depthScale; }
	long getIndex(int x, int y, int z) { return ( x + (y * (*this).getRowScale()) + (z * (*this).getDepthScale()) ); }
	
	void allocDataArray();
	void fillDataArray(BitBlock &_bitblock, double _resolution, double _gValue, int _gDirection);
	void fillData(long _index, double _data);
	void readDataFromFile(string _file);
	void show();	
};

#endif