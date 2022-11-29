#ifndef WALKER_H_
#define WALKER_H_

#include "Defs.h"
#include "Pos3d.h"
#include "BitBlock.h"

class Walker
{
private:
    // Object attributes:
    Pos3d initialPosition;
    Pos3d currentPosition;
    direction nextDirection;
    uint64_t initialSeed;
    uint64_t currentSeed;

    // physical properties
    double surfaceRelaxivity;
    double decreaseFactor;
    uint collisions;
    uint tCollisions;
    double xiRate;
    double energy;

public:
    // default constructor
    Walker();
    Walker(bool _3rdDim);
    Walker(int _x, int _y, int _z, bool _3rdDim);
    // copy constructor
    Walker(const Walker &_walker);
    // virtual destructor
    virtual ~Walker(){};

    // Set methods:
    void setInitialPosition(Pos3d _pos){ this->initialPosition = _pos;}
    void setInitialPosition(int _x, int _y, int _z){ 
        this->initialPosition.setX(_x);
        this->initialPosition.setY(_y);
        this->initialPosition.setZ(_z);
    }
    void setInitialPositionX(int _x){this->initialPosition.setX(_x);}
    void setInitialPositionY(int _y){this->initialPosition.setY(_y);}
    void setInitialPositionZ(int _z){this->initialPosition.setZ(_z);}

    void setCurrentPosition(Pos3d _pos){ this->currentPosition = _pos;}
    void setCurrentPosition(int _x, int _y, int _z){ 
        this->currentPosition.setX(_x);
        this->currentPosition.setY(_y);
        this->currentPosition.setZ(_z);
    }
    void setCurrentPositionX(int _x){this->currentPosition.setX(_x);}
    void setCurrentPositionY(int _y){this->currentPosition.setY(_y);}
    void setCurrentPositionZ(int _z){this->currentPosition.setZ(_z);}
    void setNextDirection(direction _nd){ this->nextDirection = _nd;}  
    void setInitialSeed(uint64_t _seed){ this->initialSeed = _seed; }
    void setCurrentSeed(uint64_t _seed){ this->currentSeed = _seed; }
    void setRandomSeed(uint64_t _seed);
    void setSurfaceRelaxivity(double _rho){ this->surfaceRelaxivity = _rho; }
    void setSurfaceRelaxivity(vector<double> &parameters)
    {
        double off = 1.e-6;

        // Set 1st Sigmoid Curve
        double K1 = parameters[0];
        double A1 = parameters[1];
        double eps1 = parameters[2];
        double B1 = parameters[3];

        double shapeFunction1;
        shapeFunction1 = A1 + ((K1 - A1) / (1 + exp((-1) * B1 * ((this->xiRate + off) - eps1))));

        // Set 2nd Sigmoid Curve
        double K2 = parameters[4];
        double A2 = parameters[5];
        double eps2 = parameters[6];
        double B2 = parameters[7];

        double shapeFunction2;
        shapeFunction2 = A2 + ((K2 - A2) / (1 + exp((-1) * B2 * ((this->xiRate + off) - eps2))));

        this->surfaceRelaxivity = shapeFunction1 + shapeFunction2;
    }
    void setDecreaseFactor(double _v){ this->decreaseFactor = _v; }
    void setCollisions(uint _c){ this->collisions = _c; }
    void setTCollisions(uint _c){ this->tCollisions = _c; }
    void setXiRate(double _v){ this->xiRate = _v; }
    void setEnergy(double _v){ this->energy = _v; }

    // Get methods
    Pos3d getInitialPosition(){ return this->initialPosition;}
    int getInitialPositionX(){ return this->initialPosition.getX(); }
    int getInitialPositionY(){ return this->initialPosition.getY(); }
    int getInitialPositionZ(){ return this->initialPosition.getZ(); }
    Pos3d getCurrentPosition(){ return this->currentPosition;}
    int getCurrentPositionX(){ return this->currentPosition.getX(); }
    int getCurrentPositionY(){ return this->currentPosition.getY(); }
    int getCurrentPositionZ(){ return this->currentPosition.getZ(); }
    direction getNextDirection(){ return this->nextDirection;}
    uint64_t getInitialSeed(){ return this->initialSeed; }
    uint64_t getCurrentSeed(){ return this->currentSeed; }
    double getSurfaceRelaxivity(){ return this->surfaceRelaxivity; }
    double getDecreaseFactor(){ return this->decreaseFactor; }
    uint getCollisions(){ return this->collisions; }
    uint getTCollisions(){ return this->tCollisions; }
    double getXiRate(){ return this->xiRate; }
    double getEnergy(){ return this->energy; }

    void createRandomSeed();

    // supermethods
    typedef void (Walker::*map_ptr)(BitBlock *);
    typedef void (Walker::*walk_ptr)(BitBlock *);

private:
    map_ptr mapPointer;
    walk_ptr walkPointer;

    // 2D
    void map_2D(BitBlock *_bitBlock);
    void walk_2D(BitBlock *_bitBlock);

