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

#include <fstream>

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
  _os << "    /l<num> - minimum number of lines required to match two scopes" << endl;
  _os << "    /S - search files recursively" << endl;
  _os << endl;
}

//----< print matches of scopes >------------

void Display::ShowMatches(const SizedScopeList& scopelist)
{
  for (auto& match : scopelist.matches())
  {
    ScopeInfo *l = match.first;
    ScopeInfo *r = match.second;

    _os << *(l->file) << " : " << l->start << "-" << (l->start + l->lines) << endl;
    if (_showDiff)
    {
      DisplayLine('-');
      DisplayFile(*(l->file), l->start, l->lines);
    }
    _os << *(r->file) << " : " << r->start << "-" << (r->start + r->lines) << endl;
    if (_showDiff)
    {
      DisplayLine('-');
      DisplayFile(*(r->file), r->start, r->lines);
    }

    DisplayLine('=');
    if (_showDiff)
      PauseForUser();
  }
}

//----< display contents of a file within a range >------------

void Display::DisplayFile(const std::string& file, size_t start, size_t lines)
{
  char buffer[1024];
  ifstream ifs (file);

  while (--start)
  {
    ifs.getline(buffer, sizeof(buffer));
    if (ifs.eof())
      return;
  }

  lines++;
  while (lines)
  {
    ifs.getline(buffer, sizeof(buffer));
    _os << buffer << endl;
    if (ifs.eof())
      return;
    lines--;
  }
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
