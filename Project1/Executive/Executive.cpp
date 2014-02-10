
#include <iostream>

#include "CLIParser.h"
#include "FileManager.h"

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
	{
		exit(-1);
	}

	cout << "Press [ENTER] to continue..." << endl;
	getchar();

	FileManager manager(options.path(), options.recursive());
	manager.search(options.patterns());

	cout << "Press [ENTER] to continue..." << endl;
	getchar();



	return 0;
}
