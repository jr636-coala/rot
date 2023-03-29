//	Dependencies
#include <iostream>
#include <string>
#include <random>
//


//##				Function Prototypes

void wait();	// Macro that requests user input in order to continue, input is thrown away
std::string rndDictionary(std::string dict, int seed); // Creates a random (reproducibly) random dictionary
std::string rot(std::string dict, int rotCode, std::string input, bool unRot = false); // Performs the actual rotations on the dataset following the dictionary
int* getRnd(int seed, int min, int max, int num); // Using the inputs creates a random output
int seedFromSeed(int seed, int max); // Uses an old seed to create a new seed, this makes it easier to use outside of the getRnd function
double clamp(double x, double min, double max); // Clamps a value between two other values		

//##

int main(int argc, char* argv[])
{
	//~~			Declarations (and apparently definitions)
	int rotCode, seed = 0;
	std::string input = "";
	//char* inputBuffer = new char[5000];
	std::string mDict = "1234567890-=qwertyuiop[]asdfghjkl;'#zxcvbnm,./\\\"!£$%^&*()_+QWERTYUIOP{}ASDFGHJKL:@~|ZXCVBNM<>?¬`¦";
	//~~

	//@@			Gathering inputs
	std::cout << "Enter seed: ";						std::cin >> seed;
	std::cout << "\nEnter ROT code: ";					std::cin >> rotCode;
	std::cout << "\nEnter text to be encoded: ";		std::cin >> input;//std::cin.clear();  std::cin.get(inputBuffer, sizeof(inputBuffer));
	//@@
	
	clamp(rotCode, 0, mDict.length());	// Don't want any weird errors, easier than adding another function to parse this into a number
										// which will not create an error; such as: rotCode % mDict.length();
	//parseString(input); // apparently useless processing

	std::cout << "Encoded: " << rot(rndDictionary(mDict, seed), rotCode, input) << '\n'  // If the input string is supposed to be encoded
				<< "Decoded: " << rot(rndDictionary(mDict, seed), rotCode, input, true); // If the input string is supposed to be decoded
	while (true) NULL; // Had some problems with the program exiting immediatelly, now it will never exit!
	return 0;
}

void wait()
{
	int tmp;
	std::cin >> tmp;
}

std::string rndDictionary(std::string dict, int seed)
{
	std::string r = ""; // No need to initialize it to an empty string as it is automatically done (as it is an object), just easier than arguing
						// with people who think otherwise
	int* tmp; // The getRnd function returns a pointer, so although we will always have one element this is nessicary
	while(dict.length()) // While it has a length ( length > 0), the procedure needs to repeat
	{
		tmp = getRnd(seed, 0, dict.length(), 1); // The number is needed twice so best not to generate it twice removing unneccisary overhead
		r += dict[tmp[0]]; //Adds in this char to new dict
		dict.erase(tmp[0],1); //remove char from this instance of orig dict
		seed = seedFromSeed(seed, dict.length()); // Creates the new seed for the next entry
	}
	delete tmp;
	return r;
}

std::string rot(std::string dict, int rotCode, std::string input, bool unRot)
{
	std::string out = "";
	for (int i = 0; i < input.length(); i++)
	{
		for (int j = 0; j < dict.length(); j++)
		{
			if (input[i] == dict[j])
			{
				switch (unRot)
				{
				case true:
						out += dict[(j + rotCode) % dict.length()];
					break;
				case false:
						out += dict[(j - rotCode) % dict.length()];
					break;
				default:
					std::cout << "What have you done.....";
				}
			}
		}
	}
	return out;
}

int* getRnd(int seed, int min, int max, int num)
{
	int* list = new int[num];
	std::uniform_int_distribution<int> dist(min, max);
	for (int i = 0; i < num; i++)
	{
		std::mt19937 gen(seed);
		list[i] = dist(gen);
		seed = seedFromSeed(seed, max);
	}
	return list;
}

int seedFromSeed(int seed, int max)
{
	std::uniform_int_distribution<int> dist(0, max+1);
	std::mt19937 gen(seed);
	return (((seed+1) % (dist(gen)+7)) + 5)*((max+2) / 3);
}

double clamp(double x, double min, double max)
{
	return x < min ? min : 
		x > max ? max : x;
}