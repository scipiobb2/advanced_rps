#ifndef COORDINATE_H
#define COORDINATE_H
#include "utils.hpp"

class Coordinate
{
public:
    Coordinate();
    Coordinate(unsigned int x, unsigned int y);
    Coordinate(std::string x, std::string y);
    Coordinate(const Coordinate &rightCoordinate);

    unsigned int getX() const {return m_x;}
    unsigned int getY() const {return m_y;}

    bool operator==(Coordinate const& other) const
    {
      return (m_x==other.m_x &&
              m_y==other.m_y);
    }

    bool operator<(Coordinate const& other) const
    {
        if ( m_x != other.m_x ) return m_x < other.m_x;
        else return m_y < other.m_y;
    }

    Coordinate& operator=(Coordinate const& other)
    {
        m_x = other.getX();
        m_y = other.getY();
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const Coordinate &coordinate);
private:
    unsigned int m_x;
    unsigned int m_y;
};

namespace std
{
    template<> struct hash<Coordinate>
    {

        size_t operator()(Coordinate const& coordinate) const
        {
          size_t h1 = std::hash<double>()(coordinate.getX());
          size_t h2 = std::hash<double>()(coordinate.getY());
          return (h1 ^ (h2 << 1));
        }
    };
}


#endif // COORDINATE_H
