
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <set>
#include <string>

class FileManager
{
public:
	using FilePath = std::string;
	using FilePattern = std::string;
	using FilePatterns = std::set<FilePattern>;
	using FileRepository = std::set<FilePath>;

	FileManager(const FilePath& root, bool recursive) : 
		_root(root), _recursive(recursive)
	{ }

	const FileRepository& repo() const { return _repo; }

	void search(const FilePatterns& patterns);

private:
	FilePath _root;
	bool _recursive;

	FileRepository _repo;

	void search(const FilePath& root, const FilePatterns& patterns);
};

#endif // FILEMANAGER_H
