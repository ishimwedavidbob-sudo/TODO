// ============================================================================
//  HANGMAN DELUXE  -  Refactored Edition
//  Developed by: Your Name
//
//  Features
//    * Three categories  : Technology, Animals, Countries
//    * Three levels      : Easy, Medium, Hard (longer words, fewer lives,
//                          higher points, costly hints)
//    * 30 words per category (10 per level), each with its own hint
//    * Two game modes    : Player vs Computer, Player vs Player
//    * Gender-aware      : the hanged figure is male or female, matching the
//                          player who is being hanged
//    * Coloured, loop-drawn gallows and figure
//    * Persistent, sorted high-score table (scores.txt)
//
//  Build : g++ -std=c++17 -O2 -Wall -Wextra hangman_deluxe.cpp -o hangman
// ============================================================================

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ----------------------------------------------------------------------------
//  Constants
// ----------------------------------------------------------------------------
namespace
{
    // ANSI colours
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* RESET   = "\033[0m";

    constexpr int    BODY_PARTS     = 6;      // head, body, 2 arms, 2 legs
    constexpr int    SCENE_ROWS     = 9;
    constexpr int    SCENE_COLS     = 14;
    constexpr int    POINTS_LETTER  = 10;     // per correct letter (x level)
    constexpr int    POINTS_BONUS   = 50;     // per word solved    (x level)
    constexpr size_t MAX_NAME_LEN   = 20;
    constexpr size_t TOP_SCORES     = 10;
    const char*      SCORE_FILE     = "scores.txt";
}

// ----------------------------------------------------------------------------
//  Data types
// ----------------------------------------------------------------------------
enum class Gender { Male = 0, Female = 1 };

struct Player
{
    string name;
    Gender gender = Gender::Male;
    int    score  = 0;
    int    streak = 0;
};

struct Entry                     // one word and its hint
{
    const char* word;
    const char* hint;
};

struct Level                     // one difficulty level
{
    const char* name;
    int lives;
    int hintCost;                // points removed when a hint is used
    int multiplier;              // score multiplier
};

struct Cell                      // one character of the hangman picture
{
    int  row;
    int  col;
    char ch;
};

using PartList = vector<Cell>;

// ----------------------------------------------------------------------------
//  Game data
// ----------------------------------------------------------------------------
const Level LEVELS[3] =
{
    { "Easy",   10,  0, 1 },
    { "Medium",  7, 10, 2 },
    { "Hard",    5, 20, 3 }
};

const char* const CATEGORY_NAMES[3] = { "Technology", "Animals", "Countries" };

