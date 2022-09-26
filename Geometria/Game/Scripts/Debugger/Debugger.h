#ifndef GAME_DEBUGGER_H
#define GAME_DEBUGGER_H

#include "Files/Files.h"

struct Debugger
{
	static bool continuesBelow;
	static int bracketDepth;
	static int skipCount;
	static std::vector<std::string> files;

	static void ReadCPPandHFiles(bool includeEngine);
	static void AnalyzeFileContent(std::string file, std::vector<std::string> content);
	static void Cleanup();

	static void RunLLDB();
	static void PrintDebugError(std::string result);

	static std::string TreatException(std::string error, std::string variable);
};

#endif
