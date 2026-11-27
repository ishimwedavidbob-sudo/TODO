#include <iostream>
using namespace std;

// Global board
char board[10] = {'0','1','2','3','4','5','6','7','8','9'};

// Function prototypes
void displayBoard();
bool isWinner();
bool isDraw();
void resetBoard();

int main()
{
    string player1, player2;
    int score1 = 0, score2 = 0;
    char replay;

    cout << "=====================================\n";
    cout << "      TIC TAC TOE GAME \n";
    cout << "=====================================\n\n";

    cout << "Enter Player 1 Name (X): ";
    cin >> player1;

    cout << "Enter Player 2 Name (O): ";
    cin >> player2;

    do
    {
        resetBoard();

        int player = 1;
        int choice;
        char mark;
        bool gameOver = false;

        while (!gameOver)
        {
            displayBoard();

            if (player == 1)
                cout << player1 << "'s turn (X): ";
            else
                cout << player2 << "'s turn (O): ";

            cin >> choice;

            mark = (player == 1) ? 'X' : 'O';

            // Validate move
            if (choice >= 1 && choice <= 9 &&
                board[choice] != 'X' &&
                board[choice] != 'O')
            {
                board[choice] = mark;
            }
            else
            {
                cout << "\n Invalid move! Try again.\n";
                continue;
            }

            // Check winner
            if (isWinner())
            {
                displayBoard();

                if (player == 1)
                {
                    cout << "\n Congratulations "
                         << player1
                         << "! You Win!\n";
                    score1++;
                }
                else
                {
                    cout << "\n Congratulations "
                         << player2
                         << "! You Win!\n";
                    score2++;
                }

                gameOver = true;
            }
            else if (isDraw())
            {
                displayBoard();
                cout << "\n Game Draw!\n";
                gameOver = true;
            }
            else
            {
                player = (player == 1) ? 2 : 1;
            }
        }

        cout << "\n========== SCOREBOARD ==========\n";
        cout << player1 << ": " << score1 << endl;
        cout << player2 << ": " << score2 << endl;
        cout << "================================\n";

        cout << "\nPlay Again? (Y/N): ";
        cin >> replay;

    } while (replay == 'Y' || replay == 'y');

    cout << "\n Thanks for Playing!\n";

    return 0;
}

// Display the game board
void displayBoard()
{
    cout << "\n\n";
    cout << "     TIC TAC TOE\n\n";

    cout << "     |     |     \n";
    cout << "  " << board[1] << "  |  "
         << board[2] << "  |  "
         << board[3] << endl;

    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";

    cout << "  " << board[4] << "  |  "
         << board[5] << "  |  "
         << board[6] << endl;

    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";

    cout << "  " << board[7] << "  |  "
         << board[8] << "  |  "
         << board[9] << endl;

    cout << "     |     |     \n\n";
}

// Check if a player has won
bool isWinner()
{
    return (

        board[1] == board[2] && board[2] == board[3] ||
        board[4] == board[5] && board[5] == board[6] ||
        board[7] == board[8] && board[8] == board[9] ||

        board[1] == board[4] && board[4] == board[7] ||
        board[2] == board[5] && board[5] == board[8] ||
        board[3] == board[6] && board[6] == board[9] ||

        board[1] == board[5] && board[5] == board[9] ||
        board[3] == board[5] && board[5] == board[7]
    );
}

// Check for draw
bool isDraw()
{
    for (int i = 1; i <= 9; i++)
    {
        if (board[i] != 'X' && board[i] != 'O')
            return false;
    }
    return true;
}

// Reset board for replay
void resetBoard()
{
    for (int i = 1; i <= 9; i++)
    {
        board[i] = '0' + i;
    }
}