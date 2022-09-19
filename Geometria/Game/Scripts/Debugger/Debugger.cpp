#include "Debugger.h"
#include <experimental/filesystem>
#include "String/StringAPI.h"

bool Debugger::continuesBelow = false;
int Debugger::bracketDepth = 0, Debugger::skipCount = 0;
std::vector<std::string> Debugger::files;

void Debugger::ReadCPPandHFiles(bool includeEngine = false)
{
	if(!includeEngine)
	{
		std::experimental::filesystem::path gamePath = std::experimental::filesystem::path("Geometria/Game/");
		std::cout << gamePath.u8string() << std::endl;
		for(auto& p: std::experimental::filesystem::recursive_directory_iterator(gamePath))
		{
			std::string pathToStr = p.path().u8string();
			std::cout << pathToStr << std::endl;
       		if(pathToStr.find(".cpp") != std::string::npos || 
       			pathToStr.find(".h") != std::string::npos || 
       			pathToStr.find(".c") != std::string::npos || 
       			pathToStr.find(".hpp") != std::string::npos)
       		{
       			files.push_back(pathToStr);
       			std::vector<std::string> fileCont = Files::ReadAndGetLines(pathToStr.c_str());
       			AnalyzeFileContent(pathToStr, fileCont);
       		}
		}
	}
}

void Debugger::AnalyzeFileContent(std::string file, std::vector<std::string> content)
{
	//if(file.find("moveset.h") != std::string::npos)
	//{
		Debugger::bracketDepth = 0;
		Debugger::continuesBelow = false;
		Debugger::skipCount = 0;
	
		std::string finalFileCont = "";
		bool functionBegin = false, functionEnd = false, addTry = false, addCatch = false;
		for(int i = 0; i < content.size(); i++)
		{
			std::string line = content[i];

			if(line.find("{") != std::string::npos)
				Debugger::bracketDepth++;

			if(line.find("}") != std::string::npos)
			{
				if(Debugger::bracketDepth == 1)
					functionEnd = true;

				Debugger::bracketDepth--;
			}

			if(Debugger::bracketDepth < 0)
				Debugger::bracketDepth = 0;

			if(line.find("struct") != std::string::npos || line.find("class") != std::string::npos)
				Debugger::bracketDepth--;

			if(Debugger::bracketDepth == 1)
			{
				if(!functionBegin)
				{
					addTry = true;
					functionBegin = true;
				}
			}
			
			if(StringAPI::ContainsLetters(line) && Debugger::bracketDepth > 0)
			{
				std::string newLine = "";
				std::string finalLine = line;

				finalLine = StringAPI::ReplaceAll(finalLine, "\"", "\\\"");
	
				if(finalLine.find(";") == std::string::npos)
					finalLine = StringAPI::ReplaceAll(finalLine, "\n", "...\n");

				if(finalLine.find("if") != std::string::npos)
				{
					if(content[i + 1].find("{") == std::string::npos || finalLine.find("{") == std::string::npos)
					{
						if(content[i + 2].find("else") != std::string::npos)
							Debugger::skipCount += 3;
						else if(content[i + 1].find("else") != std::string::npos)
							Debugger::skipCount += 1;
					}
				}

				bool checkForFnBrackets = finalLine.find("{") != std::string::npos && Debugger::bracketDepth == 1;
	
				if(!continuesBelow && !checkForFnBrackets)
				{
					newLine = "_Geometria_CoreLog::WriteCurrentLine(\"" + finalLine + "\", \"" + Files::GetFilenameFromDirectory(file.c_str()) + "\", " + std::to_string(i) + ");\n" + line;
				}
				
				bool syntaxContinueCheck = line.find(";") == std::string::npos && line.find("{") != std::string::npos;

				if(syntaxContinueCheck || Debugger::skipCount != 0)
					continuesBelow = true;
				else
					continuesBelow = false;
	
				if(newLine != "")
				{
					line = newLine;
				}

				if(Debugger::skipCount != 0)
					skipCount--;
			}

			if(functionEnd)
			{
				functionBegin = false;
				functionEnd = false;
				addCatch = true;
			}
				
			if(addTry)
			{
				std::string tryLine = "";

				tryLine += "_GEO_COMPILER(typedef void (*SignalHandlerPointer)(int);)\n";
				tryLine += "_GEO_COMPILER(SignalHandlerPointer previousHandler;)\n";
				tryLine += "_GEO_COMPILER(previousHandler = signal(SIGSEGV , DebugTools::SignalHandler);)\n";
				tryLine += "_GEO_COMPILER(try {)\n";

				finalFileCont += line + "\n" + tryLine;
				addTry = false;
			}
			else if(addCatch)
			{
				std::string catchLine = "_GEO_COMPILER(})\n _GEO_COMPILER(catch(const std::exception& e))\n_GEO_COMPILER({)\n_GEO_COMPILER(std::cout << \"Uh oh!\" << std::endl; exit(1);)\n_GEO_COMPILER(})";
				finalFileCont += catchLine + "\n" + line + "\n";
				addCatch = false;
			}
			else
				finalFileCont += line + "\n";
		}
	
		Files::Write(file.c_str(), finalFileCont);
	//}
}

