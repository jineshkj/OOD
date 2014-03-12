//////////////////////////////////////////////////////////////////////////
//    CLIParser.cpp - source file for CLIParser class                   //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "CLIParser.h"
#include <iostream>

//----< parse the arguments passed into the program >------------

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

//----< test stub >--------------------------------------------

#ifdef TEST_CLIPARSER
#include <iostream>

#include "CLIParser.h"

int main(int argc, char *argv[])
{
  CLIOptions options = CLIParser::parse(_argc, _argv);

  std::cout << options;

  if (!options)
  {
    return -1;
  }

  if (options.recursive())
    std::cout << "/s option is provided";

  if (options.compact())
    std::cout << "/b option is provided";

  return 0;
}
#endif
