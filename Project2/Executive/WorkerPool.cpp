/////////////////////////////////////////////////////////////////////
//  WorkerPool.cpp - Implementation for WorkerPool and             //
//                   WorkerThread classes                          //
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

#include <Windows.h>

#include "WorkerPool.h"

WorkerPool::WorkerPool(const IWorker *creator, size_t qsize) :
    _inq(qsize), _outq(qsize)
{
  size_t num_workers = numProcessors();

  if (num_workers == 0) // should ideally never happen
    num_workers = 1;

  for (size_t i = 0; i < num_workers; i++)
    _wthreads.push_back(new WorkerThread(this, creator->CreateWorker()));
}

WorkerPool::~WorkerPool()
{
  for (auto& w : _wthreads)
    delete w;
}

IWork* WorkerPool::exec(IWork *work)
{
  IWork * compl = NULL;

  // Check _outq, if there are any completed work
  (void) _outq.remove(compl, true);

  // Add work to _inq. Block if _inq is full until space is available
  (void) _inq.insert(work);

  // Check _outq, if there are any completed work, return it
  if (compl == NULL)
    _outq.remove(compl, true);

  return compl;
}

IWork * WorkerPool::wait()
{
  IWork * compl = NULL;

retry:

  // If there are still tasks in inq, then simply wait for a 
  // completed task and return it unless its a End-of-Work marker (NULL)
  if (!_inq.empty())
  {
    _outq.remove(compl);

    if (compl)
      return compl;
    else
      goto retry;
  }

  bool running = false;

  for (auto& w : _wthreads)
  {
    if (w->running())
    {
      running = true;
      _inq.insert(NULL); // End-of-Work marker
      _outq.remove(compl);
      break;
    }
  }

  if (running == false)
    return NULL;

  if (!compl)
    goto retry;

  return compl;
}

IWork* WorkerPool::in()
{
  IWork *work;

  // Take out a work from _inq if available. otherwise block
  (void) _inq.remove(work);

  return work;
}

void WorkerPool::out(IWork *work)
{
  // Put the given work to _outq if space is available, otherwise block
  (void) _outq.insert(work);
}

size_t WorkerPool::numProcessors() const
{
  SYSTEM_INFO info;

  GetSystemInfo(&info);

  return info.dwNumberOfProcessors;
}

WorkerThread::WorkerThread(WorkerPool *pool, IWorker *worker) : 
_pool(pool), _worker(worker), _run(1, 1), _running(true)
{
  _thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WorkerThread::entry, this, 0, NULL);

  if (_thread == NULL)
    throw "Failed to create win32 thread";
}

WorkerThread::~WorkerThread()
{
  CloseHandle(_thread);
  _thread = NULL;
}

void WorkerThread::entry(void *arg)
{
  WorkerThread *thr = static_cast<WorkerThread*>(arg);

  thr->exec();
}

void WorkerThread::exec()
{
  while (true)
  {
    _run.down();
    _running = true;

    // Fetch work from pool
    IWork * work = _pool->in();

    // Execute work using worker
    if (work != NULL)
    {
      _worker->Exec(work);
      _run.up();
    }
    else
    {
      _running = false;  
    }

    // Put the completed work back to pool
    _pool->out(work);
  }
}

#ifdef TEST_WORKERPOOL

#include <iostream>

struct TestWork : public IWork
{
  TestWork(int i) : data(i) { }

  int data;
};

class TestWorker : public IWorker
{
public:
  IWorker * CreateWorker() const { return new TestWorker; }

  void Exec(IWork *work)
  {
    TestWork * w = static_cast<TestWork*>(work);

    w->data += 100;
  }
};

void main()
{
  TestWorker sample;
  WorkerPool p(&sample, 128);

  for (int i = 1; i < 1024; i++)
  {
    IWork * work = new TestWork(i);
    //std::cout << "Sending " << (void*) work << " " << i << std::endl;

    IWork *compl = p.exec(work);
    if (compl) {
      std::cout << "Received " << (void*) compl << " " << static_cast<TestWork*>(compl)->data << std::endl;
    }
  }

  IWork *compl;

  while ((compl = p.wait()) != NULL)
  {
    std::cout << "Received " << (void*)compl << " " << static_cast<TestWork*>(compl)->data << std::endl;
  }
  
}

#endif
