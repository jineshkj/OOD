
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <ostream>

#include <set>
#include <vector>
#include <string>

class FileManager
{
public:
	using FilePath = std::string;
	using FilePattern = std::string;
	using FilePatterns = std::set<FilePattern>;
	using FileRepository = std::vector<FilePath>;

	FileManager(const FilePath& root, bool recursive, std::ostream& disp) : 
      _root(root), _recursive(recursive), _disp(disp)
	{ }

	const FileRepository& repo() const { return _repo; }

	void search(const FilePatterns& patterns);

private:
	FilePath _root;
	bool _recursive;
    std::ostream& _disp;

	FileRepository _repo;

	void search(const FilePath& root, const FilePatterns& patterns);
};

#endif // FILEMANAGER_H
