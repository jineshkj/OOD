/////////////////////////////////////////////////////////////////////////////
// ConnectionHandler.cpp - Implements the ConnectionHandler class          //
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

#include "ConnectionHandler.h"

using namespace NetJoin;

//----< constructor for ConnectionHandler class >--------------

ConnectionHandler::ConnectionHandler(MessageQueue& inQ, Connection *c) : _inQ(inQ), _con(c)
{
  _thr1 = new std::thread(Thread1Entry, std::ref(*this));
  _thr2 = new std::thread(Thread2Entry, std::ref(*this));
}

//----< destructor for ConnectionHandler class >--------------

ConnectionHandler::~ConnectionHandler()
{
  Shutdown();
  Wait();

  delete _con; _con = 0;
  delete _thr1; _thr1 = 0;
  delete _thr2; _thr2 = 0;
}

//----< shutdown connection handler thread >--------------

void ConnectionHandler::Shutdown()
{
  _con->Close();
  _outQ.enQ(0);

  NotifyListener();
}

//----< wait for connection handler thread to finish >--------------

void ConnectionHandler::Wait()
{
  if (_thr1->joinable()) _thr1->join();
  if (_thr2->joinable()) _thr2->join();
}


//----< loop for sending out messages through connection >--------------

void ConnectionHandler::MessageSender()
{
  std::cout << "Connection handler message sender started" << std::endl;

  while (true)
  {
    Message *m = _outQ.deQ();

    if (m == 0) // termination
      break;

    if (!_con->Send(m)) // connection failure
      break;
  }

  std::cout << "Connection handler message sender stopped" << std::endl;

  Shutdown();
}

//----< loop for receiving messages from connection >--------------

void ConnectionHandler::NetworkReceiver()
{
  std::cout << "Connection handler network receiver started" << std::endl;

  while (true)
  {
    Message *m = _con->Recv();

    if (m == NULL) // end of connection
      break;

    _inQ.enQ(m);
  }

  std::cout << "Connection handler network receiver stopped" << std::endl;

  Shutdown();
}

//----< send message to listener via dispatcher about self termination >--------------

void ConnectionHandler::NotifyListener()
{
  // Notify listener by sending an empty message to dispatcher
  Message *m = new Message(Message::Out, "");
  m->SetConn(_con);
  _inQ.enQ(m);
}

//----< thread entry routine for first thread >--------------

void ConnectionHandler::Thread1Entry(ConnectionHandler& ch)
{
  ch.NetworkReceiver();
}

//----< thred entry routine for second thread >--------------

void ConnectionHandler::Thread2Entry(ConnectionHandler& ch)
{
  ch.MessageSender();
}

#ifdef TEST_CONNECTIONHANDLER

#include "Message.h"
#include "Connections/TCPConnection.h"

void main()
{
  TCPConnection c("localhost", 50002);
  if (con.Open() == false)
    std::cout << "open error" << std::endl;

  MessageQueue msgQ;
  ConnectionHandler h(msgQ, &con);

  Message *m = new Message(Message::Out, "Echo");
  h.Send(m);

  m = msgQ.deQ();
  std::cout << "Cmd = " << m->Cmd() << std::endl;
  delete m;
}

#endif
