#ifndef DISPLAY_H
#define DISPLAY_H

#include <ostream>
#include "CPPAnalyzer.h"
#include "XmlWriter.h"

class Display
{
public:
  enum FieldWidth 
  {
    FW_TYPE  = 10,
    FW_START = 7,
    FW_END   = 5,
    FW_NAME  = 45,
    FW_NODES = 7
  };

  Display(std::ostream& stream) : _os(stream)
  { }

  void PrintBanner();
  void PrintUsage();
  void output(ElementList& elements);

  void PauseForUser()
  {
    _os << "Press [ENTER] to continue..." << std::endl;
    getchar();
  }

  void DisplayLine(char ch = '-')
  {
    for (int i = 0; i < 79; i++)
      _os << ch;
    _os << std::endl;
  }

  void DisplayHeading(const std::string& text)
  {    
    _os << text << std::endl;
    DisplayLine();
  }

  std::ostream& stream() { return _os; }
  
  void DisplayCompact(bool compact = false)
  {
    using namespace std;

    _compact = compact;

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
