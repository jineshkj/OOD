#ifndef EXECUTIVE_H
#define EXECUTIVE_H

#include <iostream>

#include "Display.h"

class Executive
{
public:
  Executive(int argc, char *argv[]);

  int run();

private:
  int _argc;
  char **_argv;
  Display _disp;
};

#endif // EXECUTIVE_H
