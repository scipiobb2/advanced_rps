
#include "board.h"

using namespace std;

Board::Board()
{}

Board::Board(unsigned int n, unsigned int m,
                                std::vector<Player *> vectorPlayers):
    m_numOfColumns(n), m_numOfRows(m)
{
    // Add all pieces and moves form the input files to the board
    for (Player *p: vectorPlayers )
    {
        if (p->getBadPositioningError().continueFlow())
        {
        addPieceVectorToBoard(p->getPieces());
        addMoveVectorToBoard(p->getPlayerId(), p->getMoves(),
                             vectorPlayers.size());
        }
    }
}

bool Board::isPieceLocationWithinRestrictions(Coordinate const coordinate ) const
{
    if (coordinate.getX() > m_numOfRows ||
        coordinate.getY() > m_numOfColumns ||
        coordinate.getX() < 1 ||
        coordinate.getY() < 1 )
        return false;

    return true;
}

void Board::addPieceToBoard(Piece* piece)
{
    Coordinate keyCoordinate = piece->getCoordinate();

    // Check if added piece is within the board boundaries
    if (!isPieceLocationWithinRestrictions(keyCoordinate))
    {
        piece->setNotValid();
        setNotValid();
        m_badPositioningError
            .addPositioningFault(piece->getOwnerId(),
                                 piece->getReadFromLine());
        return;
    }


    auto existingKey = m_piecesOnBoard.find(keyCoordinate);

    // If the key exists already (Maybe without pieces that died)
    if (existingKey != m_piecesOnBoard.end())
    {
        existingKey->second.push_back(piece);

        // If there was a piece at the same location already
        if (existingKey->second.size() > 1)
        {
            // To Do - Assumes only two players
            // If the new piece is from the same owner that is an error
            if (existingKey->second[0]->getOwnerId() ==
                    existingKey->second[1]->getOwnerId())
            {
                piece->setNotValid();
                setNotValid();
                m_badPositioningError
                    .addPositioningFault(piece->getOwnerId(),
                                         piece->getReadFromLine());
            }
            else
                // Different owners means a fight
                m_disputedCoordinates.push_back(existingKey->first);
        }
    }
    else
    {
        vector<Piece*> value{piece};
        pair<Coordinate, vector<Piece*>> keyValue (keyCoordinate, value);
        m_piecesOnBoard.insert(keyValue);

    }
}

void Board::addMoveToBoard(unsigned int const moveNumber,
                    unsigned int const playerId, Move const &move)
{
    pair<unsigned int, Move> newMove (playerId, move);

    m_moves.insert(pair<unsigned int, pair<unsigned int, Move>>
                   (moveNumber, newMove));
}

void Board::addPieceVectorToBoard(vector<Piece>* vectorPiece)
{

    for (Piece &p: * vectorPiece )
    {
        Board::addPieceToBoard(&p);
    }
}

void Board::addMoveVectorToBoard(unsigned int const playerId,
                                 vector<Move> const &vectorMove,
                                 unsigned int numberOfPlayers)
{
    // Moves are add separately for each player but enough room
    // is left for the other players to fill in their moves
    int i = 0;
    for (Move const &m : vectorMove)
    {
        while ((i % numberOfPlayers) != (playerId - 1))
            i++;
        addMoveToBoard(i, playerId, m);
        i++;
    }
}

// To Do - Assumes only two players are in dispute
void Board::resolveDisputedCoordinates(){
    for (auto &c : m_disputedCoordinates)
    {
        // Part of the m_disputedCoordinates promise is that
        // the coordinate is on the board
        auto existingKey = m_piecesOnBoard.find(c);

        int winner = Advanced_RPS_Fight::twoPiecesFight(
                    existingKey->second[0]->pieceNameForFight(),
                    existingKey->second[1]->pieceNameForFight());
        if (winner == 0)
        {
            existingKey->second[0]->setToDead();
            existingKey->second[1]->setToDead();
            existingKey->second.clear();
        }
        else if (winner == 1)
        {
            existingKey->second[1]->setToDead();
            existingKey->second.erase(existingKey->second.begin() + 1);
        }
        else
        {
            existingKey->second[0]->setToDead();
            existingKey->second.erase(existingKey->second.begin());
        }

        //cout << c << "\t\twinner : " << winner << "\n";
    }

    m_disputedCoordinates.clear();
}

