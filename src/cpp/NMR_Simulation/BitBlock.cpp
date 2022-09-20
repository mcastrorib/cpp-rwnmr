// include OpenCV core functions
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

// include C++ standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <vector>
#include <string>

// include OpenMP for multicore implementation
#include <omp.h>

//include
#include "../Math/RNG/xorshift.h"
#include "../Utils/ProgressBar.h"
#include "BitBlock.h"
#include "Walker.h"
#include "NMR_Simulation.h"

using namespace std;
using namespace cv;

BitBlock::BitBlock()
{
    this->blocks = NULL;
}

BitBlock::BitBlock(const BitBlock &_bitBlock)
{
    this->blocks = _bitBlock.blocks;
    this->numberOfBlocks = _bitBlock.numberOfBlocks;
    this->imageRows = _bitBlock.imageRows;
    this->imageColumns = _bitBlock.imageColumns;
    this->imageDepth = _bitBlock.imageDepth;
    this->blockRows = _bitBlock.blockRows;
    this->blockColumns = _bitBlock.blockColumns;
    this->blockDepth = _bitBlock.blockDepth;
}

void BitBlock::createBlockMap(vector<Mat> &_binaryMap)
{
    if (_binaryMap.size() == 1)
    {
        setBlockMapDimensions_2D(_binaryMap[0]);
        createBitBlocksArray_2D(_binaryMap[0]);
    }
    else
    {
        setBlockMapDimensions_3D(_binaryMap);
        createBitBlocksArray_3D(_binaryMap);
    }
}

// 2D block
void BitBlock::setBlockMapDimensions_2D(Mat &_binaryMap)
{
    this->imageRows = _binaryMap.rows;
    this->imageColumns = _binaryMap.cols;

    // for 2D blocks, depth parameter is meaningless
    this->imageDepth = 1;
    this->blockDepth = 0;

    this->blockRows = this->imageRows / ROWSPERBLOCK2D;
    if (this->imageRows % ROWSPERBLOCK2D != 0)
    {
        this->blockRows++;
    }

    this->blockColumns = this->imageColumns / COLUMNSPERBLOCK2D;
    if (this->imageColumns % COLUMNSPERBLOCK2D != 0)
    {
        this->blockColumns++;
    }

    this->numberOfBlocks = this->blockRows * this->blockColumns;

    // alloc memory for bitblocks
    this->blocks = new uint64_t[this->numberOfBlocks];
}

void BitBlock::createBitBlocksArray_2D(Mat &_binaryMap)
{
    uint64_t newBlock;

    for (int block_y = 0; block_y < this->blockRows; block_y++)
    {
        for (int block_x = 0; block_x < this->blockColumns; block_x++)
        {
            // initialize block
            newBlock = 0;

            for (int bit_y = 0; bit_y < ROWSPERBLOCK2D; bit_y++)
            {
                for (int bit_x = 0; bit_x < COLUMNSPERBLOCK2D; bit_x++)
                {
                    uint mapPixel_x = (block_x * COLUMNSPERBLOCK2D) + bit_x;
                    uint mapPixel_y = (block_y * ROWSPERBLOCK2D) + bit_y;

                    // check if pixel is inside image resolution
                    if (mapPixel_x < this->imageColumns && mapPixel_y < this->imageRows)
                    {
                        Point3D pixel(mapPixel_x, mapPixel_y, 0);

                        if (!pixel.isPore(_binaryMap))
                        {
                            int bit = (bit_y * COLUMNSPERBLOCK2D) + bit_x;
                            newBlock |= (1ull << bit);
                        }
                    }
                }
            }

            // assign new block to blocks array
            blocks[IDX2C(block_y, block_x, this->blockColumns)] = newBlock;
        }
    }
}

void BitBlock::saveBitBlockArray_2D(string filename)
{
    ofstream fileObject;
    fileObject.open(filename, ios::out);
    if (fileObject.fail())
    {
        cout << "Could not open file from disc" << endl;
    }

    // write properties
    fileObject << "blocks, ";
    fileObject << "bRows, ";
    fileObject << "bColumns, ";
    fileObject << "imgRows, ";
    fileObject << "imgColumns, " << endl;
    fileObject << this->numberOfBlocks << ", ";
    fileObject << this->blockRows << ", ";
    fileObject << this->blockColumns << ", ";
    fileObject << this->imageRows << ", ";
    fileObject << this->imageColumns << endl;

    fileObject << endl;
    fileObject << "blockID, ";
    fileObject << "blockData" << endl;

    for (int index = 0; index < this->numberOfBlocks; index++)
    {
        fileObject << index << ", ";
        fileObject << this->blocks[index] << endl;
    }

    fileObject.close();
}

