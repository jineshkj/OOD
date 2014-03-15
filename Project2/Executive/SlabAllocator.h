#ifndef OOD_SLABALLOCATOR_H
#define OOD_SLABALLOCATOR_H
/////////////////////////////////////////////////////////////////////
//  SlabAllocator.h - Allocate memory using slabs                  //
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
This module defines the SlabAllocator template that can be used to 
allocate objects from slabs. The allocator creates and maintains slabs
that expand based on usage in a power-of-2 fashion. New slabs are added
to the storage pool and the size of new one is double the size of last 
one added. First slab always start with size of 1.

Public Interface:
=================
SlabAllocator<int> a;
int * i = a.alloc();

Build Process:
==============
Required files
- SlabAllocator.h
Build commands (either one)
- cl /EHsc /DTEST_SLABALLOCATOR SlabAllocator.cpp

Maintenance History:
=====================
ver 1.0 : 12 Mar 2014
- first release

*/

#include <vector>

#ifdef TEST_SLABALLOCATOR
#include <iostream>
#endif

template <class T>
class SlabAllocator
{
public:
  SlabAllocator() : _used(0)
  {
    _stor.push_back(new T[1]);
  }

  ~SlabAllocator()
  {
    for (auto s : _stor)
      delete[] s;
  }

  size_t slot_capacity() const
  {
    return (1 << (_stor.size() - 1));
  }

  T * alloc()
  {
    T * item = (_used < slot_capacity()) ? 
                   (_stor[_stor.size() - 1] + _used) : expand();

#ifdef TEST_SLABALLOCATOR
    std::cout << "Slot: " << (_stor.size() - 1)  << " Capacity: " << slot_capacity() << " Used: " << _used << std::endl;
#endif

    _used++;

    return item;
  }

private:
  size_t _used;
  std::vector<T*> _stor;

  T * expand()
  {
    T * new_slot = new T[_stor.size() << 1];

    _used = 0;
    _stor.push_back(new_slot);

    return new_slot;
  }
};

#endif // OOD_SLABALLOCATOR_H
