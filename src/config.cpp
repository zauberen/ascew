#include <fstream> // for reading .ascew
#include <iostream>
#include <iomanip>
#include <windows.h> // for system() and other (less scandalous) methods
#include <direct.h>  // finds current working directory
#include <string>	// for string functions
#include <stdlib.h>  // atoi
#include <sstream>   // for parsing .ascew
#include <vector>	// for vectors
#include <iterator>

#include "ascewapi.hpp"
#include "config.hpp" // Everything for .ascew files
#include "json.hpp" // json parser

using json = nlohmann::json; //for ease of access to the json parser
using namespace std;

config::config()
{
	// Initialize to defaults
	settings.originFolder = _getcwd(NULL, 0);
	settings.startFolder = getenv("HOMEPATH");
	settings.originFolder += "\\";
	settings.startFolder += "\\";
	settings.basicMode = false;
	settings.noTextMode = false;
	settings.configPresent = false;
	settings.debug = false;
	settings.aliasCount = 0;
	settings.inputTextColor = 15;
	settings.outputTextColor = 15;
	settings.guiInputColor = "White";
	settings.guiOutputColor = "White";
	settings.guiBgColor = "Black";
	settings.guiDirColor = "Red";
	settings.bgColor = 0;
	settings.dirColor = 12;

	// terminal color setup
	settings.inputTextColor = settings.inputTextColor + settings.bgColor * 16;
	settings.dirColor = settings.dirColor + settings.bgColor * 16;
	settings.outputTextColor = settings.outputTextColor + settings.bgColor * 16;

	apiHandle = ascewapi();

	apiHandle.setConf(settings);
}

bool config::readConfig()
{
	/* When this is actually finished this is required for proper functioning of colors.
	conf.inputTextColor = conf.inputTextColor + conf.bgColor * 16;
	conf.dirColor = conf.dirColor + conf.bgColor * 16;
	*/
	try
	{
		json jsonReader;

		ifstream i(settings.originFolder + "settings.json");
		if (i)
		{
			// Check for a json configuration file
			//TODO: Once the json config is ready, uncomment below
			// settings.configPresent = true;
			i >> jsonReader;
			try
			{
				settings.startFolder = jsonReader["startFolder"];
				settings.basicMode = jsonReader["basicMode"];
				settings.noTextMode = jsonReader["noTextMode"];
				settings.debug = jsonReader["debug"];
				settings.inputTextColor = jsonReader["inputTextColor"];
				settings.outputTextColor = jsonReader["outputTextColor"];
				settings.bgColor = jsonReader["bgColor"];
				settings.dirColor = jsonReader["dirColor"];
				
				//TODO: Fix this stuff 
				/*
				std::vector<std::string> executable<std::string>.insert(executable.begin(), std::begin(jsonReader["aliases"]["executable"]), std::end(jsonReader["aliases"]["executable"]));
				std::vector<std::string> alias<std::string>.insert(alias.begin(), std::begin(jsonReader["aliases"]["alias"]), std::end(jsonReader["aliases"]["alias"]));
				*/
				// settings.alias
				// settings.executable
			}
			catch (...)
			{
				apiHandle.alert("Something went wrong with the jsonreader");
			}
		}
		else
		{
			// Checks for a .ascew file instead
			i.close();
			i.open(settings.originFolder + ".ascew");
			if (i)
			{
				settings.configPresent = true;
				apiHandle.alert("Using legacy config file");
				i.close();
				settings = SetPath();
				apiHandle.alert("Creating new config based upon legacy config");
				saveConfig();
			}
			else
			{
				i.close();
				apiHandle.alert("No config files found. Using defaults");
			}
		}
	}
	catch (...)
	{
		apiHandle.alert("Something happened while reading the configuration files.");
		return false;
	}
	apiHandle.setConf(settings);
}

