#ifndef GAME_H
#define GAME_H
#include "utils.hpp"
#include "my_board.h"
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
    my_Board m_board;
    std::vector<Player *> m_players;
    unsigned int m_winnerId = 0;
    my_Move * m_nextMove;
    bool m_isEnded = false;
    std::string m_endGameReason;

    //unsigned int whoPlayesNext() const;
    void checkInitialPositionValidity();
    void checkInitialMovesValidity();

    unsigned int whoPlayesNext() const;

    void checkIfSomeoneWon();
    void addMoveIfNeeded();
    void checkIfNoMoreMoves();
    void updateAllPlayersCounts();
    void checkIfFlagsCapturesInInitialPosition();
    void checkIfBothPlayersCantMove();
    void checkPieceValidity();

    std::string getGameWinnerAnnouncementString() const;

};

#endif // GAME_H
