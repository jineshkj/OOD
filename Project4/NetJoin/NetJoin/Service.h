#ifndef NETJOIN_SERVICE_H
#define NETJOIN_SERVICE_H
/////////////////////////////////////////////////////////////////////////////
// Service.h - Provides NetJoin main service class                         //
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
* This module provides the Service.
*
* Service class takes care of instantiating all necessary NetJoin services 
* like Dispatcher, NetworkListener, ConnectionHandler, etc. Using the service
* is optional if all you need is to act as a client.
* 
* Public Interface:
* =================
* Config cfg;
* cfg.port = 10000;
* 
* Service svc(cfg);
* svc.Wait();
*
* Required Files:
* ===============
* Service.h, Service.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_SERVICE Service.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include "Config.h"

#include "Dispatcher.h"
#include "NetworkListener.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// Service

class Service
{
public:
  Service(const Config& c);
  ~Service();

  void Wait();

  static void Execute(Command& cmd, const std::string& host = "localhost", int port = Config::DEF_PORT);

private:
  const Config& _cfg;

  Dispatcher *_dispatcher;
};

} // namespace NetJoin

#endif // NETJOIN_SERVICE_H
