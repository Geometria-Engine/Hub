#pragma once

#ifndef HUBMANAGER_H
#define HUBMANAGER_H

#include "geometria.h"

struct HubManager : public ScriptBehaviour
{
	static bool defaultValuesAdded;
	static ImGUIElement* win;
	static ImGUIElement* status;
	static bool creating, isGitThreadSpawned, isGitFinished, isProjectCreationFinished, isFromCommandLine;
	static std::string currentUrl, currentProjectName;

	void OnStartup()
	{
		isUniversal = true;
		defaultValuesAdded = false;
		creating = false;
	}

	void OnUpdate()
	{
		if (!defaultValuesAdded)
		{
			if (win != nullptr)
			{
				ImGUIElement* dirInput = win->FindElementWithName("Directory");
				if (dirInput != nullptr)
				{
					dirInput->strData = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/Projects";
					std::cout << "[STRDATA] " << dirInput->strData << std::endl;
				}
				else
				{
					std::cout << "[WARNING] Dir Input is nullptr!" << std::endl;
				}
				ImGUIElement* exitBtn = win->FindElementWithName("CloseBtn");
				
				if (exitBtn != nullptr)
					exitBtn->OnClick([this] {ExitButton(); });

				Files::ClearConsole();
				defaultValuesAdded = true;
			}
			else
				std::cout << "[WARNING] win is nullptr!" << std::endl;
		}

		std::string bMessage = Broadcast::ShowBroadcastMessage("--hub-create {Project Name}|{Directory}");

		bool startCreating = Broadcast::Recieve(bMessage) || Input::GetKeyDown(GLFW_KEY_ENTER);
		if (startCreating && !creating)
		{
			bMessage = StringAPI::RemoveAll(bMessage, "--hub-create ");
			std::vector<std::string> split = StringAPI::SplitIntoVector(bMessage, "|");

			if (split[0] != "" || split[1] != "")
			{
				CreateProject(split[0], split[1]);
				Broadcast::Send("--hub-creating");
				creating = true;
			}
		}

		if (creating)
		{
			if (isGitThreadSpawned && isGitFinished)
			{
				std::cout << "Finished Downloading!" << std::endl;
				std::cout << "Setting Up Project..." << std::endl;
				SetUpProject();
				isGitThreadSpawned = isGitFinished = false;
			}

			if (isProjectCreationFinished)
			{
				if (status != nullptr)
				{
					status->text = "Done Creating \"" + currentProjectName + "\"!";
				}

				std::cout << "Done Creating \"" + currentProjectName + "\"!" << std::endl;

				isProjectCreationFinished = false;
				creating = false;
			}
		}

	}

	static void ChangeCurrentURL(std::string path)
	{
		currentUrl = path;
	}

	static void ChangeCurrentProjectName(std::string path)
	{
		currentProjectName = path;
	}

	static void DownloadPremakeFromGitHubRepo(std::string path)
	{
		std::string content = Web::Get("https://raw.githubusercontent.com/Geometria-Engine/Geometria/main/premake5.lua", false);
		Files::Write(path.c_str(), content);
	}

	static void SetUpProject()
	{
		std::string premakeUrl = currentUrl + "/premake5.lua";
		std::string premakeFile = Files::Read(premakeUrl.c_str());
		if(premakeFile == "")
		{
			std::cout << "WARNING: premake5.lua is empty! Trying to download from GitHub repo...";
			DownloadPremakeFromGitHubRepo(premakeUrl);
			premakeFile = Files::Read(premakeUrl.c_str());
		}

		std::string projectNameWithNoSpaces = StringAPI::ReplaceAll(currentProjectName, " ", "");

		premakeFile = StringAPI::ReplaceAll(premakeFile,
			"project \"Geometria\"",
			"project \"" + projectNameWithNoSpaces + "\"");

		premakeFile = StringAPI::ReplaceAll(premakeFile,
			"workspace \"Geometria\"",
			"workspace \"" + projectNameWithNoSpaces + "\"");

		premakeFile = StringAPI::ReplaceAll(premakeFile,
			"startproject \"Geometria\"",
			"startproject \"" + projectNameWithNoSpaces + "\"");

		premakeFile = StringAPI::ReplaceAll(premakeFile,
			"project \"\"",
			"project \"" + projectNameWithNoSpaces + "\"");

		premakeFile = StringAPI::ReplaceAll(premakeFile,
			"workspace \"\"",
			"workspace \"" + projectNameWithNoSpaces + "\"");

		premakeFile = StringAPI::ReplaceAll(premakeFile,
			"startproject \"\"",
			"startproject \"" + projectNameWithNoSpaces + "\"");

		Files::Write(premakeUrl.c_str(), premakeFile);

		BuildProjectFile();
		CopyPasteToolkit();
	}

