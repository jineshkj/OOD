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
  std::chrono::high_resolution_clock::time_point start_time =
    std::chrono::high_resolution_clock::now();

  if (_cmd == "PutFile")
    runPutFile();
  else if (_cmd == "SearchString")
    runSearchString();

  std::chrono::high_resolution_clock::time_point end_time =
    std::chrono::high_resolution_clock::now();

  std::chrono::milliseconds total_ms;
  total_ms = std::chrono::duration_cast<milliseconds>(end_time - start_time);
  std::cout << "--- Overall Duration : " << total_ms.count() << " ms ---" << std::endl;

  return 0;
}

//----< print program usage >--------------

void Executive::usage()
{
  std::cout << "Usage: Client <NIterations> <Nthreads> <Command> [Args]" << std::endl << std::endl;
  std::cout << "Commands+Args: " << std::endl;
  std::cout << "    PutFile <filename>" << std::endl;
  std::cout << "    SearchString <string>" << std::endl;
}

//----< PutFile action handler >--------------

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
    std::cout << "Exception : " << e.what() << std::endl;
    exit(-1);
  }

  if (cmd.status() == NetJoin::Command::SUCCEEDED)
    std::cout << "File transfer succeeded" << std::endl;
  else
    std::cout << "File transfer failed" << std::endl;
}

//----< SearchString action handler >--------------

void Executive::runSearchString()
{
  std::string str = _arg;
  std::chrono::milliseconds client_ms;

  if (str.size() == 0)
  {
    std::cout << "Search string empty. Aborting search." << std::endl;
    return;
  }

  for (int i = 0; i < _iters; i++)
  {
    NetJoin::TextSearch::SearchString cmd(str, _threads);
    long long server_ms = 0;

    try {
      std::chrono::high_resolution_clock::time_point start_time =
        std::chrono::high_resolution_clock::now();

      server_ms = NetJoin::Service::Execute(cmd);

      std::chrono::high_resolution_clock::time_point end_time =
        std::chrono::high_resolution_clock::now();

      client_ms = std::chrono::duration_cast<milliseconds>(end_time - start_time);
    }
    catch (std::exception e)
    {
      std::cout << "Error : " << e.what() << std::endl;
      exit(-1);
    }

    if (cmd.status() == NetJoin::Command::SUCCEEDED)
    {
      std::cout << "Search succeeded:" << std::endl;
      std::cout << "  Server Duration = " << server_ms << " ms" << std::endl;
      std::cout << "  Client Duration = " << client_ms.count() << " ms" << std::endl;
    }
    else
    {
      std::cout << "*** Search failed ***" << std::endl;
    }
  }
}
