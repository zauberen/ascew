#ifndef ASCEW_API_H
#define ASCEW_API_H
/** Front-end API v2.0.0
 * This API is used to tunnel output from functions and config appropriately for the given frontend.
 * A template is provided for this class, simply fill in the functions 
 * that are noted in the comments below.
 * Required functions: print, alert, setTitle
 * Also required: an exit function in the front-end. (not required to be in ascewapi)
 * Note: To ensure compatibility, make sure the above version number corresponds with the backend 
 *  version number found in config.h and console.h. Further versions may have more required functions.
 */

#include <string>
#include <vector>
#include <windows.h>

LPCWSTR s2lpcwstr(const std::string& s); // hack to make strings compliant for some windows functions

typedef struct 
{
	std::string originFolder; // Stores the folder that ascew is located in.
	std::string startFolder; // Stores the directory in which the application opens to by default.
	std::vector<std::string> executable; // Paths to the files in relation to to executable. eg ../../nano/nano.exe
	std::vector<std::string> alias; // Alias to the related executable path. eg nano
	bool basicMode; // Stores whether the application has been started in basic mode
	bool noTextMode; // Stores whether the application has been started in no-text mode
	bool configPresent; // Stores whether there is a .ascew file
	bool debug; // Stores whether the application is in debug mode
	int aliasCount; // The amount of items in the executable array
	int inputTextColor; // color of user input text d = 15
	int outputTextColor; // color of output text d = 15
	std::string guiInputColor; // color of user input text in gui mode
	std::string guiOutputColor; // color of output text in gui mode
	std::string guiBgColor; // background color in gui mode
	std::string guiDirColor; //color of directory output
	int bgColor; // Background color d = 0
	int dirColor; // Directory output color d = 12
} _conf;

class ascewapi
{
public:
	ascewapi();

	// <api functions>
	void print(std::string text); // This is normal output from an internal class, output this as normal text (eg: the help command uses this)
	void alert(std::string text); // This is output from internal errors in backend classes. (eg: if DEBUG is enabled, this will be where it is tunnelled, also if config has issues setting variables/etc and it needs reporting)
	void setTitle(std::string text); // This is output from an internal function such as cd which changes the title of the application.
	void clear(); // This is to clear the console area. an example in cmd would be "cls"
	std::string getMiscInput(); // This is used to get input not related to commands.
	std::string getMiscInput(std::string text); // Allows a prompt before the input check
	// </api functions>
	
	void setConf(_conf c); // Set the _path variable to a new struct
private:
	_conf settings; //This is here to allow access to things such as color variables in the api.
	HANDLE hConsole; // Handle for console colors
};
#endif