// WORDS[category][level]  -  Easy: 4-6 letters, Medium: 7-8, Hard: 9+
const vector<Entry> WORDS[3][3] =
{
    {   // ---------------- TECHNOLOGY ----------------
        {   // Easy
            { "code",   "Instructions written for a computer" },
            { "mouse",  "You click with it" },
            { "cloud",  "Online storage in the sky" },
            { "email",  "Electronic mail" },
            { "robot",  "A programmable machine" },
            { "byte",   "Eight bits" },
            { "wifi",   "Wireless internet" },
            { "pixel",  "Smallest dot on a screen" },
            { "login",  "Enter your account" },
            { "laptop", "Portable computer" }
        },
        {   // Medium
            { "computer", "Electronic machine" },
            { "internet", "Global network of networks" },
            { "database", "Organised collection of data" },
            { "network",  "Connected computers" },
            { "software", "Programs and apps" },
            { "hardware", "Physical parts of a computer" },
            { "browser",  "Chrome or Firefox" },
            { "keyboard", "You type on it" },
            { "firewall", "Blocks unwanted network traffic" },
            { "password", "Secret word to log in" }
        },
        {   // Hard
            { "programming",   "Writing code" },
            { "algorithm",     "Step-by-step procedure" },
            { "encryption",    "Scrambling data to keep it secret" },
            { "bandwidth",     "Data capacity of a connection" },
            { "developer",     "Person who builds software" },
            { "cybersecurity", "Protecting systems from attacks" },
            { "processor",     "The brain of the computer" },
            { "motherboard",   "Main circuit board" },
            { "application",   "Program for a user task" },
            { "javascript",    "Language of web pages" }
        }
    },
    {   // ---------------- ANIMALS ----------------
        {   // Easy
            { "lion",  "King of the jungle" },
            { "zebra", "Striped African horse relative" },
            { "tiger", "Big striped cat" },
            { "panda", "Black and white bamboo eater" },
            { "camel", "Desert animal with humps" },
            { "koala", "Australian eucalyptus eater" },
            { "hippo", "Large river-dwelling African mammal" },
            { "whale", "Largest ocean mammal" },
            { "shark", "Ocean predator with fins" },
            { "eagle", "Bird of prey" }
        },
        {   // Medium
            { "elephant", "Largest land animal" },
            { "giraffe",  "Tallest animal" },
            { "kangaroo", "Hops and carries its young in a pouch" },
            { "leopard",  "Spotted big cat" },
            { "buffalo",  "Wild horned grazer" },
            { "gorilla",  "Largest primate, found in Rwanda" },
            { "cheetah",  "Fastest land animal" },
            { "penguin",  "Flightless bird of the cold" },
            { "dolphin",  "Intelligent ocean mammal" },
            { "ostrich",  "Largest bird" }
        },
        {   // Hard
            { "chimpanzee",   "Intelligent ape, close to humans" },
            { "rhinoceros",  "Horned heavyweight" },
            { "hippopotamus", "River horse" },
            { "crocodile",   "Large reptile with powerful jaws" },
            { "orangutan",   "Red-haired ape of Asia" },
            { "salamander",  "Amphibian that resembles a lizard" },
            { "chameleon",   "Changes its colour" },
            { "armadillo",   "Armoured mammal" },
            { "porcupine",   "Spiny rodent" },
            { "alligator",   "Crocodile's cousin" }
        }
    },
    {   // ---------------- COUNTRIES ----------------
        {   // Easy
            { "rwanda", "Land of a Thousand Hills" },
            { "uganda", "The Pearl of Africa" },
            { "kenya",  "Home of the Maasai Mara" },
            { "canada", "Maple leaf country" },
            { "japan",  "Land of the Rising Sun" },
            { "china",  "Home of the Great Wall" },
            { "india",  "Home of the Taj Mahal" },
            { "egypt",  "Land of the pyramids" },
            { "ghana",  "First sub-Saharan country to gain independence" },
            { "spain",  "Famous for flamenco and paella" }
        },
        {   // Medium
            { "tanzania", "Home of Mount Kilimanjaro" },
            { "burundi",  "Neighbour of Rwanda to the south" },
            { "nigeria",  "Most populous African country" },
            { "ethiopia", "Birthplace of coffee" },
            { "germany",  "Country of Berlin" },
            { "morocco",  "North African country with Marrakech" },
            { "senegal",  "West African country with Dakar" },
            { "vietnam",  "Country of Hanoi" },
            { "thailand", "Land of Smiles" },
            { "pakistan", "Country of Islamabad" }
        },
        {   // Hard
            { "argentina",   "Birthplace of the tango" },
            { "australia",   "Land Down Under" },
            { "madagascar",  "Large island off Africa's east coast" },
            { "mozambique",  "Southeast African coastal country" },
            { "switzerland", "Famous for watches and chocolate" },
            { "netherlands", "Land of tulips and windmills" },
            { "philippines", "Island nation of over 7,000 islands" },
            { "indonesia",   "Largest archipelago in the world" },
            { "venezuela",   "Home of Angel Falls" },
            { "kazakhstan",  "Largest landlocked country" }
        }
    }
};

