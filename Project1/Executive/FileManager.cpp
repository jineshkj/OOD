//////////////////////////////////////////////////////////////////////////
//    FileManager.cpp - source file for FileManager class               //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "FileSystem.h"

#include "FileManager.h"

using namespace FileSystem;

//----< search for all files matching a pattern under configured root dir >------------

void FileManager::search(const FileManager::FilePatterns& patterns)
{
  _disp << "Scanning directories: " << std::endl;

  FileManager::search(Path::getFullFileSpec(_root), patterns);

  _disp << "Found " << _repo.size() << " files:" << std::endl;

  for (auto &file : _repo)
    _disp << file << std::endl;
}

//----< search for all files matching patterns under a configured root dir >------------

void FileManager::search(const FilePath& rootdir, const FileManager::FilePatterns& patterns)
{
  _disp << rootdir << std::endl;

  for (auto &pattern : patterns)
  {
	std::vector<std::string> files = Directory::getFiles(rootdir, pattern);

	for (auto &file : files)
	  _repo.push_back(rootdir + "\\" + file);
  }

  if (!_recursive)
	return;

  std::vector<std::string> subdirs = Directory::getDirectories(rootdir, "*");

  for (auto &subdir : subdirs)
  {
	/* do not try recursing into . and .. directories */
	if (subdir == "." || subdir == "..")
	  continue;

	FileManager::search(rootdir + "\\" + subdir, patterns);
  }
}

//----< test stub >--------------------------------------------

#ifdef TEST_FILEMANAGER
#include <iostream>

int main(int argc, char *argv[])
{
  bool recursive = true;

  FileManager::FilePatterns patts{ "*.cpp", "*.h" };

  FileManager manager("..", recursive, std::cout);
  manager.search(patts);

  for (auto &file : manager.repo())
  {
    std::cout << file << std::endl;
  }

  return 0;
}
#endif
