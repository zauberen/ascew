// Used in: ASCEW
// Author: dandreas
// Description: Handles config files

#include <fstream> // for reading .ascew
#include <iostream>
#include <iomanip>
#include <windows.h> // for system() and other (less scandalous) methods
#include <direct.h> // finds current working directory
#include <string> // for string functions
#include <stdlib.h> // atoi
#include <sstream> // for parsing .ascew
#include <vector> // for vectors

#include "config.h" // Everything for .ascew files

using namespace std;

// I have no idea what this function was supposed to be for...
// _path Read(); // Does everything but

_path SetPath()
{
    _path path; // The return variable, set throughout the remainder of the application
    path.sOrigin = _getcwd(NULL, 0); // Sets the origin directory
    path.bIsActive = false; // Assumes there isn't a config file (changed later if neccissary)
    path.count = 0;

    ifstream ifile(".ascew");

    if(!ifile)
    {
		// If the local file doesn't exist, check if there is one in the home directory
        ifile.close();
        ifile.open("%USERPROFILE%/.ascew");
    }

    if(ifile)
    { //get the variables
        string sLine; // Holds each line as the file is read
        int iCount = 0; // Holds the amount of variables read
        string sTemp; // Holds each specific part to each line as it's parsed
        int temp = 0; // counts the number of parts pased per line.
        bool bSet[] = {false, false, false}; // Checks if colors are set
        int iRGBcolor = 0; // holds the color index

        while(getline(ifile,sLine))
        {
            istringstream iss(sLine);

            if(sLine[0] == '#')
            { // Either a comment or cosmetic setting
                iss >> sTemp;
                if(sTemp == "#color=")
                { // Sets the text color to the given value (user input)
                    iss >> sTemp;
                    iRGBcolor = atoi(sTemp.c_str());
                    if (iRGBcolor < 16)
                    {
                        path.iColor = iRGBcolor;
                    }
                    else
                    {
                        path.iColor = 15;
                    }
                    bSet[0] = true;
                }
                else if(sTemp == "#bgcolor=")
                { // Sets the background color to the given value
                    iss >> sTemp;
                    iRGBcolor = atoi(sTemp.c_str());
                    if (iRGBcolor < 16)
                    {
                        path.iBgcolor = iRGBcolor;
                    }
                    else
                    {
                        path.iBgcolor = 0;
                    }
                    bSet[1] = true;
                }
                else if(sTemp == "#dircolor=")
                { // Sets the directory color to the given value (this text: C:/>)
                    iss >> sTemp;
                    iRGBcolor = atoi(sTemp.c_str());
                    if (iRGBcolor < 16)
                    {
                        path.iDircolor = iRGBcolor;
                    }
                    else
                    {
                        path.iDircolor = 12;
                    }
                    bSet[2] = true;
                }
            }
            else if(sLine.empty())
            {
                // The line is empty
            }
            else
            {
				// TODO: Redo this section, the do while loop is completely redundant
				// and could be replaced by a much simpler and tidier solution.

                do
                { // Iterates through the line, reading executable paths to their vector, then alias' to their vector
                    iss >> sTemp;

                    if (temp == 0)
                    {
                        if (DEBUG)
                        {
                            cout << sTemp << iCount << endl;
                        }

                        path.sExecutable.push_back(sTemp);
                        temp++; // 1
                    }
                    else if (temp == 1)
                    {
                        if (DEBUG)
                        {
                            cout << sTemp << iCount << endl;
                        }

                        path.sAlias.push_back(sTemp);
                        iCount++;
                        temp++; // 2
                    }
                    else if (temp == 2)
                    {
                        temp = 0;
                    }
                }
                while (iss);
            }
        }

        if(!bSet[0])
        {
            path.iColor = 15;
        }
        if(!bSet[1])
        {
            path.iBgcolor = 0;
        }
        if(!bSet[2])
        {
            path.iDircolor = 12;
        }

        // Determines if there are errors in the .ascew file
        if(path.sExecutable.size() == path.sAlias.size())
        {
            path.count = iCount;
            path.bIsActive = true;
        }
        else
        {
            string sInput = "";
            cout << "ERROR: ascew configuration file contains errors." << "\n"
                 << "Display config table contents?(Y/N): ";
            getline(cin,sInput);

            if(sInput == "Y" || sInput == "y")
            {
                // Determines which vector has the discrepency
                if(path.sExecutable.size() > path.sAlias.size())
                {
                    // Prints the contents of both vectors to the console
                    for(int i = 0; i < path.sExecutable.size(); i++)
                    {
                        cout << "Executable: " << path.sExecutable[i] << "\n";
                        cout << "Alias: ";
                        if(path.sAlias.size() > i)
                        {
                            cout << path.sAlias[i] << "\n";
                        }
                        else
                        {
                            cout << "<Not Set>" << "\n";
                        }
                    }
                }
                else
                {
                    // Prints the contents of both vectors to the console
                    for(int i = 0; i < path.sAlias.size(); i++)
                    {
                        cout << "Executable: ";

                        if(path.sExecutable.size() > i)
                        {
                            cout << path.sExecutable[i] << "\n";
                        }
                        else
                        {
                            cout << "<Not Set>" << "\n";
                        }

                        cout << "Alias: " << path.sAlias[i] << "\n";
                    }
                }

                // Waits for the user to examine the information
                cout << "\n\n" << "Press any key to start the program...";
                cin.get();

                system("cls"); // Clears the screen and proceeds to start the program
            }
            else if(sInput == "N" || sInput == "n")
            {
                system("cls"); // Clears the screen and proceeds to start the program
            }
        }

    }
    else
    {
        // RGB values are ALWAYS set
        path.iColor = 15;
        path.iBgcolor = 0;
        path.iDircolor = 12;
    }

    path.iColor = path.iColor + path.iBgcolor * 16;
    path.iDircolor = path.iDircolor + path.iBgcolor * 16;

    return path;
}
