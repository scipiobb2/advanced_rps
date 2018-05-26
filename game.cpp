#include "game.h"

using namespace std;

Game::Game(Player *p1, Player *p2, string fileLocationPrefix):
    m_fileLocationPrefix(fileLocationPrefix)
{
    m_players = {p1, p2};
    m_board = my_Board(N, M, m_players);
}

unsigned int Game::whoPlayesNext() const
{
    return (m_board.getMoveNumberInMovesMap() % m_players.size()) + 1;
}

void Game::playGame()
{
    bool isPrintBoard = true;
    checkInitialPositionValidity();
    if (!m_isEnded)
    {
        m_board.resolveDisputedCoordinates();
        updateAllPlayersCounts();
        checkIfFlagsCapturesInInitialPosition();
        checkIfBothPlayersCantMove();
    }
    else
        isPrintBoard = false;

    checkInitialMovesValidity();

    // There is an error on the first line of the first player
    if (m_board.getInitialMovesErrorLine() == 1 && m_winnerId != 1)
    {
        m_isEnded = true;
    }

    while (!m_isEnded)
    {
        checkIfSomeoneWon();
        if (m_isEnded)
            break;

        //addMoveIfNeeded();

        checkIfNoMoreMoves();
        if (m_isEnded)
        {
            m_endGameReason = "Reason: A tie - both Moves input files done without a winner\n";
            break;
        }
        m_board.makeNextMove(m_winnerId);

        if (!m_board.getIsValid() && m_board.getInitialMovesErrorLine())
            // Initial bad move
            m_isEnded = true;

        else if (!m_board.getBadMovesInputError().continueFlow())
            // In game bad move
        {
            m_isEnded = true;
            std::ostringstream temp_os;
            temp_os << m_board.getBadMovesInputError();
            m_endGameReason = temp_os.str();

            if (m_board.getBadMovesInputError().getPlayersFaults()[0].first == 1)
                m_winnerId = 2;
            else
                m_winnerId = 1;
        }
    }

    std::ofstream out(m_fileLocationPrefix + "rps.output");
    out << getGameWinnerAnnouncementString() << "\n";
    if (isPrintBoard)
        out << m_board.getBoardPrint();
    out.close();

    //cout << getGameWinnerAnnouncementString() << "\n";
    //cout << m_board.getBoardPrint() << endl;

}

string Game::getGameWinnerAnnouncementString() const
{
    if (m_winnerId == 0)
        return "Winner: 0\n" + m_endGameReason;
    else if (m_winnerId == 1)
    {
        return "Winner: 1\n" + m_endGameReason;
    }
    else if (m_winnerId == 2)
    {
        return "Winner: 2\n" + m_endGameReason;
    }
    else
        return "";
}

void Game::checkIfFlagsCapturesInInitialPosition()
{
    bool allFlagsCaptured = true;
    for (Player *p: m_players )
    {
        if (p->getNumberOfFlagsAlive() > 0)
        {
            allFlagsCaptured = false;
            break;
        }
    }
    if (allFlagsCaptured)
    {
        m_isEnded = true;
        m_endGameReason = "Reason: A tie - all flags are eaten by both players in the position files\n";
    }
}

// Only used for the initial position
void Game::checkIfBothPlayersCantMove()
{
    bool allPlayersCantMove = true;
    for (Player *p: m_players )
    {
        if (p->getNumberOfPiecesThatCanMove() > 0)
        {
            allPlayersCantMove = false;
            break;
        }
    }
    if (allPlayersCantMove)
    {
        m_isEnded = true;
        m_winnerId = 2;
        m_endGameReason = "Reason: All moving PIECEs of the opponent are eaten\n";
    }
}

void Game::updateAllPlayersCounts()
{
    for (Player *p: m_players )
    {
        p->updateAlivePiecesCount();
    }
}

