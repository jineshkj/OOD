/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                  //
//  ver 1.3                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jinesh Jayakumar                                //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "FoldingRules.h"

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  pFoldingRules->doFold(pTokColl);
  return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
	//std::cout << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV" << std::endl;
	//for (int i = 0; i < (*pTokColl).length(); i++)
	//{
	//	std::cout << (*pTokColl)[i] << " ";
	//}
	//std::cout << std::endl;
	//std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;

  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    // for (int j = 0; j < (*pTokColl).length(); j++)
    //  std::cout << '[' << (*pTokColl)[j] << ']';
    // std::cout << std::endl;

    void *arg;
    if ((arg = rules[i]->doTest(*pTokColl)) != NULL)
    {
      switch (rules[i]->doActions(*pTokColl, arg))
      {
      case IAction::ACT_SUCC_CONT:
        succeeded = true;
      case IAction::ACT_FAIL_CONT:
        break;

      case IAction::ACT_SUCC_STOP:
        return true;

      case IAction::ACT_FAIL_STOP:
        return false;
      }
    }
  }

#if 0
  if (succeeded == false) {
    std::cout << "FAILED: ";
    for (int i = 0 ; i < (*pTokColl).length() ; i++)
    {
      std::cout << (*pTokColl)[i] << " ";
    }
    std::cout << std::endl;
   }
#endif

  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

IAction::ActionStatus IRule::doActions(ITokCollection& tc, void *arg)
{
  IAction::ActionStatus status = IAction::ACT_SUCC_CONT;

  if (actions.size() > 0)
  {
    for (size_t i = 0; i < actions.size(); ++i)
    {
      status = actions[i]->doAction(tc, arg);

      if (status == IAction::ACT_FAIL_STOP || status == IAction::ACT_SUCC_STOP)
        break;
    }      
  }

  return status;
}

//----< test stub >--------------------------------------------

#ifdef TEST_PARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing Parser class\n "
            << std::string(22,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
