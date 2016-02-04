// Title: SCEW (Simple Console Emulator for Windows)
// Author: Dillon Andreas
// Description: 'Emulates' a windows console, useful for systems that restrict the use of cmd.exe.

#include <fstream> // for reading .scew
#include <iostream>
#include <iomanip>
#include <windows.h> // for system()
#include <direct.h> // finds current working directory
#include <string> // for string functions
#include <sstream> // for parsing .scew
#include <vector> // for vectors

using namespace std;

const bool DEBUG = false; // Enables debug mode to inspect the IO properties

typedef struct
{
    bool bIsActive; // Tells if there is a config file present
    int count; // The (base 1) amount of items in the sExecutable array
    vector<string> sExecutable; // Paths to the files in relation to to executable. eg ../../nano/nano.exe
    vector<string> sAlias; // Alias to the related executable, eg "nano" for the above example
} _path;

_path SetPath(); // Sets up the _path struct

int main()
{
    const double VERSION = 1.20; // Build version number. Patch updates aren't displayed
    const double UPDATE = 3; // Updates since last version
    
    const std::string AUTHOR = "dandreas (belfieldcecil@gmail.com)";
    
    _path pPath = SetPath(); // Grabs settings from the local settings file
    
    std::string sInput = ""; // Stores the user input
    std::string sTitle = ""; // Title for the application
    std::string sDirectory = ""; // Stores the working directory
    
    sDirectory = _getcwd(NULL, 0); // Gets the directory
    sTitle = "SCEW " + sDirectory;
    SetConsoleTitle(sTitle.c_str()); // Sets the console name
    
    cout << "Simple Console Emulator for Windows (SCEW) v" << VERSION << "u" << UPDATE << endl
         << "Made by: " << AUTHOR << "\n"
         << "NOTE: Some commands may not work as intended.\nTo avoid most complications, do all commands on seperate lines." << "\n" << "\n";

    // Any pre-execution debug stuff should be put in here
    if (DEBUG)
    {
        cout << "Number of custom commands: " <<  pPath.count << "\n";
    }
    
    while(true)
    {
        bool bCustom = false; // Tells if a custom command is being used
        
        cout << sDirectory << ">";
        getline(cin,sInput);
        
        // If there isn't a config file, skip the whole jazz
        if(pPath.bIsActive)
        {
            // Tests if the command is a custom made command
            for(int i = 0; i < pPath.count; i++)
            {
                if(sInput == pPath.sAlias[i])
                {
                    bCustom = true; // Switches the custom option on
                    sInput = pPath.sExecutable[i]; // Auto-sets the user input to the location of the executable
                }
            }
        }
        
        // Runs the appropriate function per the user's input
        if(bCustom)
        {
            // Runs the user's specified executable
            system(sInput.c_str());
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
                sTitle = "SCEW " + sDirectory;
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

_path SetPath()
{
    _path path;
    path.bIsActive = false;
    
    ifstream ifile(".scew");
    if(ifile)
    {
        //get the variables
        string sLine;
        int iCount = 0;
        string sTemp;
        int temp = 0;

        
        while(getline(ifile,sLine))
        {
            istringstream iss(sLine);
            
            if(sLine[0] == '#')
            {
                // The line is a comment
            }
            else
            {
                do
                {
                    iss >> sTemp;
                    
                    if(sTemp != "")
                    {
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
                }
                while (iss);
            }
        }
        
        path.count = iCount;
        path.bIsActive = true;
    }
    return path;
}
