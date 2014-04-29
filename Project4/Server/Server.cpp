/////////////////////////////////////////////////////////////////////////////
// Server.cpp - Server application for project 4                           //
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
* NOTICE: NetJoin::Service is the executive package for Server. Also, display
*         package is intentionally avoided since it's considered bad practice
*         for server to output results on console. Instead, all relevant 
*         output is given back to the client for displaying to end user.
*/

#include <iostream>
#include <cstdlib>

#include <NetJoin.h>

void main()
{
  NetJoin::Config cfg; // create the default config

  try {
    NetJoin::Service svc(cfg);
    svc.Wait();
  }
  catch (std::exception e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    exit(1);
  }
}
