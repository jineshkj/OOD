
#include <iostream>
#include <cstdlib>

#include <Netjoin.h>

static void usage()
{
  std::cout << "Usage: prog <filename>" << std::endl << std::endl;
}

void main(int argc, char *argv[])
{
  if (argc != 2)
  {
    usage();
    return;
  }

  std::string file = argv[1];

  NetJoin::Config cfg; // create the default config
  cfg.port = 50001; // using different port since both processes run on same machine

  NetJoin::FileTransfer::PutFile cmd(file);

  try {
    NetJoin::Service::Execute("localhost", 50002, cmd);
  } catch (std::exception e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    exit(1);
  }

  if (cmd.status() == NetJoin::Command::SUCCEEDED)
    std::cout << "File transfer succeeded" << std::endl;
  else
    std::cout << "File transfer failed" << std::endl;
}
