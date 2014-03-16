//////////////////////////////////////////////////////////////////////////
//    CLIOptions.cpp - source file for CLIOptions class                 //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "CLIOptions.h"

using namespace std;

//----< output formatted contents of CLIOpions to ostream >------------

ostream& operator<<(ostream& os, CLIOptions& options)
{
  os << "CLI Options" << endl;
  os << "-----------" << endl;
  os << endl;

  os << "Root      : " << (options._root.size() ? options._root : "<MISSING>") << endl;

  os << "Patterns  : ";
  if (options._patterns.size() == 0)
  {
	os << "<MISSING>";
  }
  else
  {
	for (auto &pat : options._patterns)
	  os << '[' << pat << ']' << " ";
  }
  os << endl;

  os << "Recursive : " << (options._recurse ? "yes" : "no") << endl;
  os << "Show Diff : " << (options._dispdiff ? "yes" : "no") << endl;
  os << "Min Lines : " << options._minLines << endl;
  os << endl;

  if (options._error.size() > 0)
	os << "Error: " << options._error << endl;

  return os;
}

//----< process an option swich like /s or /b >------------

bool CLIOptions::ProcessSwitch(const string& sw)
{
  if (sw == "S")
  {
	_recurse = true;
  }
  else if (sw == "d")
  {
	_dispdiff = true;
  }
  else if (sw[0] == 'l')
  {
    _minLines = atoi(sw.c_str() + 1);

  } else
  {
    // set an error string internally   
	_error += "Invalid usage or unknown option: ";
	_error += sw;
	_error += "\n";

	return false;
  }

  return true;
}

//----< store root path >------------

void CLIOptions::SetPath(const FilePath& path)
{
  _root = path;
}

//----< add a pattern to the pattern list >------------

void CLIOptions::AddPattern(const FilePattern& pattern)
{
  _patterns.insert(_patterns.end(), pattern);

  if (!_complete && _root.size())
	_complete = true;
}

//----< test stub >--------------------------------------------

#ifdef TEST_CLIOPTIONS
#include <iostream>

#include "CLIParser.h"

int main(int argc, char *argv[])
{
  CLIOptions options = CLIParser::parse(_argc, _argv);

  std::cout << options;

  if (!options)
  {
    return -1;
  }
  
  if (options.recursive()) 
    std::cout << "/s option is provided";

  if (options.compact())
    std::cout << "/b option is provided";

  return 0;
}
#endif
