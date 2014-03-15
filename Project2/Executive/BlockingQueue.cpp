#ifdef TEST_BLOCKINGQUEUE

#include <windows.h>

//void SetThreadPriority(int prio)
//{
//  (void)SetThreadPriority(GetCurrentThread(), prio);
//}

#include <iostream>

#include "BlockingQueue.h"

using Queue = BlockingQueue<int>;

int i = 0;
int r = -1;

void producer (void *arg)
{
  Queue *q = static_cast<Queue*>(arg);

  while (true)
    q->insert(i++);
}

void consumer (void *arg)
{
  Queue *q = static_cast<Queue*>(arg);

  while (true) {
    q->remove(r);
    std::cout << i << std::endl;
  }
    
}

void main()
{
  Queue q(128);

  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) producer, &q, 0, NULL);

  Sleep(5000); // wait 5 seconds before starting consumption

  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) consumer, &q, 0, NULL);

  Sleep(5000); // wait 5 seconds before exiting

  std::cout << "i: " << i << " r: " << r << std::endl;
}

#endif
