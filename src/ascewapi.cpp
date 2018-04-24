#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h> // for title change function

#include "ascewapi.hpp" // Source header file

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
	LPCSTR t = ("ascew " + text).c_str(); // !This works even though it throws an error!
	SetConsoleTitle(t);
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

LPCWSTR s2lpcwstr(const std::string& s)
{
	// hack credit to https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode with minor modifications
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring ws(buf);
    delete[] buf;
	LPCWSTR r = ws.c_str();
    return r;
}