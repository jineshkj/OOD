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
  _os << "       Scope Analyzer version 1.0       " << endl;
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
  _os << "    /b - display less function details" << endl;
  _os << "    /s - search files recursively" << endl;
  _os << endl;
}

//----< output all the given elements to the configured ostream >------------

void Display::output(ElementList& elements)
{
  size_t maxFunComplexity = 0;
  ScopeNode * maxFun = NULL;

  _os << _tableHeading << endl;

  for (auto& elem : elements)
  {
    if (elem.type == "keyword" || elem.type == "anonymous")
      continue;
    
    if (elem.type != "function")
    {
      outputElement(elem);
      continue;
    }

    // handle functions specially since we need to find also the maximum depth function
    outputElement(elem, true);

    if (!_compact)
    {
      if (elem.methodScopes.second >= maxFunComplexity)
      {
        maxFunComplexity = elem.methodScopes.second;
        maxFun = elem.methodScopes.first.get();
      }
    }

  }

  if (!_compact)
  {
    if (maxFun != NULL)
    {
      _os << "*** XML for '" << maxFun->value() << "' with complexity " << maxFunComplexity << " ***" << endl;
      outputXml(maxFun);
    }
    else
    {
      _os << "***** No function found in this file *****" << endl;
    }
  }

  DisplayLine('=');

  _os << endl << endl;
}

//----< print table heading >------------

void Display::outputTableHeading()
{
  _os << _tableHeading << endl;
  DisplayLine('-');
}

//----< print out a single element >------------

void Display::outputElement(element& elem, bool isFunction)
{
  _os << setw(FW_TYPE) << right << elem.type;
  
  if (_compact && isFunction)
    _os << setw(FW_START) << ' ' << setw(FW_END+3) << left << (elem.endLine - elem.startLine + 1);
  else
    _os << setw(FW_START) << elem.startLine << " - " << setw(FW_END) << left << elem.endLine;
    
  _os << setw(FW_NAME) << elem.name ;
  
  if (isFunction)
    _os << setw(FW_NODES) << elem.methodScopes.second;

  _os << endl;
}

//----< convert a ScopeNode to xml and write to the ostream object >------------

void Display::outputXml(ScopeNode * node)
{
  XmlWriter wrt;

  wrt.indent(true);
  wrt.addDeclaration();

  ScopeToXml(wrt, node);

  _os << wrt.xml() << std::endl;
}

//----< convert a ScopeNode to an XML representation >------------

bool Display::ScopeToXml(XmlWriter& wrt, ScopeNode * node)
{
  if (node == NULL) return false;

  wrt.start(node->value());
  
  // make sure that we start with all marks cleared and also clear mark
  // when exiting so that future calls will not be affected
  node->clearMarks();
  while (ScopeToXml(wrt, node->nextUnmarkedChild()));
  node->clearMarks();

  wrt.end();

  return true;
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
