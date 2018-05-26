#include "utils.hpp"
#include "player.h"
#include "my_board.h"
#include "game.h"

using namespace std;

int main()
{
    int testMode = 1;
    int NumberOfTests = 28;

    unordered_map<string, unsigned int> restrictions =
    {
        {"maxXSize", N},
        {"maxYSize", M},
        {"numberOfFlagsAllowed", MAX_F},
        {"numberOfPaperAllowed", MAX_P},
        {"numberOfScissorsAllowed", MAX_S},
        {"numberOfRockAllowed", MAX_R},
        {"numberOfBombAllowed", MAX_B},
        {"numberOfJokersAllowed", MAX_J}
    };

    string fileLocationPrefix = "";

    //static_assert(std::is_abstract<my_Board>(),
    //              "my_Board ought to be abstract.");

    while (NumberOfTests > 0)
    {
        fileLocationPrefix = "";

        if (testMode)
            fileLocationPrefix += "test" + to_string(NumberOfTests) + "/";

        Player p1(1, fileLocationPrefix + "player1.rps_board",
                  fileLocationPrefix + "player1.rps_moves",  restrictions);

        if (!p1.getMissingFileError().continueGame())
        {
            cout << p1.getMissingFileError();
            return 0;
        }

        Player p2(2, fileLocationPrefix + "player2.rps_board",
                  fileLocationPrefix + "player2.rps_moves", restrictions);

        if (!p2.getMissingFileError().continueGame())
        {
            cout << p2.getMissingFileError();
            return 0;
        }
        try
        {
            Game game(&p1, &p2, fileLocationPrefix);


            game.playGame();

            if (testMode)
            {
                string textResult = game.compare_files(
                    fileLocationPrefix + "rps.expected",
                    fileLocationPrefix + "rps.output") ?
                    "Identical" : "Not identical";

                cout << "Test " << NumberOfTests
                    << " was ran,  files are " << textResult
                    << endl;
            }

        }
        catch(...)
        {
            cout << "An unexpected error has occurred"
                 << "We are continuing to work on this issue" << endl;
            return 0;
        }

        NumberOfTests--;

        if (!testMode)
            break;
    }


    return 0;
}
