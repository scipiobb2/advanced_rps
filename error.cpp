#include "error.h"

using namespace std;

Error::Error()
{
    m_stopGame = false;
    m_stopFlow = false;
}

void Error::addPlayerFault(unsigned int playerId, unsigned int line)
{
    pair<unsigned int, unsigned int> newPair(playerId, line);
    m_idsAndLinesOfPlayersInFault.push_back(newPair);
}

MissingFileError::MissingFileError(): Error()
{
    m_errorType = ErrorType::BadFile;
}

void MissingFileError::addBadFile (unsigned int playerId,
                                    string fileName)
{
    m_playerId = playerId;
    m_fileName = fileName;
    m_stopGame = true;
    m_stopFlow = true;

}



BadPositioningError::BadPositioningError(): Error()
{
    m_errorType = ErrorType::BadPositioningFile;
}

void BadPositioningError::addPositioningFault(unsigned int playerId, unsigned int line){
    addPlayerFault(playerId, line);
    m_stopGame = false;
    m_stopFlow = true;
}



BadMovesInputError::BadMovesInputError(): Error()
{
    m_errorType = ErrorType::BadMovesInputError;
}

void BadMovesInputError::addBadMove(unsigned int playerId, unsigned int line){
    addPlayerFault(playerId, line);
    m_stopGame = false;
    m_stopFlow = true;
}


ostream &operator<<(std::ostream &os,
        const MissingFileError &missingFileError)
{
    os << "File: " << missingFileError.m_fileName
       << " is expected from player "
       << missingFileError.m_playerId << "\n"
       << "But it is missing or can not be opened\n"
       << "The game will not continue" ;
    return os << endl;
}

ostream &operator<<(std::ostream &os,
        const BadPositioningError &badPositioningError)
{
    os << "Reason: Bad Positioning input file for player "
       << badPositioningError.getPlayersFaults()[0].first
       << " - line " << badPositioningError.getPlayersFaults()[0].second;
       //<< "\n";
    return os << endl;
}

ostream &operator<<(std::ostream &os,
        const BadMovesInputError &badMovesInput)
{
    os << "Reason: Bad Moves input file for player "
       << badMovesInput.getPlayersFaults()[0].first
       << " - line " << badMovesInput.getPlayersFaults()[0].second;
       //<< "\n";
    return os << endl;
}
