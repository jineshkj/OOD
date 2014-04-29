#ifndef OOD_DISPLAY_H
#define OOD_DISPLAY_H
//////////////////////////////////////////////////////////////////////////
//    Display.h    - header file for Display class                      //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 4                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module provides the Display class that is used to display results on
screen.

Public Interface:
=================
Display();
void PrintBanner();
void PrintUsage();

void PauseForUser();
void DisplayLine(char ch = '-');
void DisplayHeading(const std::string& text);


Required Files:
===============
Display.h, Display.cpp

Build Command:
==============
cl /EHa /DTEST_DISPLAY Display.cpp

Maintenance History:
====================
ver 1.0 : 28 Apr 2014
- first release

*/

#include <string>
#include <iostream>

class Display
{
public:
  //----< waits for user to press an ENTER key >------------

  void PauseForUser()
  {
    std::cout << "Press [ENTER] to continue..." << std::endl;
    getchar();
  }

  //----< display a horizontal line on stream >------------

  void DisplayLine(char ch = '-')
  {
    for (int i = 0; i < 79; i++)
      std::cout << ch;
    std::cout << std::endl;
  }

  //----< print a given text as heading underlined >------------

  void DisplayHeading(const std::string& text)
  {
    std::cout << text << std::endl;
    DisplayLine();
  }
};

#endif // OOD_DISPLAY_H