// FIGURE[gender][part] : the characters that make up each body part.
// Parts are revealed in this order: head, body, left arm, right arm,
// left leg, right leg.  The female figure wears a dress.
const PartList FIGURE[2][BODY_PARTS] =
{
    {   // ----- Male -----
        { {2, 8, 'O'} },
        { {3, 8, '|'}, {4, 8, '|'} },
        { {3, 7, '/'} },
        { {3, 9, '\\'} },
        { {5, 7, '/'},  {6, 6, '/'} },
        { {5, 9, '\\'}, {6, 10, '\\'} }
    },
    {   // ----- Female -----
        { {2, 7, '('}, {2, 8, 'O'}, {2, 9, ')'} },
        { {3, 8, '|'}, {4, 7, '/'}, {4, 9, '\\'},
          {5, 6, '/'}, {5, 7, '_'}, {5, 8, '_'}, {5, 9, '_'}, {5, 10, '\\'} },
        { {3, 7, '/'} },
        { {3, 9, '\\'} },
        { {6, 7, '|'} },
        { {6, 9, '|'} }
    }
};

mt19937 rng{ random_device{}() };

// ----------------------------------------------------------------------------
//  Console helpers
// ----------------------------------------------------------------------------
void enableAnsiColours()
{
#ifdef _WIN32
    HANDLE out  = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  mode = 0;
    if (GetConsoleMode(out, &mode))
        SetConsoleMode(out, mode | 0x0004);   // ENABLE_VIRTUAL_TERMINAL_PROCESSING
#endif
}

string trim(const string& text)
{
    const size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    const size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

string readLine()
{
    string line;
    if (!getline(cin, line))        // input closed: leave the program cleanly
    {
        cout << "\nGoodbye!\n";
        exit(0);
    }
    return trim(line);
}

// Asks until the user types a whole number between low and high.
int readInt(const string& prompt, int low, int high)
{
    while (true)
    {
        cout << prompt;
        const string line = readLine();

        try
        {
            size_t used = 0;
            const int value = stoi(line, &used);
            if (used == line.size() && value >= low && value <= high)
                return value;
        }
        catch (...) { /* not a number: fall through */ }

        cout << RED << "Please enter a number from " << low << " to " << high
             << "." << RESET << endl;
    }
}

bool readYesNo(const string& prompt)
{
    while (true)
    {
        cout << prompt;
        const string line = readLine();
        if (line.size() == 1 && (tolower(line[0]) == 'y' || tolower(line[0]) == 'n'))
            return tolower(line[0]) == 'y';
        cout << RED << "Please answer y or n." << RESET << endl;
    }
}

const char* genderLabel(Gender gender)
{
    return gender == Gender::Male ? "Male" : "Female";
}

const char* genderColour(Gender gender)
{
    return gender == Gender::Male ? CYAN : MAGENTA;
}

// ----------------------------------------------------------------------------
//  Screens
// ----------------------------------------------------------------------------
void displayTitle()
{
    cout << CYAN << BOLD << R"ART(

 _   _    _    _   _  ____ __  __    _    _   _
| | | |  / \  | \ | |/ ___|  \/  |  / \  | \ | |
| |_| | / _ \ |  \| | |  _| |\/| | / _ \ |  \| |
|  _  |/ ___ \| |\  | |_| | |  | |/ ___ \| |\  |
|_| |_/_/   \_\_| \_|\____|_|  |_/_/   \_\_| \_|
)ART" << RESET;

    cout << YELLOW << "                  D E L U X E   E D I T I O N\n" << RESET;
}

int menu()
{
    cout << "\n===== MAIN MENU =====\n"
         << "1. Play Game\n"
         << "2. High Scores\n"
         << "3. Exit\n";
    return readInt("Choose Option: ", 1, 3);
}

int chooseCategory()
{
    cout << "\nChoose Category\n";
    for (int i = 0; i < 3; i++)
        cout << i + 1 << ". " << CATEGORY_NAMES[i] << "\n";
    return readInt("Choice: ", 1, 3) - 1;
}

