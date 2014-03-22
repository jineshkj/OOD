#ifndef CPPRULES_H
#define CPPRULES_H

//////////////////////////////////////////////////////////////////////////
//    CPPRules.h - header file for various specializations of IRule     //
//                 specific for CPP language                            //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//    Source       - Jim Fawcett, CST 4-187, Syracuse University        //
//                   (315) 443-3948, jfawcett@twcny.rr.com              //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_CPPANALYZER parser.cpp CPPAnalyzer.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 1.0 : 12 Feb 2014
- first release

*/

#include <string>

#include "Parser.h"
#include "ITokCollection.h"

///////////////////////////////////////////////////////////////
// A specialization of IRule for C++ files

class CPPRule : public IRule
{
  static const char ExprTypeChar[];

public:
  enum ExprType { // Tightly linked to ExprTypeChar
    EXPR_UNKNOWN = 0,
    EXPR_PREPROC,
    EXPR_SCOPE_OPEN,
    EXPR_NAMESPACE,
    EXPR_CLASS,
    EXPR_STRUCT,
    EXPR_UNION,
    EXPR_TEMPLATE,
    EXPR_ENUM,
    EXPR_FUNCTION,
    EXPR_IF,
    EXPR_ELSE,
    EXPR_WHILE,
    EXPR_DO,
    EXPR_SWITCH,
    EXPR_FOR,
    EXPR_TRY,
    EXPR_CATCH,
    EXPR_SCOPE_CLOSE,
    EXPR_MISC,
    EXPR_MAX
  };

  static char getTypeChar(ExprType type) { return ExprTypeChar[type]; }
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public CPPRule
{
public:
  void * doTest(ITokCollection& tc)
  {
    size_t pos = tc.find("{");
    return (void*) ((pos < tc.length()) ? EXPR_SCOPE_OPEN : EXPR_UNKNOWN);
  }

};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public CPPRule
{
public:
  void * doTest(ITokCollection& tc)
  {
    size_t pos = tc.find("}");
    return (void*)((pos < tc.length()) ? EXPR_SCOPE_CLOSE : EXPR_UNKNOWN);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public CPPRule
{
public:
  void *doTest(ITokCollection& tc)
  {
    size_t pos = tc.find("#");
    return (void*)((pos < tc.length()) ? EXPR_PREPROC : EXPR_UNKNOWN);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect semicolon/braceless statements

class SemiColon : public CPPRule
{
public:
  void *doTest(ITokCollection& tc)
  {
    size_t pos = tc.find(";");
    return (void*)((pos < tc.length()) ? EXPR_MISC : EXPR_UNKNOWN);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect template

class TemplateDefinition : public CPPRule
{
public:
  void * doTest(ITokCollection& tc)
  {
    if ((tc[tc.length() - 1] == "{") && (tc[0] == "template"))
      return (void*) EXPR_SCOPE_CLOSE;
    
    return (void*) EXPR_UNKNOWN;
  }
};


///////////////////////////////////////////////////////////////
// rule to detect enclosure definitions - namespace, class, struct, union

class EnclosureDefinition : public CPPRule
{
public:
  void * isEnclosure(const std::string& tok)
  {
    const static std::string keys[]
      = { "namespace", "class", "struct", "union" };
    const static ExprType types[]
      = { EXPR_NAMESPACE, EXPR_CLASS, EXPR_STRUCT, EXPR_UNION };

    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return (void*) types[i];
    }
    return NULL;
  }

  void * doTest(ITokCollection& tc)
  {
    if (tc[tc.length() - 1] == "{")
    {
      return isEnclosure(tc[0]);
    }

    return NULL;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect enum

class EnumDefinition : public CPPRule
{
public:
  void * doTest(ITokCollection& tc)
  {
    if ((tc[tc.length() - 1] == "{") && tc.length() >= 3 &&
      (tc[tc.length() - 3] == "enum")) {
      return (void*) EXPR_ENUM;
    }

    return NULL;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class SpecialKeyword : public CPPRule
{
public:
  void * isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "else", "try", "catch", "do" };
    const static ExprType types[]
      = { EXPR_FOR, EXPR_WHILE, EXPR_SWITCH, EXPR_IF, EXPR_ELSE, EXPR_TRY, EXPR_CATCH, EXPR_DO };

    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return (void*) types[i];
    }
    return NULL;
  }

  void * doTest(ITokCollection& tc)
  {
    if (tc[tc.length() - 1] == "{")
    {
      return isSpecialKeyWord(tc[0]);
      //size_t len = tc.find("(");
      //if (len < tc.length())
      //  return isSpecialKeyWord(tc[len - 1]);
    }
    return NULL;
  }
};


///////////////////////////////////////////////////////////////
// rule to detect special keywords like for, while, switch, if, try or catch

class FunctionDefinition : public CPPRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };

    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return true;
    }
    return false;
  }

  void * doTest(ITokCollection& tc)
  {
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
      {
        return (void*) EXPR_FUNCTION;
      }
    }
    return NULL;
  }
};

#endif // CPPRULES_H
