#ifndef EXECUTIVE_H
#define EXECUTIVE_H

//////////////////////////////////////////////////////////////////////////
//    Executive.h - header file for Executive class                     //
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
This module provides the Executive class for the Scope Analyzer 
application.

Public Interface:
=================
Executive(int argc, char *argv[]);
int run();

Required Files:
===============
Display.h

Build Command:
==============
cl /EHa  Main.cpp

Maintenance History:
====================
ver 1.0 : 12 Feb 2014
- first release

*/

#include <iostream>

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
};

#endif // EXECUTIVE_H
