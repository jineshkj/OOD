//////////////////////////////////////////////////////////////////////////
//    CPPRule.cpp - implementation of CPPRules class                    //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//    Source       - Jim Fawcett, CST 4-187, Syracuse University        //
//                   (315) 443-3948, jfawcett@twcny.rr.com              //
//////////////////////////////////////////////////////////////////////////

#include "CPPRules.h"

const char CPPRule::ExprTypeChar[] = {
  '.', // EXPR_UNKNOWN = 0,
  '#', // EXPR_PREPROC,
  '{', // EXPR_SCOPE_OPEN,
  'N', // EXPR_NAMESPACE,
  'C', // EXPR_CLASS,
  'S', // EXPR_STRUCT,
  'U', // EXPR_UNION,
  'T', // EXPR_TEMPLATE,
  'E', // EXPR_ENUM,
  'F', // EXPR_FUNCTION,
  'i', // EXPR_IF,
  'e', // EXPR_ELSE,
  'w', // EXPR_WHILE,
  'd', // EXPR_DO,
  's', // EXPR_SWITCH,
  'f', // EXPR_FOR,
  't', // EXPR_TRY,
  'c', // EXPR_CATCH,
  '}', // EXPR_SCOPE_CLOSE,
  '?', // EXPR_MISC,
  '*', // EXPR_MAX
};
