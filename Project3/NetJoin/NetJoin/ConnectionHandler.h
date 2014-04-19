#ifndef NETJOIN_CONNECTIONHANDLER_H
#define NETJOIN_CONNECTIONHANDLER_H
/////////////////////////////////////////////////////////////////////////////
// ConnectionHandler.h - ConnectionHandler class                           //
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
* This module provides the ConnectionHandler class.
*
* ConnectionHandler can be used to manage a network Connection. It starts 2
* threads - one for sending and another for receiving messages from the given
* Connection.
* 
* Public Interface:
* =================
* Connection *c;
* MessageQueue inQ;
* ConnectionHandelr h(inQ, c);
* 
* h->Send(new Message(Message::Out, "Echo"));
*
* h->Wait();
*
* Required Files:
* ===============
* ConnectionHandler.h, ConnectionHandler.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_CONNECTIONHANDLER ConnectionHandelr.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <thread>

#include "Message.h"
#include "Connection.h"
#include "BlockingQueue.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// ConnectionHandler

class ConnectionHandler
{
public:
  ConnectionHandler(MessageQueue& inQ, Connection *c);
  ~ConnectionHandler();

  void Shutdown();
  void Wait();

  //----< Enqueue a message for sending out >--------------

  void Send(Message *m) { _outQ.enQ(m); }

private:
  std::thread *_thr1, *_thr2;

  MessageQueue& _inQ; // typically points to dispatcher's queue
  MessageQueue _outQ;

  Connection *_con; // takes ownership of connection

  void MessageSender();
  void NetworkReceiver();
  void NotifyListener();

  static void Thread1Entry(ConnectionHandler& ch);
  static void Thread2Entry(ConnectionHandler& ch);
};

} //namespace NetJoin

#endif // NETJOIN_CONNECTIONHANDLER_H
