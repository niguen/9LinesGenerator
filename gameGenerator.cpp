/* main.cpp */
#include <iostream>
#include "gameGenerator.hpp"
#include "./json.hpp"
#include <bitset>

#include <fstream>


using json = nlohmann::json; // JSON-Namespace definieren


/// <summary>
/// Generate a new Str8ts game
/// </summary>
/// <param name="game">Field where the new game is stored</param>
/// <param name="difficulty">Difficulty level as integer between 0 and 5, i.e. there exist 6 levels where 0 is the most difficult</param>
/// <param name="generatorCount">Number of previous retries to generate the black game fields</param>
/// <param name="numberGeneratorCount">Number of previous retries to generate all numbers</param>
void generate(std::vector<Field> &game, int difficulty, int generatorCount = 0, int numberGeneratorCount = 0)
{
	//Increase number of retries to generate a new game
	generatorCount++;

	// Initialize game field
	game.assign(81, Field());
}

/// <summary>
/// Encode and format the game in a human-readable format
/// </summary>
/// <param name="game">The game to be encoded</param>
/// <returns>The game in a human-readable format</returns>
std::string gameToString(const std::vector<Field>& game)
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
std::string encodeGame(const std::vector<Field>& game)
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
void printHelloWorld() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {

	int difficulty = 0;

	// create an empty field 
	std::vector<Field> game;
	generate(game, difficulty);

	std::cout << "Here is an empty game: " << std::endl << gameToString(game) << std::endl;

	// encode game
	std::string gameEncoding = encodeGame(game);

	std::cout << "This is the encoded game: " << gameEncoding << std::endl;

	// save the game to json
	// Create a JSON object
    json jsonObject;
    jsonObject["myStringKey"] = gameEncoding;

    // Save JSON object to a file
    std::ofstream outFile("games.json");
    if (outFile.is_open()) {
        outFile << jsonObject.dump(4);  // Use 4 for pretty-printed JSON with indentations
        outFile.close();
        std::cout << "String saved to output.json successfully!" << std::endl;
    } else {
        std::cerr << "Could not open the file for writing!" << std::endl;
    }




    // Call the function to print Hello, World!
    // printHelloWorld();
    return 0;
}