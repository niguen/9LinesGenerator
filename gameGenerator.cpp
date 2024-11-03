/* main.cpp */
#include <iostream>
#include "gameGenerator.hpp"
#include "./json.hpp"
#include <bitset>
#include <fstream>

#include <algorithm>
#include <random>
#include <ctime>


using json = nlohmann::json; // JSON-Namespace definieren


bool isValidInsertion(std::vector<Field> &game, int number, int row, int col) {
    // Check if the number already exists in the row
    for (int c = 0; c < 9; ++c) {
        if (game[row * 8 + c].number == number) {
            return false;
        }
    }

    // Check if the number already exists in the column
    for (int r = 0; r < 9; ++r) {
        if (game[r * 8 + col].number == number) {
            return false;
        }
    }

    // Additional checks for straights can be implemented here if needed

    return true;
}



void fillField(std::vector<Field> &game){

// Constants to define cell types
const int WHITE_CELL = 1;
const int BLACK_CELL_EMPTY = 3;
const int BLACK_CELL_WITH_NUMBER = 2;

    std::srand(std::time(0));  // Seed for random number generation

    // itterate over all rows
    for (int row = 0; row < 9; ++row) {
        // numbers to be filled in the row
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(std::rand()));

        int col = 0;
        while (col < 9) {
            if (game[row * 9 + col].mode != WHITE_CELL) {
                // Skip black cells
                ++col;
                continue;
            }

            int number = numbers.front();
            numbers.erase(numbers.begin());  // Remove number from the front

            if (isValidInsertion(game, number, row, col)) {
                game[row * 8 + col].number = number;
                ++col;  // Move to the next cell
            } else {
                numbers.push_back(number);  // Place the number at the end of the list
            }

            // If no valid numbers remain, reset the row and reshuffle
            if (numbers.empty()) {
                for (int c = 0; c < 9; ++c) {
                    if (game[row * 8 + c].mode == WHITE_CELL) {
                        game[row * 8 + c].number = 0;  // Clear the row
                    }
                }
                std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(std::rand()));  // Reshuffle
                col = 0;  // Restart the row
            }
        }
    }

    // After row filling, validate columns to ensure no duplicates
    for (int col = 0; col < 9; ++col) {
        std::vector<bool> used(10, false);
        for (int row = 0; row < 9; ++row) {
            if (game[row * 8 + col].number != 0) {
                if (used[game[row * 8 + col].number]) {
                    return;  // Duplicate found, invalid solution
                }
                used[game[row * 8 + col].number] = true;
            }
        }
    }

    return;
}


/// <summary>
/// Generate a new Str8ts game
/// </summary>
/// <param name="game">Field where the new game is stored</param>
/// <param name="difficulty">Difficulty level as integer between 1 and 3, i.e. there exist 1 ist easy, 2 is medium and 3 is hard</param>
/// <param name="generatorCount">Number of previous retries to generate the black game fields</param>
/// <param name="numberGeneratorCount">Number of previous retries to generate all numbers</param>
void generate(std::vector<Field> &game, int difficulty, int generatorCount = 0, int numberGeneratorCount = 0)
{
    // Increase number of retries to generate a new game
    generatorCount++;

    // Initialize game field
    game.assign(81, Field());

    // Select the predefined layouts based on the difficulty level
    // select and read a random layout (between 0 and 29) from the predefined layouts in the hpp file
    std::vector<std::string> layouts;
    switch (difficulty)
    {
    case 1:
        layouts = easyLayouts;
        break;
    case 2:
        layouts = mediumLayouts;
        break;
    case 3:
        layouts = hardLayouts;
        break;    
        }

    // Randomly choose a layout from the selected difficulty layouts
    std::string layout = layouts[rand() % 30];

    // apply the layout to the game datascruptre following the following encoding: 1 = white cell, 2 = black with number and 3 = empty black cell
    for (int i = 0; i < 81; i++)
    {
        switch (layout[i])
        {
        case '1':
            game[i].mode = USER;
            break;
        case '2':
            game[i].mode = BLACKKNOWN;
            game[i].number = rand() % 9 + 1;
            break;
        case '3':
            game[i].mode = BLACK;
            break;
        }
    }

    fillField(game);

}




