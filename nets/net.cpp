#include "net.h"

int Net::turnToDirection(int currentDirection, int turn)
{
    int direction = (currentDirection+turn)%countDirection;
    if(direction<0)
        direction += countDirection;
    return direction;
}

int Net::getMinTurn()
{
    return minTurn;
}

int Net::getMaxTurn()
{
    return maxTurn;
}


Net::Net()
{

}
