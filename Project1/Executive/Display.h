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

  void PrintBanner();
  void output(ElementList& elements, bool compact);

  void PauseForUser()
  {
    _os << "Press [ENTER] to continue..." << std::endl;
    getchar();
  }

  void DisplayHeading(const std::string& text)
  {    
    _os << text << std::endl;
    for (int i = 0 ; i < 75; i++)
      _os << '-';
    _os << std::endl;
  }

  std::ostream& stream() { return _os; }
  
private:
  std::ostream& _os;

  void outputHeader();
  void outputElement(element& elem, bool dispComplexity = false);
  void outputXml(ScopeNode * node);
  bool ScopeToXml(XmlWriter& wrt, ScopeNode * node);
};

#endif // DISPLAY_H
