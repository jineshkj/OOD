//////////////////////////////////////////////////////////////////////////
//  Semaphore.cpp - Semaphore class implementation and test application //
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

#include "Semaphore.h"

Semaphore::Semaphore(int initial, int max)
{
  _sem = CreateSemaphore(NULL, initial, max, NULL);

  if (_sem == NULL)
    throw "Failed to create windows semaphore";
}

Semaphore::~Semaphore()
{
  CloseHandle(_sem);
  _sem = NULL;
}

bool Semaphore::up(int count)
{
  return ReleaseSemaphore(_sem, count, NULL) == TRUE;
}

bool Semaphore::down()
{
  return WaitForSingleObject(_sem, INFINITE) == WAIT_OBJECT_0;
}

#ifdef TEST_SEMAPHORE

#include <iostream>

void main()
{
  Semaphore s(1);

  std::cout << "Down: " << s.down() << std::endl;

  std::cout << "Up: " << s.up() << std::endl;
  std::cout << "Up: " << s.up() << std::endl; // should fail since max value is 1

  std::cout << "Down: " << s.down() << std::endl;

  std::cout << "Next down call should get blocked" << std::endl;
  std::cout << "Down: " << s.down() << std::endl;
}

#endif
