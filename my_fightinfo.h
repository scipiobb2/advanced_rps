#ifndef MY_FIGHTINFO_H
#define MY_FIGHTINFO_H
#include "utils.hpp"
#include "coordinate.h"
#include "piece.h"
#include "advanced_rps_fight.h"
#include "FightInfo.h"

class my_FightInfo : public FightInfo
{
public:
    my_FightInfo(Coordinate const &location, Piece const &piece1,
                 Piece const &piece2);

    int getWinner() const {return m_winner;}
    const Point& getPosition() const {return m_location;}
    char getPiece(int player) const;

private:
    Coordinate m_location;
    Piece m_piece1;
    Piece m_piece2;
    int m_winner;
};

#endif // MY_FIGHTINFO_H