	static void BuildProjectFile()
	{
		std::string premakeExecutable, premakeCommand, premakeLuaFile;
		std::string visualStudioVersion;

		if (Application::IsPlatform(Application::Windows))
		{
			premakeExecutable = Files::ConvertToWindowsCmdPath(currentUrl) + "/premake5-windows.exe";
			premakeLuaFile = currentUrl + "/premake5.lua";
			visualStudioVersion = Files::GetValueFromCommand(Files::ConvertToWindowsCmdPath("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe") + " -latest -prerelease -products * -property productLineVersion");
			Files::ChangeCurrentDirectory(currentUrl);
			premakeCommand = premakeExecutable + " --file=\"" + premakeLuaFile + "\" vs" + visualStudioVersion;
		}
		else if (Application::IsPlatform(Application::Linux))
		{
			premakeExecutable = "./" + currentUrl + "/premake5-linux";
			premakeCommand = premakeExecutable + " gmake2";
		}

		if (premakeCommand != "")
		{
			system(premakeCommand.c_str());
			isProjectCreationFinished = true;
		}
	}

	static void CopyPasteToolkit()
	{
		if(Application::IsPlatform(Application::Windows))
		{
			std::string cnt = Files::Read(Files::GetExecutablePath().c_str(), true);
			std::string finalUrl = currentUrl + "/Geometria.exe";
			Files::Write(finalUrl.c_str(), cnt);
		}
	}

	static void OpenPrjInFileExplorer()
	{

	}

	void ExitButton()
	{
		std::cout << "Exiting!" << std::endl;
		Graphics::Exit();
	}

	static void LoadGitDownload()
	{
		system("start https://git-scm.com/downloads");
	}

	static void LoadVSBuildToolsDownload()
	{
		system("start https://visualstudio.microsoft.com/visual-cpp-build-tools/");
	}

	static void CreateProject(std::string projectName, std::string url)
	{
		if(win != nullptr)
		{
			status = win->FindElementWithName("Status");

			win->FindElementWithName("GitButton")->OnClick([&] {LoadGitDownload(); });
			win->FindElementWithName("VSBuildToolsButton")->OnClick([&] {LoadVSBuildToolsDownload(); });
		}

		CreateProjectDirectories(projectName, url);
	}

	static void GitClone()
	{
		std::string gitCmd = "git -C \"" + currentUrl + "\" clone https://github.com/Geometria-Engine/Geometria.git .";
		system(gitCmd.c_str()); 
		isGitFinished = true;
	}

	static void CreateProjectDirectories(std::string projectName, std::string url)
	{
		std::string finalUrl = url + "/" + projectName;
		if (!Files::DirectoryExists(finalUrl.c_str()))
		{
			if (Application::IsPlatform(Application::Windows))
			{
				bool _gitInstalled = Files::WhereIs("git") != "";
				bool _vsbuildInstalled = Files::GetPathFromCommand(Files::ConvertToWindowsCmdPath("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe") + " -latest -prerelease -products * -requires Microsoft.Component.MSBuild -find MSBuild/**/Bin/MSBuild.exe") != "";

				if (_gitInstalled && _vsbuildInstalled)
				{
					Files::ClearConsole();
					if(status != nullptr)
						status->text = "Creating \"" + projectName + "\"...\n(See Console for more info...)";

					std::cout << "Creating \"" + projectName + "\"\n";

					currentUrl = finalUrl;
					currentProjectName = projectName;

					Files::CreateDirectory(finalUrl.c_str());

					if (!isGitThreadSpawned && !isFromCommandLine)
					{
						Multithreading::RunThread([&] {GitClone(); });
						isGitThreadSpawned = true;
					}
				}
				else
				{
					Broadcast::Send("install-box");

					win->FindElementWithName("GitButton")->isEnabled = true;
					win->FindElementWithName("VSBuildToolsButton")->isEnabled = true;

					if (_gitInstalled && !_vsbuildInstalled)
					{
						if (status != nullptr)
							status->text = "I'm sorry, but in order to continue\n you still need to install\n\"Visual Studio C++ Build Tools\".";

						std::cout << "I'm sorry, but in order to continue you still need to install \"Visual Studio C++ Build Tools\".\n";

						win->FindElementWithName("GitButton")->isEnabled = false;
					}
					else if (!_gitInstalled && _vsbuildInstalled)
					{
						if (status != nullptr)
							status->text = "I'm sorry, but in order to continue\n you still need to install\n\"Git\".";

						std::cout << "I'm sorry, but in order to continue you still need to install \"Git\".\n";

						win->FindElementWithName("VSBuildToolsButton")->isEnabled = false;
					}
					else if(!_gitInstalled && !_vsbuildInstalled)
					{
						if (status != nullptr)
							status->text = "I'm sorry, but in order to continue\n you need to install\n\"Git\"\nand\n\"Visual Studio C++ Build Tools\".";

						std::cout << "I'm sorry, but in order to continu you need to install \"Git\" and \"Visual Studio C++ Build Tools\".\n";
					}
				}
			}
		}
	}
};

#endif