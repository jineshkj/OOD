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

#include <cstdlib>

#include <chrono>
#include <iostream>

#include <Netjoin.h>

#include "Executive.h"

using namespace std;
using namespace std::chrono;

//----< Executive class constructor >------------

Executive::Executive(int argc, char *argv[]) :
_argc(argc), _argv(argv)
{
  if (argc != 5)
  {
    usage();
    exit(1);
  }

  _iters = atoi(argv[1]);
  _threads = atoi(argv[2]);
  _cmd = argv[3];
  _arg = argv[4];
}

//----< the main routine of Executive class >------------

int Executive::run()
{
  if (_cmd == "PutFile")
    runPutFile();
  else if (_cmd == "SearchString")
    runSearchString();

  //_disp.PauseForUser();

  return 0;
}

void Executive::usage()
{
  std::cout << "Usage: Client <NIterations> <Nthreads> <Command> [Args]" << std::endl << std::endl;
  std::cout << "Commands+Args: " << std::endl;
  std::cout << "    PutFile <filename>" << std::endl;
  std::cout << "    SearchString <string>" << std::endl;
}

void Executive::runPutFile()
{
  std::string file = _arg;

  // NOTE: Iterative count is ignored for this command

  NetJoin::FileTransfer::PutFile cmd(file);

  try {
    NetJoin::Service::Execute(cmd);
  }
  catch (std::exception e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    exit(-1);
  }

  if (cmd.status() == NetJoin::Command::SUCCEEDED)
    std::cout << "File transfer succeeded" << std::endl;
  else
    std::cout << "File transfer failed" << std::endl;
}

void Executive::runSearchString()
{
}
