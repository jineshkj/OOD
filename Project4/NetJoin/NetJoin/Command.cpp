#ifdef TEST_COMMAND
/////////////////////////////////////////////////////////////////////////////
// Command.cpp - Implements the Command class tester                       //
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

#include <string>
#include <iostream>

#include "Command.h"

using namespace NetJoin;

class Test : Command
{
public:
  Test(const std::string& s) : Command("TestCommand"), dat(s) { }

  void WriteMessage(Message& msg) const
  {
    msg.AddHeader("Data", dat);
  }

  void ReadMessage(Message& msg)
  {
    dat = msg.Headers()["Data"]
  }

private:
  std::string dat;
};

void main()
{
  Test t("Hello");
  Message *m = new Message(Message::Out, "TestCommand");
  t.WriteMessage(*m);

  std::cout << m->Data() << std::endl;
}

#endif
