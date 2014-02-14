#ifndef FILEMANAGER_H
#define FILEMANAGER_H

//////////////////////////////////////////////////////////////////////////
//    FileManager.h - header file for FileManager class                   //
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
This module provides the FileManager class for the Scope Analyzer
application.

Public Interface:
=================
FileManager(const FilePath& root, bool recursive, std::ostream& disp);
const FileRepository& repo();
void search(const FilePatterns& patterns);

Required Files:
===============
Display.h

Build Command:
==============
cl /EHa Main.cpp

Maintenance History:
====================
ver 1.0 : 12 Feb 2014
- first release

*/

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
