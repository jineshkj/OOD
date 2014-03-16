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

#include "CPPParser.h"

//----< CPPAnalyzer constructor >------------

CPPParser::CPPParser() : _semi(&_toker), _parser(&_semi), _repo(_toker, _siAllocator),
_aPushKeyword(_repo), _aPushFunction(_repo), _aPushTemplate(_repo), _aPushEnclosure(_repo), _aPushEnum(_repo), _aPush(_repo), _aPop(_repo), _aMeasureSize(_repo)
{
  // we don't need to parse comments or look at newline tokens
  //_toker.returnComments();
  //_semi.verbose(true);
  _semi.returnNewLines(false);

  // add code folding rules
  _parser.addFoldingRules(&_foldingRules);

  // configure to manage scope
  _rPreProc.addAction(&_aDiscard);
  _parser.addRule(&_rPreProc);

  _rSemiColon.addAction(&_aMeasureSize);
  _parser.addRule(&_rSemiColon);

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

void CPPParser::Exec(IWork * work)
{
  // NOTE: Can use dynamic cast to be more safe
  ParseFile *info = static_cast<ParseFile*>(work);

  _repo.reset(info);

  if (_toker.attach(*(info->file), true) == false)
  {
    info->success = false;
    return;
  }

  while (_parser.next()) // parse individual tokens
    if (_parser.parse() == false)
      std::cout << "FAILED" << std::endl;

  info->success = true;
}

//----< test stub >--------------------------------------------

#ifdef TEST_CPPPARSER
#include <iostream>

#include "WorkerPool.h"

void main()
{
  CPPParser sample;
  WorkerPool pool(&sample);

  ParseFile parse;
  parse.file = new std::string("..\\Executive\\CPPParser.cpp");

  IWork * w = pool.exec(&parse);

  if (!w)
    w = pool.wait();

  if (w)
  {
    ParseFile * comp = static_cast<ParseFile*>(w);

    std::cout << "success : " << comp->success << std::endl;
  }

}

#endif