void Game::checkIfSomeoneWon()
{
    unsigned int possibleWinnerId = 0;
    //unsigned int possibleWinnerByCloseDeathId = 0;
    unsigned int numberOfPlayersLost = 0;

    unsigned int whoPlaysNext = whoPlayesNext();

    for (Player *p: m_players )
    {

        p->updateAlivePiecesCount();
        // Flags eaten lose
        if (p->getNumberOfFlagsAlive() == 0)
        {
            p->setLostGame();
            numberOfPlayersLost++;
            m_endGameReason = "Reason: All flags of the opponent are captured\n";
        }
        // Not pieces that can move lose
        else if (p->getNumberOfPiecesThatCanMove() == 0 &&
                 (whoPlaysNext == p->getPlayerId() ||
                                     m_board.getNumberOfMovesLeft() == 0))
        {

            p->setLostGame();
            numberOfPlayersLost++;
            if (m_endGameReason == "")
                m_endGameReason = "Reason: All moving PIECEs of the opponent are eaten\n";
        }
        else
            possibleWinnerId = p->getPlayerId();
    }

    if (numberOfPlayersLost == m_players.size())
    {
        m_isEnded = true;
        if (m_board.getNumberOfMovesPlayed() == 0)
            m_winnerId = 0;
        else
        {
            m_winnerId =
                    (m_board.getNumberOfMovesPlayed() % m_players.size());
            if (m_winnerId == 0)
                m_winnerId = m_players.size();

        }
    }
    else if (numberOfPlayersLost + 1 == m_players.size())
    {
        m_isEnded = true;
        m_winnerId = possibleWinnerId;
    }
}

void Game::checkIfNoMoreMoves()
{
    if (m_board.getNumberOfMovesLeft() == 0)
        m_isEnded = true;
}

void Game::checkInitialPositionValidity()
{
    unsigned int possibleWinnerId = 0;
    unsigned int numberOfPlayersLost = 0;

    //Find lossers
    for (Player *p: m_players )
    {
        if (!p->getBadPositioningError().continueFlow())
        {
            m_isEnded = true;
            std::ostringstream os;
            os << p->getBadPositioningError();
            m_endGameReason = os.str();
            numberOfPlayersLost++;
        }
        else
        {
            possibleWinnerId = p->getPlayerId();
        }
    }
    // Everyone lost
    // To Do - Assumes only two players are playing
    if (numberOfPlayersLost == m_players.size())
    {
        m_winnerId = 0;
        // Get first error lines
        unsigned int p1FaultLine =
                m_players[0] ->
                getBadPositioningError().getPlayersFaults()[0].second;

        unsigned int p2FaultLine =
                m_players[1] ->
                getBadPositioningError().getPlayersFaults()[0].second;

        m_endGameReason = "Reason: Bad Positioning input file for both players - player 1: line "
                + to_string(p1FaultLine) +
                ", player 2: line "
                + to_string(p2FaultLine);

        m_isEnded = true;
    }
    else if (numberOfPlayersLost > 0)
    {
        m_winnerId = possibleWinnerId;
        m_isEnded = true;
    }

}

void Game::checkInitialMovesValidity()
{
    unsigned int minErrorLine = 0;
    unsigned int possibleWinnerId = 0;

    for (Player *p: m_players )
    {
        if (!p->getBadMovesInputError().continueFlow())
        {
            unsigned int thisErrorLine = p ->
                    getBadMovesInputError().getPlayersFaults()[0].second;
            if (minErrorLine == 0 || thisErrorLine < minErrorLine)
            {
                // Game needs to run until the bad move
                //m_isEnded = true;
                std::ostringstream os;
                os << p->getBadMovesInputError();
                m_endGameReason = os.str();
                minErrorLine = thisErrorLine;
            }
            else
            {
                possibleWinnerId = p->getPlayerId();
            }
        }
        else
        {
            possibleWinnerId = p->getPlayerId();
        }
    }

    if (minErrorLine != 0)
    {
        m_winnerId = possibleWinnerId;
        m_board.setInitialMovesErrorLine(minErrorLine);
    }

}

//For testing
template<typename InputIterator1, typename InputIterator2>
bool
range_equal(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 != *first2) return false;
        ++first1;
        ++first2;
    }
    return (first1 == last1) && (first2 == last2);
}

//For testing
bool Game::compare_files(const std::string& filename1,
                         const std::string& filename2)
{
    std::ifstream file1(filename1);
    std::ifstream file2(filename2);

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    std::istreambuf_iterator<char> end;

    return range_equal(begin1, end, begin2, end);
}
