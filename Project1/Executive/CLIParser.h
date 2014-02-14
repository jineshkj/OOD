#ifndef CLIPARSER_H
#define CLIPARSER_H

//////////////////////////////////////////////////////////////////////////
//    CLIParser.h - header file for CLIParser class                     //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module provides the CLIParser class.

CLIParser class supports parsing of different command line
options supported by the Source Analyzer application.

Public Interface:
=================
static CLIOptions parse(int argc, char *argv[]);

Required Files:
===============
CLIOptions.h

Build Command:
==============
cl /EHa /DTEST_CLIPARSER CLIParser.cpp

Maintenance History:
====================
ver 1.0 : 12 Feb 2014
- first release

*/

#include "CLIOptions.h"

class CLIParser
{
public:
	static CLIOptions parse(int argc, char *argv[]);

private:
  //----< checks to see if a given program argument is an option switch >------------

  static bool IsASwitch(const char *arg) { return arg[0] == '/'; }
};

#endif // CLIPARSER_H
