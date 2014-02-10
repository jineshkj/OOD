
#include "CLIOptions.h"

using namespace std;

ostream& operator<<(ostream& os, CLIOptions& options)
{
	if (options._path.size() == 0)
	{
		os << "Path missing" << endl;
	}
	else
	{
		os << "Path      : " << options._path << endl;
	}

	if (options._patterns.size() == 0)
	{
		os << "Patterns missing" << endl;
	}
	else
	{
		os << "Patterns  : ";
		for (auto &pat : options._patterns)
		{
			os << '[' << pat << ']' << " ";
		}

		os << endl;
	}

	os << "Recursive : " << (options._recurse ? "yes" : "no") << endl;
	os << "Compact   : " << (options._compact ? "yes" : "no") << endl;

	if (options._error.size() > 0)
	{
		os << "Error: " << options._error << endl;
	}

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
	_path = path;
}

void CLIOptions::AddPattern(const FilePattern& pattern)
{
	_patterns.insert(_patterns.begin(), pattern);
}
