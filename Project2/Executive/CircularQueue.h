#ifndef OOD_CIRCULARQUEUE_H
#define OOD_CIRCULARQUEUE_H
/////////////////////////////////////////////////////////////////////
//  CircularQueue.h - A non-blocking circular queue implementation //
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
This module defines the CircularQueue template that can be used to
create a circular queue of a specific type. All queue operations are
non-blocking and returns false upon failure. Failure can happen only
under two situations: attempting insert when queue is full and 
attempting removal when queue is empty. Queue capacity is currently 
fixed and need to be specified during queue creation.

Public Interface:
=================
CircularQueue<int> q(5);

if (q.insert(100) == true)
  std::cout << "success";

int i;
if (q.remove(i) == true)
  std::cout << "success. got " << i;


Build Process:
==============
Required files
- CircularQueue.h
Build commands (either one)
- cl /EHsc /DTEST_CIRCULAR CircularQueue.cpp

Maintenance History:
=====================
ver 1.0 : 13 Mar 2014
- first release

*/

template <class T>
class CircularQueue
{
public:
  CircularQueue(size_t qsize) : _size(qsize), _head(0), _tail(0)
  {
    _items = new T[_size];
  }

  ~CircularQueue()
  {
    delete[] _items;
  }

  size_t capacity()
  {
    return _size;
  }

  bool empty()
  {
    return (_head == _tail);
  }

  bool full()
  {
    return (_tail == _size);
  }

  size_t count()
  {
    if (full())
      return _size;

    if (_head <= _tail)
      return _tail - _head;
    else
      return _size - (_head - _tail); // capacity minus available
  }

  bool insert(const T& t)
  {
    if (count() == capacity())
      return false;

#ifdef TEST_CIRCULARQUEUE
    std::cout << "Inserting item at " << _tail << std::endl;
#endif

    _items[_tail++] = t;

    if (_tail == _size)
      _tail = 0;

    if (_head == _tail) // when queue becomes full, _tail comes over _head
      _tail = _size;

    return true;
  }

  bool remove(T& t)
  {
    if (empty())
      return false;
      
#ifdef TEST_CIRCULARQUEUE
    std::cout << "Removing item at " << _head << std::endl;
#endif
    
    if (full())
      _tail = _head;

    t = _items[_head++];

    if (_head == _size)
      _head = 0;

    return true;
  }

private:
  size_t _size;

  // _head always point to first item in queue and _tail points to the next
  // free location for new item unless:
  //  when queue is empty, _head == _tail
  //  when queue is full, _tail == _size
  size_t _head, _tail;

  T *_items;
};

#endif // OOD_CIRCULARQUEUE_H
