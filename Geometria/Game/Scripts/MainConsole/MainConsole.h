#ifndef GAME_MAIN_CONSOLE_H
#define GAME_MAIN_CONSOLE_H

#include "geometria.h"

struct MainConsole
{
	static void Start();

	static std::string _input;
	static std::vector<std::string> _cmdWhiteList;

	static void PrintWelcomeScreen();
	static void PrintArrow();
	static std::string& GetInput();
	static void WaitForInput();
	static void RunCommand();

	static std::vector<std::string>& CmdWhiteList();
	static void ConsoleLogic();
};

#endif