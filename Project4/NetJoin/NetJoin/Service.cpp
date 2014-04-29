/////////////////////////////////////////////////////////////////////////////
// Service.cpp - Implements the Service class                              //
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

#include "Connections/TCPConnection.h"

#include "ConnectionHandler.h"

#include "Service.h"

using namespace NetJoin;

//----< Service class constructor >--------------

Service::Service(const Config& c) : _cfg(c), _dispatcher(0)
{
  switch (c.proto)
  {
  case Config::Tcp:
    _dispatcher = new Dispatcher(c.port); // start dispatcher thread
    break;

  case Config::Udp:
    throw "UDP network protocol not supported";

  default:
    throw "Configured network protocol not recognized";
  }
}

//----< Service class destructor >--------------

Service::~Service()
{
  _dispatcher->Shutdown();

  Wait();

  delete _dispatcher; _dispatcher = 0;
}

//----< wait for all internal services to finish >--------------

void Service::Wait()
{
  _dispatcher->Wait();
}

//----< execute a command on remote host >--------------

void Service::Execute(Command& cmd, const std::string& host, int port)
{
  cmd.status() = Command::EXECUTING;

  Connection *con = new TCPConnection(host, port);

  if (!con->Open())
  {
    delete con;
    throw "Could not connect to " + host;
  }

  con->Send(&cmd.Msg());

  Message *rsp = con->Recv();
  cmd.ReadMessage(*rsp);
  delete rsp;
}

#ifdef TEST_SERVICE

void main()
{
  Config cfg;
  Service svc(cfg);

  svc.Execute("localhost", 50003, 0 /*dummy*/);

  svc.Wait();
}

#endif
