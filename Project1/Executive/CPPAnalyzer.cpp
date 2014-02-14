//////////////////////////////////////////////////////////////////////////
//    CPPAnalyzer.cpp - header file for CPPAnalyzer class                 //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "CPPAnalyzer.h"

//----< count number of nodes in a ScopeNode >------------

int PopScope::count(ScopeNode * node)
{
  int c = 1;
  ScopeNode *child;

  while ((child = node->nextUnmarkedChild()) != NULL)
    c += count(child);

  node->clearMarks();

  return c;
}

//----< CPPAnalyzer constructor >------------

CPPAnalyzer::CPPAnalyzer(ElementList& elements) : _semi(&_toker), _parser(&_semi), _repo(&_toker, elements),
_aPushKeyword(&_repo), _aPushFunction(&_repo), _aPushTemplate(&_repo), _aPushEnclosure(&_repo), _aPushEnum(&_repo), _aPush(&_repo), _aPop(&_repo)
{
  // we don't need to parse comments or look at newline tokens
  //_toker.returnComments();
  //_semi.verbose(true);
  _semi.returnNewLines(false);

  // add code folding rules
  _parser.addFoldingRules(&_foldingRules);

  // configure to manage scope

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

//----< parse a given CPP source or header file >------------

size_t CPPAnalyzer::parse(const FilePath& file)
{
  // clear repository so that the previous file's element list
  // gets empty
  _repo.clear();

  _toker.attach(file, true);

  while (_parser.next()) // parse individual tokens
	_parser.parse();
  
  return _repo.elements().size();
}

//----< test stub >--------------------------------------------

#ifdef TEST_CPPANALYZER
#include <iostream>

#include "Display.h"
#include "CLIParser.h"

int main(int argc, char *argv[])
{
  Display disp(std::cout);

  ElementList elements;
  CPPAnalyzer analyzer(elements);

  analyzer.parse(file);
  disp.output(elements);

  return 0;
}
#endif
