#ifndef GAME_H
#define GAME_H
#include "utils.hpp"
#include "board.h"
#include "player.h"

class Game
{
public:
    Game(Player *p1, Player *p2, std::string fileLocationPrefix);

    void playGame();

    bool compare_files(const std::string& filename1,
                       const std::string& filename2);
private:
    std::string m_fileLocationPrefix;
    Board m_board;
    std::vector<Player *> m_players;
    unsigned int m_winnerId = 0;
    bool m_isEnded = false;
    unsigned int m_errorLine = 0;
    std::string m_endGameReason;

    bool checkInitialPositionValidity();
    bool checkInitialMovesValidity();

    void checkIfSomeoneWon();
    void checkIfNoMoreMoves();
    void updateAllPlayersCounts();
    void checkIfFlagsCapturesInInitialPosition();
    void checkIfBothPlayersCantMove();
    void checkPieceValidity();

};

#endif // GAME_H