    // 3D
    void map_3D(BitBlock *_bitBlock);
    void walk_3D(BitBlock *_bitBlock);

public:
    void map(BitBlock *_bitBlock);
    void walk(BitBlock *_bitBlock);
    void associateMap(bool _3rdDim);
    void associateWalk(bool _3rdDim);

    // Inline methods
    inline void resetPosition()
    {
        (*this).setCurrentPosition((*this).getInitialPosition());
    };

    inline void resetCollisions()
    {
        this->collisions = 0;
    };

    inline void resetTCollisions()
    {
        this->tCollisions = 0;
    };

    inline void resetEnergy()
    {
        this->energy = WALKER_INITIAL_ENERGY;
    };

    inline void resetSeed()
    {
        this->currentSeed = this->initialSeed;
    };


    void updateXiRate(uint _numberOfSteps)
    {
        double steps = (double) _numberOfSteps;
        if(LOG_XIRATE) 
        {
            this->xiRate = log10((double) this->collisions) / log10(steps);
        } else 
        {
            this->xiRate =  (double) this->collisions / steps;
        }
    }

    inline void computeDecreaseFactor(double _walkerStepLength,
                                      double _diffusionCoefficient)
    {
        // A factor: see ref. Bergman 1995 -- 1.e-3 is used to convert surface relaxivity measure from um/s to um/ms
        double Afactor = 2.0 / 3.0;
        this->decreaseFactor = 1.0 - Afactor * ((this->surfaceRelaxivity * _walkerStepLength) / (_diffusionCoefficient * 1e03));
    };

    // generate a random number using xorshift64
    inline uint64_t generateRandomNumber(uint64_t _seed)
    {
        xorshift64_state xor_state;
        xor_state.a = _seed;
        return xorShift64(&xor_state);
    }

    // 2D movements
    inline void computeNextDirection_2D()
    {
        // generate a random number using xorshift library
        uint64_t rand = generateRandomNumber(this->currentSeed);

        // update current seed for next move
        this->currentSeed = rand;

        // set direction based on the random number
        rand = rand & (4 - 1);
        this->nextDirection = (direction)(rand + 1);
    };

    inline Pos3d computeNextPosition_2D()
    {
        Pos3d nextPosition = {this->currentPosition.getX(), 
                                this->currentPosition.getY(), 
                                this->currentPosition.getZ()};
        switch (nextDirection)
        {
        case North:
            nextPosition.setY(nextPosition.getY() - 1);
            break;

        case West:
            nextPosition.setX(nextPosition.getX() - 1);
            break;

        case South:
            nextPosition.setY(nextPosition.getY() + 1);
            break;

        case East:
            nextPosition.setX(nextPosition.getX() + 1);
            break;
        }

        return nextPosition;
    };

    inline void checkBorder_2D(Mat &_binaryMap)
    {
        switch (this->nextDirection)
        {
        case North:
            if (this->currentPosition.getY() == 0)
            {
                nextDirection = South;
            }
            break;

        case South:

            if (this->currentPosition.getY() == _binaryMap.rows - 1)
            {
                nextDirection = North;
            }
            break;

        case West:
            if (this->currentPosition.getX() == 0)
            {
                nextDirection = East;
            }
            break;

        case East:
            if (this->currentPosition.getX() == _binaryMap.cols - 1)
            {
                nextDirection = West;
            }
            break;
        }
    };

    inline void checkBorder_2D(BitBlock *_bitBlock)
    {
        switch (this->nextDirection)
        {
        case North:
            if (this->currentPosition.getY() == 0)
            {
                nextDirection = South;
            }
            break;

        case South:

            if (this->currentPosition.getY() == _bitBlock->getImageRows() - 1)
            {
                nextDirection = North;
            }
            break;

        case West:
            if (this->currentPosition.getX() == 0)
            {
                nextDirection = East;
            }
            break;

        case East:
            if (this->currentPosition.getX() == _bitBlock->getImageColumns() - 1)
            {
                nextDirection = West;
            }
            break;
        }
    };

    inline bool checkNextPosition_2D(Pos3d _nextPosition, Mat &_binaryMap, uchar _poreColor)
    {
        return (_nextPosition.isPore(_binaryMap, _poreColor));
    };

