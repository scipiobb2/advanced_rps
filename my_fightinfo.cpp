#include "my_fightinfo.h"

my_FightInfo::my_FightInfo(Coordinate const &location,
                           Piece const &piece1,
                           Piece const &piece2):
    m_location(location), m_piece1(piece1), m_piece2(piece2)
{
    m_winner = Advanced_RPS_Fight::twoPiecesFight(
                piece1.pieceNameForFight(),
                piece2.pieceNameForFight());
}

char my_FightInfo::getPiece(int player) const
{
    Piece const *tempPiece;
    if ( (int) m_piece1.getOwnerId() == player)
        tempPiece = &m_piece1;
    else if ( (int) m_piece2.getOwnerId() == player)
        tempPiece = &m_piece2;

    return (tempPiece->isJoker())
            ? tempPiece->getJokerLetter() :
              tempPiece->getLetter();
}

