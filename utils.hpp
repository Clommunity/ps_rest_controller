#ifndef __UTILS__HPP__
#define __UTILS__HPP__ 1

#include <sstream>

namespace Utils {
	inline void debug(string msg) {std::cout << "[DEBUG] " << msg << std::endl;}

	inline void error(string msg) {std::cerr << "[ERROR] " << msg << std::endl;}

	inline bool file_exist (const std::string& name) {
  	struct stat buffer;   
  	return (stat (name.c_str(), &buffer) == 0); 
	}

	inline string int2str(int n)
	{
		std::ostringstream ss;
		ss << n;
		return ss.str();
	}
}

#endif
