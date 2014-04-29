#ifndef NETJOIN_TEXTSEARCH_H
#define NETJOIN_TEXTSEARCH_H
/////////////////////////////////////////////////////////////////////////////
// TextSearch.h - Provides FileTransfer communicator and its commands      //
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
/*
* Module Operations:
* ==================
* This module provides the TextSearch and TextSearch::SearchString classes.
*
*
* Public Interface:
* =================
*
* Required Files:
* ===============
* TextSearch.h, TextSearch.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_TEXTSEARCH TextSearch.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 28 Apr 14
* - first release
*/

#include <atomic>
#include <thread>

#include "../Command.h"
#include "../Communicator.h"

namespace NetJoin {

  /////////////////////////////////////////////////////////
  // TextSearch

  class TextSearch : public Communicator
  {
  public:
    /////////////////////////////////////////////////////////
    // SearchString

    class SearchString : public Command
    {
    public:
      SearchString(const std::string& str, int jobs = 1);

      void WriteMessage(Message& msg) const;
      void ReadMessage(Message& msg);

      friend class TextSearch;

    private:
      std::string _str;
      int _jobs; // numbers of jobs to run in parallel

      TextSearch *_svc;
      BlockingQueue<std::string*> _files;
      std::atomic<int> _completed;
    };

    TextSearch(MessageQueue& dispQ);
    ~TextSearch();

    void Wait();

    //----< return list of supported commands >--------------

    const CommandList& Commands() const { return _commands; }

    Command *CreateCommand(const std::string& cmdname) const;

    void ProcessCompletedSearch(SearchString *ss);

  private:
    std::thread *_thr;
    static const CommandList _commands;

    void Run();
    static void ThreadEntry(TextSearch& svc);
    static void SearchThread(SearchString *ss);
  };

} // namespace NetJoin

#endif // NETJOIN_TEXTSEARCH_H
