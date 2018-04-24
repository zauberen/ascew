#ifndef CONSOLE_H
#define CONSOLE_H
/** Console Back-end v2.0.0
 * This class will handle all of the console functionality, which includes built in functions
 * such as help and cd, but also handling all user input related to the console. An example 
 * of this functionality would be "ipconfig /all".
 * The main functionalities which this class will NOT do are:
 * 1. Handling configuration files (this is handled by config.h)
 * 2. Cosmetic things. (color of various parts of the console) (this functionality is handled by the front-end).
 *
 * Note: To ensure compatibility, make sure the above version number corresponds with the backend 
 *  version number found in config.h and ascewapi.h.
 */

#include "config.hpp"
#include "ascewapi.hpp"

class console
{
public:
	console(bool debug, bool noTextMode, bool basicMode, ascewapi api, config conf);

	void parse(std::string text); // This function parses user input and will output with ascewapi.print the results.
	void greeting(); // prints the greeting if config.settings.noTextMode is false
	// Getters
	_conf getConf();
	std::string getCurrentDir();
	// Setters
	void setConf(_conf c);
	void setCurrentDir(std::string path);
private:
	void cd(std::string text); // cd command clone
	void help();

	// Version/author
	std::string version;
	std::string author;

	ascewapi apiHandle; // Required for functioning with the front-end
	config ascewConfig; // Handles the ascew configuration file;
	std::string currentDir; // The current working directory.
	/* These have been moved to config
	bool mobileInstall; // Determines what the installtype is, set in the constructor (default = false)
	bool DEBUG; // User defined variable, turns on a verbose feed of variables during internal functions
	bool basicMode; // User defined variable, uses default colors/no config
	bool noTextMode; // User defined variable which hides opening text
	*/
};
#endif