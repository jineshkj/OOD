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

#include "ITokCollection.h"

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    return (pTc->find("{") < pTc->length());
  }

};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    return (pTc->find("}") < pTc->length());
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if (pTc->find("#") < pTc->length())
    {
      return true;
    }
    return false;
  }
};


///////////////////////////////////////////////////////////////
// rule to detect template

class TemplateDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    return (tc[tc.length() - 1] == "{") && (tc[0] == "template");
  }
};


///////////////////////////////////////////////////////////////
// rule to detect enclosure definitions - namespace, class, struct, union

class EnclosureDefinition : public IRule
{
public:
  bool isEnclosure(const std::string& tok)
  {
    const static std::string keys[]
      = { "namespace", "class", "struct", "union" };
    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return true;
    }
    return false;
  }

  bool doTest(ITokCollection*& pTc)
  {
    // std::cout << "Test Enc" << std::endl;
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      return isEnclosure(tc[0]);
    }

    return false;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect enum

class EnumDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    return (tc[tc.length() - 1] == "{") && tc.length() >= 3 && 
           ( tc[tc.length() - 3] == "enum") ;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class SpecialKeyword : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "try" "catch" };

    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return true;
    }
    return false;
  }

  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length())
        return isSpecialKeyWord(tc[len - 1]);
    }
    return false;
  }
};


///////////////////////////////////////////////////////////////
// rule to detect special keywords like for, while, switch, if, try or catch

class FunctionDefinition : public IRule
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

  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
      {
        return true;
      }
    }
    return false;
  }
};

#endif // CPPRULES_H
