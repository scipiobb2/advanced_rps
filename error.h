#ifndef ERROR_H
#define ERROR_H
#include "utils.hpp"

class Error
{
public:
    Error();

    bool continueFlow() const {return !m_stopFlow;}
    bool continueGame() const {return !m_stopGame;}

    std::vector<std::pair<unsigned int, unsigned int>>
        getPlayersFaults() const {return m_idsAndLinesOfPlayersInFault;}

protected:
    void addPlayerFault(unsigned int playerId, unsigned int line);

    ErrorType m_errorType{};
    bool m_stopGame{};
    bool m_stopFlow{};

private:
    std::vector<std::pair<unsigned int, unsigned int>>
        m_idsAndLinesOfPlayersInFault{};


};

class MissingFileError : public Error
{
public:
    using Error::Error;
    MissingFileError();

    void addBadFile(unsigned int playerId, std::string fileName);

    friend std::ostream &operator<<(std::ostream &os,
            const MissingFileError &missingFileError);
private:
    unsigned int m_playerId{};
    std::string m_fileName{};
};

class BadPositioningError : public Error
{
public:
    using Error::Error;
    BadPositioningError();

    void addPositioningFault(unsigned int playerId, unsigned int line);

    friend std::ostream &operator<<(std::ostream &os,
            const BadPositioningError &badPositioningError);
};

class BadMovesInputError : public Error
{
public:
    using Error::Error;
    BadMovesInputError();

    void addBadMove(unsigned int playerId, unsigned int line);

    friend std::ostream &operator<<(std::ostream &os,
            const BadMovesInputError &badMovesInput);
};


#endif // ERROR_H
