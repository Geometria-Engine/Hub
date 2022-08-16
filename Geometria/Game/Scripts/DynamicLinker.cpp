#include "DynamicLinker.h"

void DynamicLinker::Link()
{
	std::cout << "Importing Dynamic Libraries..." << std::endl;

	if(Application::IsPlatform(Application::Platform::Windows))
	{
		if(Files::DirectoryExists("DynamicLibraries/Windows/32"))
			std::cout << "32bit Dynamic Libraries Exists!" << std::endl;
		else
			std::cout << "Oh... 32bit Dynamic Libraries Exists do not exist..." << std::endl;
	}
}