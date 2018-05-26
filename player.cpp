#include "player.h"

using namespace std;

Player::Player(int id,
            std::string initialPositionsFileName,
            std::string movesFileName,
            unordered_map<string, unsigned int> restrictions):
                m_id(id),
                m_initialPositionsFileName(initialPositionsFileName),
                m_movesFileName(movesFileName)
{
    m_maxXSize = restrictions["maxXSize"];
    m_maxYSize = restrictions["maxYSize"];

    m_pieceCount.insert({PieceName::Flag,
        pair<unsigned int, unsigned int>
            (restrictions["numberOfFlagsAllowed"], 0)});
    m_pieceCount.insert({PieceName::Paper,
        pair<unsigned int, unsigned int>
            (restrictions["numberOfPaperAllowed"], 0)});
    m_pieceCount.insert({PieceName::Scissors,
        pair<unsigned int, unsigned int>
            (restrictions["numberOfScissorsAllowed"], 0)});
    m_pieceCount.insert({PieceName::Rock,
        pair<unsigned int, unsigned int>
            (restrictions["numberOfRockAllowed"], 0)});
    m_pieceCount.insert({PieceName::Bomb,
        pair<unsigned int, unsigned int>
            (restrictions["numberOfBombAllowed"], 0)});
    m_pieceCount.insert({PieceName::Joker,
        pair<unsigned int, unsigned int>
            (restrictions["numberOfJokersAllowed"], 0)});

    readInitialPosition();
    if (m_missingFileError.continueFlow())
        readMoves();
}

unsigned int Player::getLocationOrZeroFromToken(string const token,
                                        unsigned int const maxSize) const
{
    unsigned int tempNumber;

    if (is_number(token))
    {
        tempNumber = (unsigned int)stoi(token);
        if (tempNumber > 0 && tempNumber <= maxSize)
            return tempNumber;
        else
            return 0;
    }
    else
        return 0;
}

bool Player::isPositioningFileLineValid
    (std::vector<std::string> const tokens,
     unordered_map<string, PieceName> PieceNameMap) const
{
    unsigned int tokensSize = tokens.size();
    PieceName pieceNameOrZero;

    // Size 3 is a regular piece, size 4 is a joker
    if (tokensSize != 3 && tokensSize != 4)
        return false;

    // The second and third arguments must be the location of the piece
    // Pieces must be located within defined limits
    if (!getLocationOrZeroFromToken(tokens[1], m_maxXSize))
        return false;

    if (!getLocationOrZeroFromToken(tokens[2], m_maxYSize))
        return false;

    pieceNameOrZero = PieceNameMap[tokens[0]];

    // Must be one of the predefined Pieces
    if (pieceNameOrZero < PieceName::Flag)
        return false;

    // Jokers mask can't be a flag but must be one of the other predefined Pieces
    if (tokensSize == 4 && PieceNameMap[tokens[3]] <= PieceName::Flag)
        return false;

    return true;
}

