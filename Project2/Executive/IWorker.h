#ifndef OOD_IWORKER_H
#define OOD_IWORKER_H
/////////////////////////////////////////////////////////////////////
//  IWorker.h - IWorker interface specification header file        //
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

class IWorker
{
public:
  virtual IWorker * CreateWorker() const = 0;
  virtual void Exec(IWork *work) = 0;

  virtual ~IWorker() { }
};

#endif // OOD_IWORKER_H
