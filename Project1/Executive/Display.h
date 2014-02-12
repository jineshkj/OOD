#ifndef DISPLAY_H
#define DISPLAY_H

#include <ostream>
#include "CPPAnalyzer.h"
#include "XmlWriter.h"

class Display
{
public:
  Display(std::ostream& stream) : _os(stream)
  { }

  void output(ScopeNode * node);

private:
  std::ostream& _os;

  void ScopeToXml(XmlWriter& wrt, ScopeNode * node);
};

#endif // DISPLAY_H