bool Player::isMovesFileLineValid
    (std::vector<std::string> const tokens,
     unordered_map<string, PieceName> PieceNameMap) const
{
    unsigned int tokensSize = tokens.size();
    unsigned int fromXCoordinate;
    unsigned int fromYCoordinate;
    unsigned int toXCoordinate;
    unsigned int toYCoordinate;
    unsigned int jokerXCoordinate;
    unsigned int jokerYCoordinate;

    // Size 4 is moving a piece,
    // Size 8 is moving a piece and changing a joker's mask
    if ((tokensSize != 4 && tokensSize != 8))
        return false;

    // Mandatory syntax for changing a joker's mask
    if (tokensSize == 8 && tokens[4] != "J:")
        return false;

    // 0-3 arguments must be the "from" and "to" locations of the piece
    fromXCoordinate = getLocationOrZeroFromToken(tokens[0], m_maxXSize);
    if (!fromXCoordinate)
        return false;

    fromYCoordinate = getLocationOrZeroFromToken(tokens[1], m_maxYSize);
    if (!fromYCoordinate)
        return false;

    toXCoordinate = getLocationOrZeroFromToken(tokens[2], m_maxXSize);
    if (!toXCoordinate)
        return false;

    toYCoordinate = getLocationOrZeroFromToken(tokens[3], m_maxYSize);
    if (!toYCoordinate)
        return false;

    // A piece can't move more then one step to the side
    // No diagonal movement is allowed
    // Staying in place is not allowed
    if (abs((int)(fromXCoordinate - toXCoordinate)) +
        abs((int)(fromYCoordinate - toYCoordinate)) != 1)
        return false;

    // Size 8 is moving a piece and changing a joker's mask
    if (tokens.size() == 8)
    {
        // 5-6 arguments must be location of the joker
        jokerXCoordinate = getLocationOrZeroFromToken(tokens[5], m_maxXSize);
        if (!jokerXCoordinate)
            return false;

        jokerYCoordinate = getLocationOrZeroFromToken(tokens[6], m_maxYSize);
        if (!jokerYCoordinate)
            return false;

        // Jokers mask can't be a flag but must be one of the other predefined Pieces
        if (PieceNameMap[tokens[7]] <= PieceName::Flag)
            return false;
    }

    return true;
}

void Player::readInitialPosition()
{
    unordered_map<string, PieceName> PieceNameMap = getInputLettersToPiecesMap();
    string line;
    ifstream myfile (m_initialPositionsFileName);
    std::set<Coordinate> CoordinatesOfPiecesAdded;

    if (myfile.is_open())
    {
        int i = 1;
        while ( getline (myfile, line) )
        {
            // Empty lines are allowed
            if (line == "")
                continue;
            istringstream iss(line);
            vector<string> tokens{istream_iterator<string>{iss},
                                  istream_iterator<string>{}};
            if (tokens.size() == 0)
                continue;

            if (!isPositioningFileLineValid(tokens, PieceNameMap))
            {
                m_badPositioningError.addPositioningFault(m_id, i);
                return;
            }

            // Check if the location is already occupied
            auto existingKey = CoordinatesOfPiecesAdded
                                        .find(Coordinate(tokens[1], tokens[2]));

            if (existingKey != CoordinatesOfPiecesAdded.end())
            {
                m_badPositioningError.addPositioningFault(m_id, i);
                return;
            }

            try
            {
                PieceName pieceName = PieceNameMap[tokens[0]];

                updateCountPieces(pieceName);

                // Check if number of pieces add exceeds limitations
                if (m_pieceCount[pieceName].second >
                        m_pieceCount[pieceName].first)
                {
                    m_badPositioningError.addPositioningFault(m_id, i);
                    return;
                }

                if (tokens.size() == 4)
                {
                    m_pieces.push_back(Piece(pieceName, tokens[0][0],
                            Coordinate(tokens[1], tokens[2]), m_id, i,
                            true,
                            PieceNameMap[tokens[3]]));
                    CoordinatesOfPiecesAdded.insert(Coordinate(tokens[1], tokens[2]));
                }
                else
                {

                    m_pieces.push_back(Piece(pieceName,tokens[0][0],
                            Coordinate(tokens[1], tokens[2]), m_id, i));
                    CoordinatesOfPiecesAdded.insert(Coordinate(tokens[1], tokens[2]));
                }
            }
            catch(...)
            {
                m_badPositioningError.addPositioningFault(m_id, i);
                return;
            }
            i++;
        }
        myfile.close();
    }

    else
    {
        m_missingFileError.addBadFile(m_id, m_initialPositionsFileName);
    }
}

