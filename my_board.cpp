
#include "my_board.h"

using namespace std;

my_Board::my_Board()
{}

my_Board::my_Board(unsigned int n, unsigned int m,
                                std::vector<Player *> vectorPlayers):
    m_numOfColumns(n), m_numOfRows(m)
{
    // Add all pieces and moves form the input files to the board
    for (Player *p: vectorPlayers )
    {
        m_playersMap.insert(pair<unsigned int, Player *>
                  (p->getPlayerId(), p));

        if (p->getBadPositioningError().continueFlow())
        {
        addPieceVectorToBoard(p->getPieces());
        addMoveVectorToBoard(p->getPlayerId(), p->getMoves(),
                             vectorPlayers.size());
        }
    }
}

bool my_Board::isPieceLocationWithinRestrictions(Coordinate const coordinate ) const
{
    if (coordinate.my_getX() > m_numOfRows ||
        coordinate.my_getY() > m_numOfColumns ||
        coordinate.my_getX() < 1 ||
        coordinate.my_getY() < 1 )
        return false;

    return true;
}

bool my_Board::isPieceAtLocation(auto const &existingKey, unsigned int playerId) const
{
    if (existingKey == m_piecesOnBoard.end())
        return false;

    if (existingKey->second.size() == 0)
        return false;

    if (existingKey->second[0]->getOwnerId() != playerId)
        return false;

    return true;
}

unsigned int my_Board::isJokerMoveAndChangingValid(Coordinate const toCoordinate,
                             auto const existingKey, Coordinate const jokerLocation) const
{
    // Not relevant for this test
    if (!(existingKey->second[0]->isJoker()))
        return 0;

    // Joker is moved before it can be changed
    if (existingKey->second[0]->getCoordinate() == jokerLocation)
        return -1;

    // Joker is moved and then it is changed
    if (toCoordinate == jokerLocation)
        return 1;

    return 0;
}

void my_Board::addPieceToBoard(Piece* piece)
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
            // If the new piece is from the same owner, it's an error
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

void my_Board::addMoveToBoard(unsigned int const moveNumber,
                    my_Move const &move)
{
    m_moves.insert(pair<unsigned int, my_Move>
                   (moveNumber, move));
}

void my_Board::addPieceVectorToBoard(vector<Piece>* vectorPiece)
{

    for (Piece &p: * vectorPiece )
    {
        my_Board::addPieceToBoard(&p);
    }
}

void my_Board::addMoveVectorToBoard(unsigned int const playerId,
                                 vector<my_Move> const &vectorMove,
                                 unsigned int numberOfPlayers)
{
    // Moves are add separately for each player but enough room
    // is left for the other players to fill in their moves
    int i = 0;
    bool empty = true;
    for (my_Move const &m : vectorMove)
    {
        while (true)
        {
            if  (m_moves.find(i) == m_moves.end())
                empty = true;
            else
                empty = false;
            if (((i % numberOfPlayers) == (playerId - 1)) && empty)
                break;
            i++;
        }
        addMoveToBoard(i, m);
        i++;
    }
}

// To Do - Assumes only two players are in dispute
void my_Board::resolveDisputedCoordinates(){
    for (auto &c : m_disputedCoordinates)
    {
        // Part of the m_disputedCoordinates promise is that
        // the coordinate is on the board
        auto existingKey = m_piecesOnBoard.find(c);

        auto tempFight = my_FightInfo(c,
                *existingKey->second[0],
                *existingKey->second[1]);
        int winner = tempFight.getWinner();

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

void my_Board::makeNextMove(unsigned int m_winnerId)
{

    bool makeJokerMove = false;
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

    auto nextMove = nextMoveSearch->second;

    //cout << nextMove.getPlayerId();
    //cout << nextMove;

    // The moves file has an error but we must play the game until
    // that error is reached
    if (nextMove.getLineNumber() == m_initialMovesErrorLine)
    {
        // If this is the winners turn, the turn is played
        // before the game ends
        if (nextMove.getPlayerId() == m_winnerId)
            delayedNotValid = true;
        // If this is the losers turn he loses immediately
        // (He can't make a turn anyway...)
        else
            {
                m_isValid = false;
                return;
            }
    }

    Coordinate keyCoordinate = nextMove.getFromCoordinate();
    Coordinate keyCoordinateJoker;

    auto existingKey = m_piecesOnBoard.find(keyCoordinate);

    // Can't initialize empty
    auto existingKeyJoker = m_piecesOnBoard.find(keyCoordinate);

    // Check move validity
    if (!(m_isValid &&
        isPieceAtLocation(existingKey, nextMove.getPlayerId()) &&
        existingKey->second[0]->canMove() &&
        isPieceLocationWithinRestrictions(nextMove.getToCoordinate())
         ))
    {
        m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());
        m_isValid = false;
        return;
    }

    // A joker move
    if (m_isValid && nextMove.getIsJokerMove())
    {
        keyCoordinateJoker = nextMove.getJokerLocation();
        existingKeyJoker = m_piecesOnBoard.find(keyCoordinateJoker);

        // Checks if joker is moving and changing in the same turn
        // 1 means good move
        // 0 means joker is not moving - do regular check
        // -1 means bad move
        unsigned int jokerValid = isJokerMoveAndChangingValid(nextMove.getToCoordinate(), existingKey, keyCoordinateJoker);

        if (jokerValid == 1 ||
                 (jokerValid == 0 &&
                 isPieceAtLocation(existingKeyJoker, nextMove.getPlayerId()) &&
                 existingKeyJoker->second[0]->isJoker()))
        {
            makeJokerMove = true;
        }
        else
        {
            m_badMovesInput.addBadMove(nextMove.getPlayerId(), nextMove.getLineNumber());
            m_isValid = false;
            return;
        }
    }

    if (m_isValid)
    {

        //p->increaseNumberOfMovesPlayed();

        auto movingPiece = existingKey->second[0];

        existingKey->second.erase(existingKey->second.begin());

        movingPiece->movePiece(nextMove.getToCoordinate());

        addPieceToBoard(movingPiece);

        resolveDisputedCoordinates();

        if (makeJokerMove)
        {
            // In case the Joker moved
            existingKeyJoker = m_piecesOnBoard.find(keyCoordinateJoker);
            existingKeyJoker->second[0]
                ->changeJokerMask(nextMove.getNewJokerMask());
        }
    }

    if (delayedNotValid)
        m_isValid = false;
}


string my_Board::getBoardPrint()
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


int my_Board::getPlayer(const Point& pos) const
{
    Coordinate keyCoordinate(
                (unsigned int)pos.getX(),
                (unsigned int)pos.getY());
    auto existingKey = m_piecesOnBoard.find(keyCoordinate);

    if (isPieceAtLocation(existingKey, 1))
        return 1;
    else if (isPieceAtLocation(existingKey, 2))
        return 2;
    else
        return 0;
}


// For testing
ostream &operator<<(ostream &os, const my_Board &board)
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
            "\tId of player:"<< move_map.second.getPlayerId() << "\n" <<
            move_map.second;

    }
    os << "Moves played: " << board.m_numOfMovesPlayed;

    return os << endl;
}
