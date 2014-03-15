#ifndef OOD_BLOCKINGQUEUE_H
#define OOD_BLOCKINGQUEUE_H
/////////////////////////////////////////////////////////////////////
//  BlockingQueue.h - A blocking circular queue implementation     //
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
This module defines the BlockingQueue template that can be used to
create a circular queue of a specific type with blocking insert and
remove operations. This template is infact a specialization of 
CircularQueue template where all operations are non-blocking.

insert and remove operations on queue return nothing since they are
expected to succeed upon return.

Public Interface:
=================
BlockingQueue<int> q(5);

...

// thread 1

int i=1;
while (i++)
  q.insert(i);

...

// thread 2

int i;
while (1) {
  q.remove(i);
  std::cout << i << std::endl;
}

Build Process:
==============
Required files
- BlockingQueue.h
Build commands (either one)
- cl /EHsc /DTEST_BLOCKINGQUEUE BlockingQueue.cpp

Maintenance History:
=====================
ver 1.0 : 14 Mar 2014
- first release

*/

#include "Mutex.h"
#include "Semaphore.h"

#include "CircularQueue.h"

template <class T>
class BlockingQueue
{
  static const int MAX_CLIENTS = 1024;

public:
  BlockingQueue(size_t qsize) : _queue(qsize), _countWait(0), _semWait(0, MAX_CLIENTS)
  {

  }

  void insert(const T& t)
  {
#ifdef TEST_BLOCKINGQUEUE
    std::cout << "Inserting element " << t << std::endl;
#endif
    _lock.lock();

    while (_queue.insert(t) == false)
    {
#ifdef TEST_BLOCKINGQUEUE
      std::cout << "Inserting element " << t << ". Queue full" << std::endl;
#endif
      // Queue is full. Let us block on wait semaphore.
      _countWait++;
      _lock.unlock();
      
      _semWait.down(); // wait until someone removes an item from queue

      _lock.lock(); // try again
      _countWait--;
#ifdef TEST_BLOCKINGQUEUE
      std::cout << "Inserting element " << t << ". Retrying." << std::endl;
#endif
    }

#ifdef TEST_BLOCKINGQUEUE
    std::cout << "Inserted element " << t << std::endl;
#endif

    // if there's someone blocked coz the queue was empty, wake them up.
    if (_countWait)
      _semWait.up();

    _lock.unlock();
  }

  void remove(T& t)
  {
#ifdef TEST_BLOCKINGQUEUE
    std::cout << "Removing element" << std::endl;
#endif
    _lock.lock();

    if (_queue.remove(t) == false)
    {
#ifdef TEST_BLOCKINGQUEUE
      std::cout << "Removing element. Queue empty" << std::endl;
#endif
      // Queue is empty, let us wait until someone puts an item
      _countWait++;
      _lock.unlock();

      _semWait.down(); // wait until someone puts an item

      _lock.lock(); // try again
      _countWait--;
#ifdef TEST_BLOCKINGQUEUE
      std::cout << "Removing element. Retrying." << std::endl;
#endif
    }

#ifdef TEST_BLOCKINGQUEUE
    std::cout << "Removed element " << t << std::endl;
#endif

    if (_countWait)
      _semWait.up();

    _lock.unlock();
  }

private:
  CircularQueue<T> _queue;

  Mutex _lock;

  // NOTE: the queue becomes blocking when either it is empty or full. Both can not
  // happen at the same time. So, we use only one set of count and semaphore.
  size_t _countWait; // number of clients blocked due to either full or empty conditions
  Semaphore _semWait; // semaphore used to inform an exit from full or empty condition
};

#endif // OOD_BLOCKINGQUEUE_H
