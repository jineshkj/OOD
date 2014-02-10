
#include "CLIOptions.h"

using namespace std;

ostream& operator<<(ostream& os, CLIOptions& options)
{
	os << "CLI Options" << endl;
	os << "===========" << endl;
	os << endl;

	os << "Root      : " << (options._root.size() ? options._root : "<MISSING>") << endl;

	os << "Patterns  : ";
	if (options._patterns.size() == 0)
	{
		os << "<MISSING>";
	}
	else
	{
		for (auto &pat : options._patterns)
			os << '[' << pat << ']' << " ";
	}
	os << endl;

	os << "Recursive : " << (options._recurse ? "yes" : "no") << endl;
	os << "Compact   : " << (options._compact ? "yes" : "no") << endl;
	os << endl;

	if (options._error.size() > 0)
		os << "Error: " << options._error << endl;

	return os;
}

bool CLIOptions::ProcessSwitch(const string& sw)
{
	if (sw == "s")
	{
		_recurse = true;
	}
	else if (sw == "b")
	{
		_compact = true;
	}
	else
	{
		_error += "Unknown option: ";
		_error += sw;
		_error += "\n";

		return false;
	}

	return true;
}

void CLIOptions::SetPath(const FilePath& path)
{
	_root = path;
}

void CLIOptions::AddPattern(const FilePattern& pattern)
{
	_patterns.insert(_patterns.end(), pattern);

	if (!_complete && _root.size())
		_complete = true;
}
