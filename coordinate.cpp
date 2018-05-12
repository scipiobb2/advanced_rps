#include "coordinate.h"

using namespace std;

Coordinate::Coordinate() {Coordinate(0,0);}

Coordinate::Coordinate(unsigned int x, unsigned int y): m_x(x), m_y(y)
{}

Coordinate::Coordinate(string x, string y):
    Coordinate((unsigned int)stoi(x), (unsigned int)stoi(y))
{}

Coordinate::Coordinate(const Coordinate &rightCoordinate):
    Coordinate(rightCoordinate.getX(), rightCoordinate.getY())
{}

ostream &operator<<(std::ostream &os, const Coordinate &coordinate)
{
    return os << "\tCordinates: " << "x:" << coordinate.m_x
              << " y:" << coordinate.m_y << std::endl;
}


