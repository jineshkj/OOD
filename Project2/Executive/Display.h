#ifndef DISPLAY_H
#define DISPLAY_H

//////////////////////////////////////////////////////////////////////////
//    Display.h    - header file for Displat class                      //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module provides the Display class that is used to display 
ElementList returned by the CPPAnalyzer.

Public Interface:
=================
Display(std::ostream& stream);
void PrintBanner();
void PrintUsage();
void output(ElementList& elements);

void PauseForUser();
void DisplayLine(char ch = '-');
void DisplayHeading(const std::string& text);
std::ostream& stream();
void DisplayCompact(bool compact = false);

...
ElementList elements;
CPPAnalyzer analyzer(elements);
analyzer.parse("Parser.cpp");
Display display(std::cout);
display.ouput(elements);
....

Required Files:
===============
CPPAnalyzer.h, XmlWriter.h

Build Command:
==============
cl /EHa /DTEST_DISPLAY DISPLAY.cpp

Maintenance History:
====================
ver 1.0 : 12 Feb 2014
- first release

*/

#include <ostream>
#include "CPPParser.h"
#include "ScopeAnalyzer.h"

class Display
{
public:

  Display(std::ostream& stream) : _os(stream), _showDiff(false)
  {  }

  void PrintBanner();
  void PrintUsage();

  //----< waits for user to press an ENTER key >------------

  void PauseForUser()
  {
    _os << "Press [ENTER] to continue..." << std::endl;
    getchar();
  }

  //----< display a horizontal line on stream >------------

  void DisplayLine(char ch = '-')
  {
    for (int i = 0; i < 79; i++)
      _os << ch;
    _os << std::endl;
  }

  //----< print a given text as heading underlined >------------

  void DisplayHeading(const std::string& text)
  {    
    _os << text << std::endl;
    DisplayLine();
  }

  //----< return back the output stream >------------

  std::ostream& stream() { return _os; }
  
  //----< set the show diff internal flag >------------

  void ShowDiff(bool show)
  {
    _showDiff = show;
  }

  void ShowMatches(const SizedScopeList& scopelist);

private:
  std::ostream& _os;
  bool _showDiff;

  void DisplayFile(const std::string& file, size_t start, size_t lines);
};

#endif // DISPLAY_H
