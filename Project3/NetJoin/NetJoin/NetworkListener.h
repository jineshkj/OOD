#ifndef NETJOIN_NETWORKLISTENER_H
#define NETJOIN_NETWORKLISTENER_H
/////////////////////////////////////////////////////////////////////////////
// NetworkListener.h - Provides the NetworkListener interface              //
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
* This module provides the NetworkListener interface.
*
* NetworkListener is an abstract class that need to be implemented by various
* types of network listeners like TCPListener. The job of a listener is to 
* listen for network connections and handle them.
*
* Public Interface:
* =================
*  <refer an implementation like TCPListener>
*
* Required Files:
* ===============
* NetworkListener.h
*
* Build Command:
* ==============
*  <no need to build since its an interface>
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include "Message.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// NetworkListener

class NetworkListener
{
public:
  virtual void Shutdown() = 0;
  virtual void Wait() = 0;
  virtual bool Send(Message *m) = 0;

protected:

  //----< NetworkListener constructor >--------------

  NetworkListener(int port, MessageQueue& inQ) : _port(port), _inQ(inQ) { }
  
protected:
  int _port;
  MessageQueue& _inQ;
};

} // namespace NetJoin

#endif // NETJOIN_NETWORKLISTENER_H
