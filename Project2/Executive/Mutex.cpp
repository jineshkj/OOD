//////////////////////////////////////////////////////////////////////////
//    Mutex.cpp - Mutex class implementation and test application       //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Any                                                //
//    Application  - CIS 687 OOD Project 2                              //
//    Author       - Jinesh Jayakumar <jineshkj@gmail.com>              //
//                                                                      //
//    Jinesh Jayakumar (c) Copyright 2014                               //
//    All rights are granted to the user except the right               //
//    to publish, and requires the retention of this notice.            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <Windows.h>

#include "Mutex.h"

Mutex::Mutex()
{
  _mutex = CreateMutex(NULL, FALSE, NULL);

  if (_mutex == NULL)
    throw "Failed to create windows mutex";
}

Mutex::~Mutex()
{
  CloseHandle(_mutex);
  _mutex = NULL;
}

bool Mutex::lock()
{
  return WaitForSingleObject(_mutex, INFINITE) == WAIT_OBJECT_0;
}

bool Mutex::unlock()
{
  return ReleaseMutex(_mutex) == TRUE;
}

#ifdef TEST_MUTEX

#include <iostream>

void main()
{
  Mutex m;

  std::cout << "Lock: " << m.lock() << std::endl;
  std::cout << "Lock: " << m.lock() << std::endl;

  std::cout << "Unlock: " << m.unlock() << std::endl;
  std::cout << "Unlock: " << m.unlock() << std::endl;

  std::cout << "Lock: " << m.lock() << std::endl;
  std::cout << "Unlock: " << m.unlock() << std::endl;
  std::cout << "Unlock: " << m.unlock() << std::endl;

  std::cout << "Lock: " << m.lock() << std::endl;
  std::cout << "Unlock: " << m.unlock() << std::endl;
  std::cout << "Unlock: " << m.unlock() << std::endl;
}

#endif
