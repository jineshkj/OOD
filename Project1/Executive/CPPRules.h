#ifndef CPPRULES_H
#define CPPRULES_H

#include <string>

#include "ITokCollection.h"

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    // std::cout << "Test Beg" << std::endl;
    //std::cout << "\n--BeginningOfScope rule";
    return (pTc->find("{") < pTc->length());
  }

};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    // std::cout << "Test End" << std::endl;
    //std::cout << "\n--EndOfScope rule";
    return (pTc->find("}") < pTc->length());
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--PreprocStatement rule";
    if (pTc->find("#") < pTc->length())
    {
      return true;
    }
    return false;
  }
};


///////////////////////////////////////////////////////////////
// rule to detect template

class TemplateDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    // std::cout << "Test Enum" << std::endl;
    ITokCollection& tc = *pTc;
    return (tc[tc.length() - 1] == "{") && (tc[0] == "template");
  }
};


///////////////////////////////////////////////////////////////
// rule to detect enclosure definitions - namespace, class, struct, union

class EnclosureDefinition : public IRule
{
public:
  bool isEnclosure(const std::string& tok)
  {
    // std::cout << "isEnc : " << tok << std::endl;

    const static std::string keys[]
      = { "namespace", "class", "struct", "union" };
    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return true;
    }
    return false;
  }

  bool doTest(ITokCollection*& pTc)
  {
    // std::cout << "Test Enc" << std::endl;
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      return isEnclosure(tc[0]);
    }

    return false;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect enum

class EnumDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    // std::cout << "Test Enum" << std::endl;
    ITokCollection& tc = *pTc;
    return (tc[tc.length() - 1] == "{") && tc.length() >= 3 && 
           ( tc[tc.length() - 3] == "enum") ;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class SpecialKeyword : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "try" "catch" };

    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return true;
    }
    return false;
  }

  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "Test Kw" << std::endl;
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length())
        return isSpecialKeyWord(tc[len - 1]);
    }
    return false;
  }
};


///////////////////////////////////////////////////////////////
// rule to detect special keywords like for, while, switch, if, try or catch

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };

    for (int i = 0; i < (sizeof(keys) / sizeof(keys[0])); ++i)
    {
      if (tok == keys[i])
        return true;
    }
    return false;
  }

  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "Test Fun" << std::endl;
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
      {
        //std::cout << "\n--FunctionDefinition rule";
        return true;
      }
    }
    return false;
  }
};

#endif // CPPRULES_H
