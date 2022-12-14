#include "Walker.h"

// default constructors
Walker::Walker() : nextDirection(None),
                   surfaceRelaxivity(WALKER_DEFAULT_RHO),
                   energy(WALKER_INITIAL_ENERGY),
                   decreaseFactor(1.0),
                   collisions(0),
                   tCollisions(0),
                   xiRate(0.0),
                   initialSeed(0),
                   currentSeed(0)
{
    this->initialPosition.setX(0);
    this->initialPosition.setY(0);
    this->initialPosition.setZ(0);
    this->currentPosition.setX(0);
    this->currentPosition.setY(0);
    this->currentPosition.setZ(0);

    // Define methods 'map' and 'walk' according to problem's dimension
    associateMap(true);
    associateWalk(true);
}

// default constructor with dimensions defined
Walker::Walker(bool _3rdDim) : nextDirection(None),
                               surfaceRelaxivity(WALKER_DEFAULT_RHO),
                               energy(WALKER_INITIAL_ENERGY),
                               decreaseFactor(1.0),
                               collisions(0),
                               tCollisions(0),
                               xiRate(0.0),
                               initialSeed(0),
                               currentSeed(0)
{
    this->initialPosition.setX(0);
    this->initialPosition.setY(0);
    this->initialPosition.setZ(0);
    this->currentPosition.setX(0);
    this->currentPosition.setY(0);
    this->currentPosition.setZ(0);

    // Define methods 'map' and 'walk' according to walker dimensionality
    (*this).associateMap(_3rdDim);
    (*this).associateWalk(_3rdDim);
}

// default constructor with positions defined
Walker::Walker(int _x, int _y, int _z, bool _3rdDim) : nextDirection(None),
                                                       surfaceRelaxivity(WALKER_DEFAULT_RHO),
                                                       energy(WALKER_INITIAL_ENERGY),
                                                       decreaseFactor(1.0),
                                                       collisions(0),
                                                       tCollisions(0),
                                                       initialSeed(0),
                                                       currentSeed(0)
{
    this->initialPosition.setX(_x);
    this->initialPosition.setY(_y);
    this->initialPosition.setZ(_z);
    this->currentPosition.setX(_x);
    this->currentPosition.setY(_y);
    this->currentPosition.setZ(_z);

    // Define methods 'map' and 'walk' according to walker's dimensionality
    (*this).associateMap(_3rdDim);
    (*this).associateWalk(_3rdDim);
}

// copy constructor
Walker::Walker(const Walker &_walker)
{
    this->initialPosition = _walker.initialPosition;
    this->currentPosition = _walker.currentPosition;
    this->nextDirection = _walker.nextDirection;

    this->initialSeed = _walker.initialSeed;
    this->currentSeed = _walker.currentSeed;

    this->surfaceRelaxivity = _walker.surfaceRelaxivity;
    this->decreaseFactor = _walker.decreaseFactor;
    this->collisions = _walker.collisions;
    this->tCollisions = _walker.tCollisions;
    this->energy = _walker.energy;
    this->xiRate = _walker.xiRate;

    this->mapPointer = _walker.mapPointer;
    this->walkPointer = _walker.walkPointer;
}

void Walker::associateMap(bool _3rdDim)
{
    if (_3rdDim == true)
    {
        mapPointer = &Walker::map_3D;
    }
    else
    {
        mapPointer = &Walker::map_2D;
    }
}

void Walker::associateWalk(bool _3rdDim)
{
    if (_3rdDim == true)
    {
        walkPointer = &Walker::walk_3D;
    }
    else
    {
        walkPointer = &Walker::walk_2D;
    }
}

void Walker::map(BitBlock *_bitBlock)
{
    (this->*mapPointer)(_bitBlock);
}

void Walker::walk(BitBlock *_bitBlock)
{
    (this->*walkPointer)(_bitBlock);
}

void Walker::map_2D(BitBlock *_bitBlock)
{
    computeNextDirection_2D();
    checkBorder_2D(_bitBlock);
    Pos3d nextPosition = computeNextPosition_2D();

    //check if next position is pore wall
    if (checkNextPosition_2D(nextPosition, _bitBlock))
    {
        moveWalker(nextPosition);
    }
    else
    {
        // walker crashes with wall and "comes back" to the same position
        this->collisions++;
    }
}

void Walker::walk_2D(BitBlock *_bitBlock)
{
    computeNextDirection_2D();
    checkBorder_2D(_bitBlock);
    Pos3d nextPosition = computeNextPosition_2D();

    if (checkNextPosition_2D(nextPosition, _bitBlock))
    {
        moveWalker(nextPosition);
    }
    else
    {
        // walker chocks with wall and comes back to the same position
        // walker loses energy due to this collision
        this->energy = this->energy * decreaseFactor;
    }
}

void Walker::map_3D(BitBlock *_bitBlock)
{
    computeNextDirection_3D();
    checkBorder_3D(_bitBlock);
    Pos3d nextPosition = computeNextPosition_3D();

    //check if next position is pore wall
    if (checkNextPosition_3D(nextPosition, _bitBlock))
    {
        moveWalker(nextPosition);
    }
    else
    {
        // walker crashes with wall and "comes back" to the same position
        this->collisions++;
    }
}

void Walker::walk_3D(BitBlock *_bitBlock)
{
    computeNextDirection_3D();
    checkBorder_3D(_bitBlock);
    Pos3d nextPosition = computeNextPosition_3D();

    if (checkNextPosition_3D(nextPosition, _bitBlock))
    {
        moveWalker(nextPosition);
    }
    else
    {
        // walker chocks with wall and comes back to the same position
        // walker loses energy due to this collision
        this->energy = this->energy * this->decreaseFactor;
    }
}

// generate seed for walker's random movement
void Walker::createRandomSeed()
{
    std::mt19937_64 myRNG;
    std::random_device device;
    myRNG.seed(device());
    std::uniform_int_distribution<uint64_t> uint64_dist;

    uint64_t garbageSeed;

    for (uint i = 0; i < 100; i++)
    {
        // assign a random seed to the simulation
        garbageSeed = uint64_dist(myRNG);
    }

    // assign a random seed to the simulation
    this->initialSeed = uint64_dist(myRNG) + 1;
    this->currentSeed = this->initialSeed;
}

void Walker::setRandomSeed(uint64_t _seed)
{
    this->initialSeed = _seed;
    this->currentSeed = this->initialSeed;
}