int chooseDifficulty()
{
    cout << "\nChoose Difficulty\n";
    for (int i = 0; i < 3; i++)
        cout << i + 1 << ". " << LEVELS[i].name << "  ("
             << LEVELS[i].lives << " lives, x" << LEVELS[i].multiplier
             << " points)\n";
    return readInt("Choice: ", 1, 3) - 1;
}

Player createPlayer(const string& namePrompt)
{
    Player player;

    cout << namePrompt;
    player.name = readLine().substr(0, MAX_NAME_LEN);
    if (player.name.empty())
        player.name = "Player";

    cout << "\nSelect gender (this decides the figure on the gallows)\n"
         << "1. Male\n"
         << "2. Female\n";
    player.gender = (readInt("Choice: ", 1, 2) == 1) ? Gender::Male : Gender::Female;

    return player;
}

// ----------------------------------------------------------------------------
//  Drawing
// ----------------------------------------------------------------------------
// Draws the gallows and the first `parts` body parts of the figure that
// matches `gender`.  A dead figure is drawn in red.
void drawHangman(Gender gender, int parts, bool dead)
{
    vector<string> gallows(SCENE_ROWS, string(SCENE_COLS, ' '));
    vector<string> figure (SCENE_ROWS, string(SCENE_COLS, ' '));

    // gallows
    gallows[0][1] = '+';
    for (int c = 2; c < 8; c++)
        gallows[0][c] = '-';
    gallows[0][8] = '+';
    gallows[1][2] = '/';                       // support brace
    gallows[1][8] = '|';                       // rope
    for (int r = 1; r < SCENE_ROWS - 1; r++)
        gallows[r][1] = '|';                   // pole
    for (int c = 0; c < SCENE_COLS; c++)
        gallows[SCENE_ROWS - 1][c] = '=';      // ground

    // figure
    const int g = static_cast<int>(gender);
    for (int part = 0; part < parts && part < BODY_PARTS; part++)
        for (const Cell& cell : FIGURE[g][part])
            figure[cell.row][cell.col] = cell.ch;

    const char* figureColour = dead ? RED : genderColour(gender);

    cout << "\n";
    for (int r = 0; r < SCENE_ROWS; r++)
    {
        cout << "      ";
        for (int c = 0; c < SCENE_COLS; c++)
        {
            if (figure[r][c] != ' ')
                cout << figureColour << BOLD << figure[r][c] << RESET;
            else if (gallows[r][c] != ' ')
                cout << YELLOW << gallows[r][c] << RESET;
            else
                cout << ' ';
        }
        cout << "\n";
    }
}

void displayWord(const string& guessed)
{
    cout << "\nWord: " << BOLD;
    for (char c : guessed)
        cout << c << " ";
    cout << RESET << endl;
}

void showLives(int lives)
{
    cout << "\nLives: " << RED;
    for (int i = 0; i < lives; i++)
        cout << "* ";
    cout << RESET << endl;
}

void showStatistics(int correct, int wrong)
{
    const double accuracy = (correct + wrong > 0)
                          ? 100.0 * correct / (correct + wrong) : 0.0;

    cout << "\n===== GAME STATS =====\n"
         << "Correct Guesses: " << correct << endl
         << "Wrong Guesses: "   << wrong   << endl
         << "Accuracy: " << fixed << setprecision(1) << accuracy << "%" << endl;
}

void achievement(int score)
{
    if (score >= 50)  cout << "\n First Champion!\n";
    if (score >= 100) cout << "\n Word Master!\n";
    if (score >= 200) cout << "\n Hangman Legend!\n";
}

