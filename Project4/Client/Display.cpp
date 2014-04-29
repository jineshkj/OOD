//////////////////////////////////////////////////////////////////////////
//    Display.cpp - source file for Display class                       //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "Display.h"

using namespace std;


//----< test stub >--------------------------------------------

#ifdef TEST_DISPLAY

#include "Display.h"

int main(int argc, char *argv[])
{
  Display disp();

  disp.DisplayHeading(argv[1]);

  disp.DisplayLine();

  return 0;
}
#endif
