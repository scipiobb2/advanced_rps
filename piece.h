#ifndef PIECE_H
#define PIECE_H
#include "utils.hpp"
#include "coordinate.h"

class Piece
{
public:
    Piece(PieceName pieceName, char letter, Coordinate coordinate,
          unsigned int ownerId, unsigned int readFromLine,
          bool isJoker = false,
          PieceName jokerMask = PieceName::Joker,
          bool isAlive = true) ;

    bool isFlag() const {return m_pieceName == PieceName::Flag;}

    bool isJoker() const {return m_pieceName == PieceName::Joker;}

    PieceName pieceNameForFight()
        const {return (m_isJoker) ? m_jokerMask : m_pieceName;}

    void movePiece(Coordinate const &newCoordinate);

    void changeJokerMask(PieceName const &newMask);

    Coordinate getCoordinate() const {return m_coordinate;}

    PieceName getPieceName() const {return m_pieceName;}

    unsigned int getOwnerId() const {return m_ownerId;}

    unsigned int getReadFromLine() const {return m_readFromLine;}

    bool getIsAlive() const {return m_isAlive;}

    char getLetter() const {return m_letter;}

    bool canMove() const;

    void setToDead(){m_isAlive = false;}
    void setToAlive(){m_isAlive = true;}
    void setOwnerId(const unsigned int ownerId){m_ownerId = ownerId;}

    void setNotValid(){m_isValid = false; }
    friend std::ostream &operator<<(std::ostream &os,
                                    const Piece &piece);

private:
    PieceName m_pieceName;
    char m_letter;
    Coordinate m_coordinate;
    unsigned int m_ownerId;
    unsigned int m_readFromLine;
    bool m_isJoker;
    PieceName m_jokerMask;
    bool m_isAlive;
    bool m_isValid;
};

#endif // PIECE_H
