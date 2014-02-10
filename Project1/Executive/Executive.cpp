
#include <iostream>

#include "CLIParser.h"

using namespace std;

int main(int argc, const char *argv[])
{
	cout << "Hello world" << endl;

	CLIOptions options = CLIParser::parse(argc, argv);

	if (!options) 
	{
		cout << options;
	}

	return 0;
}
