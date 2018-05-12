#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <map>
#include <stdlib.h>
#include <set>
#include <cmath>

#define N 10
#define M 10
#define MAX_R 2
#define MAX_P 5
#define MAX_S 1
#define MAX_B 2
#define MAX_J 2
#define MAX_F 1

enum class PieceName
    {Flag = 1, Rock, Paper, Scissors, Joker, Bomb};

namespace std
{
    template<> struct hash<PieceName>
    {

        size_t operator()(PieceName const& pieceName) const
        {
          return std::hash<int>()(static_cast <int>(pieceName));
        }
    };
}

inline std::ostream& operator << (std::ostream& os,
                                  const PieceName& pieceName)
{
   if (pieceName == PieceName::Bomb) return os << "Bomb";
   else if (pieceName == PieceName::Flag) return os << "Flag";
   else if (pieceName == PieceName::Joker) return os << "Joker";
   else if (pieceName == PieceName::Paper) return os << "Paper";
   else if (pieceName == PieceName::Rock) return os << "Rock";
   else if (pieceName == PieceName::Scissors) return os << "Scissors";
   else return os << "";
}

enum class ErrorType
    {BadFile = 1, BadPositioningFile, BadMovesInputError};


inline bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

#endif // UTILS_HPP
