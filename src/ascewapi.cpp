#include <iostream>
#include <iomanip>

#include <windows.h> // for title change function

#include "ascewapi.h" // Source header file

using namespace std;

ascewapi::ascewapi()
{
	//<required parts>
	// settings = c;
	//</required parts>

	if(!settings.basicMode)
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
}

void ascewapi::print(std::string text)
{
	if(!settings.basicMode)
	{
		SetConsoleTextAttribute(hConsole,settings.outputTextColor);
	}
	
	cout << "\n"
		 << text << "\n";

}

void ascewapi::alert(std::string text)
{
	if(!settings.basicMode)
	{
		SetConsoleTextAttribute(hConsole,settings.outputTextColor);
	}

	cout << "\n"
		 << "ERROR FEED:" << "\n"
		 << text << "\n";
}

void ascewapi::setTitle(std::string text)
{
	SetConsoleTitle("ascew " + text.c_str());
}

void ascewapi::clear()
{
	system("cls");
}

std::string ascewapi::getMiscInput()
{
	std::string input;
	getline(cin,input);
	return input;
}

std::string ascewapi::getMiscInput(std::string text)
{
	print(text);
	if(!settings.basicMode)
	{
		SetConsoleTextAttribute(hConsole,settings.inputTextColor);
	}
	std::string input;
	getline(cin,input);
	return input;
}

void ascewapi::setConf(_conf c)
{
	settings = c;
}