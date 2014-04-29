/////////////////////////////////////////////////////////////////////////////
// TCPConnection.cpp - Implements the TCPConnecion class                   //
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

#include <iostream>
#include <cstdlib>

#include <string>

#include "TCPConnection.h"

using namespace NetJoin;

//----< TCPConnection class constructor >--------------

TCPConnection::TCPConnection(SOCKET s) : _sock(s), _host(_sock.System().getRemoteIP(&_sock)), _port(_sock.System().getRemotePort(&_sock))
{
}

//----< connect to a remote host/port >--------------

bool TCPConnection::Open()
{
  return _sock.connect(_host, _port, true);
}

//----< close current connection >--------------

void TCPConnection::Close()
{
  _sock.disconnect();
}

//----< send a message out through connection >--------------

bool TCPConnection::Send(Message *m)
{
  std::cout << "Sending message " << m->Cmd() << " to TCPConnection" << std::endl;

  std::lock_guard<std::mutex> lk(_slock);

  m->AddHeader("Dst", _host);

  _sock.writeLine(m->CommandLine());

  for (auto& hdr : m->Headers())
    _sock.writeLine(hdr.first + ":" + hdr.second);

  std::string data = m->Data();

  if (m->Owner() == 0) // delete message if no one owns it
    delete m;

  if (data.size() != 0)
  {
    std::ostringstream cl;
    cl << "Content-length:" << data.size();
    _sock.writeLine(cl.str());
  }

  _sock.writeLine(""); // end of headers

  if (_sock.send(data.c_str(), data.size()) == data.size())
    return true;

  return false;
}

//----< receive a message from connection >--------------

Message * TCPConnection::Recv()
{
  std::lock_guard<std::mutex> lk(_rlock);
  std::string cmdline = _sock.readLine();

  if (cmdline.size() == 0)
    return 0;

  Message *m = new Message(Message::In, cmdline);

  if (m->CommandLine().size() == 0)
  {
    delete m;
    throw "Invalid message command line received";
  }

  if ((m = RecvHeaders(m)) == 0)
    return 0;

  if ((m = RecvData(m)) == 0)
    return 0;

  m->SetConn(this);

  std::cout << "Received message " << m->Cmd() << " from TCPConnection" << std::endl;

  return m;
}

//----< receive message headers from connection >--------------

Message * TCPConnection::RecvHeaders(Message *m)
{
  m->AddHeader("Src", _host);

  while (true)
  {
    std::string line = _sock.readLine();

    if (line.size() == 0) // connection terminated
    {
      delete m;
      return 0;
    }

    if (line.size() == 1) // just eol
      break;

    int col = line.find(':');

    if (col == line.npos)
      continue; // silently ignore error in parsing header

    m->AddHeader(line.substr(0, col), line.substr(col + 1, line.size() - col - 2));
  }

  return m;
}

//----< receive message data from connection >--------------

Message * TCPConnection::RecvData(Message *m)
{
  Message::HeaderList::const_iterator cl = m->Headers().find("Content-length");

  if (cl != m->Headers().end())
  {
    const int data_size = atoi(cl->second.c_str());
    std::cout << "DataSize : " << data_size << std::endl;
    char buffer[4096];

    int count = 0;
    while (count < data_size)
    {
      int to_read = data_size - count;
      if (to_read > sizeof(buffer))
        to_read = sizeof(buffer);

      if (_sock.recvAll(buffer, to_read, false) == true)
      {
        count += to_read;
        m->WriteData(buffer, to_read);
      }
      else
      { // connection error
        delete m;
        return 0;
      }
    }
  }

  return m;
}

#ifdef TEST_TCPCONNECTION

void main()
{
  TCPConnection c("127.0.0.1", 50000);
  
  Message *m = new Message(Message::Out, "Echo");
  m->WriteData("Hello World", 11);

  c.Send(m);
  m = c.Recv();

  std::cout << m->Data() << std::endl;
}

#endif
