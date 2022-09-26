#pragma once

#include "geometria.h"
#include "MainMenu.h"

struct GameMain
{
	static void Init()
	{
		MainMenu::Run();
	}
};