void Player::readMoves()
{
    unordered_map<string, PieceName> PieceNameMap = getInputLettersToPiecesMap();
    string line;
    ifstream myfile (m_movesFileName);
    if (myfile.is_open())
    {
        int i = 1;
        while ( getline (myfile, line) )
        {
            // Empty lines are allowed
            if (line == "")
                continue;
            istringstream iss(line);
            vector<string> tokens{istream_iterator<string>{iss},
                                  istream_iterator<string>{}};

            if (tokens.size() == 0)
                continue;

            if (!isMovesFileLineValid(tokens, PieceNameMap))
            {
                m_badMovesInput.addBadMove(m_id, i);
                return;
            }

            try
            {
                if (tokens.size() == 8)
                {
                    m_moves.push_back(my_Move(
                    Coordinate(tokens[0], tokens[1]),
                    Coordinate(tokens[2], tokens[3]),
                    m_id, i,
                    Coordinate(tokens[5], tokens[6]),
                    PieceNameMap[tokens[7]]));
                }
                else
                {
                    m_moves.push_back(my_Move(Coordinate(tokens[0], tokens[1]),
                        Coordinate(tokens[2], tokens[3]), m_id, i));
                }
            }
            catch(...)
            {
                m_badMovesInput.addBadMove(m_id, i);
                return;
            }
            i++;
        }
        myfile.close();
    }

    else
        m_missingFileError.addBadFile(m_id, m_movesFileName);

}

void Player::updateCountPieces(PieceName pieceName, int difference)
{
    auto newPair = m_pieceCount[pieceName];
    newPair.second += difference;
    m_pieceCount[pieceName] = newPair;
}

void Player::updateAlivePiecesCount()
{
    unsigned int new_numberOfPiecesAlive = 0;
    unsigned int new_numberOfFlagsAlive = 0;
    unsigned int numberOfPiecesThatCanMove = 0;
    for (auto p : m_pieces)
    {
        if (p.getIsAlive())
        {
            new_numberOfPiecesAlive++;
            if (p.isFlag())
                new_numberOfFlagsAlive++;
            if (p.canMove())
                numberOfPiecesThatCanMove++;
        }
    }

    m_numberOfPiecesAlive = new_numberOfPiecesAlive;
    m_numberOfFlagsAlive = new_numberOfFlagsAlive;
    m_numberOfPiecesThatCanMove = numberOfPiecesThatCanMove;
}

void Player::getInitialPositions(int player,
    std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{
    if (player == (int) m_id)
    {
        for (auto p : m_pieces)
        {
            vectorToFill.push_back( make_unique<Piece>(p));
        }
    }
}

unique_ptr<Move> Player::getMove()
{
    unsigned int i = 0;
    for (auto m : m_moves)
    {
        if (m_numberOfMovesPlayed == i++)
            return make_unique<my_Move>(m);
    }

    // Should not get here
    return nullptr;
}

my_Move * Player::getFileMove()
{
    if (m_numberOfMovesPlayed >= m_moves.size())
        return NULL;

    return &m_moves[m_numberOfMovesPlayed];
}

unique_ptr<JokerChange> Player::getJokerChange()
{
    unsigned int i = 0;
    for (auto m : m_moves)
    {
        if (m_numberOfMovesPlayed == i++)
        {
            if (m.getIsJokerMove())
                return make_unique<my_Move>(m);
            else
                return nullptr;
        }
    }

    // Should not get here
    return nullptr;
}

ostream &operator<<(std::ostream &os,
                                const Player &player)
{
    os << "Player id:" << player.m_id << "\n"
        << "Player's points:" << player.m_points << "\n\n"
        << "Player's number of pieces alive:" << player.m_numberOfPiecesAlive << "\n\n"
        << "Player's number of flags alive:" << player.m_numberOfFlagsAlive << "\n\n";

    os << "Player's pieces:\n ";
    int i = 1;
    for (auto &p: player.m_pieces )
    {
        os << "  Pieces number:" << i << "\n"
            << p;
        i++;
    }

    os << "\nPlayer's moves:\n ";
    i = 1;
    for (auto &m: player.m_moves )
    {
        os << "\tmove number:" << i << "\n"
            << m;
        i++;
    }
    return os << endl;
}

