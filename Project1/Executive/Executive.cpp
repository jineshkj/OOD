
#include <iostream>

#include "CLIParser.h"
#include "FileManager.h"
#include "CPPAnalyzer.h"

using namespace std;

void PrintBanner()
{
	cout << "       Scope Analyzer version 1.0       " << endl;
	cout << "  Copyright (c) Jinesh Jayakumar, 2014  " << endl;
	cout << "----------------------------------------" << endl;

	cout << endl;
}

int main(int argc, const char *argv[])
{
	PrintBanner();

	CLIOptions options = CLIParser::parse(argc, argv);

	cout << options << endl;
	if (!options)
		exit(-1);

	cout << "Press [ENTER] to continue..." << endl;
	getchar();

	FileManager manager(options.path(), options.recursive());
	manager.search(options.patterns());

	cout << "Press [ENTER] to continue..." << endl;
	getchar();

	// ConfigureParser 

	CPPAnalyzer analyzer;

	for (auto &file : manager.repo())
	{
      analyzer.parse(file);
	}

#if 0
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();

	if (pParser)
	{
		for (auto &file : manager.repo())
		{
			cout << "File: " << file << endl;
			if (!configure.Attach(file))
			{
				std::cout << "\n  could not open file " << file << std::endl;
				continue;
			}

			while (pParser->next())
				pParser->parse();

			cout << endl;
		}
	}
	else
	{
		std::cout << "\n\n  Parser not built\n\n";
		return 1;
	}
#endif

	return 0;
}
