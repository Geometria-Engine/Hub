#ifndef ENGINEUPDATER_H
#define ENGINEUPDATER_H

struct EngineUpdater
{

	static void Update()
	{
		BackupGameFolder();
		LookForMacros();
		GitPullFromMainRepo();
		ReturnGameFolder();
		ReturnBackupFiles();
	}

	static void CreateBackupFilesFolder()
	{
		std::string backupFilesPath = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/.BackupFiles";

		if(!Files::DirectoryExists(backupFilesPath.c_str()))
			Files::CreateDirectory(backupFilesPath.c_str());
	}

	static void LookForMacros()
	{
		std::cout << "Looking for Toolkit Macros..." << std::endl;

		CreateBackupFilesFolder();

		for(auto& p: std::experimental::filesystem::recursive_directory_iterator(std::experimental::filesystem::current_path()))
		{
			std::string backupFilesPath = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/.BackupFiles";
			std::string pathToStr = p.path().u8string();

        	if(pathToStr.find(".cpp") != std::string::npos || 
        		pathToStr.find(".h") != std::string::npos || 
        		pathToStr.find(".c") != std::string::npos || 
        		pathToStr.find(".hpp") != std::string::npos)
        	{
        		std::string fileCont = Files::Read(pathToStr.c_str());

        		MacroCheck_DontUpdateFile(pathToStr, fileCont, backupFilesPath);
        	}
		}

		std::cout << "Done Looking for Macros!" << std::endl;
	}

	static void BackupGameFolder()
	{
		std::cout << "Backing up Game Folder..." << std::endl;

		std::string backupGFPath = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/BackupGame";
		std::string gamePath = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/Geometria/Game";

		if(Files::DirectoryExists(gamePath.c_str()))
		{
			if(!Files::DirectoryExists(backupGFPath.c_str()))
				Files::CreateDirectory(backupGFPath.c_str());

			Files::CopyDirectory(gamePath.c_str(), backupGFPath.c_str());

			std::cout << "Backup done!" << std::endl;
		}
		else
		{
			std::cout << "ERROR: No Game Folder Found! Cancelling Update..." << std::endl;
			exit(0);
		}
	}

	static void GitPullFromMainRepo()
	{
		std::cout << "Updating Engine from Git..." << std::endl;

		std::string gitignoreContent = Files::Read(".gitignore");
		gitignoreContent = StringAPI::ReplaceAll(gitignoreContent, "geo.exe", "!geo.exe");
		Files::Write(".gitignore", gitignoreContent);

		system("git update-index --skip-worktree \"BackupGame/\"");
		system("git update-index --skip-worktree \".BackupFiles/\"");
		system("git add *");
		system("git stash");
		system("git remote add upstream https://github.com/Geometria-Engine/Geometria.git");
		system("git pull upstream main");
		system("git remote rm upstream");

		gitignoreContent = StringAPI::ReplaceAll(gitignoreContent, "!geo.exe", "geo.exe");
		Files::Write(".gitignore", gitignoreContent);

		std::cout << "Update done!" << std::endl;
	}

	static void ReturnGameFolder()
	{
		std::cout << "Returning Game Folder from Backup..." << std::endl;
		std::string backupGFPath = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/BackupGame";
		std::string gamePath = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/Geometria/Game";

		Files::CopyDirectory(backupGFPath.c_str(), gamePath.c_str());
		std::cout << "Returning Game Folder from Backup done!" << std::endl;
	}

	static void MacroCheck_DontUpdateFile(std::string path, std::string content, std::string backupDest)
	{
		if(path.find("MainHeader\\geometria.h") == std::string::npos && path.find("MainHeader/geometria.h") == std::string::npos)
		{
			if(content.find("\"DONT_UPDATE_FILE()\"") == std::string::npos)
			{
				if(content.find("DONT_UPDATE_FILE()") != std::string::npos)
				{
					std::string finalContent;

					finalContent += "// GEOMETRIA METADATA:\n";
					finalContent += "// Backup Reason: DONT_UPDATE_FILE().\n";
					finalContent += "// File Location: [" + path + "].\n";

					finalContent += content;
					std::string backupPath = backupDest + "/" + Files::GetFilenameFromDirectory(path.c_str());
					Files::Write(backupPath.c_str(), finalContent);
				}
			}
		}
	}

	static void ReturnBackupFiles()
	{
		std::cout << "Returning Backup Files..." << std::endl;

		std::experimental::filesystem::path backupPathFS = Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "\\.BackupFiles";
		for(auto& p: std::experimental::filesystem::recursive_directory_iterator(backupPathFS))
		{
			std::string pathToStr = p.path().u8string();
			std::string fileCont = Files::Read(pathToStr.c_str());
			std::string lastPath = StringAPI::GetSubstringBetween(fileCont, "// File Location: [", "].");

			std::string meta = StringAPI::GetSubstringBetween(fileCont, "// GEOMETRIA METADATA:", "].");

			std::string metadataContent;
			metadataContent += "// GEOMETRIA METADATA:";
			metadataContent += meta;
			metadataContent += "].\n";

			fileCont = StringAPI::RemoveAll(fileCont, metadataContent);
			Files::Write(lastPath.c_str(), fileCont);
		}

		Files::DeleteDirectory(backupPathFS.u8string().c_str());

		std::cout << "Returning Backup Files Done!" << std::endl;
	}

};

#endif