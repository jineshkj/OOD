#ifndef OOD_WORKERPOOL_H
#define OOD_WORKERPOOL_H
/////////////////////////////////////////////////////////////////////
//  WorkerPool.h - A worker pool implementation                    //
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
This module defines the WorkerPool class that can be used to create 
worker pools so that different jobs can be offloaded by the main thread
into the thread pool. Apart from the WorkerPool class, a WorkerThread
class is implemented but is meant to be used only internally within
the WorkerPool implementation. As such documentation for using WorkerThread
class is not given here.

Public Interface:
=================

struct MyWork : public IWork
{
...
};

struct MyWorker : public IWorker
{
...
  IWorker * CreateWorker();
  void Exec(IWork*);
  ...
};

...

MyWorker sample;
WorkerPool p(&sample);

IWork *completed;

completed = p.exec(new MyWork);
if (completed)
  process_completed_work(completed);

...

while ((completed = p.wait()) != NULL)
  process_completed_work(completed);

...

Build Process:
==============
Required files
- WorkerPool.h
Build commands (either one)
- cl /EHsc /DTEST_WORKERPOOL WorkerPool.cpp

Maintenance History:
=====================
ver 1.0 : 14 Mar 2014
- first release

*/

#include <vector>

#include "IWork.h"
#include "IWorker.h"

#include "Mutex.h"
#include "Semaphore.h"
#include "BlockingQueue.h"

class WorkerThread;

class WorkerPool
{
  static const int DEFAULT_WORKERS = 2;
  static const size_t DEFAULT_QSIZE = 128;

public:
  WorkerPool(const IWorker *creator, size_t qsize = DEFAULT_QSIZE);
  ~WorkerPool();

  IWork* exec(IWork *work);
  IWork* wait();

  IWork* in();
  void out(IWork *work);

private:
  std::vector<WorkerThread*> _wthreads;
  BlockingQueue<IWork*> _inq, _outq;

  size_t numProcessors() const;
};

class WorkerThread
{
public:
  WorkerThread(WorkerPool *pool, IWorker *worker);
  ~WorkerThread();

  bool running() { return _running; }
  void run() { _run.up(); }

private:
  WorkerPool *_pool;
  IWorker *_worker;
  void *_thread;
  Semaphore _run;
  bool _running;

  static void entry(void *arg);

  void exec();
};

#endif // OOD_WORKERPOOL_H
