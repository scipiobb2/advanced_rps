#include "piece.h"

using namespace std;

Piece::Piece(PieceName pieceName, char letter, Coordinate coordinate,
             unsigned int ownerId, unsigned int readFromLine,
             bool isJoker, PieceName jokerMask,
             bool isAlive):
        m_pieceName(pieceName), m_letter(letter),
        m_coordinate(coordinate), m_ownerId(ownerId),
        m_readFromLine(readFromLine),
        m_isJoker(isJoker), m_jokerMask(jokerMask), m_isAlive(isAlive)
{

}

bool Piece::canMove() const
{
    if (m_pieceName == PieceName::Bomb ||
            m_pieceName == PieceName::Flag ||
            (m_isJoker && m_jokerMask == PieceName::Bomb))
        return false;
    return true;
}

void Piece::movePiece(Coordinate const &newCoordinate){
    m_coordinate = newCoordinate;
}

void Piece::changeJokerMask(PieceName const &newMask)
{
    m_jokerMask = newMask;
}

char Piece::getJokerLetter() const
{
   if (m_isJoker)
       return getLetterFromPieceName(m_jokerMask);
   else
       return '#';
}

ostream &operator<<(ostream &os,
                                const Piece &piece)
{
    string isAliveString;
    if (piece.m_isAlive == 1) isAliveString = "Yes";
    else isAliveString = "No";

    os << "\tPiece name: " << piece.m_pieceName << "\n"
        << piece.m_coordinate
        << "\tIs Alive? " << isAliveString << "\n"
        << "\tOwner id:" << piece.m_ownerId;

    if (piece.m_pieceName == PieceName::Joker)
        return os << "\n"
              << "\tJoker mask: " << piece.m_jokerMask << endl;
    else
        return os << endl;
}
