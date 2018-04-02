#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include "ascewapi.h"

/** Config Back-end v2.0.0
 * This class handles all configuration file-based backend work.
 * This class will find, read, and store the information in a .ascew file,
 * the class's functions can then be used to access and 
 * change said information.
 * 
 * Note: To ensure compatibility, make sure the above version number corresponds with the backend 
 *  version number found in console.h and ascew.h.
 */

_conf SetPath(); // Sets up the _path struct using the .ascew file (if present) (*Depreciated*)

// Replacement for ALL of the functions of the _path variable.
// Current functions: All functions of the previous format (reads/stores all config stuff), adds a few new variables
// Planned functions: Allow the ability to write .ascew in-app (eg. changevar bgColor 2), read set config vars in app (eg. getvar aliases (which would return both vectors in a table-esque format))
class config
{
public:
	config(); // constructor
	bool readConfig(); // Reads the user.ascew and machine.ascew files into the variables of the class.
	void saveConfig(); // Prints the stored _conf var to machine.ascew
	void createConfig(); //Creates a new config fileset with the default options.
	std::string checkInput(std::string text); // Determines if the given string command is a stored custom command.
	// Getters
	_conf getConf();
	std::string getOriginFolder();
	std::string getStartFolder();
	std::string getGuiInputColor();
	std::string getGuiOutputColor();
	std::string getGuiDirColor();
	int getAliasCount();
	bool getBasicMode();
	bool getNoTextMode();
	bool getDebug();
	bool getConfigPresent();
	int getInputTextColor();
	int getOutputTextColor();
	int getBgColor();
	int getDirColor();
	// Setters
	void setStartFolder(std::string value);
	void setGuiInputColor(std::string value);
	void setGuiOutputColor(std::string value);
	void setGuiDirColor(std::string value);
	void setDebug(bool value);
	void setBasicMode(bool value);
	void setNoTextMode(bool value);
	void setConf(_conf value);
	void setInputTextColor(int value);
	void setOutputTextColor(int value);
	void setBgColor(int value);
	void setDirColor(int value);
private:
	ascewapi apiHandle; // Required for functioning with the front-end
	_conf settings;
};

#endif