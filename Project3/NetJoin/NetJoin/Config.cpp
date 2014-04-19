#ifdef TEST_CONFIG
/////////////////////////////////////////////////////////////////////////////
// Config.cpp - Implements the Config class tester                         //
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

#include "Config.h"

void main()
{
  Config cfg;

  std::cout << cfg.port << " " << cfg.proto << std::endl;

  cfg.port = 100;
  std::cout << cfg.port << " " << cfg.proto << std::endl;
}

#endif
