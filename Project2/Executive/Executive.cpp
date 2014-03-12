//////////////////////////////////////////////////////////////////////////
//    Executive.cpp - source file for Executive class                   //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "CLIParser.h"
#include "FileManager.h"
#include "CPPAnalyzer.h"

#include "Executive.h"

using namespace std;

//----< Executive class constructor >------------

Executive::Executive(int argc, char *argv[]) :
    _argc(argc), _argv(argv), _disp(std::cout)
{ 
}

//----< the main routine of Executive class >------------

int Executive::run()
{
  _disp.PrintBanner();

  CLIOptions options = CLIParser::parse(_argc, _argv);

  // print out options for user to verify
  _disp.stream() << options;

  // in case of error, we display usage and exit with error
  if (!options)
  {
    _disp.PrintUsage();
    return -1;
  }    
  
  _disp.PauseForUser();

  FileManager manager(options.path(), options.recursive(), _disp.stream());
  manager.search(options.patterns());

  _disp.PauseForUser();

  ElementList elements;
  CPPAnalyzer analyzer(elements);

  _disp.DisplayCompact(options.compact());
  for (auto &file : manager.repo())
  {
    _disp.DisplayHeading(file);

    analyzer.parse(file);

    _disp.output(elements);
  }

  //_disp.PauseForUser();

  return 0;
}
