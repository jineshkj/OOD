/////////////////////////////////////////////////////////////////////////////
// Message.cpp - Implements the Message class                              //
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

#include "Message.h"

using namespace NetJoin;

//----< Message constructror >--------------

Message::Message(Direction d, const std::string& cmdline, void *owner) : _dir(d), _owner(owner), _conn(0)
{
  // std::cout << "Cmdline: " << cmdline << std::endl;

  if (cmdline.find(' ') != std::string::npos)
  {
    std::istringstream cl(cmdline);

    std::string prefix;
    cl >> prefix;

    if (prefix != NJ_PROTO_PREFIX)
      throw "invalid command line";

    cl >> _cmd;
  }
  else
  {
    _cmd = cmdline;
  }
}

//----< generate command line of message >--------------

const std::string Message::CommandLine() const
{
  std::ostringstream cl;
  cl << NJ_PROTO_PREFIX << ' ' << _cmd << std::endl;
  return cl.str();
}

#ifdef TEST_MESSAGE

#include <iostream>

void main()
{
  Message *m = new Message(Message::Out, "Test");
  m->WriteData("Test", 4);

  std::cout << m->Cmd() << " : " << m->Data() << std::endl;
}

#endif
