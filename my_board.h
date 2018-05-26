#ifndef BOARD_H
#define BOARD_H
#include "utils.hpp"
#include "coordinate.h"
#include "piece.h"
#include "my_Move.h"
#include "player.h"
#include "my_fightinfo.h"
#include "Board.h"

class my_Board : public Board
{

public:
    my_Board();

    my_Board(unsigned int n, unsigned int m, std::vector<Player *> vectorPlayers);

    int getPlayer(const Point& pos) const;

    void addPieceVectorToBoard(std::vector<Piece> *vectorPiece);

    void addMoveVectorToBoard(unsigned int const playerId,
                              std::vector<my_Move> const &vectorMove,
                              unsigned int numberOfPlayers);

    void resolveDisputedCoordinates();

    void makeNextMove(unsigned int m_winnerId = 0);


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

    bool getIsValid() const {return  m_isValid;}
    void setNotValid(){m_isValid = false; }

    unsigned int getInitialMovesErrorLine() const {return m_initialMovesErrorLine;}
    void setInitialMovesErrorLine(unsigned int lineNumber){m_initialMovesErrorLine = lineNumber;}

    friend std::ostream &operator<<(std::ostream &os, const my_Board &board);

private:
    unsigned int m_numOfColumns;
    unsigned int m_numOfRows;

    std::unordered_map<unsigned int, Player *> m_playersMap;

    void addPieceToBoard(Piece *piece);

    std::unordered_map<Coordinate, std::vector<Piece *>> m_piecesOnBoard;

    bool isPieceLocationWithinRestrictions(Coordinate const coordinate ) const;

    bool isPieceAtLocation(auto const &existingKey, unsigned int playerId) const;

    unsigned int isJokerMoveAndChangingValid(Coordinate const toCoordinate,
                                 auto const existingKey, Coordinate const jokerLocation) const;
    bool m_isValid = true;
    BadPositioningError m_badPositioningError{};
    BadMovesInputError m_badMovesInput{};

    void addMoveToBoard(unsigned int const moveNumber,
                        my_Move const &move);

    // (moveNumber, move)
    //std::map<unsigned int, Move> m_moves;
    std::map<unsigned int, my_Move> m_moves;


    std::vector<Coordinate> m_disputedCoordinates;

    unsigned int m_numOfMovesPlayed = 0;
    unsigned int m_moveNumberInMovesMap = 0;

    unsigned int m_maxMoveNumberInMovesMap = 0;

    unsigned int m_initialMovesErrorLine = 0;

};

#endif // BOARD_H