void Debugger::Cleanup()
{
	std::cout << "Cleaning up..." << std::endl;

	for(auto i : Debugger::files)
	{
		std::string finalFileCont = "";
		std::vector<std::string> fileCont = Files::ReadAndGetLines(i.c_str());
		for(auto f : fileCont)
		{
			if(f.find("_Geometria_CoreLog::WriteCurrentLine(") == std::string::npos && f.find("_GEO_COMPILER(") == std::string::npos)
			{
				finalFileCont += f + "\n";
			}
		}

		Files::Write(i.c_str(), finalFileCont);
	}

	std::cout << "Cleanup Done!" << std::endl;
}

void Debugger::RunLLDB()
{
	std::cout << "Checking for Python..." << std::endl;

	if(Files::WhereIs("python") != "")
	{
		Files::Remove("currentLine.log");
		std::cout << "Starting LLDB Environment..." << std::endl;
		std::string dir = Files::GetDirectoryOf(Files::GetExecutablePath().c_str());
    	dir += "/Tools/WinLLDB/runlldb.bat";
    	Files::ChangeCurrentDirectory("Geometria");
    	std::string cmd = Files::ConvertToWindowsCmdPath(dir.c_str());
    	std::string runLLDB = Files::RunCommand(cmd.c_str());
    	PrintDebugError(runLLDB);
	}
	else
	{
		std::string input = "";
		std::cout << "Python is not currently installed in your system!" << std::endl;
		std::cout << "Do you want to download and install now? (yes/no)" << std::endl;

		std::cin >> input;

		if(input == "yes")
		{
			system("start \"https://www.python.org/downloads/\"");
			std::cout << "Try the command again once Python's installation is complete." << std::endl;
		}
		else
		{
			std::cout << "Sure, no worries! Feel free to try it again later :D" << std::endl;
			std::cout << "Cancelling Python install & --run command..." << std::endl;
			exit(0);
		}
	}
}

void Debugger::PrintDebugError(std::string result)
{
	if(result.find("stop reason =") != std::string::npos)
	{
		std::string getException = StringAPI::GetSubstringBetween(result, "stop reason = ", "frame");
		
		std::string getError = Files::Read("currentLine.log");

		if(getError != "")
		{
			std::string getNullptrList = StringAPI::GetSubstringBetween(getError, "={}=", "");
			getError = StringAPI::RemoveAll(getError, getNullptrList);
		
			getNullptrList = StringAPI::GetSubstringBetween(getNullptrList, "NullptrList:[", "|]");
			std::vector<std::string> nullptrValues = StringAPI::SplitIntoVector(getNullptrList, "|");
		
			getError = StringAPI::RemoveAll(getError, "={}=");
		
			std::string getNullVal;
			bool isErrorNull = false;
		
			for(auto i : nullptrValues)
			{
				if(i != "")
				{
					if(getError.find(i) != std::string::npos)
					{
						getNullVal = i;
						isErrorNull = true;
					}
				}
			}
			
			if(isErrorNull)
			{
				std::cout << std::endl;
				std::cout << "A runtime error occured: tried to dereference a nullptr" << std::endl;
				std::cout << getError << std::endl;
				std::cout << std::endl;
		
				std::cout << Debugger::TreatException(getException, getNullVal) << std::endl;
				std::cout << std::endl;
			}
			else
			{
				std::cout << std::endl;
				std::cout << "A runtime error occured at:" << std::endl;
				std::cout << getError << std::endl;
				std::cout << std::endl;

				std::cout << "The error in question is not recognized correctly by 'Hub/geo'" << std::endl;
				std::cout << "If you believe this is an issue, contact Geometria developers." << std::endl;
				std::cout << std::endl;
				std::cout << "Compiler error: " << getException << std::endl;
			}
		}
		else
		{
			std::cout << std::endl;
			std::cout << "A runtime error occured, but there's not enough information." << std::endl;
			std::cout << " --> Hub/geo:" << __LINE__ << std::endl;
			std::cout << "  |  " << std::endl;
			std::cout << "  |  Debug Information about the crash is not found." << std::endl;
			std::cout << "  |  To get more info, try compiling on 'Debug' mode" << std::endl;
			std::cout << "  |  instead of 'Release' with \"geo --compile debug\"" << std::endl;
			std::cout << std::endl;
			std::cout << "Compiler error: " << getException << std::endl;
		}
	}
	else
	{
		std::cout << "Game/App closed successfully!" << std::endl;
	}

	Files::Remove("currentLine.log");
}

std::string Debugger::TreatException(std::string error, std::string variable)
{
	std::string fullError = "";

	if(error.find("0x00000000") != std::string::npos)
		fullError += "> \"" + variable + "\" was a 'null pointer'.";
	else
		fullError += "> \"" + variable + "\" was either a 'null pointer' or 'corrupted'.";

	fullError += "\n\n";
	fullError += "Hint: Try to initialize it first before accesing or running a function.\n";
	fullError += "Example: " + variable + " = new ...\n";

	fullError += "\n";
	fullError += "Compiler error:\n  " + error + "\n";

	return fullError;
}