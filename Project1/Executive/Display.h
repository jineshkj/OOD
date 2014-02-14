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
#include "CPPAnalyzer.h"
#include "XmlWriter.h"

class Display
{
public:
  // various field widths for each element in the table. changing value here will change
  // both table heading and data
  enum FieldWidth 
  {
    FW_TYPE  = 10,
    FW_START = 7,
    FW_END   = 5,
    FW_NAME  = 45,
    FW_NODES = 7
  };

  Display(std::ostream& stream) : _os(stream)
  { DisplayCompact(false); }

  void PrintBanner();
  void PrintUsage();
  void output(ElementList& elements);

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

  std::ostream& stream() { return _os; }
  
  //----< sets output mode to compact >------------

  void DisplayCompact(bool compact = false)
  {
    using namespace std;

    _compact = compact;

    // we compute the table heading and keep as an optimization
    std::ostringstream tableHeading;

    if (_compact)
      tableHeading << setw(FW_TYPE) << right << "TYPE" << setw(FW_START + FW_END + 3) << left << "   LINES/SIZE" << setw(FW_NAME) << "NAME" << setw(FW_NODES) << "COMPL";
    else
      tableHeading << setw(FW_TYPE) << right << "TYPE" << setw(FW_START + FW_END + 3) << left << "      LINES" << setw(FW_NAME) << "NAME" << setw(FW_NODES) << "NODES";

    _tableHeading = tableHeading.str();
  }

private:
  std::ostream& _os;
  bool _compact;
  std::string _tableHeading;

  void outputTableHeading();
  void outputElement(element& elem, bool isFunction = false);
  void outputXml(ScopeNode * node);
  bool ScopeToXml(XmlWriter& wrt, ScopeNode * node);
};

#endif // DISPLAY_H
