//include
#include "BitBlock.h"

BitBlock::BitBlock() : blocks(NULL)
{}

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

void BitBlock::createBlockMap(vector<Mat> &_binaryMap, uchar poreColor)
{
    if (_binaryMap.size() == 1)
    {
        setBlockMapDimensions_2D(_binaryMap[0]);
        createBitBlocksArray_2D(_binaryMap[0], poreColor);
    }
    else
    {
        setBlockMapDimensions_3D(_binaryMap);
        createBitBlocksArray_3D(_binaryMap, poreColor);
    }
}

// 2D block
void BitBlock::setBlockMapDimensions_2D(Mat &_binaryMap)
{
    (*this).setImageRows(_binaryMap.rows);
    (*this).setImageColumns(_binaryMap.cols);

    // for 2D blocks, depth parameter is meaningless
    (*this).setImageDepth(1);
    (*this).setBlockDepth(0);

    (*this).setBlockRows((*this).getImageRows() / ROWSPERBLOCK2D);
    if ((*this).getImageRows() % ROWSPERBLOCK2D != 0)
    {
        (*this).setBlockRows((*this).getBlockRows() + 1);
    }

    (*this).setBlockColumns((*this).getImageColumns() / COLUMNSPERBLOCK2D);
    if ((*this).getImageColumns() % COLUMNSPERBLOCK2D != 0)
    {
        (*this).setBlockColumns((*this).getBlockColumns() + 1);
    }

    (*this).setNumberOfBlocks((*this).getBlockRows() * (*this).getBlockColumns());

    // alloc memory for bitblocks
    (*this).allocBlocks((*this).getNumberOfBlocks());
}

void BitBlock::createBitBlocksArray_2D(Mat &_binaryMap, uchar poreColor)
{
    uint64_t newBlock;

    for (int block_y = 0; block_y < (*this).getBlockRows(); block_y++)
    {
        for (int block_x = 0; block_x < (*this).getBlockColumns(); block_x++)
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
                    if (mapPixel_x < (*this).getImageColumns() && mapPixel_y < (*this).getImageRows())
                    {
                        Pos3d pixel(mapPixel_x, mapPixel_y, 0);

                        if (!pixel.isPore(_binaryMap, poreColor))
                        {
                            int bit = (bit_y * COLUMNSPERBLOCK2D) + bit_x;
                            newBlock |= (1ull << bit);
                        }
                    }
                }
            }

            // assign new block to blocks array
            (*this).setBlock(newBlock, IDX2C(block_y, block_x, (*this).getBlockColumns()));
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
    fileObject << (*this).getNumberOfBlocks() << ", ";
    fileObject << (*this).getBlockRows() << ", ";
    fileObject << (*this).getBlockColumns() << ", ";
    fileObject << (*this).getImageRows() << ", ";
    fileObject << (*this).getImageColumns() << endl;

    fileObject << endl;
    fileObject << "blockID, ";
    fileObject << "blockData" << endl;

    for (int index = 0; index < (*this).getNumberOfBlocks(); index++)
    {
        fileObject << index << ", ";
        fileObject << (*this).getBlock(index) << endl;
    }

    fileObject.close();
}

// 3D block
void BitBlock::setBlockMapDimensions_3D(vector<Mat> &_binaryMap)
{
    (*this).setImageRows(_binaryMap[0].rows);
    (*this).setImageColumns(_binaryMap[0].cols);
    (*this).setImageDepth(_binaryMap.size());

    (*this).setBlockRows((*this).getImageRows() / ROWSPERBLOCK3D);
    if ((*this).getImageRows() % ROWSPERBLOCK3D != 0)
    {
        (*this).setBlockRows((*this).getBlockRows() + 1);
    }

    (*this).setBlockColumns((*this).getImageColumns() / COLUMNSPERBLOCK3D);
    if ((*this).getImageColumns() % COLUMNSPERBLOCK3D != 0)
    {
        (*this).setBlockColumns((*this).getBlockColumns() + 1);
    }

    (*this).setBlockDepth((*this).getImageDepth() / DEPTHPERBLOCK3D);
    if ((*this).getImageDepth() % DEPTHPERBLOCK3D != 0)
    {
        (*this).setBlockDepth((*this).getBlockDepth() + 1);
    }

    (*this).setNumberOfBlocks((*this).getBlockRows() * (*this).getBlockColumns() * (*this).getBlockDepth());
    
    // alloc memory for bitblocks
    (*this).allocBlocks((*this).getNumberOfBlocks());
}

void BitBlock::createBitBlocksArray_3D(vector<Mat> &_binaryMap, uchar poreColor)
{
    uint64_t newBlock;

    // Create progress bar object
    ProgressBar pBar((double) ((*this).getBlockDepth()));

    for (int block_z = 0; block_z < (*this).getBlockDepth(); block_z++)
    {
        for (int block_y = 0; block_y < (*this).getBlockRows(); block_y++)
        {
            for (int block_x = 0; block_x < (*this).getBlockColumns(); block_x++)
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
                            if (mapPixel_x < (*this).getImageColumns() && mapPixel_y < (*this).getImageRows() && mapPixel_z < (*this).getImageDepth())
                            {
                                Pos3d pixel(mapPixel_x, mapPixel_y, mapPixel_z);

                                if (!pixel.isPore(_binaryMap, poreColor))
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
                (*this).setBlock(newBlock, IDX2C_3D(block_x, block_y, block_z, (*this).getBlockColumns(), (*this).getBlockRows()));
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
    fileObject << (*this).getNumberOfBlocks() << ", ";
    fileObject << (*this).getBlockRows() << ", ";
    fileObject << (*this).getBlockColumns() << ", ";
    fileObject << (*this).getBlockDepth() << ", ";
    fileObject << (*this).getImageRows() << ", ";
    fileObject << (*this).getImageColumns() << ", ";
    fileObject << (*this).getImageDepth() << endl;

    fileObject << endl;
    fileObject << "blockID, ";
    fileObject << "blockData" << endl;

    for (int index = 0; index < (*this).getNumberOfBlocks(); index++)
    {
        fileObject << index << ", ";
        fileObject << (*this).getBlock(index) << endl;
    }

    fileObject.close();
}