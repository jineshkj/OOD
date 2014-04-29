#ifndef NETJOIN_CONFIG_H
#define NETJOIN_CONFIG_H
/////////////////////////////////////////////////////////////////////////////
// Config.h - Netjoin configuration structure                              //
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
* This module provides the Config structure.
*
* Config is a structure that can hold all major service configuration 
* parameters of the NetJoin service. 
*
* Public Interface:
* =================
* cmd.port = 50000;
* cmd.proto = Config::Udp;
*
* Required Files:
* ===============
* Config.h, Config.cpp
*
* Build Command:
* ==============
*  cl /EHa /DTEST_CONFIG Config.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <cstdint>
#include <string>

namespace NetJoin {

/////////////////////////////////////////////////////////
// Config

struct Config
{
  //----< supported network protocols >--------------
  enum Protocol {
    Tcp,
    Udp
  };

  Protocol proto;
  uint16_t port;
  std::string tmpdir;

  static const int DEF_PORT = 50000;
  static const Protocol DEF_PROTO = Tcp;

  //----< Config structure constructor >--------------

  Config() : port(DEF_PORT), proto(DEF_PROTO) { }
};

} // namespace NetJoin

#endif // NETJOIN_CONFIG_H
