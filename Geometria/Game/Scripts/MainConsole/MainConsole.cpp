#include "MainConsole.h"
#include "Game/Scripts/GeoMenus/GeoMenus.h"

std::string MainConsole::_input;
std::vector<std::string> MainConsole::_cmdWhiteList = {

	"--compile",
	"--path-compiler",
	"--run",
	"--update-engine",
	"--update-toolkit",
	"--create",
	"--get-exec-path",
	"--version",
	"--macro-test",
	"--geomenu",
	"--create-package"

};

void MainConsole::Start()
{
	PrintWelcomeScreen();
	ConsoleLogic();
}

void MainConsole::ConsoleLogic()
{
	PrintArrow();
	WaitForInput();
	RunCommand();
}

void MainConsole::PrintWelcomeScreen()
{
	std::cout << "Welcome to Hub/geo's Full CLI Mode! (v0.2.5)" << "\n";
}

void MainConsole::PrintArrow()
{
	std::cout << ">> ";
}

void MainConsole::WaitForInput()
{
	std::getline(std::cin, GetInput());
}

std::string& MainConsole::GetInput()
{
	return _input;
}

std::vector<std::string>& MainConsole::CmdWhiteList()
{
	return _cmdWhiteList;
}

void MainConsole::RunCommand()
{
	if(_input == "--clear")
		Files::ClearConsole();
	else if(_input == "--exit")
		exit(0);
	else
	{
		for(auto i : GeoMenus::savedCmds)
		{
			_cmdWhiteList.push_back(i.second);
		}

		bool nothingFound = true;
		for(auto i : CmdWhiteList())
		{
			if(GetInput().find(i) != std::string::npos)
			{
				std::string finalCmd = "geo " + GetInput();
				//std::cout << finalCmd << std::endl;
				system(finalCmd.c_str());
				nothingFound = false;
				break;
			}
		}

		if(nothingFound)
		{
			std::cout << "Sorry, but no such command was found...\n";
		}
	}

	ConsoleLogic();
}