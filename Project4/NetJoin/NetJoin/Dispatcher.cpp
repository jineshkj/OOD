/////////////////////////////////////////////////////////////////////////////
// Dispatcher.cpp - Implements the Dispatcher class                        //
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

#include "Listeners/TCPListener.h"
#include "Communicators/FileTransfer.h"
#include "Communicators/TextSearch.h"

#include <iostream>

#include "Connection.h"
#include "ConnectionHandler.h"

#include "Dispatcher.h"

using namespace NetJoin;

//----< dispatcher constructor >--------------

Dispatcher::Dispatcher(int lport)
{
  _listener = new TCPListener(lport, _msgQ);

  RegisterCommunicator(new FileTransfer(_msgQ));
  RegisterCommunicator(new TextSearch(_msgQ));

  _thr = new std::thread(Dispatcher::ThreadEntry, std::ref(*this));
}

//----< dispatcher destructor >--------------

Dispatcher::~Dispatcher()
{
  Shutdown();
  Wait();

  for (auto& c : _communicators)
    delete c;

  _communicators.clear();
  _commandMap.clear();

  delete _listener; _listener = 0;
  delete _thr; _thr = 0;
}

//----< register a communicator with dispatcher >--------------

void Dispatcher::RegisterCommunicator(Communicator *c)
{
  _communicators.push_back(c);

  for (auto& cmd : c->Commands())
    _commandMap[cmd] = c;
}

//----< wait for the dispatcher thread to complete >--------------

void Dispatcher::Wait()
{
  _listener->Wait();

  for (auto& c : _communicators)
    c->Wait();

  if (_thr->joinable()) _thr->join();
}

//----< shutdown the dispatcher thread >--------------

void Dispatcher::Shutdown()
{
  _listener->Shutdown();

  for (auto& c : _communicators)
    c->Shutdown();

  _msgQ.enQ(0);
}

//----< dispatcher thread main loop >--------------

void Dispatcher::Run()
{
  std::cout << "Dispatcher started" << std::endl;

  // get a message from listner
  while (true)
  {
    Message * m = _msgQ.deQ();

    if (m == 0) // shutdown signal received
      break;

    //std::cout << "Dispatcher received " << ((m->Dir() == Message::In)?"IN":"OUT") << " message " << m->Cmd() << std::endl;

    Process(m);
  }
  
  std::cout << "Dispatcher finished" << std::endl;
}

//----< process a message extracted from queue >--------------

void Dispatcher::Process(Message *m)
{
  if (m->Dir() == Message::In)
    ProcessIncoming(m);
  else
    ProcessOutgoing(m);
}

//----< process incoming message >--------------

void Dispatcher::ProcessIncoming(Message *m)
{
  // send the message to the correct communicator
  CommandMap::iterator i = _commandMap.find(m->Cmd());
  if (i == _commandMap.end())
    throw "No registered communicator for " + m->Cmd();

  Communicator *c = i->second;
  Command *cmd = c->MsgToCmd(m);
  c->EnQ(cmd);
}

//----< process outgoing message >--------------

void Dispatcher::ProcessOutgoing(Message *m)
{
  // send the message out using its connection handler
  if (!_listener->Send(m))
  {
    if (m->Owner() == 0)
      delete m;

    throw "No handler present for outgoing message";
  }
}

//----< entry point for std:thread object >--------------

void Dispatcher::ThreadEntry(Dispatcher& d)
{
  d.Run();
}

#ifdef TEST_DISPATCHER

#include "Communicators/FileTransfer.h"
#include "Connections/TCPConnection.h"

void main()
{
  Dispatcher d(50005);
  TCPConnection con("localhost", 50005);
  if (con.Open() == false)
    std::cout << "open error" << std::endl;

  FileTransfer::Putfile pf("C:\\a.txt");

  con.Send(&pf.Msg());
  Message *rsp = con.Recv();

  std::cout << rsp->Cmd() << std::endl;
}

#endif
