#ifndef BITBLOCK_H_
#define BITBLOCK_H_

#include "Defs.h"
#include "Point3D.h"

// define 2D block properties
#define ROWSPERBLOCK2D 8
#define COLUMNSPERBLOCK2D 8
#define IDX2C(i, j, ld) (((i) * (ld)) + (j))

// define 3D block properties
#define ROWSPERBLOCK3D 4
#define COLUMNSPERBLOCK3D 4
#define DEPTHPERBLOCK3D 4
#define IDX2C_3D(i, j, k, lx, ly) ((k * (lx * ly)) + ((j) * (lx)) + (i))

class BitBlock
{
private:
    uint64_t *blocks;
    int numberOfBlocks;
    int imageRows;
    int imageColumns;
    int imageDepth;
    int blockRows;
    int blockColumns;
    int blockDepth;

public:
    // methods:

    // class constructor
    BitBlock();

    // class constructor by copy
    BitBlock(const BitBlock &_bitBlock);

    // class destructor
    virtual ~BitBlock()
    {
        delete[] this->blocks;
        this->blocks = NULL;
    };

    void setBlocks(uint64_t *_blocks){ this->blocks = _blocks; }
    void setBlock(uint64_t _block, uint idx){ this->blocks[idx] = _block; }
    void setNumberOfBlocks(int _n){ this->numberOfBlocks = _n; }
    void setImageRows(int _rows){ this->imageRows = _rows; }
    void setImageColumns(int _cols){ this->imageColumns = _cols; }
    void setImageDepth(int _depth){ this->imageDepth = _depth; }
    void setBlockRows(int _rows){ this->blockRows = _rows; }
    void setBlockColumns(int _cols){ this->blockColumns = _cols; }
    void setBlockDepth(int _depth){ this->blockDepth = _depth; }


    uint64_t *getBlocks(){ return this->blocks; }
    uint64_t getBlock(uint idx){ return this->blocks[idx]; }
    int getNumberOfBlocks(){ return this->numberOfBlocks;}
    int getImageRows(){ return this->imageRows; }
    int getImageColumns(){ return this->imageColumns; };
    int getImageDepth(){ return this->imageDepth; };
    int getBlockRows(){ return this->blockRows; };
    int getBlockColumns(){ return this->blockColumns; };
    int getBlockDepth(){ return this->blockDepth; };

    void allocBlocks(uint size){
        this->blocks = new uint64_t[size];
    }

    void clear()
    {
        this->numberOfBlocks = 0;
        this->imageRows = 0;
        this->imageColumns = 0;
        this->imageDepth = 0;
        this->blockRows = 0;
        this->blockColumns = 0;
        this->blockDepth = 0;
        if(this->blocks != NULL)
        {
            delete[] this->blocks;
            this->blocks = NULL;
        }
    }

    void createBlockMap(vector<Mat> &_binaryMap);
    void setBlockMapDimensions_2D(Mat &_binaryMap);
    void createBitBlocksArray_2D(Mat &_binaryMap);
    void saveBitBlockArray_2D(string filename);
    void setBlockMapDimensions_3D(vector<Mat> &_binaryMap);
    void createBitBlocksArray_3D(vector<Mat> &_binaryMap);
    void saveBitBlockArray_3D(string filename);

    // inline methods
    inline void saveBitBlockArray(string filename)
    {
        if (this->imageDepth == 1)
        {
            saveBitBlockArray_2D(filename);
        }
        else
        {
            saveBitBlockArray_3D(filename);
        }
    }

    // 2D block
    inline int findBlock(int _position_x, int _position_y)
    {
        // "x >> 3" is like "x / 8" in bitwise operation
        int block_x = _position_x >> 3;
        int block_y = _position_y >> 3;
        int blockIndex = block_x + block_y * this->blockColumns;

        return blockIndex;
    }

    inline int findBitInBlock(int _position_x, int _position_y)
    {
        // "x & (n - 1)" is lise "x % n" in bitwise operation
        int bit_x = _position_x & (COLUMNSPERBLOCK2D - 1);
        int bit_y = _position_y & (ROWSPERBLOCK2D - 1);
        // "x << 3" is like "x * 8" in bitwise operation
        int bitIndex = bit_x + (bit_y << 3);

        return bitIndex;
    }

    // 3D block
    inline int findBlock(int _position_x, int _position_y, int _position_z)
    {
        // "x >> 3" is like "x / 8" in bitwise operation
        int block_x = _position_x >> 2;
        int block_y = _position_y >> 2;
        int block_z = _position_z >> 2;
        int blockIndex = block_x + block_y * this->blockColumns + (block_z * (this->blockColumns * this->blockRows));

        return blockIndex;
    }

    inline int findBitInBlock(int _position_x, int _position_y, int _position_z)
    {
        // "x & (n - 1)" is "x % n" in bitwise operation
        int bit_x = _position_x & (COLUMNSPERBLOCK3D - 1);
        int bit_y = _position_y & (ROWSPERBLOCK3D - 1);
        int bit_z = _position_z & (DEPTHPERBLOCK3D - 1);
        // "x << 3" is like "x * 8" in bitwise operation
        int bitIndex = bit_x + (bit_y << 2) + ((bit_z << 2) << 2);

        return bitIndex;
    }

    inline bool checkIfBitIsWall(int _blockIndex, int _bitIndex)
    {
        return ((this->blocks[_blockIndex] >> _bitIndex) & 1ull);
    }
};

#endif