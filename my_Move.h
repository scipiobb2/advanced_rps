#ifndef MOVE_H
#define MOVE_H
#include "utils.hpp"
#include "coordinate.h"
#include "Move.h"
#include "JokerChange.h"

class my_Move : public Move, public JokerChange
{
public:
    my_Move();

    my_Move(Coordinate fromCoordinate, Coordinate toCoordinate,
         unsigned int playerId, unsigned int lineNumber,
         bool isJokerMove = false);

    my_Move(Coordinate fromCoordinate, Coordinate toCoordinate,
         unsigned int playerId, unsigned int lineNumber,
         Coordinate m_JokerLocation, PieceName newJokerMask,
         bool isJokerMove = true);

    const Coordinate& getFrom() const {return getFromCoordinate();}

    const Coordinate& getTo() const {return getToCoordinate();}

    const Coordinate& getJokerChangePosition()
                            const {return getJokerLocation();}

    char getJokerNewRep() const
        {return getLetterFromPieceName(getNewJokerMask());}


    const Coordinate& getFromCoordinate() const {return m_fromCoordinate;}

    const Coordinate& getToCoordinate() const {return m_toCoordinate;}

    const Coordinate& getJokerLocation() const {return m_jokerLocation;}

    bool getIsJokerMove() const {return m_isJokerMove;}

    PieceName getNewJokerMask() const {return m_newJokerMask;}

    unsigned int getPlayerId() const {return m_playerId;}

    unsigned int getLineNumber() const {return m_lineNumber;}



    friend std::ostream &operator<<(std::ostream &os,
                                    const my_Move &move);
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