void config::saveConfig()
{
	json jsonSettings; // Used to convert _conf to json

	// Store all of the settings to json keys
	jsonSettings["startFolder"] = settings.startFolder;
	jsonSettings["basicMode"] = settings.basicMode;
	jsonSettings["noTextMode"] = settings.noTextMode;
	jsonSettings["debug"] = settings.debug;
	jsonSettings["inputTextColor"] = settings.inputTextColor;
	jsonSettings["outputTextColor"] = settings.outputTextColor;
	jsonSettings["guiInputColor"] = settings.guiInputColor;
	jsonSettings["guiOutputColor"] = settings.guiOutputColor;
	jsonSettings["guiBgColor"] = settings.guiBgColor;
	jsonSettings["guiDirColor"] = settings.guiDirColor;
	jsonSettings["bgColor"] = settings.bgColor;
	jsonSettings["dirColor"] = settings.dirColor;
	jsonSettings["aliases"]["executable"] = settings.executable;
	jsonSettings["aliases"]["alias"] = settings.alias;

	try
	{
		ofstream o(settings.originFolder + "settings.json");
		o << std::setw(4) << jsonSettings << endl;
		o.close();
	}
	catch (...)
	{
		apiHandle.alert("Unable to save config");
	}
	apiHandle.setConf(settings);
}

std::string config::checkInput(std::string text)
{
	bool bCustom = false; // placeholder for a loop
	
	// If there isn't a config file, skip the whole jazz
	if (settings.configPresent)
	{
		// Trim leading whitespace on text (causes issues down the line) credit: http://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
		text.erase(text.begin(), std::find_if(text.begin(), text.end(), std::bind1st(std::not_equal_to<char>(), ' ')));

		// Only checks the actual command
		stringstream ss;
		ss << text;
		string sTemp;
		ss >> sTemp;

		// Tests if the command is a custom made command
		for (int i = 0; i < settings.aliasCount; i++)
		{
			if (sTemp == settings.alias[i] && !bCustom)
			{
				bCustom = true; // Switches the custom option on
				if (text.length() == sTemp.length())
				{
					text = settings.executable[i]; // Sets the user input to the location of the executable
				}
				else
				{
					sTemp = text.substr(text.find_first_of(" \t") + 1);
					text = settings.executable[i] + " " + sTemp;
				}
			}
		}
	}

	return text;
}

//<getters>
_conf config::getConf()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings;
}

std::string config::getOriginFolder()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.originFolder;
}

std::string config::getStartFolder()
{
	return settings.startFolder;
}

std::string config::getGuiInputColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.guiInputColor;
}

std::string config::getGuiOutputColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.guiOutputColor;
}

std::string config::getGuiDirColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.guiDirColor;
}

int config::getAliasCount()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.aliasCount;
}

bool config::getBasicMode()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.basicMode;
}

bool config::getNoTextMode()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.noTextMode;
}

bool config::getDebug()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.debug;
}

bool config::getConfigPresent()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.configPresent;
}

int config::getInputTextColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.inputTextColor;
}

int config::getOutputTextColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.outputTextColor;
}

int config::getBgColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.bgColor;
}

int config::getDirColor()
{
	if (!settings.configPresent)
		apiHandle.alert("No config present!");
	return settings.dirColor;
}
//</getters>

//<setters>
void config::setStartFolder(std::string value)
{
	settings.startFolder = value;
}
void config::setGuiInputColor(std::string value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.guiInputColor = value;
}

void config::setGuiOutputColor(std::string value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.guiOutputColor = value;
}

void config::setGuiDirColor(std::string value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.guiDirColor = value;
}

void config::setDebug(bool value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.debug = value;
}

void config::setBasicMode(bool value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.basicMode = value;
}

void config::setNoTextMode(bool value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.noTextMode = value;
}

void config::setConf(_conf value)
{
	settings = value;

	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");
}

void config::setInputTextColor(int value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.inputTextColor = value;
}

void config::setOutputTextColor(int value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.outputTextColor = value;
}

void config::setBgColor(int value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.bgColor = value;
}

void config::setDirColor(int value)
{
	if (!settings.configPresent)
		apiHandle.alert("No config present! Please create one to make your changes permanent");

	settings.dirColor = value;
}
//</setters>

