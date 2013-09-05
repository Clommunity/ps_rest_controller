#ifndef __UTILS__HPP__
#define __UTILS__HPP__ 1

#include <sstream>
#include <fstream>
#include <conf_manager.hpp>

namespace Utils {
	inline void debug(string msg) {
		if(ConfManager::debug() == true)
			std::cout << "[DEBUG] " << msg << std::endl;
	}

	inline void error(string msg) {std::cerr << "[ERROR] " << msg << std::endl;}

	inline bool file_exist (const std::string& name) {
		std::ifstream file (name.c_str());
		bool ex = file.good();
		if(ex)
			file.close();
  	return ex; 
	}

	inline string int2str(int n)
	{
		std::ostringstream ss;
		ss << n;
		return ss.str();
	}
	inline int random_int(int min,int max)
	{
			srand(time(NULL));
			return min + (rand() % (max-min));
	}

	static bool validate_conf()
	{
		bool success = true;
		if(!file_exist(ConfManager::streamer_file()))
		{
			success = false;
			Utils::error("Streamer file \""+ ConfManager::streamer_file() +"\" not found..");
		}
		if(ConfManager::http_port() < 1025)
		{
			success = false;
			error("Invalid HTTP port.");
		}
		if(ConfManager::udp_port() < 1025)
		{
			success = false;
			error("Invalid UDP port.");
		}
		if(ConfManager::udp_port() == ConfManager::http_port())
		{
			success = false;
			error("Same udp and http port number. Maybe you should sleep a little..");
		}			
		if(!file_exist(ConfManager::public_folder()))
		{
			success = false;
			error("The public files folder choosed (" + ConfManager::public_folder() + ") doesn't exist.");
		}			
			
		return success;
	}
}

#endif
