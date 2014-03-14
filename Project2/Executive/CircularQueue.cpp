#ifdef TEST_CIRCULARQUEUE
//////////////////////////////////////////////////////////////////////////
//    CircularQueue.cpp - CircularQueue template test application       //
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

#include <iostream>

#include "CircularQueue.h"

void main()
{
  CircularQueue<int> q(5);

  std::cout << "1. " << q.insert(1000) << std::endl;
  std::cout << "2. " << q.insert(2000) << std::endl;
  std::cout << "3. " << q.insert(3000) << std::endl;
  std::cout << "4. " << q.insert(4000) << std::endl;
  std::cout << "5. " << q.insert(5000) << std::endl;
  std::cout << "6. " << q.insert(6000) << std::endl;
  
  int i;

  std::cout << "1. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "2. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "3. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "4. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "1. " << q.insert(1001) << std::endl;
  std::cout << "2. " << q.insert(2001) << std::endl;

  std::cout << "1. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "2. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "3. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "3. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;

  std::cout << "1. " << q.insert(1234) << std::endl;
  std::cout << "1. " << q.remove(i) << std::endl;
  std::cout << i << std::endl;
}

#endif
