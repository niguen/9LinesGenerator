/* gameGenerator.hpp */
#ifndef GAMEGENERATOR_HPP
#define GAMEGENERATOR_HPP

#include <vector>


enum Mode
{
	USER, KNOWN, BLACK, BLACKKNOWN
};

struct Field
{
	Mode mode;
	int number;
};

const int ENCODING_VERSION = 2;
const char BASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"; // 64 characters used for the base64url encoding


// Function declaration to print Hello, World!
void printHelloWorld();
std::string encodeGame(const std::vector<Field>& game);
std::string gameToString(const std::vector<Field>& game);


#endif // HELLO_HPP