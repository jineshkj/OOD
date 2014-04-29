#ifndef NETJOIN_DISPATCHER_H
#define NETJOIN_DISPATCHER_H
/////////////////////////////////////////////////////////////////////////////
// Dispatcher.h - Dispatcher class                                         //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Jinesh Jayakumar, 2014                                      //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Windows 7 SP1                                              //
// Application: OOD Project, Spring 2014                                   //
// Author:      Jinesh Jayakumar <jineshkj at gmail dot com>               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module provides the Dispatcher class.
*
* Dispatcher class dispatches messages like a router. Incoming messages from 
* network listener is routed to appropriate communicator. Outgoing messages 
* from a communicator goes to the listener.
*
* Dispatcher starts a newwork listener and registers one or more communicator
* endpoints.
* 
* Public Interface:
* =================
* Dispatcher d(50000);
* d.Shutdown();
* d.Wait();
*
* Required Files:
* ===============
* Dispatcher.h, Dispatcher.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_DISPATCHER Dispatcher.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <thread>
#include <string>

#include <map>
#include <vector>

#include "NetworkListener.h"
#include "Communicator.h"
#include "BlockingQueue.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// Dispatcher

class Dispatcher
{
public:
  using CommunicatorList = std::vector<Communicator*>;
  using CommandMap = std::map<std::string, Communicator*>;

  Dispatcher(int listenerport);
  ~Dispatcher();

  void Wait();
  void Shutdown();

private:
  std::thread *_thr;
  NetworkListener *_listener;

  CommunicatorList _communicators;
  CommandMap _commandMap;

  BlockingQueue<Message*> _msgQ;

  void Run();
  void Process(Message *m);
  void ProcessIncoming(Message *m);
  void ProcessOutgoing(Message *m);

  void RegisterCommunicator(Communicator *c);
  static void ThreadEntry(Dispatcher& d);
};

} // namespace NetJoin

#endif // NETJOIN_DISPATCHER_H
