#ifndef NETJOIN_TCPCONNECTION_H
#define NETJOIN_TCPCONNECTION_H
/////////////////////////////////////////////////////////////////////////////
// TCPConnection.h - Provides the TCPConnection class                      //
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
* This module provides the TCPConneciton class.
*
* TCPConnecition class is used to create and/or manage a TCP network connection
* based on Prof. Fawcett's Sockets module.
*
* Public Interface:
* =================
* TCPConnection con("localhost", 500002);
* if (con.Open() == false) {
*   std::cout << "Connection failed to open" << std::end;
* }
* 
* con.Send(new Message(Message::Out, "test"));
* Message *rsp = con.Recv();
*
* std::cout << rsp->Data << std::endl;
*
* Required Files:
* ===============
* TCPConnection.h, TCPConnection.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_TCPCONNECTION TCPConnection.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <mutex>
#include <string>

#include "../Sockets.h"
#include "../Connection.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// TCPConnection

class TCPConnection : public Connection
{
public:
  TCPConnection(SOCKET s);
  TCPConnection(const std::string& host, int port) : _host(host), _port(port) { }

  bool Open();
  void Close();

  //----< test to see of the tcp connection is still open >--------------

  bool IsOpen() { return INVALID_SOCKET != (SOCKET)_sock; }

  bool Send(Message *m);
  Message * Recv();

private:
  std::mutex _rlock, _slock;

  Socket _sock;

  const std::string _host;
  int _port;

  Message * RecvHeaders(Message *m);
  Message * RecvData(Message *m);
};

} //namespace NetJoin

#endif // NETJOIN_TCPCONNECTION_H
