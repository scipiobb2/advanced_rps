#include "my_Move.h"

using namespace std;

my_Move::my_Move(Coordinate fromCoordinate, Coordinate toCoordinate,
           unsigned int playerId, unsigned int lineNumber,
           bool isJokerMove ):
    m_fromCoordinate(fromCoordinate), m_toCoordinate(toCoordinate),
     m_playerId(playerId), m_lineNumber(lineNumber),
     m_isJokerMove(isJokerMove)
{

}

my_Move::my_Move(Coordinate fromCoordinate, Coordinate toCoordinate,
           unsigned int playerId, unsigned int lineNumber,
           Coordinate jokerLocation, PieceName newJokerMask,
           bool isJokerMove ):
    m_fromCoordinate(fromCoordinate), m_toCoordinate(toCoordinate),
    m_playerId(playerId), m_lineNumber(lineNumber),
    m_isJokerMove(isJokerMove), m_jokerLocation(jokerLocation),
    m_newJokerMask(newJokerMask)
{

}

ostream &operator<<(ostream &os,
                                const my_Move &move)
{
    os<< "\tMove"<<
        "\t\t from location: " << move.m_fromCoordinate <<
        "\t\t\t To location: " << move.m_toCoordinate;

    if (move.m_isJokerMove)
    {
        os << "\tJoker move\n" <<
              "\t\t joker location: " << move.m_jokerLocation <<
              "\t\t joker new mask: " << move.m_newJokerMask << "\n";
    }

    return os << endl;
}
