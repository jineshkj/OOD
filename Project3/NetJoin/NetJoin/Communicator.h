#ifndef NETJOIN_COMMUNICATOR_H
#define NETJOIN_COMMUNICATOR_H
/////////////////////////////////////////////////////////////////////////////
// Command.h - Generic communicator interface                              //
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
* This module provides the Communicator class.
*
* Communicator class is a generic class that need to be implemented for different
* communicators.
*
* Public Interface:
* =================
* communicator.EnQ(cmd);
* communicator.Shutdown();
* communicator.Wait();
*
* Required Files:
* ===============
* Communicator.h
*
* Build Command:
* ==============
*   <no need to build since its an interface>
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <string>
#include <vector>

#include "Command.h"
#include "Message.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// Communicator

class Communicator
{
public:
  using CommandList = std::vector<std::string>;

  //----< Communicator class constructor >--------------

  Communicator(MessageQueue& dispQ) : _dispatcherQ(dispQ) { }

  //----< Communicator class destructor >--------------

  virtual ~Communicator() { Shutdown(); }

  //----< Enqueue a command for execution >--------------
  
  void EnQ(Command *c) { _cmdQ.enQ(c); }

  //----< send shutdown message to the communicator >--------------

  void Shutdown() { EnQ(0); }

  virtual void Wait() = 0;
  virtual const CommandList& Commands() const = 0;
  virtual Command *CreateCommand(const std::string& cmdname) const = 0;

  //----< create Command from a Message >--------------
  Command * MsgToCmd(Message *m) const
  {
    Command * cmd = CreateCommand(m->Cmd());
    cmd->SetMsg(m);
    return cmd;
  }

protected:
  //----< send the message to Dispatcher >--------------

  void Dispatch(Message *m) { _dispatcherQ.enQ(m); }

  //----< Retrieve a message from queue >--------------

  Command * DeQ() { return _cmdQ.deQ(); }

private:
  MessageQueue& _dispatcherQ;
  CommandQueue _cmdQ;
};

} // namespace NetJoin

#endif // NETJOIN_COMMUNICATOR_H
