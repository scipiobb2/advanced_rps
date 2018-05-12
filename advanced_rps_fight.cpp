#include "advanced_rps_fight.h"

// return 1 - piece1 is the winner
// return 2 - Piece2 is the winner
// return 0 - A tie
int Advanced_RPS_Fight::twoPiecesFight(PieceName piece1, PieceName piece2)
{
    if (piece1 == piece2) return 0;

    if (piece1 > piece2)
    {
        if (piece1 == PieceName::Bomb)
            return 0;
        else if (piece1 == PieceName::Scissors && piece2 == PieceName::Rock)
            return 2;
        else
            return 1;
    }
    else
    {
        if (piece2 == PieceName::Bomb)
            return 0;
        else if (piece2 == PieceName::Scissors && piece1 == PieceName::Rock)
            return 1;
        else
            return 2;
    }

}

