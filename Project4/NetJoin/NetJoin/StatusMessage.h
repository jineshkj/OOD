#ifndef NETJOIN_STATUSMESSAGE_H
#define NETJOIN_STATUSMESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// Command.h - Provides StatusMessage class                                //
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
* This module provides the StatusMessage class.
*
* StatusMessage is-a-kind-of Message that is usually send out from a 
* communicator as a response to a Command it executed.
* 
* Public Interface:
* =================
* StatusMessage st1(true); //success;
* StatusMessage st2(false, "Permission denied"); // failure
*
* Required Files:
* ===============
* StatusMessage.h, StatusMessage.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_STATUSMESSAGE StatusMessage.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 18 Apr 14
* - first release
*/

#include <string>

#include "Message.h"

namespace NetJoin {

/////////////////////////////////////////////////////////
// StatusMessage

class StatusMessage : public Message
{
public:

  //----< StatusMessage class constructor >--------------

  StatusMessage(bool status, const std::string& desc="") : Message(Message::Out, "Status")
  {
    AddHeader("Success", status ? "True" : "False");
    if (desc.size())
      WriteData(desc.c_str(), desc.size());
  }
};

} // namespace NetJoin

#endif // NETJOIN_STATUSMESSAGE_H
