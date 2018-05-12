#ifndef PLAYER_H
#define PLAYER_H
#include "utils.hpp"
#include "coordinate.h"
#include "piece.h"
#include "move.h"
#include "error.h"

class Player
{
public:
    Player(int id,
            std::string initialPositionsFileName,
            std::string movesFileName,
            std::unordered_map<std::string, unsigned int> restrictions);

    void readInitialPosition();

    std::vector<Piece>* getPieces() {return &m_pieces;}

    int getPoints() const {return m_points;}

    unsigned int getPlayerId() const {return m_id;}

    std::vector<Move> getMoves() const {return m_moves;}


    void updateAlivePiecesCount();

    void readMoves();

    unsigned int getNumberOfPiecesAlive() const {return m_numberOfPiecesAlive;}

    unsigned int getNumberOfPiecesThatCanMove() const {return m_numberOfPiecesThatCanMove;}

    unsigned int getNumberOfFlagsAlive() const {return m_numberOfFlagsAlive;}

    bool getIsLostGame() const {return m_isLostGame;}

    void setLostGame() {m_isLostGame = true;}

    MissingFileError getMissingFileError() const
        {return m_missingFileError;}

    BadPositioningError getBadPositioningError() const
        {return m_badPositioningError;}

    BadMovesInputError getBadMovesInputError() const
        {return m_badMovesInput;}

    friend std::ostream &operator<<(std::ostream &os,
                                    const Player &player);
private:
    unsigned int m_id;
    std::vector<Piece> m_pieces;
    int m_points = 0;
    std::vector<Move> m_moves;
    std::string m_initialPositionsFileName;
    std::string m_movesFileName;
    MissingFileError m_missingFileError{};
    BadPositioningError m_badPositioningError{};
    BadMovesInputError m_badMovesInput{};
    unsigned int m_numberOfPiecesAlive;
    unsigned int m_numberOfPiecesThatCanMove;
    unsigned int m_numberOfFlagsAlive;
    unsigned int m_maxXSize;
    unsigned int m_maxYSize;
    bool m_isLostGame = false;

    std::unordered_map<PieceName, std::pair<unsigned int, unsigned int>>
        m_pieceCount;

    void updateCountPieces(PieceName pieceName, int difference = 1);

    bool isPositioningFileLineValid(std::vector<std::string> const tokens,
        std::unordered_map<std::string, PieceName> PieceNameMap) const;

    bool isMovesFileLineValid
        (std::vector<std::string> const tokens,
         std::unordered_map<std::string, PieceName> PieceNameMap) const;

    unsigned int getLocationOrZeroFromToken(std::string const token,
                                            unsigned int const maxSize) const;

    std::unordered_map<std::string, PieceName> getInputLettersToPiecesMap()
    {
        std::unordered_map<std::string, PieceName> PieceNameMap =
                {
                    {"R", PieceName::Rock},
                    {"P", PieceName::Paper},
                    {"S", PieceName::Scissors},
                    {"B", PieceName::Bomb},
                    {"J", PieceName::Joker},
                    {"F", PieceName::Flag}
                };
        return PieceNameMap;
    }
};
#endif // PLAYER_H
