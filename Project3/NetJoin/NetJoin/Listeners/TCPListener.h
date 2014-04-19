#ifndef NETJOIN_TCPLISTENER_H
#define NETJOIN_TCPLISTENER_H
/////////////////////////////////////////////////////////////////////////////
// TCPListener.h - Provides the TCPListener class                          //
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
* This module provides the TCPListener class.
*
* TCPListener class implement the NetworkListener interface for TCP based
* connections.
*
* Public Interface:
* =================
* MessageQueue mq;
* TCPListener l(50000, mq);
* 
* Message *m = mq.deQ();
* std::cout << m->Cmd() << std::endl;
* 
* l.Shutdown();
* l.Wait();
*
* Required Files:
* ===============
* TCPListener.h, TCPListener.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_TCPLISTENER TCPListener.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <map>
#include <thread>

#include "../Sockets.h"

#include "../NetworkListener.h"
#include "../BlockingQueue.h"
#include "../ConnectionHandler.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// TCPListener

class TCPListener : public NetworkListener
{
public:
  TCPListener(int port, MessageQueue& dispQ);
  ~TCPListener();

  void Shutdown();
  void Wait();
  bool Send(Message *m);

private:
  std::thread *_thr;
  SocketListener *_sock;

  std::map<Connection*,ConnectionHandler*> _conHndlrs;

  void Run();

  static void ThreadEntry(TCPListener& l);
};

} // namespace NetJoin

#endif // NETJOIN_TCPLISTENER_H