// ----------------------------------------------------------------------------
//  Words
// ----------------------------------------------------------------------------
// Picks a random word that has not been used yet at this category/level.
// When every word has been used the cycle starts again.
const Entry& pickEntry(int category, int level)
{
    static set<size_t> used[3][3];

    const vector<Entry>& list = WORDS[category][level];
    set<size_t>& taken = used[category][level];

    if (taken.size() == list.size())
        taken.clear();

    uniform_int_distribution<size_t> pick(0, list.size() - 1);
    size_t index;
    do { index = pick(rng); } while (taken.count(index));

    taken.insert(index);
    return list[index];
}

// ----------------------------------------------------------------------------
//  Scores
// ----------------------------------------------------------------------------
void saveScore(const Player& player)
{
    if (player.score <= 0)
        return;

    string name = player.name;
    replace(name.begin(), name.end(), ' ', '_');   // keeps the file one-word-per-name

    ofstream file(SCORE_FILE, ios::app);
    if (file)
        file << name << " " << player.score << endl;
}

void showScores()
{
    ifstream file(SCORE_FILE);
    vector<pair<int, string>> table;

    string name;
    int score;
    while (file >> name >> score)
    {
        replace(name.begin(), name.end(), '_', ' ');
        table.emplace_back(score, name);
    }

    sort(table.begin(), table.end(),
         [](const pair<int, string>& a, const pair<int, string>& b)
         { return a.first > b.first; });

    cout << "\n===== HIGH SCORES =====\n";
    if (table.empty())
        cout << "No scores yet. Be the first!\n";

    for (size_t i = 0; i < table.size() && i < TOP_SCORES; i++)
        cout << table[i].second << " : " << table[i].first << endl;
}

// ----------------------------------------------------------------------------
//  One round: `player` is the one being hanged
// ----------------------------------------------------------------------------
void playRound(Player& player, int category, int level)
{
    const Level& rules  = LEVELS[level];
    const Entry& entry  = pickEntry(category, level);
    const string secret = entry.word;

    string shown(secret.size(), '_');
    string guessedLetters;
    string wrongLetters;

    int  lives        = rules.lives;
    int  correctCount = 0;
    int  wrongCount   = 0;
    bool hintUsed     = false;

    cout << "\n" << genderColour(player.gender) << BOLD
         << player.name << " (" << genderLabel(player.gender) << ")" << RESET
         << " - " << CATEGORY_NAMES[category] << " - " << rules.name
         << " - " << secret.size() << " letters\n";

    while (lives > 0 && shown != secret)
    {
        // the figure grows in proportion to the lives lost
        const int parts = (wrongCount * BODY_PARTS + rules.lives - 1) / rules.lives;

        cout << "\n================================\n";
        drawHangman(player.gender, parts, false);
        displayWord(shown);
        showLives(lives);
        cout << "\nScore: " << player.score << endl;

        cout << "Wrong letters: ";
        for (char letter : wrongLetters)
            cout << letter << " ";
        cout << endl;

        cout << "\nEnter a letter (? for hint): ";
        const string input = readLine();

        if (input == "?")
        {
            if (hintUsed)
            {
                cout << "\nHint already used.\n";
            }
            else
            {
                cout << "\nHint: " << entry.hint << endl;
                if (rules.hintCost > 0)
                {
                    player.score = max(0, player.score - rules.hintCost);
                    cout << YELLOW << "(Hint cost: -" << rules.hintCost
                         << " points)" << RESET << endl;
                }
                hintUsed = true;
            }
            continue;
        }

        if (input.size() != 1 || !isalpha(static_cast<unsigned char>(input[0])))
        {
            cout << RED << "\nPlease enter a valid letter.\n" << RESET;
            continue;
        }

        const char guess = static_cast<char>(tolower(static_cast<unsigned char>(input[0])));

        if (guessedLetters.find(guess) != string::npos)
        {
            cout << YELLOW << "\nYou already guessed that letter.\n" << RESET;
            continue;
        }
        guessedLetters += guess;

        bool found = false;
        for (size_t i = 0; i < secret.size(); i++)
        {
            if (secret[i] == guess)
            {
                shown[i] = guess;
                found = true;
            }
        }

        if (found)
        {
            cout << GREEN << "\nCorrect!\n" << RESET;
            player.score += POINTS_LETTER * rules.multiplier;
            correctCount++;
        }
        else
        {
            cout << RED << "\nWrong!\n" << RESET;
            wrongLetters += guess;
            lives--;
            wrongCount++;
        }
    }

    cout << "\n================================\n";

    if (shown == secret)
    {
        const int bonus = POINTS_BONUS * rules.multiplier;

        cout << GREEN
             << "\n CONGRATULATIONS!\n"
             << "You guessed the word: " << secret << endl
             << "\nBonus +" << bonus << " points!\n";

        player.score += bonus;
        player.streak++;
        achievement(player.score);
        cout << RESET;
    }
    else
    {
        drawHangman(player.gender, BODY_PARTS, true);

        cout << RED
             << "\n GAME OVER!\n"
             << player.name << " has been hanged!\n"
             << "Correct word was: " << secret << endl
             << RESET;

        player.streak = 0;
    }

    showStatistics(correctCount, wrongCount);

    cout << "\nCurrent Score: " << player.score << endl
         << "Win Streak: "    << player.streak << endl;
}

