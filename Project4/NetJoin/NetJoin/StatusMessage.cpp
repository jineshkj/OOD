#ifdef TEST_STATUSMESSAGE
/////////////////////////////////////////////////////////////////////////////
// StatusMessage.cpp - StatusMessage class test file                       //
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

#include <iostream>

#include "StatusMessage.h"

void main()
{
  StatusMessage failure(false, "Test Message");

  std::cout << failure.Cmd() << std::endl;
  for (auto& h : failure.Headers())
  {
    std::cout << "Key : " << h.first << ", Val : " << h.second << std::endl;
  }
  std::cout << failure.Data() << std::endl;
}

#endif
