
#include "CPPAnalyzer.h"

CPPAnalyzer::CPPAnalyzer() : _semi(&_toker), _parser(&_semi), _repo(&_toker),
	_aPush(&_repo), _aPop(&_repo), _aPushFunction(&_repo)
{
	//_toker.returnComments();
	//_semi.verbose(true);

	// add code folding rules
	_parser.addFoldingRules(&_foldingRules);

	// configure to manage scope
	// these must come first - they return true on match
	// so rule checking continues

	_rBeginningOfScope.addAction(&_aPush);
	_parser.addRule(&_rBeginningOfScope);

	_rEndOfScope.addAction(&_aPop);
	_parser.addRule(&_rEndOfScope);

	// configure to detect and act on function definitions
	// these will stop further rule checking by returning false

	_rFunctionDefinition.addAction(&_aPushFunction);
	_parser.addRule(&_rFunctionDefinition);
}

void CPPAnalyzer::parse(const FilePath& file)
{
	_toker.attach(file, true);

	while (_parser.next())
		_parser.parse();
}
