#ifndef BOARD_H
#define BOARD_H
#include "utils.hpp"
#include "coordinate.h"
#include "piece.h"
#include "move.h"
#include "player.h"
#include "advanced_rps_fight.h"

class Board
{

public:
    Board();

    Board(unsigned int n, unsigned int m, std::vector<Player *> vectorPlayers);

    void addPieceVectorToBoard(std::vector<Piece> *vectorPiece);

    void addMoveVectorToBoard(unsigned int const playerId,
                              std::vector<Move> const &vectorMove,
                              unsigned int numberOfPlayers);

    void resolveDisputedCoordinates();

    void makeNextMove(unsigned int errorLine = 0,
                      unsigned int m_winnerId = 0);

    //For debug
    void makeXMoves(unsigned int numberOfMoves);

    //For debug
    void makeAllMoves();


    unsigned int getNumberOfMovesLeft() const
        {return m_moves.size() - m_numOfMovesPlayed;}

    unsigned int getNumberOfMovesPlayed() const
        {return m_numOfMovesPlayed;}

    unsigned int getMoveNumberInMovesMap() const
        {return m_moveNumberInMovesMap;}

    std::unordered_map<Coordinate, std::vector<Piece *>>
        getPiecesOnBoard() {return m_piecesOnBoard;}

    std::string getBoardPrint();

    BadPositioningError getBadPositioningError() const
        {return m_badPositioningError;}

    BadMovesInputError getBadMovesInputError() const
        {return m_badMovesInput;}

    bool getmIsValid() const {return  m_isValid;}
    void setNotValid(){m_isValid = false; }


    friend std::ostream &operator<<(std::ostream &os, const Board &board);

private:
    unsigned int m_numOfColumns;
    unsigned int m_numOfRows;

    void addPieceToBoard(Piece *piece);
    std::unordered_map<Coordinate, std::vector<Piece *>> m_piecesOnBoard;
    bool isPieceLocationWithinRestrictions(Coordinate const coordinate ) const;
    bool m_isValid = true;
    BadPositioningError m_badPositioningError{};
    BadMovesInputError m_badMovesInput{};

    void addMoveToBoard(unsigned int const moveNumber,
                        unsigned int const playerId, Move const &move);

    // (moveNumber, (playerId, move)
    std::map<unsigned int, std::pair<unsigned int, Move>> m_moves;


    std::vector<Coordinate> m_disputedCoordinates;

    unsigned int m_numOfMovesPlayed = 0;
    unsigned int m_moveNumberInMovesMap = 0;

};

#endif // BOARD_H