    inline bool checkNextPosition_2D(Pos3d _nextPosition, BitBlock *_bitBlock)
    {
        int next_x = _nextPosition.getX();
        int next_y = _nextPosition.getY();
        int nextBlock = _bitBlock->findBlock(next_x, next_y);
        int nextBit = _bitBlock->findBitInBlock(next_x, next_y);

        return (!_bitBlock->checkIfBitIsWall(nextBlock, nextBit));
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////// 3D movements
    //////////////////////////////
    inline void computeNextDirection_3D()
    {
        // generate a random number using xorshift library
        uint64_t rand = generateRandomNumber(this->currentSeed);

        // update current seed for next move
        this->currentSeed = rand;

        // set direction based on the random number
        //rand = rand % 6;
        rand = mod6(rand);
        this->nextDirection = (direction)(rand + 1);
    };

    inline Pos3d computeNextPosition_3D()
    {
        Pos3d nextPosition = {this->currentPosition.getX(), 
                                this->currentPosition.getY(), 
                                this->currentPosition.getZ()};
        switch (nextDirection)
        {
        case North:
            nextPosition.setY(nextPosition.getY() - 1);
            break;

        case West:
            nextPosition.setX(nextPosition.getX() - 1);
            break;

        case South:
            nextPosition.setY(nextPosition.getY() + 1);
            break;

        case East:
            nextPosition.setX(nextPosition.getX() + 1);
            break;

        case Up:
            nextPosition.setZ(nextPosition.getZ() + 1);
            break;

        case Down:
            nextPosition.setZ(nextPosition.getZ() - 1);
            break;
        }

        return nextPosition;
    };

    inline void checkBorder_3D(vector<Mat> &_binaryMap)
    {
        uint slice = this->currentPosition.getZ();

        switch (this->nextDirection)
        {
        case North:
            if (this->currentPosition.getY() == 0)
            {
                nextDirection = South;
            }
            break;

        case South:

            if (this->currentPosition.getY() == _binaryMap[slice].rows - 1)
            {
                nextDirection = North;
            }
            break;

        case West:
            if (this->currentPosition.getX() == 0)
            {
                nextDirection = East;
            }
            break;

        case East:
            if (this->currentPosition.getX() == _binaryMap[slice].cols - 1)
            {
                nextDirection = West;
            }
            break;

        case Up:
            if (this->currentPosition.getZ() == _binaryMap.size() - 1)
            {
                nextDirection = Down;
            }
            break;

        case Down:
            if (this->currentPosition.getZ() == 0)
            {
                nextDirection = Up;
            }
            break;
        }
    };

    inline void checkBorder_3D(BitBlock *_bitBlock)
    {
        switch (this->nextDirection)
        {
        case North:
            if (this->currentPosition.getY() == 0)
            {
                nextDirection = South;
            }
            break;

        case South:

            if (this->currentPosition.getY() == _bitBlock->getImageRows() - 1)
            {
                nextDirection = North;
            }
            break;

        case West:
            if (this->currentPosition.getX() == 0)
            {
                nextDirection = East;
            }
            break;

        case East:
            if (this->currentPosition.getX() == _bitBlock->getImageColumns() - 1)
            {
                nextDirection = West;
            }
            break;

        case Up:
            if (this->currentPosition.getZ() == _bitBlock->getImageDepth() - 1)
            {
                nextDirection = Down;
            }
            break;

        case Down:
            if (this->currentPosition.getZ() == 0)
            {
                nextDirection = Up;
            }
            break;
        }
    };

    inline bool checkNextPosition_3D(Pos3d _nextPosition, vector<Mat> &_binaryMap, uchar _poreColor)
    {
        return (_nextPosition.isPore(_binaryMap, _poreColor));
    };

    inline bool checkNextPosition_3D(Pos3d _nextPosition, BitBlock *_bitBlock)
    {
        int next_x = _nextPosition.getX();
        int next_y = _nextPosition.getY();
        int next_z = _nextPosition.getZ();
        int nextBlock = _bitBlock->findBlock(next_x, next_y, next_z);
        int nextBit = _bitBlock->findBitInBlock(next_x, next_y, next_z);

        return (!_bitBlock->checkIfBitIsWall(nextBlock, nextBit));
    };

    inline void moveWalker(Pos3d _nextPosition)
    {
        this->currentPosition.setX(_nextPosition.getX());
        this->currentPosition.setY(_nextPosition.getY());
        this->currentPosition.setZ(_nextPosition.getZ());
    };

    inline void placeWalker(int x0 = 0, int y0 = 0, int z0 = 0)
    {
        this->initialPosition.setX(x0);
        this->initialPosition.setY(y0);
        this->initialPosition.setZ(z0);
        (*this).resetPosition();
    }

    inline void printInitialPosition()
    {
        cout << "{" << this->initialPosition.getX() 
        << ", " << this->initialPosition.getY() 
        << ", " << this->initialPosition.getZ() << "}" << endl;
    };

    inline void printCurrentPosition()
    {
        cout << "{" << this->currentPosition.getX() 
        << ", " << currentPosition.getY() 
        << ", " << currentPosition.getZ() << "}" << endl;
    };

    inline void printPosition(Pos3d position)
    {
        cout << "{" << position.getX() << ", " << position.getY() << ", " << position.getZ() << "}" << endl;
    };


private:
    inline uint64_t xorShift64(struct xorshift64_state *state)
    {
        uint64_t x = state->a;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return state->a = x;
    }

    inline uint64_t mod6(uint64_t a)
    {
        while (a > 11)
        {
            int s = 0; /* accumulator for the sum of the digits */
            while (a != 0)
            {
                s = s + (a & 7);
                a = (a >> 2) & -2;
            }
            a = s;
        }
        /* note, at this point: a < 12 */
        if (a > 5)
            a = a - 6;
        return a;
    }
};

#endif