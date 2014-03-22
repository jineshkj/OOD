#ifndef OOD_SCOPEANALYZER_H
#define OOD_SCOPEANALYZER_H

#include <map>
#include <vector>
#include <utility>

#include "Mutex.h"
#include "CPPParser.h"

///////////////////////////////////////////////////////////////
// class that implements list of scopes in grouped in their 
// sizez and also mathes between scopes

class SizedScopeList
{
public:
  using ScopeList = std::vector<ScopeInfo*>;
  using ScopeMatch = std::pair<ScopeInfo*,ScopeInfo*>;

  //----< set minimum expected lines for a scope to match >------------

  static void SetMinLines(int count)
  {
    _minLines = count;
  }

  //----< add a scope to the collection >------------

  void AddScope(ScopeInfo *scope)
  {
    if (scope->lines < _minLines)
      return;

    if (scope->type == CPPRule::EXPR_SCOPE_OPEN || scope->type == CPPRule::EXPR_SCOPE_CLOSE || scope->type == CPPRule::EXPR_UNKNOWN || scope->type >= CPPRule::EXPR_MAX)
      return;

    _lock.lock();
    ScopeList& sl = _scopes[scope->size];
    for (auto& match : sl)
    {
      // for all the scopes with same size and signature, we add a match
      if (scope->signature == match->signature)
      {
        // std::cout << *scope << std::endl << *match << std::endl << std::endl;
        _matches.push_back(ScopeMatch(scope, match));
      }
    }
    sl.push_back(scope);
    _lock.unlock();
  }

  //----< retrieve the list of matching scopes >------------

  const std::vector<ScopeMatch>& matches() const
  {
    return _matches;
  }

private:
  Mutex _lock;
  std::map<int,ScopeList> _scopes;
  std::vector<ScopeMatch> _matches;

  static size_t _minLines;
};

#include "IWorker.h"

///////////////////////////////////////////////////////////////
// class that implements the scope analyzing workers

class ScopeAnalyzer : public IWorker
{
public:
  //----< create new ScopeAnalyzer >------------

  IWorker * CreateWorker() const
  {
    return new ScopeAnalyzer;
  }

  //----< minimum line requirement for match >------------

  static void SetMinLines(int count)
  {
    SizedScopeList::SetMinLines(count);
  }

  void Exec(IWork *work);

  //----< retrieve scopelist of a particular type >------------

  static const SizedScopeList& GetSizedScopeList(int type)
  {
    return _exprList[type];
  }

private:
  static SizedScopeList _exprList[CPPRule::EXPR_MAX];
};

#endif // OOD_SCOPEANALYZER_H
