//////////////////////////////////////////////////////////////////////////
//    CLIOptions.h - header file for CLIOptions class                   //
//    version 1.0                                                       //
//                                                                      //
//    Language     - C++                                                //
//    Platform     - Windows 7                                          //
//    Application  - CIS 687 OOD Project 1                              //
//    Author       - Jinesh Jayakumar <jkunnath@syr.edu>                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef CLIOPTIONS_H
#define CLIOPTIONS_H

#include <set>
#include <string>
#include <ostream>

class CLIOptions
{
public:
	using FilePath = std::string;
	using FilePattern = std::string;
	using FilePatterns = std::set<FilePattern>;

	CLIOptions() : 
		_recurse(false), _compact(false), _complete(false)
	{ }

	operator bool() const { return !_error.size() && _complete;  }

	const FilePath& path() const { return _path; }
	const FilePatterns& patterns() const { return _patterns; }
	bool recursive() const { return _recurse; }
	bool compact() const { return _compact; }

	bool ProcessSwitch(const std::string& sw);
	void SetPath(const FilePath& path);
	void AddPattern(const FilePattern& pattern);

private:
	FilePath _path;
	FilePatterns _patterns;

	bool _recurse;
	bool _compact;

	bool _complete; /* set to true if all mandatory options have been detected */
	std::string _error; /* set to an error string if any error is found in processing */

	friend std::ostream& operator<<(std::ostream& os, CLIOptions& options);
};

std::ostream& operator<<(std::ostream& os, CLIOptions& options);

#endif // CLIOPTIONS_H