// 3D block
void BitBlock::setBlockMapDimensions_3D(vector<Mat> &_binaryMap)
{
    this->imageRows = _binaryMap[0].rows;
    this->imageColumns = _binaryMap[0].cols;
    this->imageDepth = _binaryMap.size();

    this->blockRows = this->imageRows / ROWSPERBLOCK3D;
    if (this->imageRows % ROWSPERBLOCK3D != 0)
    {
        this->blockRows++;
    }

    this->blockColumns = this->imageColumns / COLUMNSPERBLOCK3D;
    if (this->imageColumns % COLUMNSPERBLOCK3D != 0)
    {
        this->blockColumns++;
    }

    this->blockDepth = this->imageDepth / DEPTHPERBLOCK3D;
    if (this->imageDepth % DEPTHPERBLOCK3D != 0)
    {
        this->blockDepth++;
    }

    this->numberOfBlocks = this->blockRows * this->blockColumns * this->blockDepth;

    // alloc memory for bitblocks
    this->blocks = new uint64_t[this->numberOfBlocks];
}

void BitBlock::createBitBlocksArray_3D(vector<Mat> &_binaryMap)
{
    uint64_t newBlock;

    // Create progress bar object
    ProgressBar pBar((double) (this->blockDepth));

    for (int block_z = 0; block_z < this->blockDepth; block_z++)
    {
        for (int block_y = 0; block_y < this->blockRows; block_y++)
        {
            for (int block_x = 0; block_x < this->blockColumns; block_x++)
            {
                // initialize block
                newBlock = 0;

                for (int bit_z = 0; bit_z < DEPTHPERBLOCK3D; bit_z++)
                {
                    for (int bit_y = 0; bit_y < ROWSPERBLOCK3D; bit_y++)
                    {
                        for (int bit_x = 0; bit_x < COLUMNSPERBLOCK3D; bit_x++)
                        {
                            uint mapPixel_x = (block_x * COLUMNSPERBLOCK3D) + bit_x;
                            uint mapPixel_y = (block_y * ROWSPERBLOCK3D) + bit_y;
                            uint mapPixel_z = (block_z * DEPTHPERBLOCK3D) + bit_z;

                            // check if pixel is inside image resolution
                            if (mapPixel_x < this->imageColumns && mapPixel_y < this->imageRows && mapPixel_z < this->imageDepth)
                            {
                                Point3D pixel(mapPixel_x, mapPixel_y, mapPixel_z);

                                if (!pixel.isPore(_binaryMap))
                                {
                                    int bit = (bit_z * (ROWSPERBLOCK3D * COLUMNSPERBLOCK3D)) // depth
                                              + (bit_y * COLUMNSPERBLOCK3D)                  // height
                                              + bit_x;                                       // width
                                    newBlock |= (1ull << bit);
                                }
                            }
                        }
                    }
                }

                // assign new block to blocks array
                blocks[IDX2C_3D(block_x, block_y, block_z, this->blockColumns, this->blockRows)] = newBlock;

            }
        }

        // Update progress bar
        pBar.update(1);
        pBar.print();
    }
}

void BitBlock::saveBitBlockArray_3D(string filename)
{    
    ofstream fileObject;
    fileObject.open(filename, ios::out);
    if (fileObject.fail())
    {
        cout << "Could not open file from disc" << endl;
    }

    // write properties
    fileObject << "blocks, ";
    fileObject << "bRows, ";
    fileObject << "bColumns, ";
    fileObject << "bDepth, ";
    fileObject << "imgRows, ";
    fileObject << "imgColumns, ";
    fileObject << "imgDepth, " << endl;
    fileObject << this->numberOfBlocks << ", ";
    fileObject << this->blockRows << ", ";
    fileObject << this->blockColumns << ", ";
    fileObject << this->blockDepth << ", ";
    fileObject << this->imageRows << ", ";
    fileObject << this->imageColumns << ", ";
    fileObject << this->imageDepth << endl;

    fileObject << endl;
    fileObject << "blockID, ";
    fileObject << "blockData" << endl;

    for (int index = 0; index < this->numberOfBlocks; index++)
    {
        fileObject << index << ", ";
        fileObject << this->blocks[index] << endl;
    }

    fileObject.close();
}