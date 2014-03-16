
#include "ScopeAnalyzer.h"

size_t SizedScopeList::_minLines = 1;

SizedScopeList ScopeAnalyzer::_exprList[CPPRule::EXPR_MAX];

//----< work execution routine of Scope Analyzer worker >------------

void ScopeAnalyzer::Exec(IWork * work)
{
  ParseFile *info = static_cast<ParseFile*>(work);

  for (auto& scope : info->scopes)
  {
    SizedScopeList& sl = _exprList[scope->type];
    sl.AddScope(scope);
  }  
}
