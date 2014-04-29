#ifndef OOD_EXECUTIVE_H
#define OOD_EXECUTIVE_H
/////////////////////////////////////////////////////////////////////////////
// Executive.h - Executive package for OOD Project 4                       //
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
* This module provides the Executive class.
*
*
* Public Interface:
* =================
*
* Required Files:
* ===============
* Executive.h, Executive.cpp
*
* Build Command:
* ==============
* cl /EHa Executive.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 28 Apr 14
* - first release
*/

/////////////////////////////////////////////////////////
// Executive

#include "Display.h"

class Executive
{
public:
  Executive(int argc, char *argv[]);

  int run();

private:
  int _argc;
  char **_argv;
  Display _disp;

  int _iters;
  int _threads;
  std::string _cmd;
  std::string _arg;

  void usage();
  void runPutFile();
  void runSearchString();
};

#endif // OOD_EXECUTIVE_H
