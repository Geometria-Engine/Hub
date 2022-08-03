#ifndef ENGINEUPDATER_H
#define ENGINEUPDATER_H

struct EngineUpdater
{

	static void Update()
	{
		BackupGameFolder();
		GitPullFromMainRepo();
		ReturnGameFolder();
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
		std::cout << "Returning done!" << std::endl;
	}

};

#endif