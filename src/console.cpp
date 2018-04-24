#include <algorithm> // for find_if and bind1st (used to trim whitespace from input)
#include <direct.h> // for finding the current working directory
#include <iostream> // default
#include <iomanip> // default
#include <string> // for string functions
#include <sstream> // for stringstream, used in various places
#include <vector> // for vector, used in various places
#include <windows.h> // for system() and cosmetic functions
#include <cstdlib>

#include "ascewapi.hpp" // API for outpu-t
#include "console.hpp" // source header file
#include "config.hpp" // config file backend

// using namespace std;

// <constructors>
console::console(bool debug = false, bool noTextMode = false, bool basicMode = false, ascewapi api = ascewapi(), config ascewconf = config())
{
	ascewConfig = ascewconf;
	setCurrentDir(ascewConfig.getStartFolder());
	
	if (!basicMode)
	{
		ascewConfig.readConfig();
		
	}

	// Set the variables to those set above, overrides the config file.
	ascewConfig.setDebug(debug);
	ascewConfig.setNoTextMode(noTextMode);
	ascewConfig.setBasicMode(basicMode);
	
	// Send the api a copy of the configuration struct
	apiHandle = api;
	apiHandle.setConf(ascewConfig.getConf());

	// Pre-execution debug stuff
	if (ascewConfig.getDebug())
	{
		if(!ascewConfig.getBasicMode())
		{
			apiHandle.alert("Number of custom commands: " + ascewConfig.getAliasCount());
			// old: cout + "Number of custom commands: " +  pPath.count + "\n";
		}
		else
		{
			apiHandle.alert("Custom commands are disabled");
			// old: cout + "Custom commands are disabled." + endl;
		}
	}

	if (ascewConfig.getDebug())
		apiHandle.alert("LOC: console::console\ncurrentDir = " + currentDir + "\n" + "");

	greeting();
}

// </constructors>

void console::parse(std::string text)
{
	bool bCustom = false; // Tells if a custom command is being used

	/* old code for handling input/directory color
		// Change color to the directory colour (if applicable)
		if(!bBasic)
		{
			SetConsoleTextAttribute(hConsole,pPath.iDircolor);
		}

		// Output the current directory
		cout + sDirectory + ">";
		// Change color to input text colour (if applicable)
		if(!bBasic)
		{
			SetConsoleTextAttribute(hConsole,pPath.iColor);
		}
		

		// Grab user input
		getline(cin,sInput);
	*/

	// This code handles determining if a command is a custom command, and editing the var text if needed
	text = ascewConfig.checkInput(text);

	// Runs the appropriate function per the user's input
	if(bCustom)
	{
		// Runs the user's specified executable
		system(text.c_str());
		if(ascewConfig.getDebug())
		{
			apiHandle.alert("A custom function is being used. var text = " + text);
			/* old:
			cout + "Custom Function" + endl
				 + "text = " + text + endl;
			*/
		}
	}
	else if(text[0] == 'c' && text[1] == 'd' && text[2] == ' ')
	{
		// cd clone, created because system("cd <dir>") won't change the programs working directory.
		cd(text);
	}
	else if(text == "exit")
	{ //This won't work anymore.
		// exit clone, created because system("exit") won't end the actual program.
		// old: return 0;
	}
	else
	{
		// Runs the user input in cmd
		system(text.c_str());
	}
}

void console::greeting()
{
	author = "dandreas (GitHub.com/dandreas/ascew)";
	version = "1.20180424"; // Version number, used for releases

	if(!ascewConfig.getNoTextMode())
	{
		apiHandle.print("A Simple Console Emulator for Windows (ASCEW) v" + version + "\n"
			 + "This is free software, type in help for more information" + "\n"
			 + "Made by: " + author
			 + "\n" + "\n");
	}
}

// <misc functions>
void console::cd(std::string text)
{
	text.erase(0,3);
	setCurrentDir(text);
}

void console::help()
{
	apiHandle.print(
		"ASCEW Help for ascew v" + version + "\n"
		+ "Built in functions:" + "\n"
		+ "cd <directory> : Move to the specified directory. Works like the default, just doesn't ever require quotes" + "\n"
		+ "help : Display this help" + "\n"
		+ "\n License information:\n This program comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under certain conditions. For full information, check the copy of the GPL which should have been provided with this software, or go to <http://www.gnu.org/licenses/gpl-3.0.txt>"
		);
}
// </misc functions>

// <getters>
_conf console::getConf()
{
	return ascewConfig.getConf();
}

std::string console::getCurrentDir()
{
	return currentDir;
}
// </getters>;

// <setters>
void console::setConf(_conf c)
{
	ascewConfig.setConf(c);
}

void console::setCurrentDir(std::string path)
{
	try
	{
		// Sets the working directory
		LPCSTR t = path.c_str(); // !this works even though it throws an error!
		SetCurrentDirectory(t); 

		// Set the directory variable if the path has changed
		if (currentDir != (std::string)_getcwd(NULL,0))
			currentDir = _getcwd(NULL,0);
		else
			apiHandle.alert("Directory change failed.\nInput: " + path + "\nCurrent Directory: " + currentDir);

		// Set the title
		apiHandle.setTitle(currentDir);
		
		// emulate actual cd output
		apiHandle.print(currentDir); 
	}
	catch(...)
	{
		apiHandle.alert("Directory change failed.\nInput: " + path + "\nCurrent Directory: " + currentDir);
	}
}
// </setters>