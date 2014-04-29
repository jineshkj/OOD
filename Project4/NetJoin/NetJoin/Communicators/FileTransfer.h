#ifndef NETJOIN_FILETRANSFER_H
#define NETJOIN_FILETRANSFER_H
/////////////////////////////////////////////////////////////////////////////
// FileTransfer.h - Provides FileTransfer communicator and is commands     //
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
* This module provides the FileTransfer and FileTransfer::PutFile classes.
*
* FileTransfer is a communicator end-point that can be used for transferring
* files across systems using the NetJoin service. FileTransfer class also
* defines the PutFile command that be sent to the FileTransfer service for
* sending a file to a remote location.
* 
* Public Interface:
* =================
* MessageQueue inQ;
* FileTransfer svc(inQ);
* 
* svc.Wait();
*
* 
* svc.EnQ(new FileTransfer("C:\\a.txt"));
* 
* Required Files:
* ===============
* FileTransfer.h, FileTransfer.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILETRANSFER FileTransfer.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <thread>

#include "../Command.h"
#include "../Communicator.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// FileTransfer

class FileTransfer : public Communicator
{
public:
  /////////////////////////////////////////////////////////
  // PutFile

  class PutFile : public Command
  {
  public:
    PutFile(const std::string& path);

    void WriteMessage(Message& msg) const;
    void ReadMessage(Message& msg);

    friend class FileTransfer;

  private:
    std::string _path, _fname;

    static std::string CreateTempFile();
  };

  FileTransfer(MessageQueue& dispQ);
  ~FileTransfer();

  void Wait();

  //----< return list of supported commands >--------------

  const CommandList& Commands() const { return _commands; }

  Command *CreateCommand(const std::string& cmdname) const;

  static const std::string& SaveDir();

private:
  std::thread *_thr;
  static const CommandList _commands;

  static std::string _saveDir;

  void Run();
  static void ThreadEntry(FileTransfer& svc);
};

} // namespace NetJoin

#endif // NETJOIN_FILETRANSFER_H
