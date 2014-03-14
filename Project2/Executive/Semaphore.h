#ifndef OOD_SEMAPHORE_H
#define OOD_SEMAPHORE_H
/////////////////////////////////////////////////////////////////////
//  Semaphore.h - Semaphore implementation                         //
//  ver 1.0                                                        //
//                                                                 //
//  Language:      C++                                             //
//  Platform:      Any                                             //
//  Application:   CIS 687 Spring 2014 Project 2                   //
//  Author:        Jinesh Jayakumar, jineshkj@gmail.com            //
//                                                                 //
//  Jinesh Jayakumar (c) Copyright 2014                            //
//  All rights are granted to the user except the right            //
//  to publish, and requires the retention of this notice.         //
//                                                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines the Semaphore class that can be used to for inter
thread communication. It need to be created with the initial value
and a maximum value. Default initial and maximum values are 0 and 1
respectively.

Public Interface:
=================
Semaphore s;

if (s.up())
  std::cout << "success";

if (s.down())
  std::cout << "success";

Build Process:
==============
Required files
- Semaphore.h, Semaphore.cpp
Build commands (either one)
- cl /EHsc /DTEST_SEMAPHORE Semaphore.cpp

Maintenance History:
=====================
ver 1.0 : 13 Mar 2014
- first release

*/

class Semaphore
{
public:
  static const int DEFAULT_MAX  = 1;
  static const int DEFAULT_INIT = 0;

  Semaphore(int initial = DEFAULT_INIT, int max = DEFAULT_MAX);
  ~Semaphore();

  bool up(int count = 1); // increment by default by 1
  bool down();

private:
  void *_sem;
};

#endif // OOD_SEMAPHORE_H
