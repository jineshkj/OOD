
#include "FileSystem.h"

#include "FileManager.h"

using namespace FileSystem;

void FileManager::search(const FileManager::FilePatterns& patterns)
{
  _disp << "Scanning directories: " << std::endl;

  FileManager::search(Path::getFullFileSpec(_root), patterns);

  _disp << "Found " << _repo.size() << " files:" << std::endl;
  for (auto &file : _repo)
    _disp << file << std::endl;
}

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
