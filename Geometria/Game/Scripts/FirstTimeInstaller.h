#ifndef FIRSTTIMEINSTALLER_H
#define FIRSTTIMEINSTALLER_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct FirstTimeInstaller
{

	static std::string input;
	static bool makeFirstTimeFile;
	static bool downloadGitInstaller;

	static std::string pkgMgr;
	static std::string linuxDistro;

	static void Start()
	{
		GetVariables();
		Introduction();
	}

	static void GetVariables()
	{
		if(Application::IsPlatform(Application::Linux))
		{
			pkgMgr = Linux::GetPkgManagerName();
			linuxDistro = Application::GetLinuxDistro();
		}

		Files::ClearConsole();
	}

	static void Introduction()
	{
		makeFirstTimeFile = false;

		Files::ClearConsole();
		std::cout << "Hello! Welcome to \"Geometria Hub\"!" << "\n";
		std::cout << "The official Toolkit for the \"Geometria Engine\"!" << "\n";
		std::cout << "\n";
		Files::PauseConsole();

		Files::ClearConsole();
		input = "";
		std::cout << "You're currently in the Pre-Installation phase because" << "\n";
		std::cout << "the program thinks this is the first time use" << "\n";
		std::cout << "on this computer." << "\n";
		std::cout << "\n";
		Files::PauseConsole();

		Files::ClearConsole();
		input = "";
		AskIfFirstTime();
	}

	static void AskIfFirstTime()
	{
		std::cout << "Is that true? (yes/no)" << "\n";
		std::cout << "\n";
		std::cout << "Type one of the two options" << "\n";
		std::cout << "and press (Enter) to continue..." << "\n";
		std::cin >> input;

		if(input == "yes")
			AgreeFirstTime();
		else if(input == "no")
			DisagreeFirstTime();
		else
		{
			Files::ClearConsole();
			std::cout << "You typed an unknown option, please try again." << "\n";
			std::cout << "\n";
			input = "";
			AskIfFirstTime();
		}
	}

	static void AgreeFirstTime()
	{
		Files::ClearConsole();
		input = "";

		std::cout << "Ok, so this is your first time!" << "\n";
		std::cout << "\n";
		std::cout << "Don't worry, that question was asked to make" << "\n";
		std::cout << "things faster for people that already went through this" << "\n";
		std::cout << "before." << "\n";
		std::cout << "\n";
		AskForInstallation();
	}

	static void DisagreeFirstTime()
	{
		Files::ClearConsole();
		input = "";
		makeFirstTimeFile = true;

		std::cout << "Ok, so this isn't your first time!" << "\n";
		std::cout << "\n";
		AskForInstallation();
	}

	static void AskForInstallation()
	{
		std::cout << "Do you want to begin with the installation process? (yes/no)" << "\n";
		std::cout << "\n";
		std::cout << "Type one of the two options" << "\n";
		std::cout << "and press (Enter) to continue..." << "\n";
		std::cin >> input;

		if(input == "yes")
		{
			Files::ClearConsole();
			input = "";
			AgreeInstall();
		}
		else if(input == "no")
			DisagreeInstall();
		else
		{
			Files::ClearConsole();
			std::cout << "You typed an unknown option, please try again." << "\n";
			std::cout << "\n";
			input = "";
			AskForInstallation();
		}
	}

	static void AgreeInstall()
	{
		std::cout << "Perfect!" << "\n";
		std::cout << "\n";
		std::cout << "Do you have any questions before installing? (yes/no)" << "\n";
		std::cout << "\n";
		std::cout << "Type one of the two options" << "\n";
		std::cout << "and press (Enter) to continue..." << "\n";
		std::cin >> input;

		if(input == "yes")
		{
			Files::ClearConsole();
			input = "";
			ShowQuestions();
		}
		else if(input == "no")
		{
			Installer::Install();
		}
		else
		{
			Files::ClearConsole();
			std::cout << "You typed an unknown option, please try again." << "\n";
			std::cout << "\n";
			input = "";

			AgreeInstall();
		}
	}

	static void DisagreeInstall()
	{
		Files::ClearConsole();
		input = "";

		std::cout << "No problem!" << "\n";
		std::cout << "\n";

		if(makeFirstTimeFile)
		{
			std::cout << "An empty file called \"firsttime\" has been created " << "\n";
			std::cout << "in the same folder as the executable." << "\n";
			std::cout << "If you want to go through this process again, make" << "\n";
			std::cout << "sure to delete it." << "\n";
			std::cout << "\n";
			Files::Write("firsttime", "");
		}

		std::cout << "Have a nice day! :D" << "\n";
		std::cout << "\n";
		Files::PauseConsole();

		exit(0);
	}

	static void ShowQuestions()
	{
		std::cout << "Go ahead! Pick a question!" << "\n";
		std::cout << "\n";
		std::cout << "1. What is it going to install?" << "\n";
		std::cout << "2. Is there any dependency i'll have to install manually after this?" << "\n";
		std::cout << "3. What happens after the installation is complete?" << "\n";
		std::cout << "4. I don't have any questions, feel free to install now." << "\n";
		std::cout << "\n";
		std::cout << "Type the number of one of these questions" << "\n";
		std::cout << "and press (Enter) to continue..." << "\n";
		std::cin >> input;

		if(input == "1")
			InstallQuestions::FirstQuestion();
		else if(input == "2")
			InstallQuestions::SecondQuestion();
		else if(input == "3")
			InstallQuestions::ThirdQuestion();
		else if(input == "4")
			Installer::Install();
		else
		{
			Files::ClearConsole();
			std::cout << "You typed an unknown option, please try again." << "\n";
			std::cout << "\n";
			input = "";

			ShowQuestions();
		}
	}

	struct InstallQuestions
	{
		static void FirstQuestion()
		{
			Files::ClearConsole();
			input = "";

			std::cout << "" << "\n";
			std::cout << "Q: What is it going to install?" << "\n";
			std::cout << "\n";

			if(Application::IsPlatform(Application::Platform::Windows))
			{
				std::cout << "A: Since you're on Windows, it'll install three things:" << "\n";
				std::cout << "\n";
				std::cout << "- Visual Studio C++ Build Tools." << "\n";
				std::cout << "- Windows SDK." << "\n";
				std::cout << "- Git." << "\n";
				std::cout << "\n";
			}
			else if(Application::IsPlatform(Application::Platform::Linux))
			{
				std::cout << "A: Since you're on " << FirstTimeInstaller::linuxDistro << ", and it uses " + FirstTimeInstaller::pkgMgr << "\n";
				std::cout << "as the package manager, it'll install all of these packages:" << "\n";
				std::cout << "\n";
				
				if(FirstTimeInstaller::pkgMgr == "APT")
				{
					for(auto i : Linux::APTPackageList())
						std::cout << "- " << i << "\n";
				}
				else if(FirstTimeInstaller::pkgMgr == "Pacman")
				{
					for(auto i : Linux::PacmanPackageList())
						std::cout << "- " << i << "\n";
				}

				std::cout << "\n";
			}

			std::cout << "The toolkit doesn't need any installation process, in fact" << "\n";
			std::cout << "you're using the toolkit right now! So after installation, you can open" << "\n";
			std::cout << "this executable again, and it'll open up, but in its normal phase." << "\n";
			std::cout << "\n";
			std::cout << "If you already have one of these components installed in" << "\n";
			std::cout << "your PC, it'll check if that's true and skip the process" << "\n";
			std::cout << "to save time." << "\n";
			std::cout << "\n";
			Files::PauseConsole();

			Files::ClearConsole();
			input = "";
			ShowQuestions();
		}

		static void SecondQuestion()
		{
			Files::ClearConsole();
			input = "";

			std::cout << "Q: Is there any dependency i'll have to install manually after this?" << "\n";
			std::cout << "\n";
			std::cout << "A: Unless something went wrong after the installation completed, " << "\n";
			std::cout << "you don't have to install anything after completing this!" << "\n";
			std::cout << "Once its done, you can start using the tools and the engine right away!" << "\n";
			std::cout << "\n";
			Files::PauseConsole();

			Files::ClearConsole();
			input = "";
			ShowQuestions();
		}

		static void ThirdQuestion()
		{
			Files::ClearConsole();
			input = "";

			std::cout << "Q: What happens after the installation is complete?" << "\n";
			std::cout << "\n";
			std::cout << "A: After the installation is complete, it'll inform you" << "\n";
			std::cout << "that finished successfully and exit afterwards." << "\n";
			std::cout << "\n";
			std::cout << "If you wanna start using the toolkit, you open this" << "\n";
			std::cout << "executable again, and it'll open in its GUI mode." << "\n";
			std::cout << "\n";
			Files::PauseConsole();

			Files::ClearConsole();
			input = "";
			ShowQuestions();
		}
	};

	struct Installer
	{
		static void MessageBeforeSetup()
		{
			std::cout << "All right, just two steps more before installation!" << "\n";
			std::cout << "After you press any key to continue, it'll start with" << "\n";
			std::cout << "simple setups of the tools it needs to install." << "\n";
			Files::PauseConsole();
		}

		static void Install()
		{
			std::string getVSDrive, getVSPath;

			if(Application::IsPlatform(Application::Windows))
			{
				Files::ClearConsole();
				input = "";
				MessageBeforeSetup();
	
				Files::ClearConsole();
				input = "";
				AskForDrive(getVSDrive);
	
				Files::ClearConsole();
				input = "";
				VS_PathInstallOptions(getVSDrive, getVSPath);
	
				Files::ClearConsole();
				input = "";
				CheckForGit();
	
				Files::ClearConsole();
				input = "";
				MessageBeforeInstalling();
	
				Files::ClearConsole();
				input = "";
				InstallVisualCPlusPlus(getVSPath);
	
				if(downloadGitInstaller)
				{
					InstallGit();
				}
			}
			else if(Application::IsPlatform(Application::Linux))
			{
				Files::ClearConsole();
				input = "";
				MessageBeforeInstalling();

				Linux::InstallPkgs();
			}

			InstallationFinish();
		}

		static void AskForDriveAgain(std::string& str)
		{
			Files::ClearConsole();
			std::cout << "You typed an unknown option, please try again." << "\n";
			std::cout << "\n";
			input = "";

			AskForDrive(str);
		}

		static void AskForDrive(std::string& str)
		{
			std::cout << "Visual Studio C++ Build Tools Setup (1/2) [1/3]" << "\n";
			std::cout << "Please select the drive where you want to install it:" << "\n";
			std::cout << "\n";

			std::vector<std::string> driveList = Files::GetListOfDrives();

			for(int i = 0; i < driveList.size(); i++)
				std::cout << i + 1 << ". " << driveList[i] << "\n";

			std::cout << "\n";
			std::cout << "Type the number of one of the options" << "\n";
			std::cout << "and press (Enter) to continue..." << "\n";
			std::cin >> input;

			if(StringAPI::IsOnlyDigits(input))
			{
				int inputToInt = std::stoi(input);
				inputToInt -= 1;
				if(inputToInt > -1 && inputToInt < driveList.size())
				{
					str = driveList[inputToInt];
				}
				else
					AskForDriveAgain(str);
			}
			else
				AskForDriveAgain(str);
		}

		static void VS_AskForInstallPathAgain(std::string drive, std::string& result)
		{
			Files::ClearConsole();
			std::cout << "You typed an unknown option, please try again." << "\n";
			std::cout << "\n";
			input = "";

			VS_PathInstallOptions(drive, result);
		}

		static void VS_PathInstallOptions(std::string drive, std::string& result)
		{
			std::vector<std::string> defaultPaths;
			defaultPaths.push_back(std::string(drive + "/Visual Studio/Desktop/BuildTools/"));
			defaultPaths.push_back(std::string(drive + "/Program Files/Visual Studio/Desktop/BuildTools/"));
			defaultPaths.push_back(std::string(drive + "/Visual Studio/BuildTools/"));
			defaultPaths.push_back(std::string(drive + "/Program Files/Visual Studio/BuildTools/"));

			std::cout << "Visual Studio C++ Build Tools Setup (2/2) [1/3]" << "\n";
			std::cout << "\n";
			std::cout << "Please select the path of your choice:" << "\n";

			for(int i = 0; i < defaultPaths.size(); i++)
			{
				std::cout << i + 1 << ". " << defaultPaths[i] << "\n";
			}

			std::cin >> input;

			if(StringAPI::IsOnlyDigits(input))
			{
				int inputToInt = std::stoi(input);

				inputToInt -= 1;
				if(inputToInt > -1 && inputToInt < defaultPaths.size())
				{
					result = defaultPaths[inputToInt];
				}
				else
					VS_AskForInstallPathAgain(drive, result);
			}
		}

		static void CheckForGit()
		{
			if(Files::WhereIs("git") == "")
			{
				input = "";
				std::cout << "It looks like you don't have \"git\" installed." << "\n";
				std::cout << "\n";
				std::cout << "Do you want the toolkit to download the installer? (yes/no)" << "\n";
				std::cout << "After VC++BT is installed, it'll execute it for you." << "\n";

				std::cout << "\n";
				std::cout << "Type one of the two options" << "\n";
				std::cout << "and press (Enter) to continue..." << "\n";
				std::cout << "\n";
				std::cin >> input;

				if(input == "yes")
				{
					downloadGitInstaller = true;
				}
				else if(input == "no")
				{
					downloadGitInstaller = false;
				}
				else
				{
					Files::ClearConsole();
					std::cout << "You typed an unknown option, please try again." << "\n";
					std::cout << "\n";
					input = "";
					CheckForGit();
				}
			}
		}

		static void MessageBeforeInstalling()
		{
			std::cout << "All right, now sit back, relax, get a cup of coffee or tea..." << "\n";
			std::cout << "\n";

			if(Application::IsPlatform(Application::Windows))
				std::cout << "After you press any key to continue, it'll install everything" << "\n";
			else if(Application::IsPlatform(Application::Linux))
				std::cout << "After you press Enter to continue, it'll install everything" << "\n";

			std::cout << "and take care of the rest." << "\n";
			Files::PauseConsole();
		}

		static void InstallVisualCPlusPlus(std::string vsPath)
		{
			std::cout << "Download Visual Studio C++ Build Tools..." << std::endl;
			std::string vsCnt = Web::Get("https://aka.ms/vs/17/release/vs_BuildTools.exe", true);
			Files::Write("vs_BuildTools.exe", vsCnt, true);
			if(std::experimental::filesystem::exists("vs_BuildTools.exe"))
			{
				std::vector<std::string> visualStudioPkgList;

				visualStudioPkgList.push_back("Microsoft.VisualStudio.Workload.VCTools");

				std::string visualStudioFullString;
				for(auto i : visualStudioPkgList)
					visualStudioFullString += " --add " + i;

				std::string fullCmd = "vs_BuildTools.exe --installPath \"" + vsPath + "\"" + visualStudioFullString + " --includeRecommended --passive --norestart";
				std::cout << fullCmd << std::endl;
				std::cout << "\n";
				std::cout << "\n";
				std::cout << "=========================\n";
				std::cout << "WARNING: Once you press any key to continue, \n";
				std::cout << "the installer is going to open and ask you for \n";
				std::cout << "administrator permissions.\n";
				std::cout << "=========================\n";
				std::cout << "\n";
				std::cout << "\n";
				Files::PauseConsole();
				system(fullCmd.c_str());
			}
		}

		static void InstallGit()
		{
			std::cout << "Looking for latest \"Git for Windows\" release..." << std::endl;

			std::vector<std::string> headerData;
			headerData.push_back("User-Agent: Geometria-Toolkit");

			std::string jsonCnt = Web::Get("https://api.github.com/repos/git-for-windows/git/releases/latest", false, headerData);
			json gitJson = json::parse(jsonCnt);

			std::string downloadLink = gitJson["assets"][0]["browser_download_url"];

			std::cout << "Downloading \"Git for Windows\" Installer..." << std::endl;
			std::string gitInstaller = Web::Get(downloadLink.c_str(), true, headerData);
			Files::Write("git-installer.exe", gitInstaller, true);

			std::cout << "\n";
			std::cout << "\n";
			std::cout << "=========================\n";
			std::cout << "WARNING: Once you press any key to continue, \n";
			std::cout << "the installer is going to open and ask you for \n";
			std::cout << "administrator permissions.\n";
			std::cout << "=========================\n";
			std::cout << "\n";
			std::cout << "\n";

			Files::PauseConsole();

			Files::OpenProgram("git-installer.exe");

			std::cout << "\n";
			std::cout << "Now the installer is gonna open in a few seconds!" << "\n";
			std::cout << "Press any key to continue once you finish" << "\n";
			std::cout << "with the \"Git for Windows\" installation." << "\n";
			std::cout << "\n";

			Files::PauseConsole();
		}

		static void InstallationFinish()
		{
			Files::ClearConsole();
			Files::Write("firsttime", "");
			std::cout << "Congrats! The installation is now complete!" << "\n";
			std::cout << "Open this program again to start using Geometria." << "\n";
			std::cout << "\n";
			std::cout << "Thank you for choosing Geometria and have fun creating! :D" << "\n";
			Files::PauseConsole();
		}
	};

	struct Linux
	{
		static std::vector<std::string> APTPackageList()
		{
			std::vector<std::string> apt;
			apt.push_back("git");
			apt.push_back("make");
			apt.push_back("build-essential");
			apt.push_back("gcc");
			apt.push_back("gcc-multilib");
			apt.push_back("libx11-dev");
			apt.push_back("libgl1-mesa-dev");
			apt.push_back("xorg-dev");
			apt.push_back("libglu1-mesa-dev");
			apt.push_back("openssl");
			apt.push_back("libssl-dev");
			apt.push_back("libcurl4-openssl-dev");

			return apt;
		}

		static std::vector<std::string> PacmanPackageList()
		{
			std::vector<std::string> pacman;
			pacman.push_back("git");
			pacman.push_back("base-devel");
			pacman.push_back("glu");

			return pacman;
		}

		static std::string PkgListToString(std::vector<std::string> v)
		{
			std::string s;
			for(auto i : v)
			{
				s += " " + i;
			}

			return s;
		}

		static std::string GetPkgManagerName()
		{
			if(Files::WhereIs("apt") != "")
				return "APT";
			else if(Files::WhereIs("pacman") != "")
				return "Pacman";

			return "";
		}

		static void InstallPkgs()
		{
			std::string cmd = "";

			std::cout << "\n";
			std::cout << "\n";
			std::cout << "=========================\n";
			std::cout << "WARNING: Once you press Enter to continue, \n";
			std::cout << "the installer is probably going to ask you for \n";
			std::cout << "sudo permissions.\n";
			std::cout << "=========================\n";
			std::cout << "\n";
			std::cout << "\n";
			Files::PauseConsole();

			if(FirstTimeInstaller::pkgMgr == "APT")
				cmd = "sudo apt install" + PkgListToString(APTPackageList());
			else if(FirstTimeInstaller::pkgMgr == "Pacman")
				cmd = "sudo pacman -S" + PkgListToString(PacmanPackageList());

			system(cmd.c_str());
			std::cout << "Installation Complete!" << std::endl;
			Files::PauseConsole();
		}
	};

};

#endif
