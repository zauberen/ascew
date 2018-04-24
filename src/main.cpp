#include <algorithm> // for find_if and bind1st (used to trim whitespace from input)
#include <direct.h>  // for finding the current working directory
#include <iostream>
#include <iomanip>
#include <string>	// for string functions
#include <sstream>   // for stringstream, used in various places
#include <vector>	// for vector, used in various places
#include <windows.h> // for system() and cosmetic functions

#include "config.hpp"   // The config backend (handles .ascew)
#include "console.hpp"  // The console backend (handles console input)
#include "ascewapi.hpp" // API for output handles in the backend

using namespace std;

int main(int argc, char *argv[])
{
	// Ye olde defaults for basicMode, noTextMode and debug. Still work the same
	bool bBasic = false;  // if true, runs the most basic console emulator, skips reading any configuration files, etc.
	bool bNoText = false; // if true, hides the opening text
	bool DEBUG = false;   // Enables debug mode to inspect the IO properties
	HANDLE hConsole;	  // Handle for console colors
	_conf conf;

	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			string sArg = std::string(argv[i]);

			if (DEBUG)
			{
				cout << "Args: " << sArg << endl;
			}

			if (sArg == "-h")
			{
				// Eventually, a help function, currently just displays help & exits
				system("help");
				return 0;
			}
			else if (sArg == "-b")
			{ // turns on basic mode, no colours, no config reading
				bBasic = true;
			}
			else if (sArg == "-nt")
			{ // no greeting text
				bNoText = true;
			}
			else if (sArg == "-d")
			{ // turns on debug mode
				DEBUG = true;
			}
			else if (sArg == "-o")
			{
				cout << "Not ready yet" << endl;
			}
		}
	}

	console ascew(DEBUG, bNoText, bBasic, ascewapi(), config());
	conf = ascew.getConf();
	// End startup

	if (!bBasic)
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	while (true)
	{
		string sInput = "";

		// Change color to the directory colour (if applicable)
		if (!bBasic)
		{
			SetConsoleTextAttribute(hConsole, conf.dirColor);
		}

		// Output the current directory
		cout << ascew.getCurrentDir() << ">";

		// Change color to input text colour (if applicable)
		if (!bBasic)
		{
			SetConsoleTextAttribute(hConsole, conf.inputTextColor);
		}

		// Grab user input
		getline(cin, sInput);

		if (sInput == "exit")
		{
			return 0;
		}

		if (!bBasic)
		{
			SetConsoleTextAttribute(hConsole, conf.outputTextColor);
		}

		ascew.parse(sInput);

		cout << endl;
	}
}
