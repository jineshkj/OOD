#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H
///////////////////////////////////////////////////////////////////
// BlockingQueue.h - Thread-safe queue that blocks on empty deQ  //
// ver 1.1                                                       //
// Language: standard C++                                        //
// Platform: Dell Dimension T7400, Windows 7, SP #1              //
// Application: Resource for DO projects                         //
// Author:      Jinesh Jayakumar <jineshkj@gmail.com>            //
// Source:      Jim Fawcett, CST 4-187, Syracuse University      //
//              jfawcett@twcny.rr.com, (315) 443-3948            //
///////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * This package contains a BlockingQueue<> class that is thread-safe
 * and blocks a deQ'er if the queue is empty.  Blocking is accomplished
 * using a ConditionVariable (introduced with Vista) and an associated
 * lock based on a CriticalSection.
 *
 * Users don't need to be aware of how this works.  They just use
 * the queue without worrying about locking.
 */
/*
 * Required Files:
 * ---------------
 * BlockingQueue.h, BlockingQueue.cpp, Locks.h, Locks.cpp, Thread.h, Thread.cpp
 *
 * Build Process:
 * --------------
 * cl /EHa /DTEST_BLOCKINGQUEUE BlockingQueue.cpp Locks.cpp Thread.cpp
 *
 * Maintenance History:
 * --------------------
 * ver 1.2 : 16 Apr 14
 * - Use c++11 locks
 * ver 1.1 : 24 Mar 13
 * - small revisions for new threadBase
 * ver 1.0 : 19 Feb 12
 * - first release
 */

#include <queue>
#include <mutex>
#include <condition_variable>

///////////////////////////////////////////////////////////////
// BlockingQueue<Msg>

template <typename Msg>
class BlockingQueue
{
public:
  BlockingQueue();
  void enQ(Msg msg);
  Msg deQ();
  size_t size();
private:
  std::queue<Msg> _Q;
  std::mutex qLock;
  std::condition_variable qCv;
};
//----< Ctor >-------------------------------------------------

template <typename Msg>
BlockingQueue<Msg>::BlockingQueue()
{
  qCv.notify_all();  // clear lock - probably not necessary
}
//----< add a message to queue >-------------------------------

template <typename Msg>
void BlockingQueue<Msg>::enQ(Msg msg)
{
  std::lock_guard<std::mutex> lk(qLock);
  _Q.push(msg);
  qCv.notify_one();
}
//----< remove a message from queue >--------------------------

template <typename Msg>
Msg BlockingQueue<Msg>::deQ()
{
  Msg msg;
  std::unique_lock<std::mutex> lk(qLock);
  if(_Q.size() > 0)
  {
    msg = _Q.front();
    _Q.pop();
    return msg;
  }
  // I'm not sure if this loop is necessary

  while(true)
  {
    qCv.wait(lk);
    if(_Q.size() == 0)
      continue;
    msg = _Q.front();
    _Q.pop();
    return msg;
  }
}
//----< return number of queueud messages >--------------------

template <typename Msg>
size_t BlockingQueue<Msg>::size()
{
  std::lock_guard<std::mutex> lk(qLock);
  return _Q.size();
}

#endif
