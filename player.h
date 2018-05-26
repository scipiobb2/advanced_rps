#ifndef PLAYER_H
#define PLAYER_H
#include "utils.hpp"
#include "coordinate.h"
#include "piece.h"
#include "my_Move.h"
#include "error.h"
#include "PlayerAlgorithm.h"

class Player : public PlayerAlgorithm
{
public:
    Player(int id,
            std::string initialPositionsFileName,
            std::string movesFileName,
            std::unordered_map<std::string, unsigned int> restrictions);

    void getInitialPositions(int player,
        std::vector<unique_ptr<PiecePosition>>& vectorToFill);

    virtual void notifyOnInitialBoard(const Board& b,
        const std::vector<unique_ptr<FightInfo>>& fights)
    {(void)b; (void)fights;}

    virtual void notifyOnOpponentMove(const Move& move) // called only on opponents move
    {(void)move;}

    virtual void notifyFightResult(const FightInfo& fightInfo) // called only if there was a fight
    {(void)fightInfo;}

    unique_ptr<Move> getMove();

    unique_ptr<JokerChange> getJokerChange(); // nullptr if no change is requested

    my_Move * getFileMove();

    bool outOfMoves() const;

    void readInitialPosition();

    std::vector<Piece>* getPieces() {return &m_pieces;}

    int getPoints() const {return m_points;}

    unsigned int getPlayerId() const {return m_id;}

    std::vector<my_Move> getMoves() const {return m_moves;}


    void updateAlivePiecesCount();

    void readMoves();

    unsigned int getNumberOfPiecesAlive() const {return m_numberOfPiecesAlive;}

    unsigned int getNumberOfPiecesThatCanMove() const {return m_numberOfPiecesThatCanMove;}

    unsigned int getNumberOfFlagsAlive() const {return m_numberOfFlagsAlive;}

    bool getIsLostGame() const {return m_isLostGame;}

    void setLostGame() {m_isLostGame = true;}

    void increaseNumberOfMovesPlayed() {m_numberOfMovesPlayed++;}

    bool isPlayingFromFile() {return m_playingFromFile;}

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
    std::vector<my_Move> m_moves;
    std::string m_initialPositionsFileName;
    std::string m_movesFileName;
    MissingFileError m_missingFileError{};
    BadPositioningError m_badPositioningError{};
    BadMovesInputError m_badMovesInput{};
    unsigned int m_numberOfMovesPlayed = 0;
    unsigned int m_numberOfPiecesAlive;
    unsigned int m_numberOfPiecesThatCanMove;
    unsigned int m_numberOfFlagsAlive;
    unsigned int m_maxXSize;
    unsigned int m_maxYSize;
    bool m_isLostGame = false;

    bool m_playingFromFile = true;

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
