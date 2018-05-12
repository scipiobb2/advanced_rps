#ifndef MOVE_H
#define MOVE_H
#include "utils.hpp"
#include "coordinate.h"

class Move
{
public:
    Move(Coordinate fromCoordinate, Coordinate toCoordinate,
         unsigned int playerId, unsigned int lineNumber,
         bool isJokerMove = false);

    Move(Coordinate fromCoordinate, Coordinate toCoordinate,
         unsigned int playerId, unsigned int lineNumber,
         Coordinate m_JokerLocation, PieceName newJokerMask,
         bool isJokerMove = true);

    Coordinate getFromCoordinate() const {return m_fromCoordinate;}

    Coordinate getToCoordinate() const {return m_toCoordinate;}

    Coordinate getJokerLocation() const {return m_jokerLocation;}

    bool getIsJokerMove() const {return m_isJokerMove;}

    PieceName getNewJokerMask() const {return m_newJokerMask;}

    unsigned int getPlayerId() const {return m_playerId;}

    unsigned int getLineNumber() const {return m_lineNumber;}



    friend std::ostream &operator<<(std::ostream &os,
                                    const Move &move);
private:
    Coordinate m_fromCoordinate;
    Coordinate m_toCoordinate;
    unsigned int m_playerId;
    unsigned int m_lineNumber;
    bool m_isJokerMove;
    Coordinate m_jokerLocation;
    PieceName m_newJokerMask;

};

#endif // MOVE_H
