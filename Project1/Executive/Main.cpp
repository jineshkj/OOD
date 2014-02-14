//////////////////////////////////////////////////////////////////////////
//    Main.cpp - source file for main() function                        //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "Executive.h"

int main(int argc, char *argv[])
{
  // any argument manipulation can go here.

  Executive exec(argc, argv);

  // Add any miscellaneous configuration here

  return exec.run();
}
