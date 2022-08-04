#ifndef TOOLKIT_UPDATER_H
#define TOOLKIT_UPDATER_H

struct ToolkitUpdater
{
	static void DownloadLatestToolkit()
	{
		std::cout << "Downloading Latest Toolkit..." << std::endl;

		if(Application::IsPlatform(Application::Platform::Windows))
			InstallNewToolkit(DWLT_Windows());
	}

	static std::string DWLT_Windows()
	{
		std::string downloadCnt = Web::Get("https://github.com/Geometria-Engine/Geometria/raw/main/geo.exe", true);
		std::cout << "Download has completed successfully!" << std::endl;
		return downloadCnt;
	}

	static void InstallNewToolkit(std::string content)
	{
		std::string exePath = Files::GetExecutablePath();
		std::string oldExePath = exePath + ".old";

		if(Files::Rename(exePath.c_str(), oldExePath.c_str()))
		{
			Files::Write(exePath.c_str(), content, true);

			std::cout << "Update completed sucessfully!" << std::endl;
			exit(0);
		}
		else
		{
			std::cout << "ERROR: Executable can't be renamed! Cancelling..." << std::endl;
			exit(0);
		}
	}

};

#endif