
#include <iostream>

#include "FileSystem.h"

#include "FileManager.h"

using namespace std;
using namespace FileSystem;

void FileManager::search(const FileManager::FilePatterns& patterns)
{
	cout << "Scanning directories: " << endl;
	FileManager::search(Path::getFullFileSpec(_root), patterns);

	cout << "Found " << _repo.size() << " files:" << endl;
	for (auto &file : _repo)
		cout << file << endl;
}

void FileManager::search(const FilePath& rootdir, const FileManager::FilePatterns& patterns)
{
	cout << rootdir << endl;

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
