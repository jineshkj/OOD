//////////////////////////////////////////////////////////////////////////
//    Executive.cpp - source file for Executive class                   //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <iostream>

#include "CLIParser.h"
#include "FileManager.h"
#include "CPPParser.h"
#include "WorkerPool.h"

#include "ScopeAnalyzer.h"

#include "Executive.h"

using namespace std;
using namespace std::chrono;

//----< Executive class constructor >------------

Executive::Executive(int argc, char *argv[]) :
    _argc(argc), _argv(argv), _disp(std::cout)
{ 
}

//----< the main routine of Executive class >------------

int Executive::run()
{
  _disp.PrintBanner();

  CLIOptions options = CLIParser::parse(_argc, _argv);

  // print out options for user to verify
  _disp.stream() << options;

  // in case of error, we display usage and exit with error
  if (!options)
  {
    _disp.PrintUsage();
    return -1;
  }    
  
  _disp.PauseForUser();

  FileManager manager(options.path(), options.recursive(), _disp.stream());
  manager.search(options.patterns());

  _disp.PauseForUser();

  high_resolution_clock::time_point start_time = high_resolution_clock::now();

  CPPParser sampleparser;
  WorkerPool parserpool(&sampleparser);
  SlabAllocator<ParseFile> parseAlloc;

  ScopeAnalyzer sampleAnalyzer;
  WorkerPool analyzerpool(&sampleAnalyzer);

  ScopeAnalyzer::SetMinLines(options.minLines());
  
  size_t total_scopes = 0;
  for (auto &file : manager.repo())
  {
    ParseFile *work = parseAlloc.alloc();

    work->file = static_cast<const std::string*>(&file);
    work = static_cast<ParseFile*>(parserpool.exec(work));

    if (work) {
      total_scopes += work->scopes.size();
      analyzerpool.exec(work);
    }
  }

  ParseFile *work;
  while ((work = static_cast<ParseFile*>(parserpool.wait())) != NULL) {
    total_scopes += work->scopes.size();
    analyzerpool.exec(work);
  }

  while (analyzerpool.wait() != NULL); // wait until all analyzer workers are done

  high_resolution_clock::time_point end_time = high_resolution_clock::now();

  size_t total_matches = 0;
  for (int type = 0; type < CPPRule::EXPR_MAX; type++)
  {
    const SizedScopeList& sl = ScopeAnalyzer::GetSizedScopeList(type);
    total_matches += sl.matches().size();
  }

  milliseconds ms = duration_cast<milliseconds>(end_time - start_time);

  _disp.stream() << "Analyzed " << total_scopes << " scopes and found " << total_matches << " matches in " << ms.count() << " milliseconds" << std::endl;
  _disp.PauseForUser();

  _disp.ShowDiff(options.showdiff());

  for (int type = 0; type < CPPRule::EXPR_MAX; type++)
  {
    const SizedScopeList& sl = ScopeAnalyzer::GetSizedScopeList(type);
    _disp.ShowMatches(sl);
  }

  //_disp.PauseForUser();

  return 0;
}
