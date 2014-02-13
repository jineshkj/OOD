
#include "CPPAnalyzer.h"

int PopScope::count(ScopeNode * node)
{
  int c = 1;
  ScopeNode *child;

  while ((child = node->nextUnmarkedChild()) != NULL)
    c += count(child);

  node->clearMarks();

  return c;
}

CPPAnalyzer::CPPAnalyzer() : _semi(&_toker), _parser(&_semi), _repo(&_toker),
_aPushKeyword(&_repo), _aPushFunction(&_repo), _aPushTemplate(&_repo), _aPushEnclosure(&_repo), _aPushEnum(&_repo), _aPush(&_repo), _aPop(&_repo)
{
	//_toker.returnComments();
	//_semi.verbose(true);
  _semi.returnNewLines(false);

	// add code folding rules
  _parser.addFoldingRules(&_foldingRules);

	// configure to manage scope
	// these must come first - they return true on match
	// so rule checking continues

  _rSpecialKeyword.addAction(&_aPushKeyword);
  _parser.addRule(&_rSpecialKeyword);

  _rFunctionDefinition.addAction(&_aPushFunction);
  _parser.addRule(&_rFunctionDefinition);

  _rTemplateDefinition.addAction(&_aPushTemplate);
  _parser.addRule(&_rTemplateDefinition);

  _rEnclosureDefinition.addAction(&_aPushEnclosure);
  _parser.addRule(&_rEnclosureDefinition);

  _rEnumDefinition.addAction(&_aPushEnum);
  _parser.addRule(&_rEnumDefinition);

  _rBeginningOfScope.addAction(&_aPush);
  _parser.addRule(&_rBeginningOfScope);

  _rEndOfScope.addAction(&_aPop);
  _parser.addRule(&_rEndOfScope);
}

ElementList& CPPAnalyzer::parse(const FilePath& file)
{
  _repo.clear();

  _toker.attach(file, true);

  while (_parser.next())
	_parser.parse();
  
  return _repo.elements();
}
