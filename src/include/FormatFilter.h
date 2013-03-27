#ifndef __FORMATFILTER__H
#define __FORMATFILTER__H

#include <set>
#include <string>
using std::set;
using std::string;

class FormatFilter{
public:
	static bool isSupportFormat(const std::string& extension);

private:
	FormatFilter();
	static void init();
	static set<string> supportedFormats;
};
#endif
