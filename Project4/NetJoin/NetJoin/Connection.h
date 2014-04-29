#ifndef NETJOIN_CONNECTION_H
#define NETJOIN_CONNECTION_H
/////////////////////////////////////////////////////////////////////////////
// Connection.h - Generic network connection class                         //
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
* This module provides the Connection class.
*
* Connection is a generic interface for a network connection like TCPConnection. 
*
* Public Interface:
* =================
* conn.Open();
* if (conn.Open() == false)
*   std::cout << "Connection failed" << std::endl;
*
* Required Files:
* ===============
* Connection.h
*
* Build Command:
* ==============
*   <can not be built since its an interface>
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include "Message.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// Connection

class Connection
{
public:
  virtual bool Open() = 0;
  virtual void Close() = 0;
  virtual bool IsOpen() = 0;

  virtual bool Send(Message *m) = 0;
  virtual Message * Recv() = 0;
};

} // namespace NetJoin

#endif // NETJOIN_CONNECTION_H
