#ifndef OOD_MUTEX_H
#define OOD_MUTEX_H
/////////////////////////////////////////////////////////////////////
//  Mutex.h - Mutex implementation                                 //
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
This module defines the Mutex class that can be used to for mutual
exclusion. It works recursively right now and the mutex by default is
unlocked at creation.

Public Interface:
=================
Mutex m;

if (m.lock())
  std::cout << "success";
  
if (m.unlock())
  std::cout << "success";

Build Process:
==============
Required files
- Mutex.h, Mutex.cpp
Build commands (either one)
- cl /EHsc /DTEST_MUTEX Mutex.cpp

Maintenance History:
=====================
ver 1.0 : 12 Mar 2014
- first release

*/

///////////////////////////////////////////////////////////////
// class that implements a Mutex

class Mutex
{
public:
  Mutex();
  ~Mutex();

  bool lock();
  bool unlock();

private:
  void *_mutex;
};

#endif // OOD_MUTEX_H
