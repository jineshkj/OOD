/////////////////////////////////////////////////////////////////////////////
// TextSearch.cpp - Implements the TextSearch class                        //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Jinesh Jayakumar, 2014                                      //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Windows 7 SP1                                              //
// Application: OOD Project, Spring 2014                                   //
// Author:      Jinesh Jayakumar <jineshkj at gmail dot com>               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include <Windows.h>

#include <iostream>
#include <fstream>
#include <algorithm>

#include "../Message.h"
#include "../StatusMessage.h"

#include "../FileSystem.h"

#include "FileTransfer.h"
#include "TextSearch.h"

#define MAX_JOBS 10

using namespace NetJoin;

//----< list of commands supported by TextSearch communicator >--------------

const Communicator::CommandList TextSearch::_commands{ "SearchString" };

//----< constructor for SearchString command >--------------

TextSearch::SearchString::SearchString(const std::string& str, int jobs) : Command("SearchString"), 
  _str(str), _jobs(jobs), _svc(0), _completed(0)
{
  if (_jobs < 1)
    _jobs = 1;
  else if (_jobs > MAX_JOBS)
    _jobs = MAX_JOBS;
}

//----< destructor for SearchString class >--------------

TextSearch::SearchString::~SearchString()
{
  while (_files.size())
  {
    delete _files.deQ();
  }

  while (_matches.size())
  {
    delete _matches.deQ();
  }
}

//----< copy SearchString data to a Message object >--------------

void TextSearch::SearchString::WriteMessage(Message& msg) const
{
  msg.AddHeader("SearchString", _str); // this should do all the time
  msg.AddHeader("NumJobs", _jobs);
}

//----< populate SearchString data from a Message object >--------------

void TextSearch::SearchString::ReadMessage(Message& msg)
{
  //std::cout << "Read Message : " << msg.Cmd() << " : " << msg.Data() << std::endl;

  if (msg.Cmd() == "SearchString")
  {
    _str = msg.Headers()["SearchString"];
    _jobs = atoi(msg.Headers()["NumJobs"].c_str());
  }
  else if (msg.Cmd() == "Status") // Read response message
  {
    std::istringstream data(msg.Data());
    
    while (!data.eof())
    {
      std::string line;
      std::getline(data, line);
      _matches.enQ(new std::string(line));
    }
    
  }
}

//----< TextSearch class constructor >--------------

TextSearch::TextSearch(MessageQueue& dispQ) : Communicator(dispQ)
{
  _thr = new std::thread(TextSearch::ThreadEntry, std::ref(*this));
}

//----< TextSearch class destructor >--------------

TextSearch::~TextSearch()
{
  Shutdown();
  Wait();

  delete _thr; _thr = 0;
}

//----< wait for thread to finish >--------------

void TextSearch::Wait()
{
  if (_thr->joinable()) _thr->join();
}

//----< create a command object from command name >--------------

Command *TextSearch::CreateCommand(const std::string& cmdname) const
{
  if (cmdname == "SearchString")
  {
    return new SearchString("");
  }
  else
  {
    throw std::runtime_error("TextSearch service does not know about " + cmdname);
  }
}

//----< process a completed search command >--------------

void TextSearch::ProcessCompletedSearch(SearchString *ss)
{
  if (ss->_completed != ss->_jobs)  // ensure all jobs have been completed
    return;

  //std::cout << "Search Completed" << std::endl;

  StatusMessage * resp = new StatusMessage(true);
  resp->SetConn(ss->Msg().Conn());

  std::chrono::milliseconds ms = 
    std::chrono::duration_cast<std::chrono::milliseconds>(resp->CreateTime() - ss->CreateTime());

  resp->AddHeader("ExecutionTime", ms.count());
  while (ss->_matches.size())
  {
    std::string *match = ss->_matches.deQ();
    resp->WriteData(match->c_str(), match->size());
    resp->WriteData("\n", 1);
    delete match;
  }

  Dispatch(resp);

  delete ss;
}


//----< main loop of the communicator >--------------

void TextSearch::Run()
{
  std::cout << "Text Search service started" << std::endl;

  while (true)
  {
    Command *cmd = DeQ();

    if (cmd == 0) // termination signal
      break;

    if (cmd->Name() == "SearchString")
    {      
      SearchString * ss = dynamic_cast<SearchString*>(cmd);

      ss->_svc = this;

      std::vector<std::string> files = FileSystem::Directory::getFiles(FileTransfer::SaveDir(), "*.*");
      for (auto& file : files)
        ss->_files.enQ(new std::string(FileTransfer::SaveDir() + "\\" + file));

      for (int i = 0; i < ss->_jobs; i++)
      {
        std::thread(TextSearch::SearchThread, ss).detach(); // we do not need the thread object
        ss->_files.enQ(0); // thread termination
      }
  
      continue; // so that we do not delete the command object here

    } else
    {
      std::cout << "Command " << cmd->Name() << " is not known" << std::endl;
    }

    delete cmd;
  }

  std::cout << "File Transfer service stopped" << std::endl;
}

//----< entry point for std::thread object >--------------

void TextSearch::ThreadEntry(TextSearch& svc)
{
  svc.Run();
}

//----< entry point for a search job thread >--------------

void TextSearch::SearchThread(SearchString *ss)
{
  std::string *file;

  std::cout << "Search worker started" << std::endl;

  while ((file = ss->_files.deQ()) != 0) // loop until termination entry
  {
    //std::cout << "Processing file : " << *file << std::endl;

    std::ostringstream matchInfo;
    matchInfo << FileSystem::Path::getName(*file) << ':';

    std::ifstream fs(*file, std::ios::in | std::ios::binary);

    if (!fs.is_open())
      throw std::runtime_error("Could not open file " + *file);
  
    char buffer[4096];
    std::ostringstream content;
    while (!fs.eof())
    {
      fs.read(buffer, sizeof(buffer));
      content.write(buffer, fs.gcount());
    }

    std::string text = content.str();

    std::string::iterator i, curr;

    curr = text.begin();
    while ((i = std::search(curr, text.end(), ss->_str.begin(), ss->_str.end())) != text.end())
    {
      //std::cout << "i = " << (i-text.begin()) << std::endl;
      matchInfo << ' ' << (i - text.begin());
      curr = i;
      curr++;
    }

    ss->_matches.enQ(new std::string(matchInfo.str()));

    delete file;
  }

  ss->_completed++;
  ss->_svc->ProcessCompletedSearch(ss);

  std::cout << "Search worker stopped" << std::endl;
}

#ifdef TEST_TEXTSEARCH

void main()
{
  MessageQueue outQ;
  TextSearch ts(outQ);

  ft.EnQ(new TextSearch::SearchString("hello"));
  Message *rsp = outQ.deQ();

  std::cout << "Success = " << rsp->Headers()["Success"] << std::endl;

  ft.EnQ(new TextSearch::SearchString("hello", 3));
  rsp = outQ.deQ();

  std::cout << "Success = " << rsp->Headers()["Success"] << std::endl;
}

#endif
