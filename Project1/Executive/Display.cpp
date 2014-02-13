
#include "Display.h"

using namespace std;

void Display::PrintBanner()
{
  _os << "       Scope Analyzer version 1.0       " << endl;
  _os << "  Copyright (c) Jinesh Jayakumar, 2014  " << endl;
  _os << "========================================" << endl;

  _os << endl;
}

void Display::output(ElementList& elements, bool compact)
{
  size_t maxFunComplexity = 0;
  ScopeNode * maxFun = NULL;

  outputHeader();

  for (auto& elem : elements)
  {
    if (elem.type == "keyword" || elem.type == "anonymous")
      continue;
    
    if (elem.type == "function")
    {
      outputElement(elem, true);

      if (elem.methodScopes.second >= maxFunComplexity)
      {
        maxFunComplexity = elem.methodScopes.second;
        maxFun = elem.methodScopes.first.get();
      }
    } else 
    {
      outputElement(elem);
    }
  }

  _os << endl;

  if (!compact)
  {
    if (maxFun != NULL)
    {
      _os << "*** Most complex function '" << maxFun->value() << "' has complexity of " << maxFunComplexity << " ***" << endl;
      outputXml(maxFun);
    }
    else
    {
      _os << "***** No function found in this file *****" << endl;
    }

    _os << endl;
  }
  
  _os << endl;
}

void Display::outputHeader()
{
  _os << setw(10) << "type" << setw(10) << "start" << " - " << setw(5) << "end" << setw(45) << "name" << setw(5) << "comp" << endl;
}

void Display::outputElement(element& elem, bool dispComplexity)
{
  _os << setw(10) << elem.type << setw(10) << elem.startLine << " - " << setw(5) << elem.endLine << setw(45) << elem.name ;
  
  if (dispComplexity)
    _os << setw(5) << elem.methodScopes.second;

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
