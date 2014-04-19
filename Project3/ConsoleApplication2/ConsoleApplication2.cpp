
#include <iostream>
#include <cstdlib>

#include <NetJoin.h>

void main()
{
  NetJoin::Config cfg; // create the default config

  cfg.port = 50002; // using different port since both processes run on same machine

  try {
    NetJoin::Service svc(cfg);
    svc.Wait();
  }
  catch (std::exception e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    exit(1);
  }
}
