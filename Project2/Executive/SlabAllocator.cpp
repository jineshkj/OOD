#ifdef TEST_SLABALLOCATOR
//////////////////////////////////////////////////////////////////////////
//    SlabAllocator.cpp - test source file for SlabAllocator template   //
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

#include <string>
#include <iostream>

#include "SlabAllocator.h"

struct testdata
{
  int i;
  std::string s;
};

void main()
{
  SlabAllocator<testdata> all;

  std::cout << "1 : " << (void *)all.alloc() << std::endl;
  std::cout << "2 : " << (void *)all.alloc() << std::endl;
  std::cout << "3 : " << (void *)all.alloc() << std::endl;
  std::cout << "4 : " << (void *)all.alloc() << std::endl;
  std::cout << "5 : " << (void *)all.alloc() << std::endl;
  std::cout << "6 : " << (void *)all.alloc() << std::endl;
}

#endif // TEST_SLABALLOCATOR
