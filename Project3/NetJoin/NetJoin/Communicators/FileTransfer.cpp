/////////////////////////////////////////////////////////////////////////////
// FileTransfer.cpp - Implements the FileTransfer class                    //
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

#include "../Message.h"
#include "../StatusMessage.h"

#include "../FileSystem.h"

#include "FileTransfer.h"

using namespace NetJoin;

//----< list of commands supported by FileTransfer communicator >--------------

const Communicator::CommandList FileTransfer::_commands{ "PutFile" };

//----< directory where downloaded files are saved >--------------

std::string FileTransfer::_saveDir(FileSystem::Path::getFullFileSpec("Downloads"));

//----< constructor for PutFile command >--------------

FileTransfer::PutFile::PutFile(const std::string& path) : Command("PutFile"), _path(path)
{
  if (_path.size() == 0) // create a temporary file.
    _path = CreateTempFile();

  _fname = FileSystem::Path::getName(_path);
}

//----< copy PutFile data to a Message object >--------------

void FileTransfer::PutFile::WriteMessage(Message& msg) const
{
  msg.AddHeader("FileName", _fname); // this should do all the time

  std::ifstream file(_path);

  if (!file.is_open())
    throw "Could not open file " + _path;

  while (!file.eof())
  {
    char buffer[1500];
    file.read(buffer, sizeof(buffer));
    msg.WriteData(buffer, file.gcount());
  }
}

//----< populate PutFile data from a Message object >--------------

void FileTransfer::PutFile::ReadMessage(Message& msg)
{
  //std::cout << "Read Message : " << msg.Cmd() << " : " << msg.Data() << std::endl;

  if (msg.Cmd() == "PutFile")
  {
    _fname = msg.Headers()["FileName"];

    std::ofstream file(_path, std::ios_base::out | std::ios_base::trunc);
    file << msg.Data();
  }
  else if (msg.Cmd() == "Status") // Read response message
  {
    if (msg.Headers()["Success"] == "True")
      status() = Command::SUCCEEDED;
    else
      status() = Command::FAILED;
  }
}

//----< create a temporary file >--------------

std::string FileTransfer::PutFile::CreateTempFile()
{
  TCHAR szTempDirPath[MAX_PATH];
  if (GetTempPath(sizeof(szTempDirPath), szTempDirPath) == 0)
    throw "Unable to find temporary directory";

  //  Generates a temporary file name. 
  TCHAR szTempFileName[MAX_PATH];
  if (GetTempFileName(szTempDirPath, TEXT("NFT"), 0, szTempFileName) == 0)
    throw "GetTempFileName failed";

  return (const char*)(szTempFileName);
}

//----< FileTransfer class constructor >--------------

FileTransfer::FileTransfer(MessageQueue& dispQ) : Communicator(dispQ)
{
  _thr = new std::thread(FileTransfer::ThreadEntry, std::ref(*this));
}

//----< FileTransfer class destructor >--------------

FileTransfer::~FileTransfer()
{
  Shutdown();
  Wait();

  delete _thr; _thr = 0;
}

//----< wait for thread to finish >--------------

void FileTransfer::Wait()
{
  if (_thr->joinable()) _thr->join();
}

//----< create a command object from command name >--------------

Command *FileTransfer::CreateCommand(const std::string& cmdname) const
{
  if (cmdname == "PutFile")
  {
    return new PutFile("");
  }
  else
  {
    throw "FileTransfer service does not know about " + cmdname;
  }
}

//----< main loop of the communicator >--------------

void FileTransfer::Run()
{
  std::cout << "File Transfer service started" << std::endl;

  while (true)
  {
    Command *cmd = DeQ();

    if (cmd == 0) // termination signal
      break;

    if (cmd->Name() == "PutFile")
    {
      PutFile * pf = dynamic_cast<PutFile*>(cmd);

      std::string newpath = _saveDir + "\\" + pf->_fname;
      bool status = MoveFile(pf->_path.c_str(), newpath.c_str()) ? true : false;

      StatusMessage * resp = new StatusMessage(status);
      resp->SetConn(cmd->Msg().Conn());
      Dispatch(resp);

    } else
    {
      std::cout << "Command " << cmd->Name() << " is not known" << std::endl;
    }

    delete cmd;
  }

  std::cout << "File Transfer service stopped" << std::endl;
}

//----< entry point for std::thread object >--------------

void FileTransfer::ThreadEntry(FileTransfer& svc)
{
  svc.Run();
}

#ifdef TEST_FILETRANSFER

void main()
{
  MessageQueue outQ;
  FileTransfer ft(outQ);
  
  ft.EnQ(new FileTransfer::PutFile("C:\\tmp\\a.txt"));
  Message *rsp = outQ.deQ();

  std::cout << "Success = " << rsp->Headers()["Success"] << std::endl;
}

#endif