/// <summary>
/// Encode and format the game in a human-readable format
/// </summary>
/// <param name="game">The game to be encoded</param>
/// <returns>The game in a human-readable format</returns>
std::string gameToString(const std::vector<Field> &game)
{
    std::string result = "";
    for (int i = 0; i < 81; i++)
    {
        if (i % 9 == 0)
            result += "   ";
        switch (game[i].mode)
        {
        case BLACK:
            result += "    ";
            break;
        case BLACKKNOWN:
            result += "[" + std::to_string(game[i].number) + "] ";
            break;
        case KNOWN:
            result += " " + std::to_string(game[i].number) + "  ";
            break;
        default:
            result += "(" + std::to_string(game[i].number) + ") ";
        }
        if ((i + 1) % 9 == 0)
            result += "\n";
    }
    return result;
}


/// <summary>
/// Encode / encrypt a Str8ts game as a url base64url string
/// (c.f. rfc464)
/// </summary>
/// <param name="game">Game field to be encoded</param>
/// <returns>The game as a base64url encoded string including the encoding version number</returns>
std::string encodeGame(const std::vector<Field> &game)
{
    // Encode game as binary
    std::string binary = std::bitset<8>(ENCODING_VERSION).to_string(); // Include encoding version number
    for (int i = 0; i < 81; i++)
    {
        int black = (game[i].mode == BLACK || game[i].mode == BLACKKNOWN) ? 1 : 0;
        int known = (game[i].mode == USER || game[i].mode == BLACK) ? 0 : 1;
        binary += std::to_string(black) + std::to_string(known) + std::bitset<4>(game[i].number - 1LL).to_string();
    }

    // Encode binary data as base 64
    std::string result = "";
    for (int i = 0; i < binary.length(); i += 6)
    {
        std::string bits;
        bits = binary.substr(i, std::min<int>(6, binary.length() - i));
        while (bits.length() < 6)
            bits += "0";
        result += BASE[stoi(bits, nullptr, 2)];
    }
    return result;
}

// Function definition for printing Hello, World!
void printHelloWorld()
{
    std::cout << "Hello, World!" << std::endl;
}

int main(int argc, char *argv[])
{

    // Check number of provided arguments
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <easyGames> <mediumGames> <hardGames>" << std::endl;
        return 1;
    }

    try
    {
        // Read number of games from command line input
        int numberEasyGames = std::stoi(argv[1]);
        int numberMediumGames = std::stoi(argv[2]);
        int numberHardGames = std::stoi(argv[3]);

        // Ensure the numbers are non-negative
        if (numberEasyGames < 0 || numberMediumGames < 0 || numberHardGames < 0)
        {
            std::cerr << "The number of games must be non-negative." << std::endl;
            return 1;
        }

        std::cout << "Easy: " << numberEasyGames << " Medium: " << numberMediumGames << " Hard: " << numberHardGames << std::endl;

        // Proceed with the rest of your code to generate games

        // Erstellen Sie ein JSON-Objekt mit drei Schlüsseln
        json j;
        j["easy"] = json::array();
        j["medium"] = json::array();
        j["hard"] = json::array();

        // Funktion zum Generieren und Kodieren von Spielen
        auto generateAndEncodeGames = [](int count, int difficulty)
        {
            std::vector<std::string> encodedGames;
            for (int i = 0; i < count; ++i)
            {
                std::vector<Field> game;
                generate(game, difficulty);
                std::cout << gameToString(game) << std::endl;
                encodedGames.push_back(encodeGame(game));
            }
            return encodedGames;
        };

        // Generieren und Hinzufügen der Spiele zu den entsprechenden Listen
        j["easy"] = generateAndEncodeGames(numberEasyGames, 1);
        j["medium"] = generateAndEncodeGames(numberMediumGames, 2);
        j["hard"] = generateAndEncodeGames(numberHardGames, 3);

        // Speichern des JSON-Objekts in eine Datei
        std::ofstream file("games.json");
        file << j.dump(4); // 4 spaces for indentation
        file.close();

        std::cout << "Games saved to games.json successfully!" << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "All inputs must be valid integers." << std::endl;
        return 1;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "One or more inputs are out of range." << std::endl;
        return 1;
    }
}