
#include "CLIParser.h"
#include <iostream>

CLIOptions CLIParser::parse(int argc, char **argv)
{
	CLIOptions options;

	bool found_path = false;
	for (int i = 1; i < argc; i++)
	{
		const char * arg = argv[i];
	
		if (IsASwitch(arg))
		{
			options.ProcessSwitch(arg + 1);
		}
		else
		{
			if (found_path)
			{
				options.AddPattern(arg);
			}			
			else
			{
				found_path = true;
				options.SetPath(arg);
			}
		}
		
	}

	return options;
}
