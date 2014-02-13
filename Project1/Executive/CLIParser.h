
#ifndef CLIPARSER_H
#define CLIPARSER_H

#include "CLIOptions.h"

class CLIParser
{
public:
	static CLIOptions parse(int argc, char *argv[]);

private:
	static bool IsASwitch(const char *arg) { return arg[0] == '/'; }
};

#endif // CLIPARSER_H
