#ifndef CLIOPTIONS_H
#define CLIOPTIONS_H

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
/*
Module Operations:
==================
This module provides the CLIOptions class.

CLIOptions class supports storing and retrieving of different command line
options supported by the Source Analyzer application.

Public Interface:
=================

CLIOptions();
operator bool() const;
const FilePath& path() const;
const FilePatterns& patterns() const;
bool recursive() const;
bool compact() const;

bool ProcessSwitch(const std::string& sw);
void SetPath(const FilePath& path);
void AddPattern(const FilePattern& pattern);

Required Files:
===============
FileManager.h

Build Command:
==============
cl /EHa /DTEST_CLIOPTIONS CLIOptions.cpp

Maintenance History:
====================
ver 1.0 : 12 Feb 2014
 - first release

*/

#include <string>
#include <ostream>

#include "FileManager.h"

class CLIOptions
{
public:
    static const int DEFAULT_MIN_LINES = 5;

	using FilePath = FileManager::FilePath;
	using FilePattern = FileManager::FilePattern;
	using FilePatterns = FileManager::FilePatterns;

    //----< CLIOptions constructor >------------

    CLIOptions() :
      _recurse(false), _dispdiff(false), _minLines(DEFAULT_MIN_LINES)
	{ }

    //----< checks validity a CLIOptions object >------------

    operator bool() const { return !_error.size() && _complete; }

	const FilePath& path() const { return _root; }
	const FilePatterns& patterns() const { return _patterns; }
	bool recursive() const { return _recurse; }
	bool showdiff() const { return _dispdiff; }
    int minLines() const { return _minLines; }

	bool ProcessSwitch(const std::string& sw);
	void SetPath(const FilePath& path);
	void AddPattern(const FilePattern& pattern);

private:
	FilePath _root;
	FilePatterns _patterns;

	bool _recurse;
	bool _dispdiff;
    int _minLines;

	bool _complete; /* set to true if all mandatory options have been detected */
	std::string _error; /* set to an error string if any error is found in processing */

	friend std::ostream& operator<<(std::ostream& os, CLIOptions& options);
};

std::ostream& operator<<(std::ostream& os, CLIOptions& options);

#endif // CLIOPTIONS_H