// ----------------------------------------------------------------------------
//  Game modes
// ----------------------------------------------------------------------------
void playVersusComputer(Player& player)
{
    player.score  = 0;
    player.streak = 0;

    do
    {
        const int category = chooseCategory();
        const int level    = chooseDifficulty();
        playRound(player, category, level);
    }
    while (readYesNo("\nPlay another round? (y/n): "));

    saveScore(player);
}

void playVersusPlayer(Player& first)
{
    cout << "\n--- Player 2 ---\n";
    Player second = createPlayer("Enter Player 2 name: ");

    first.score  = second.score  = 0;
    first.streak = second.streak = 0;

    const int rounds   = readInt("\nRounds per player (1-5): ", 1, 5);
    const int category = chooseCategory();
    const int level    = chooseDifficulty();

    for (int round = 1; round <= rounds; round++)
    {
        Player* order[2] = { &first, &second };

        for (int turn = 0; turn < 2; turn++)
        {
            Player& hanged = *order[turn];
            Player& rival  = *order[1 - turn];

            cout << "\n" << YELLOW << "########  ROUND " << round << " of " << rounds
                 << "  ########" << RESET << "\n"
                 << rival.name << " watches while " << hanged.name
                 << " tries to survive!\n";

            playRound(hanged, category, level);
        }
    }

    cout << "\n===== MATCH RESULT =====\n"
         << first.name  << " : " << first.score  << endl
         << second.name << " : " << second.score << endl;

    if (first.score == second.score)
        cout << YELLOW << "It's a draw!\n" << RESET;
    else
        cout << GREEN << "Winner: "
             << (first.score > second.score ? first.name : second.name)
             << "!\n" << RESET;

    saveScore(first);
    saveScore(second);
}

void playGame(Player& player)
{
    cout << "\nChoose Game Mode\n"
         << "1. Player vs Computer\n"
         << "2. Player vs Player\n";

    if (readInt("Choice: ", 1, 2) == 1)
        playVersusComputer(player);
    else
        playVersusPlayer(player);
}

// ----------------------------------------------------------------------------
//  Program entry
// ----------------------------------------------------------------------------
int main()
{
    enableAnsiColours();
    displayTitle();

    Player player = createPlayer("\nEnter your name: ");

    int choice;
    do
    {
        choice = menu();

        switch (choice)
        {
        case 1:
            playGame(player);
            break;
        case 2:
            showScores();
            break;
        case 3:
            cout << "\nThank you for playing Hangman Deluxe!\n";
            break;
        }
    }
    while (choice != 3);

    return 0;
}