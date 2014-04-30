#ifndef NETJOIN_COMMAND_H
#define NETJOIN_COMMAND_H
/////////////////////////////////////////////////////////////////////////////
// Command.h - Generic command interface                                   //
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
* This module provides the Command class.
*
* Command class is a generic class that need to be implemented for different
* operations supported by any communicator.
*
* Public Interface:
* =================
* cmd.Name();
* 
* Message *m = new Message(..);
* cmd.WriteMessage(*m);
*
* Required Files:
* ===============
* Command.h, Command.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_COMMAND Command.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <chrono>

#include "Message.h"
#include "BlockingQueue.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// Command

class Command
{
public:

  //----< various statuses for a command >--------------

  enum Status {
    CREATED,
    EXECUTING,
    SUCCEEDED,
    FAILED
  };

  //----< Command class constructor >--------------

  Command(const std::string& name, Message *m=0) : _name(name), 
    _status(CREATED), _msg(m), _create_time(std::chrono::high_resolution_clock::now()) { }

  //----< Command class destructor >--------------

  virtual ~Command() { delete _msg; _msg = 0; }

  //----< get command name >--------------

  const std::string& Name() const { return _name; }

  //----< get command status >--------------

  Status& status() { return _status; }

  //----< get command status >--------------

  Status status() const { return _status; }

  //----< set associated message buffer >--------------

  void SetMsg(Message *m)
  {
    if (_msg)
      delete _msg;

    _msg = m;

    if (_msg)
    {
      _msg->SetOwner(this);
      ReadMessage(*_msg);
    }
  }

  //----< get associated message buffer >--------------

  Message& Msg()
  {
    if (_msg == 0)
    {
      _msg = new Message(Message::Out, _name, this);
      WriteMessage(*_msg);
    }

    return *_msg;
  }

  void HandleResponse(Message& m)
  {
    ReadMessage(m);

    if (m.Cmd() == "Status") // Get status
    {
      if (m.Headers()["Success"] == "True")
        _status = Command::SUCCEEDED;
      else
        _status = Command::FAILED;
    }
  }

  //----< get associated message buffer >--------------

  const std::chrono::high_resolution_clock::time_point& CreateTime() const 
  {
    return _create_time;
  }

private:
  const std::string _name;
  Status _status;

  Message *_msg;
  std::chrono::high_resolution_clock::time_point _create_time;

  virtual void ReadMessage(Message& msg) = 0;
  virtual void WriteMessage(Message& msg) const = 0;
};

using CommandQueue = BlockingQueue<Command*>;

} //namespace NetJoin

#endif // NETJOIN_COMMAND_H
