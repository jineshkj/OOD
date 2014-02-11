#ifndef CPPANALYZER_H
#define CPPANALYZER_H

#include <sstream>

#include "FileManager.h"

#include "Parser.h"
#include "Tokenizer.h"
#include "FoldingRules.h"
#include "SemiExpression.h"

#include "CPPRules.h"
#include "ScopeStack.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
  std::string type;
  std::string name;
  size_t lineCount;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")";
    return temp.str();
  }
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
  ScopeStack<element> stack;
  Toker* p_Toker;
public:
  Repository(Toker* pToker)
  {
    p_Toker = pToker;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->lines());
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    return ACT_SUCC_CONT;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    if (p_Repos->scopeStack().size() == 0)
      return ACT_FAIL_CONT;

    element elem = p_Repos->scopeStack().pop();
    if (elem.type == "function")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }

    return ACT_SUCC_CONT;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name;

    size_t close_brace = pTc->find("(");

    if ((close_brace >= 2) && (*pTc)[close_brace - 2] == "::")
    {
      if (close_brace >= 3)
        name += (*pTc)[close_brace - 3];
      name += "::";
    }

    if ((close_brace >= 2) && (*pTc)[close_brace - 2] == "~")
    {
      if ((close_brace >= 3) && (*pTc)[close_brace - 3] == "::")
      {
        if (close_brace >= 4)
          name += (*pTc)[close_brace - 4];
        name += "::";
      }

      name += "~";
    }

    name += (*pTc)[close_brace - 1];

    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    return ACT_SUCC_CONT;
  }
};

class CPPAnalyzer
{
public:
	using FilePath = FileManager::FilePath;

    CPPAnalyzer();
	//~CPPParser();

	void parse(const FilePath& file);

private:

	Toker _toker;
	SemiExp _semi;
	Parser _parser;
	Repository _repo;

	// add folding rules

	FoldingRules _foldingRules;

	// add Rules and Actions
    BeginningOfScope _rBeginningOfScope;
    HandlePush _aPush;
    EndOfScope _rEndOfScope;
    HandlePop _aPop;
    FunctionDefinition _rFunctionDefinition;
    PushFunction _aPushFunction;
};

#endif // CPPANALYZER_H
