#include "GeoMenus.h"
#include <experimental/filesystem>

std::vector<std::pair<std::string, std::string>> GeoMenus::savedCmds;

void GeoMenus::LoadAllGeoMenus()
{
	std::experimental::filesystem::path geoMenusPath = std::string(Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/GeoMenus");

	if(!Files::DirectoryExists(geoMenusPath.u8string().c_str()))
		Files::CreateDirectory(geoMenusPath.u8string().c_str());

	for(auto& p: std::experimental::filesystem::recursive_directory_iterator(geoMenusPath))
	{
		std::string pathToStr = p.path().u8string();

		if(pathToStr.find(".py") != std::string::npos)
		{
			std::string name = Files::GetFilenameFromDirectory(pathToStr.c_str());
			name = StringAPI::RemoveAll(name, ".py");
			savedCmds.push_back(std::make_pair(pathToStr, name));
		}
	}
}

bool GeoMenus::IsPythonScriptSecure(std::string content)
{
	std::string contentWithHello = StringAPI::GetSubstringBetween(content, "start Geometria", "def");
	contentWithHello += StringAPI::GetSubstringBetween(content, "hello():", "import");

	if(contentWithHello.find("import") != std::string::npos)
	{
		std::cout << "For security reasons, the use of \"import\" before \"hello()\"\n";
		std::cout << "(even if its mentioned inside strings/text) is not allowed.\n\n";

		return false;
	}

	return true;
}

void GeoMenus::Run(std::string url, bool runWithPrivileges, std::string command)
{
	std::string input;

	if(command != "")
		std::cout << "To run a command, this script needs elevated privileges.\n";

	if(command != "" || runWithPrivileges)
	{
		std::cout << "Are you sure you want to execute this with elevated privileges? (y/n)\n";
		std::cout << ">> ";
		std::cin >> input;

		if(input == "y" || input == "yes")
			runWithPrivileges = true;
		else
			exit(0);
	}

	std::string finalUrl;

	std::cout << "Current command: \"" << command << "\"\n";

	if(url == "")
		finalUrl = "Geometria.py";
	else if(url.find("local/") != std::string::npos)
	{
		finalUrl = url;
		finalUrl = StringAPI::ReplaceAll(finalUrl, "local/", "Geometria/Game/Packages/");
		finalUrl += "/Geometria.py";
	}
	else if(url.find("github/") != std::string::npos)
	{
		std::cout << std::endl;
		std::cout << "Loading..." << std::endl;

		finalUrl = url;
		finalUrl = StringAPI::RemoveAll(finalUrl, "github/");
		finalUrl = "https://raw.githubusercontent.com/" + finalUrl + "/Geometria.py";
	}
	else if(url.find(".py") != std::string::npos)
	{
		finalUrl = url;
	}

	bool isWeb = finalUrl.find("http") != std::string::npos;
	bool doesExist = std::experimental::filesystem::exists(finalUrl.c_str()) || isWeb;

	if(doesExist)
	{
		std::string currentGM;

		if(!isWeb)
			currentGM = Files::Read(finalUrl.c_str());
		else
		{
			std::cout << "Downloading Geometria.py from the Web..." << std::endl;
			currentGM = Web::Get(finalUrl.c_str(), true);

			if(currentGM == "404: Not Found")
			{
				std::cout << "[ERROR] " << currentGM << std::endl;
				exit(0);
			}
		}

		std::string projectCmd;

		std::vector<std::string> GMlines;

		if(currentGM.find("\r\n") != std::string::npos)
			currentGM = StringAPI::ReplaceAll(currentGM, "\r\n", "\n");

		GMlines = StringAPI::SplitIntoVector(currentGM, "\n");

		for(auto i : GMlines)
		{
			if(i.find("cmd = ") != std::string::npos)
			{
				projectCmd = i;
				break;
			}
		}

		projectCmd = StringAPI::RemoveAll(projectCmd, "\"");
		projectCmd = StringAPI::RemoveAll(projectCmd, "cmd = ");
		projectCmd = StringAPI::RemoveAll(projectCmd, " ");

		std::string comments = StringAPI::GetSubstringBetween(projectCmd, "#", "");

		if(comments != "")
			projectCmd = StringAPI::RemoveAll(projectCmd, comments.c_str());

		projectCmd = StringAPI::RemoveAll(projectCmd, "#");
		projectCmd = StringAPI::RemoveAll(projectCmd, "\t");
		projectCmd = StringAPI::RemoveAll(projectCmd, "\n");
		projectCmd = StringAPI::RemoveAll(projectCmd, "\r\n");
		projectCmd = StringAPI::RemoveAll(projectCmd, "\r");

		if(isWeb)
		{
			finalUrl = "GeoMenus/" + projectCmd + ".py";



			std::cout << "Writing locally to \"" << finalUrl << "\" ..." << std::endl;
			Files::Write(finalUrl.c_str(), currentGM);
		}

		if(IsPythonScriptSecure(currentGM) || runWithPrivileges)
		{
			std::string modifiedContent;
			if(!runWithPrivileges)
			{
				std::string contentWithoutImports;
				contentWithoutImports = StringAPI::GetSubstringBetween(currentGM, "end_safezone()", "");

				std::vector<std::string> getContentLines = StringAPI::SplitIntoVector(contentWithoutImports, "\n");
				for(auto i : getContentLines)
				{
					std::string iModified = "# " + i;
					i = iModified;
				}

				contentWithoutImports = "start Geometria";
				contentWithoutImports += StringAPI::GetSubstringBetween(currentGM, "start Geometria", "end_safezone()");
				contentWithoutImports += "#end_safezone()";
				for(auto i : getContentLines)
				{
					contentWithoutImports += "# " + i + "\n";
				}

				modifiedContent = contentWithoutImports;
			}
			else
				modifiedContent = currentGM;

			modifiedContent = StringAPI::ReplaceAll(modifiedContent, "start Geometria", "from Tools.PythonAPI.geomenuapi import *");
			Files::Write(finalUrl.c_str(), modifiedContent);

			std::string pathToPython = StringAPI::RemoveAll(finalUrl, ".py");
			pathToPython = StringAPI::ReplaceAll(pathToPython, "/", ".");
			pathToPython = StringAPI::ReplaceAll(pathToPython, "\\", ".");

			std::string pythonPath;
			if(pathToPython.find("Geometria.Game.Packages") != std::string::npos)
			{
				pythonPath = StringAPI::GetSubstringBetween(pathToPython, "Packages.", "");
			}
			else if(pathToPython.find("GeoMenus") != std::string::npos)
			{
				pythonPath = StringAPI::GetSubstringBetween(pathToPython, "GeoMenus.", "");
				if(pythonPath.find("GeoMenus.") == std::string::npos)
					pythonPath = "GeoMenus." + pythonPath;
			}
			else if(url == "")
			{
				pythonPath = "Geometria";
			}

			Files::ClearConsole();

			if(command == "")
			{
				std::string finalCmd = "python -c \"import sys; sys.path.insert(1, 'Geometria/Game/Packages'); del sys; from " + pythonPath + " import hello; hello();\"";
				system(finalCmd.c_str());
			}
			else
			{
				std::string finalCmd = "python -c \"import sys; sys.path.insert(1, 'Geometria/Game/Packages'); del sys; from " + pythonPath + " import cmd; cmd('" + command + "');\"";
				system(finalCmd.c_str());
			}

			Files::Write(finalUrl.c_str(), currentGM);

			if(url != "Geometria.py")
			{
				std::string newMenuPath = "GeoMenus/" + projectCmd + ".py";
				Files::Write(newMenuPath.c_str(), currentGM);
			}
		}
	}
	else
	{
		std::cout << "Geometria.py not found!" << std::endl;
	}
}