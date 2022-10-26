#ifndef GAME_GEO_MENUS_H
#define GAME_GEO_MENUS_H

#include "geometria.h"

struct GeoMenus
{
	static std::vector<std::pair<std::string, std::string>> savedCmds;

	static void LoadAllGeoMenus();

	static bool IsPythonScriptSecure(std::string content);

	static void Run(std::string url, bool runWithPrivileges, std::string command);

	static std::string GetPackageCommand()
	{
		std::string input;

		std::cout << "-> Set a command prefix " << std::endl;
		std::cout << "|  (press Enter without sending anything " << std::endl;
		std::cout << "|  if you want to skip it, you can add it" << std::endl;
		std::cout << "|  later in the \"Geometria.py\" script)." << std::endl;
		std::cout << "-->> ";
		std::cin >> input;

		if(StringAPI::IsOnlyLetters)
		{
			return input;
		}
		else
		{
			std::cout << "|" << std::endl;
			std::cout << "-> Sorry, but this can only contain letters from A to Z. Try again." << std::endl;
			std::cout << "|" << std::endl;
			return GetPackageCommand();
		}
	}

	static void CreatePackage(std::string name)
	{
		if(name == "")
		{
			std::cout << "Sorry, but the name is empty: " << std::endl;
			std::cout << std::endl;
			std::cout << "Usage: \"--create-package <name>\"" << std::endl;
			exit(0);
		}

		std::string fullPath = "Geometria/Game/Packages/" + name;
		if(!Files::DirectoryExists(fullPath.c_str()))
		{
			std::cout << "Creating " << name << "..." << std::endl;
			std::cout << "-> Creating Package Folder..." << std::endl;

			Files::CreateDirectory(fullPath.c_str());

			std::string input = GetPackageCommand();

			std::cout << "-> Creating Geometria.py..." << std::endl;

			std::string content;

			content += "start Geometria\n\n";

			content += "class Project:\n";
			content += "\tname = \"" + name + "\" # Your project's name.\n";
			content += "\tversion = \"0.1\" # Your project's version.\n";
			content += "\tcmd = \"" + input + "\" # Your project's command prefix.\n\n";

			content += "Dependencies = [] # Add your dependencies here.\n\n";

			content += "def hello():\n";
			content += "\tprint(f\"\"\"\n";
			content += "Welcome to {Project.name}! (v{Project.version})\n\n";

			content += "To get started, run:\n\n";

			content += "\t\"{Project.cmd} add\"\n\n";

			content += "to add this package to your Geometria project.\n";
			content += "\"\"\")\n\n";

			content += "end_safezone() # The wall between normal and privileged operations. Removing this from the script makes the entire Geometria.py code unusable.\n";

			std::string fullGeometriaPYPath = fullPath + "/Geometria.py";
			Files::Write(fullGeometriaPYPath.c_str(), content);

			std::cout << "-> Geometria Package \"" << name << "\" successfully created!" << std::endl;
			std::cout << "--> Go check it out in \"" << fullPath << "\"" << std::endl;
		}
		else
		{
			std::cout << "Sorry, but this folder already exists... Try again with a different name." << std::endl;
			exit(0);
		}
	}
};

#endif