#ifndef NETJOIN_MESSAGE_H
#define NETJOIN_MESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// Message.h - Provides generic Message class                              //
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
* This module provides the Message class.
*
* Message class is a generic network message abstraction for all messages 
* exchanged between NetJoin services. The message contains an initial command
* line followed by headers which are key-value pairs. Data if any, follows
* later. Message format is similar to HTTP messages.
* 
* Public Interface:
* =================
* Message m(Message::Out, "TestMessage");
* m.AddHeader("Key1", "Val1");
* m.WriteData("Hello", 5);
* 
* for (auto& h : m.Headers())
* {
*   std::cout << "Key: " << h.first << ", Val: " << h.second << std::endl;
* }
* 
* std::cout << m.Data() << std::endl;
*
* Required Files:
* ===============
* Message.h, Message.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_MESSAGE Message.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <map>
#include <string>
#include <sstream>

#include "BlockingQueue.h"

#define NJ_PROTO_PREFIX "NETJOIN"

namespace NetJoin {

class Connection;

/////////////////////////////////////////////////////////
// Message

// Message command/first line format: NETJOIN <command name|Status>
class Message
{
public:
  //----< direction of message respective to the system >--------------
  enum Direction {
    In,
    Out
  };

  using Property   = std::string;
  using Value      = std::string;
  using HeaderList = std::map<Property,Value>;

  Message(Direction d, const std::string& cmdline, void *owner=0);

  //----< Message class destructor >--------------

  ~Message() { }

  //----< get direction of message >--------------

  Direction Dir() const { return _dir; }

  //----< get command name present in Message >--------------

  const std::string& Cmd() const { return _cmd; }

  //----< add a header to the message >--------------

  void AddHeader(const Property& p, const Value& v) { _headers[p] = v; }

  //----< write to message data buffer >--------------

  void WriteData(const char *data, std::streamsize size) { _data.write(data, size); }

  const std::string CommandLine() const;

  //----< return header list >--------------

  HeaderList& Headers() { return _headers; }

  //----< return message data >--------------

  std::string Data() const { return _data.str(); }

  //----< set an owner for message object >--------------

  void SetOwner(void *o) { _owner = o; }

  //----< get owner of the message object >--------------

  void * Owner() const { return _owner; }

  //----< set associated Connection object >--------------

  void SetConn(Connection* c) { _conn = c; }

  //----< get associated Connection object >--------------

  Connection * Conn() { return _conn; }

private:
  Direction _dir;
  std::string _cmd;

  HeaderList _headers;
  std::ostringstream _data;

  void *_owner;
  Connection *_conn;
};

using MessageQueue = BlockingQueue<Message*>;

} // namespace NetJoin

#endif // NETJOIN_MESSAGE_H
