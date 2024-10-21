/* main.cpp */
#include <iostream>
#include "gameGenerator.hpp"
#include "./json.hpp"

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
    // Call the function to print Hello, World!
    printHelloWorld();
    return 0;
}