
#include "Display.h"

using namespace std;

void Display::PrintBanner()
{
  _os << "       Scope Analyzer version 1.0       " << endl;
  _os << "  Copyright (c) 2014, Jinesh Jayakumar  " << endl;
  _os << "========================================" << endl;

  _os << endl;
}

void Display::PrintUsage()
{
  _os << "Usage: Executive [OPTIONS] <root> <pattern>..." << endl;
  _os << endl;
  _os << "Options:" << endl;
  _os << "    /b - display less function details" << endl;
  _os << "    /s - search files recursively" << endl;
  _os << endl;
}

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

void Display::outputTableHeading()
{
  _os << _tableHeading << endl;
  DisplayLine('-');
}

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

void Display::outputXml(ScopeNode * node)
{
  XmlWriter wrt;

  wrt.indent(true);
  wrt.addDeclaration();

  ScopeToXml(wrt, node);

  _os << wrt.xml() << std::endl;
}

bool Display::ScopeToXml(XmlWriter& wrt, ScopeNode * node)
{
  if (node == NULL) return false;

  wrt.start(node->value());

  node->clearMarks();
  while (ScopeToXml(wrt, node->nextUnmarkedChild()));

  wrt.end();

  return true;
}
