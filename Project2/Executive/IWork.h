#ifndef OOD_IWORK_H
#define OOD_IWORK_H
/////////////////////////////////////////////////////////////////////
//  IWork.h - IWork interface specification header file            //
//  ver 1.0                                                        //
//                                                                 //
//  Language:      C++                                             //
//  Platform:      Any                                             //
//  Application:   CIS 687 Spring 2014 Project 2                   //
//  Author:        Jinesh Jayakumar, jineshkj@gmail.com            //
//                                                                 //
//  Jinesh Jayakumar (c) Copyright 2014                            //
//  All rights are granted to the user except the right            //
//  to publish, and requires the retention of this notice.         //
//                                                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Purpose:
===============
IWork is an interface defined for all work objects to be passed into
a worker pool.

Maintenance History:
====================
ver 1.0 : 14 Mar 14
- first release
*/

class IWork
{
public:
  virtual ~IWork() { }
};

#endif // OOD_IWORK_H
