#ifndef CPPANALYZER_H
#define CPPANALYZER_H

#include <iomanip>

#include <map>
#include <stack>
#include <sstream>

#include "FileManager.h"

#include "Parser.h"
#include "Tokenizer.h"
#include "FoldingRules.h"
#include "SemiExpression.h"

#include "MNode.h"
#include "CPPRules.h"
#include "ScopeStack.h"

using ScopeNode = TMTree::MNode<std::string>;

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
  std::string type;
  std::string name;
  size_t startLine, endLine;
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
  ScopeStack<element> stack;
  Toker* p_Toker;
  std::map<std::string, ScopeNode*> _methodStats;
  std::stack<ScopeNode*> _methodStack;

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
  std::map<std::string, ScopeNode*>& methStats()
  {
    return _methodStats;
  }
  std::stack<ScopeNode*>& methStack()
  {
    return _methodStack;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class PushAnonymous : public IAction
{
  Repository* p_Repos;
public:
  PushAnonymous(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.startLine = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    ScopeNode *scope = new ScopeNode(elem.name);

    if (p_Repos->methStack().size() > 0)
    {
      p_Repos->methStack().top()->addChild(scope);
      p_Repos->methStack().push(scope);
    } else
    {
      std::cerr << "BUG: PushAnonymous called for a scope outside method" << std::endl;
    }      

    return ACT_SUCC_CONT;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushKeyword : public IAction
{
  Repository* p_Repos;
public:
  PushKeyword(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    // push function scope
    std::string name;

    size_t close_brace = pTc->find("(");

    element elem;
    elem.type = "keyword";
    elem.name = (*pTc)[close_brace-1];
    elem.startLine = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    ScopeNode *scope = new ScopeNode(elem.name);
    p_Repos->methStack().top()->addChild(scope);
    p_Repos->methStack().push(scope);

    return ACT_SUCC_STOP;
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
    elem.startLine = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    ScopeNode *topNode = NULL;
    while (p_Repos->methStack().size())
    {
      topNode = p_Repos->methStack().top();
      p_Repos->methStack().pop();
    }
    delete topNode;

    ScopeNode *funcNode = new ScopeNode(elem.name);
    p_Repos->methStats()[elem.name] = funcNode;
    p_Repos->methStack().push(funcNode);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push template onto ScopeStack

class PushTemplate : public IAction
{
  Repository* p_Repos;
public:
  PushTemplate(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;

    element elem;
    elem.type = "template"; // type = template

    size_t close_bracket = tc.rfind(">");
    if (close_bracket < tc.length())
      elem.name = tc[close_bracket + 2]; // name of the template. what about inhereted classes
    else
      elem.name = "unknown";

    elem.startLine = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push enclosure name onto ScopeStack

class PushEnclosure : public IAction
{
  Repository* p_Repos;
public:
  PushEnclosure(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;

    element elem;
    elem.type = tc[0]; // type = class | struct | union | namespace
    elem.name = tc[1]; // name of the class, etc.
    elem.startLine = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push enum onto ScopeStack

class PushEnum : public IAction
{
  Repository* p_Repos;
public:
  PushEnum(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;

    element elem;
    elem.type = "enum"; // type = enum
    elem.name = tc[tc.length() - 2]; // name of the enum
    elem.startLine = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class PopScope : public IAction
{
  Repository* p_Repos;
public:
  PopScope(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  ActionStatus doAction(ITokCollection*& pTc)
  {
    if (p_Repos->scopeStack().size() == 0)
      return ACT_FAIL_CONT;

    element elem = p_Repos->scopeStack().pop();
    elem.endLine = p_Repos->lineCount();

    if (p_Repos->methStack().size())
      p_Repos->methStack().pop();

    std::cout << std::setw(10) << elem.type << std::setw(5) << elem.startLine << " - " << std::setw(2) << elem.endLine << std::setw(15) << elem.name << std::endl;

    return ACT_SUCC_STOP;
  }
};

class CPPAnalyzer
{
public:
	using FilePath = FileManager::FilePath;

    CPPAnalyzer();
	//~CPPParser();

	ScopeNode* parse(const FilePath& file);

private:

	Toker _toker;
	SemiExp _semi;
	Parser _parser;
	Repository _repo;

	// add folding rules

	FoldingRules _foldingRules;

	// add Rules and Actions
    SpecialKeyword _rSpecialKeyword;
    PushKeyword _aPushKeyword;

    FunctionDefinition _rFunctionDefinition;
    PushFunction _aPushFunction;

    TemplateDefinition _rTemplateDefinition;
    PushTemplate _aPushTemplate;

    EnclosureDefinition _rEnclosureDefinition;
    PushEnclosure _aPushEnclosure;

    EnumDefinition _rEnumDefinition;
    PushEnum _aPushEnum;

    BeginningOfScope _rBeginningOfScope;
    PushAnonymous _aPush;

    EndOfScope _rEndOfScope;
    PopScope _aPop;

    int count(ScopeNode * node);
};

#endif // CPPANALYZER_H
