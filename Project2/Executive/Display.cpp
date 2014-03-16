//////////////////////////////////////////////////////////////////////////
//    Display.cpp - source file for Display class                       //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "Display.h"

using namespace std;

//----< print application banner >------------

void Display::PrintBanner()
{
  _os << "       Code Analyzer version 1.0       " << endl;
  _os << "  Copyright (c) 2014, Jinesh Jayakumar  " << endl;
  _os << "========================================" << endl;

  _os << endl;
}

//----< print application usage help >------------

void Display::PrintUsage()
{
  _os << "Usage: Executive [OPTIONS] <root> <pattern>..." << endl;
  _os << endl;
  _os << "Options:" << endl;
  _os << "    /d - display differences between similar regions" << endl;
  _os << "    /s - search files recursively" << endl;
  _os << endl;
}

//----< test stub >--------------------------------------------

#ifdef TEST_DISPLAY
#include <iostream>

#include "Display.h"
#include "CLIParser.h"

int main(int argc, char *argv[])
{
  Display disp(std::cout);
  disp.DisplayCompact(true);

  disp.stream() << "Manual output" << std::endl;

  ElementList elements;
  CPPAnalyzer analyzer(elements);

  disp.DisplayHeading(argv[1]);

  analyzer.parse(argv[1]);
  disp.output(elements);

  disp.DisplayLine();

  return 0;
}
#endif
