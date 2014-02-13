
#include <iostream>

#include "CLIParser.h"
#include "FileManager.h"
#include "CPPAnalyzer.h"

#include "Executive.h"

using namespace std;

Executive::Executive(int argc, char *argv[]) : 
    _argc(argc), _argv(argv), _disp(std::cout)
{ 
}

int Executive::run()
{
  _disp.PrintBanner();

  CLIOptions options = CLIParser::parse(_argc, _argv);

  _disp.stream() << options;

  if (!options)
  {
    _disp.PrintUsage();
    return -1;
  }    
  
  _disp.PauseForUser();

  FileManager manager(options.path(), options.recursive(), _disp.stream());
  manager.search(options.patterns());

  _disp.PauseForUser();

  ElementList elements;
  CPPAnalyzer analyzer(elements);

  _disp.DisplayCompact(options.compact());
  for (auto &file : manager.repo())
  {
    _disp.DisplayHeading(file);

    analyzer.parse(file);

    _disp.output(elements);
  }

  //_disp.PauseForUser();

  return 0;
}
