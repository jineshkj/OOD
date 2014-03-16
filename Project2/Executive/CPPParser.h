#ifndef CPPPARSER_H
#define CPPPARSER_H
//////////////////////////////////////////////////////////////////////////
//    CPPParser.h - header file for CPPParser class  and actions        //
//                    for various CPPRules                              //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stack>
#include <vector>
#include <ostream>

#include "Parser.h"
#include "Tokenizer.h"
#include "FoldingRules.h"
#include "SemiExpression.h"

#include "CPPRules.h"
#include "ScopeStack.h"

#include "IWork.h"
#include "IWorker.h"

#include "SlabAllocator.h"

struct ScopeInfo
{
  ScopeInfo(int t = CPPRule::EXPR_UNKNOWN, ScopeInfo* paren = NULL) : 
      type(t), parent(paren), start(0), size(0), file(NULL)
  {
  }

  int type;
  size_t start, size;
  const std::string* file;

  ScopeInfo *parent;
};

inline std::ostream& operator<<(std::ostream& os, const ScopeInfo& si)
{
  return (os << "Type: " << si.type << " Start: " << si.start << " Size: " << si.size << " Parent: " << (si.parent ? si.parent->start : 0));
}

struct ParseFile : public IWork
{
  const std::string *file;
  bool success;

  std::vector<ScopeInfo*> scopes;

#if 0
  // Each map is indexed with a scope size. When indexed with a scopp
  // size, we get back a vector of all scopes with that size.
  using ScopeList = std::vector<ScopeInfo*>;
  using SizedScopeLists = std::map<size_t, ScopeList>;
  // We create a map for each type of expression we could possibly
  // find in any source file.
  SizedScopeLists exprList[CPPRule::EXPR_MAX];
  void AddScope(ScopeInfo* scope)
  {
    ParseFile::SizedScopeLists& ssl = exprList[scope->type];
    ParseFile::ScopeList& sl = ssl[scope->size];
    sl.push_back(scope);
  }
#endif
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
public:
  Repository(Toker& toker, SlabAllocator<ScopeInfo>& alloc) : 
    _toker(toker), _siAllocator(alloc), _work(NULL)
  {    
  }

  friend class PushAnonymous;
  friend class PushKeyword;
  friend class PushFunction;
  friend class PushTemplate;
  friend class PushEnclosure;
  friend class PushEnum;
  friend class PopScope;
  friend class CPPParser;

  void reset(ParseFile *work)
  {
    _work = work;

    while (!_scopes.empty()) // this should ideally not be required
      _scopes.pop();
  }

private:
  Toker& _toker;
  SlabAllocator<ScopeInfo>& _siAllocator;
  std::stack<ScopeInfo*> _scopes;

  ParseFile *_work;
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushKeyword : public IAction
{
  Repository& _repo;
public:
  PushKeyword(Repository& repo) : _repo(repo)
  {
  }
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    // ITokCollection& tc = *pTc;
    ScopeInfo * scope = _repo._siAllocator.alloc();

    scope->file = _repo._work->file;
    scope->type = (int) arg;
    scope->parent = _repo._scopes.empty() ? NULL : _repo._scopes.top();
    scope->start = _repo._toker.lines(); // store starting line number now

    _repo._scopes.push(scope);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository& _repo;
public:
  PushFunction(Repository& repo) : _repo(repo)
  {
  }

  // finding function name is a bit tricky since the function name to be
  // proper, we also need to include the class name (if present). we try
  // to keep function names as accurate as possible.
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    // ITokCollection& tc = *pTc;
    ScopeInfo * scope = _repo._siAllocator.alloc();

    scope->file = _repo._work->file;
    scope->type = (int)arg;
    scope->parent = _repo._scopes.empty() ? NULL : _repo._scopes.top();
    scope->start = _repo._toker.lines(); // store starting line number now

    _repo._scopes.push(scope);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push template onto ScopeStack

class PushTemplate : public IAction
{
  Repository& _repo;
public:
  PushTemplate(Repository& repo) : _repo(repo)
  {
  }
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    // ITokCollection& tc = *pTc;
    ScopeInfo * scope = _repo._siAllocator.alloc();

    scope->file = _repo._work->file;
    scope->type = (int)arg;
    scope->parent = _repo._scopes.empty() ? NULL : _repo._scopes.top();
    scope->start = _repo._toker.lines(); // store starting line number now

    _repo._scopes.push(scope);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push enclosure name onto ScopeStack

class PushEnclosure : public IAction
{
  Repository& _repo;
public:
  PushEnclosure(Repository& repo) : _repo(repo)
  {
  }
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    // ITokCollection& tc = *pTc;
    ScopeInfo * scope = _repo._siAllocator.alloc();

    scope->file = _repo._work->file;
    scope->type = (int)arg;
    scope->parent = _repo._scopes.empty() ? NULL : _repo._scopes.top();
    scope->start = _repo._toker.lines(); // store starting line number now

    _repo._scopes.push(scope);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to push enum onto ScopeStack

class PushEnum : public IAction
{
  Repository& _repo;
public:
  PushEnum(Repository& repo) : _repo(repo)
  {
  }
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    // ITokCollection& tc = *pTc;
    ScopeInfo * scope = _repo._siAllocator.alloc();

    scope->file = _repo._work->file;
    scope->type = (int)arg;
    scope->parent = _repo._scopes.empty() ? NULL : _repo._scopes.top();
    scope->start = _repo._toker.lines(); // store starting line number now

    _repo._scopes.push(scope);

    return ACT_SUCC_STOP;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class PushAnonymous : public IAction
{
  Repository& _repo;

public:
  PushAnonymous(Repository& repo) : _repo(repo)
  {
  }
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    // ITokCollection& tc = *pTc;
    ScopeInfo * scope = _repo._siAllocator.alloc();

    scope->file = _repo._work->file;
    scope->type = (int)arg;
    scope->parent = _repo._scopes.empty() ? NULL : _repo._scopes.top();
    scope->start = _repo._toker.lines(); // store starting line number now

    _repo._scopes.push(scope);

    return ACT_SUCC_CONT;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class PopScope : public IAction
{
  Repository& _repo;

public:
  PopScope(Repository& repo) : _repo(repo)
  {
  }

  // when we pop out scopes, we add it to the element list contained in the
  // repository so that the caller of CPPAnalyzer::parse() can look at the
  // elements.
  ActionStatus doAction(ITokCollection& tc, void *arg)
  {
    if (_repo._scopes.empty())
    {
      // NOTE: Error in parsing
      // TODO: Find a graceful way to handle this
      return ACT_SUCC_CONT;
    }

    ScopeInfo * scope = _repo._scopes.top();

    _repo._scopes.pop();
    scope->size = _repo._toker.lines() - scope->start + 1;
    _repo._work->scopes.push_back(scope);

#ifdef TEST_CPPPARSER
    std::cout << *scope << std::endl;
#endif

    return ACT_SUCC_STOP;
  }
};

class CPPParser : public IWorker
{
public:
  CPPParser();
    
  CPPParser * CreateWorker() const
  {
    return new CPPParser();
  }

  void Exec(IWork* work);

private:
  Toker _toker;
  SemiExp _semi;
  Parser _parser;

  SlabAllocator<ScopeInfo> _siAllocator;

  Repository _repo;

  // add folding rules
  codeFoldingRules _foldingRules;

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
};

#endif // CPPPARSER_H