_conf SetPath() // soon to be depreciated
{
	_conf conf;
	// Note on sOrigin: it is only really used in order to determine drive letter for mobile installs currently
	// It will also determine the directory that the application starts in. (assuming no directory is supplied)
	conf.originFolder = _getcwd(NULL, 0);
	conf.configPresent = false; // Automatically assumes a config file is not present (this is changed if necessary later)
	conf.aliasCount = 0;

	ifstream ifile(".ascew");

	if (!ifile)
	{
		// Tries the appdata folder when there isn't a local config
		ifile.close();
		ifile.open(conf.originFolder + "ascew/.ascew");
	}

	if (ifile)
	{										 // Get the variables from the file opened
		string sLine;						 // Holds each line as the file is read
		int iCount = 0;						 // Holds the amount of variables read
		string sTemp;						 // Holds each specific part to each line as it's parsed
		bool bSet[] = {false, false, false}; // Checks if colors are set
		int iRGBcolor = 0;					 // holds the color index

		while (getline(ifile, sLine))
		{ // Iterates through the lines in the file
			istringstream iss(sLine);

			if (sLine[0] == '#')
			{ // Either a comment or cosmetic setting
				iss >> sTemp;
				if (sTemp == "#color=")
				{ // User input text color
					iss >> sTemp;
					iRGBcolor = atoi(sTemp.c_str());
					if (iRGBcolor < 16)
					{
						conf.inputTextColor = iRGBcolor;
					}
					else
					{
						conf.inputTextColor = 15;
					}
					bSet[0] = true;
				}
				else if (sTemp == "#bgcolor=")
				{ // Background color
					iss >> sTemp;
					iRGBcolor = atoi(sTemp.c_str());
					if (iRGBcolor < 16)
					{
						conf.bgColor = iRGBcolor;
					}
					else
					{
						conf.bgColor = 0;
					}
					bSet[1] = true;
				}
				else if (sTemp == "#dircolor=")
				{ // Directory text color (C:/>)
					iss >> sTemp;
					iRGBcolor = atoi(sTemp.c_str());
					if (iRGBcolor < 16)
					{
						conf.dirColor = iRGBcolor;
					}
					else
					{
						conf.dirColor = 12;
					}
					bSet[2] = true;
				}
			}
			else if (sLine.empty())
			{
				// The line is empty
			}
			else
			{ // Stores the first two words of text as an executable conf, then alias
				iss >> sTemp;
				// Allows programs (when run off a flash drive) to be dynamic
				if (sTemp[0] == ':' && sTemp[1] == '/' || sTemp[1] == '\\')
				{
					sTemp = conf.originFolder[0] + sTemp;
				}

				conf.executable.push_back(sTemp);

				iss >> sTemp;

				conf.alias.push_back(sTemp);
				iCount++;
			}
		}

		if (!bSet[0])
		{
			conf.inputTextColor = 15;
		}
		if (!bSet[1])
		{
			conf.bgColor = 0;
		}
		if (!bSet[2])
		{
			conf.dirColor = 12;
		}

		conf.outputTextColor = conf.inputTextColor; // Old config had these both be the same. In order to be compatible, the same convention is held here

		// Determines if there are errors in the .ascew file
		if (conf.executable.size() == conf.alias.size())
		{
			conf.aliasCount = iCount;
			conf.configPresent = true;
		}
		else
		{
			string sInput = "";
			cout << "ERROR: ascew configuration file contains errors."
				 << "\n"
				 << "Display config table contents?(Y/N): ";
			getline(cin, sInput);

			if (sInput == "Y" || sInput == "y")
			{
				// Determines which vector has the discrepency
				if (conf.executable.size() > conf.alias.size())
				{
					// Prints the contents of both vectors to the console
					for (int i = 0; i < conf.executable.size(); i++)
					{
						cout << "Executable: " << conf.executable[i] << "\n";
						cout << "Alias: ";

						if (conf.alias.size() > i)
						{
							cout << conf.alias[i] << "\n";
						}
						else
						{
							cout << "<Not Set>"
								 << "\n";
						}
					}
				}
				else
				{
					// Prints the contents of both vectors to the console
					for (int i = 0; i < conf.alias.size(); i++)
					{
						cout << "Executable: ";

						if (conf.executable.size() > i)
						{
							cout << conf.executable[i] << "\n";
						}
						else
						{
							cout << "<Not Set>"
								 << "\n";
						}

						cout << "Alias: " << conf.alias[i] << "\n";
					}
				}

				// Waits for the user to examine the information
				cout << "\n\n"
					 << "Press any key to start the program...";
				cin.get();

				system("cls"); // Clears the screen and proceeds to start the program
			}
			else if (sInput == "N" || sInput == "n")
			{
				system("cls"); // Clears the screen and proceeds to start the program
			}
		}
	}
	else
	{
		// RGB values are ALWAYS set
		conf.inputTextColor = 15;
		conf.bgColor = 0;
		conf.dirColor = 12;
	}

	conf.inputTextColor = conf.inputTextColor + conf.bgColor * 16;
	conf.dirColor = conf.dirColor + conf.bgColor * 16;

	return conf;
}
