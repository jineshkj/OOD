/////////////////////////////////////////////////////////////////////////////
// TCPListener.cpp - Implements the TCPListener class                      //
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

#include "../Connections/TCPConnection.h"
#include "TCPListener.h"

using namespace NetJoin;

//----< TCPListener constructor >--------------

TCPListener::TCPListener(int port, MessageQueue& dispQ) : NetworkListener(port, dispQ), _sock(new SocketListener(port))
{
  _thr = new std::thread(TCPListener::ThreadEntry, std::ref(*this));
}

//----< TCPListener destructor >--------------

TCPListener::~TCPListener()
{
  delete _sock; _sock = 0;
}

//----< shutdown TCP listener >--------------

void TCPListener::Shutdown()
{
  _sock->stop();

  for (auto& c : _conHndlrs)
    c.second->Shutdown();
}

//----< wait for listener to finish >--------------

void TCPListener::Wait()
{
  for (auto& c : _conHndlrs)
    c.second->Wait();

  if (_thr->joinable()) _thr->join();
}

//----< queue a message to its connection handler >--------------

bool TCPListener::Send(Message *m)
{
  auto it = _conHndlrs.find(m->Conn());

  if (it != _conHndlrs.end())
  {
    Connection *c = it->first;
    ConnectionHandler *h = it->second;

    if (c->IsOpen())
    {
      h->Send(m);

    } else
    {
      _conHndlrs.erase(it);
      delete h;

      if (m->Owner() == 0)
        delete m;
    }

    return true;

  }
  else if (m->Cmd() == "") // skip dummy messages
  {
    if (m->Owner() == 0)
      delete m;

    return true;
  }

  return false;
}

//----< TCP listener main loop >--------------

void TCPListener::Run()
{
  std::cout << "TCP Listener started" << std::endl;

  while (true)
  {
    SOCKET s = INVALID_SOCKET;

    try {
      s = _sock->waitForConnect();
    }
    catch (std::exception e) { // in case of socket error, just stop the loop
      break;
    }

    Connection *c = new TCPConnection(s);
    _conHndlrs[c] = new ConnectionHandler(_inQ, c);
  }
  
  std::cout << "TCP Listener stopped" << std::endl;
}

//----< entry point for thread >--------------

void TCPListener::ThreadEntry(TCPListener& l)
{
  l.Run();
}

#ifdef TEST_TCPLISTENER

void main()
{
  MessageQueue mq;
  TCPListener l(50000, mq);

  Message *m = mq.deQ();
  std::cout << m->Cmd() << std::endl;

  l.Shutdown();
  l.Wait();
}

#endif
