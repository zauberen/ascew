// Title: ASCEW (Simple Console Emulator for Windows)
// Author: dandreas
// Description: 'Emulates' a windows console, useful for systems that restrict the use of cmd.exe.

#include <algorithm> // for find_if and bind1st (used to trim whitespace from input)
#include <direct.h> // for finding the current working directory
#include <iostream>
#include <iomanip>
#include <string> // for string functions
#include <sstream> // for stringstream, used in various places
#include <vector> // for vector, used in various places
#include <windows.h> // for system() and cosmetic functions

#include "config.h" // Everything for .ascew files

using namespace std;

int main(int argc, char* argv[])
{
    const std::string AUTHOR = "dandreas (GitHub.com/dandreas/ascew)";
    const double VERSION = 1.51; // Version number, used for releases.
    // Startup command checks
    bool bBasic = false; // if true, runs the most basic console emulator, skips reading any configuration files, etc.
    bool bNoText = false; // if true, hides the opening text
	bool DEBUG = false; // Enables debug mode to inspect the IO properties

    if(argc > 1)
    {
        for(int i = 0;i < argc;i++)
        {
            string sArg = std::string(argv[i]);

            if(DEBUG)
            {
                cout << "Args: " << sArg << endl;
            }

            if(sArg == "-h")
            {
                // Eventually, a help function, currently just displays help & exits
                system("help");
                return 0;
            }
            else if(sArg == "-b")
            { // turns on basic mode, no colours, no config reading
                bBasic = true;
            }
            else if(sArg == "-nt")
            { // no greeting text
                bNoText = true;
            }
			else if(sArg == "-d")
			{ // turns on debug mode
				DEBUG = true;
			}

        }
    }

    // End startup
    HANDLE hConsole; // Handle for console colors
    if(!bBasic)
    {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    _path pPath;

    if(!bBasic)
    {
        pPath = SetPath(); // Grabs settings from the local settings file
    }

    std::string sInput = ""; // Stores the user input
    std::string sTitle = ""; // Title for the application
    std::string sDirectory = ""; // Stores the working directory

    sDirectory = _getcwd(NULL, 0); // Gets the directory
    sTitle = "ascew " + sDirectory;
    SetConsoleTitle(sTitle.c_str()); // Sets the console name

    if(!bBasic)
    {
        SetConsoleTextAttribute(hConsole,pPath.iColor);
        system("cls");
    }

    if(!bNoText)
    {
        cout << "A Simple Console Emulator for Windows (ASCEW) v" << VERSION << endl
             << "This is free software liscenced under version 3 of the the General Public License" << endl
             << "Made by: " << AUTHOR
             << "\n" << "\n";
    }

    // Any pre-execution debug stuff should be put in here
    if (DEBUG)
    {
        if(!bBasic)
        {
            cout << "Number of custom commands: " <<  pPath.count << "\n";
        }
        else
        {
            cout << "Custom commands are disabled." << endl;
        }
    }

    while(true)
    {
        bool bCustom = false; // Tells if a custom command is being used

		// Change color to the directory colour (if applicable)
        if(!bBasic)
        {
            SetConsoleTextAttribute(hConsole,pPath.iDircolor);
        }

		// Output the current directory
        cout << sDirectory << ">";

		// Change color to input text colour (if applicable)
        if(!bBasic)
        {
            SetConsoleTextAttribute(hConsole,pPath.iColor);
        }

		// Grab user input
        getline(cin,sInput);

        if(!bBasic)
        {
            // If there isn't a config file, skip the whole jazz
            if(pPath.bIsActive)
            {
				// Trim leading whitespace on sInput (causes issues down the line) credit: http://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
				sInput.erase(sInput.begin(), std::find_if(sInput.begin(), sInput.end(), std::bind1st(std::not_equal_to<char>(), ' ')));

                // Only checks the actual command
                stringstream ss;
                ss << sInput;
                string sTemp;
                ss >> sTemp;

                // Tests if the command is a custom made command
                for(int i = 0; i < pPath.count; i++)
                {
                    if(sTemp == pPath.sAlias[i] && !bCustom)
                    {
                        bCustom = true; // Switches the custom option on
						if(sInput.length() == sTemp.length())
						{
                        	sInput = pPath.sExecutable[i]; // Sets the user input to the location of the executable
						}
						else
						{
							sTemp = sInput.substr(sInput.find_first_of(" \t") + 1);
							sInput = pPath.sExecutable[i] + " " + sTemp;
						}
                    }
                }
            }
        }

        // Runs the appropriate function per the user's input
        if(bCustom)
        {
            // Runs the user's specified executable
            system(sInput.c_str());
			if(DEBUG)
			{
				cout << "DEBUG FEED" << endl
					 << "sInput = " << sInput << endl;
			}
        }
        else if(sInput[0] == 'c' && sInput[1] == 'd' && sInput[2] == ' ')
        {
            // cd clone, created because system("cd <dir>") won't change the programs working directory.
            try
            {
                sInput.erase(0,3); // Removes "cd " from the string
                SetCurrentDirectory(sInput.c_str()); // Sets the working directory

                // Set the directory variable, reset console title
                sDirectory = _getcwd(NULL, 0);
                sTitle = "ascew " + sDirectory;
                SetConsoleTitle(sTitle.c_str());

                cout << sDirectory << endl; // emulate actual cd output
            }
            catch(...)
            {
                cout << "Directory Change failed. Input: " << sInput << endl;
            }
        }
        else if(sInput == "exit")
        {
            // exit clone, created because system("exit") won't end the actual program.
            return 0;
        }
        else
        {
            // Runs the user input in cmd (hopefully) bypassing any console locks
            system(sInput.c_str());
        }

        cout << endl;
    }
}
