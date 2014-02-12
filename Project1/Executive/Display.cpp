
#include "Display.h"

void Display::output(ScopeNode * node)
{
  XmlWriter wrt;

  wrt.indent(true);
  wrt.addDeclaration();

  ScopeToXml(wrt, node);

  _os << wrt.xml() << std::endl;
}

void Display::ScopeToXml(XmlWriter& wrt, ScopeNode * node)
{
  wrt.start(node->value());

  ScopeNode *child;
  while ((child = node->nextUnmarkedChild()) != NULL)
  {
    ScopeToXml(wrt, child);
  }

  wrt.end();
}