void Board::makeNextMove(unsigned int errorLine, unsigned int m_winnerId)
{
    bool makeJokerMove;
    bool delayedNotValid = false;

    // A safety, to guard against the next while loop
    // going infinity
    if (m_numOfMovesPlayed++ >= m_moves.size())
        return;

    //If one move file is done others may still have moves
    auto nextMoveSearch = m_moves.find(m_moveNumberInMovesMap++);
    while (nextMoveSearch == m_moves.end())
    {
        nextMoveSearch = m_moves.find(m_moveNumberInMovesMap++);
    }

    auto nextMove = nextMoveSearch->second.second;

    //m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());

    //pair<unsigned int, std::string> badMovePair(nextMove.getPlayerId(),
    //"Reason: Bad Moves input file for player " +
    //        to_string(nextMove.getPlayerId())+
    //        " - line " + to_string(nextMove.getLineNumber()) + "\n");

    if (nextMove.getLineNumber() == errorLine)
    {
        if (nextMove.getPlayerId() == m_winnerId)
            delayedNotValid = true;
        else
            {
                m_isValid = false;
                return;
            }
    }

    Coordinate keyCoordinate = nextMove.getFromCoordinate();
    Coordinate keyCoordinateJoker = nextMove.getFromCoordinate();
    auto existingKey = m_piecesOnBoard.find(keyCoordinate);
    auto existingKeyJoker = m_piecesOnBoard.find(keyCoordinate);

    if (existingKey != m_piecesOnBoard.end() && m_isValid)
    {
        if (existingKey->second.size() == 0 ||
            existingKey->second[0]->getOwnerId()
                != nextMove.getPlayerId() ||
            !existingKey->second[0]->canMove() ||
                nextMove.getToCoordinate().getX() > m_numOfRows ||
                nextMove.getToCoordinate().getY() > m_numOfColumns
                )
        {
            m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());
            m_isValid = false;
            return;
        }
    }
    else
    {
        m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());
        m_isValid = false;
        return;
    }

    if (nextMove.getIsJokerMove() && m_isValid)
    {
        keyCoordinateJoker = nextMove.getJokerLocation();
        if (!((nextMove.getToCoordinate() == keyCoordinateJoker)
            && existingKey->second[0]->isJoker()))
        {
            existingKeyJoker = m_piecesOnBoard.find(keyCoordinateJoker);
            if (existingKeyJoker != m_piecesOnBoard.end())
            {
                if (existingKeyJoker->second.size() == 0 ||
                        existingKeyJoker->second[0]->getOwnerId() != nextMove.getPlayerId())
                {
                    m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());
                    m_isValid = false;
                    return;
                }
                else
                    makeJokerMove = true;
            }
            else
            {
                m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());
                m_isValid = false;
                return;
            }
        }
        else
            makeJokerMove = true;

    }

    if (m_isValid)
    {
        auto movingPiece = existingKey->second[0];

        existingKey->second.erase(existingKey->second.begin());

        movingPiece->movePiece(nextMove.getToCoordinate());

        addPieceToBoard(movingPiece);

        resolveDisputedCoordinates();

        if (makeJokerMove)
            existingKeyJoker->second[0]
                ->changeJokerMask(nextMove.getNewJokerMask());
    }

    if (delayedNotValid)
        m_isValid = false;
}

// Broken
void Board::makeXMoves(unsigned int numberOfMoves)
{
    for (unsigned int i = 0; i < numberOfMoves; i++)
        makeNextMove();
}

// Broken
void Board::makeAllMoves()
{
    for (unsigned int i = 0; i < m_moves.size(); i++)
        makeNextMove();
}

string Board::getBoardPrint()
{
    //string returnString = " 12345678910\n";
    string returnString = "";
    for (unsigned int i = 1; i <= m_numOfRows; i++)
    {
        //returnString += to_string(i);
        for (unsigned int j = 1; j <= m_numOfColumns; j++)
        {
            Coordinate keyCoordinate(j, i);
            auto existingKey = m_piecesOnBoard.find(keyCoordinate);
            if (existingKey != m_piecesOnBoard.end())
            {
                if (existingKey->second.size() == 1)
                {
                    char returnChar = existingKey->second[0]->getLetter();
                    if (existingKey->second[0]->getOwnerId() == 2)
                        returnString += tolower(returnChar, locale());
                    else
                        returnString += returnChar;
                }
                else
                    returnString += " ";
            }
            else
                returnString += " ";
        }
        returnString += "\n";
    }
    return returnString;
}

// For testing
ostream &operator<<(ostream &os, const Board &board)
{
    os << "Board pieces:" << "\n";

    int i = 1;
    for (const auto & p: board.m_piecesOnBoard )
    {
        if (p.second.size() > 1)
        {
            os << "  \nThis coordinate has a collision\n";
            for ( auto p2 : p.second)
            {
                os << "    Piece number:" << i++ << "\n" << *p2;
            }
            os << "\n";
        }
        else if (p.second.size() != 0)
        {
            os << "  Piece number:" << i++ << "\n" << *p.second[0];
        }
    }

    os << "\nDisputed coordinates:" << "\n";
    for (const auto & c: board.m_disputedCoordinates )
        os << c;


    os << "Board moves:" << "\n";
    i = 0;
    for (auto const & move_map: board.m_moves )
    {
        os << "\tMove number:" << 1 + i++ << "  " <<
            "\tId of player:"<< move_map.second.first << "\n" <<
            move_map.second.second;

    }
    os << "Moves played: " << board.m_numOfMovesPlayed;

    return os << endl;
}
