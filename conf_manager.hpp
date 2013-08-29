#ifndef __CONF_MANAGER_HPP__
#define __CONF_MANAGER_HPP__ 1

#include<string>
#include<iostream>
#include<utils.hpp>

namespace ConfManager {
		static int http_port = 1234;
		static string streamer_file = "./streamer-udp-grapes-static";
		static int udp_port = 7710;

		static void print_conf()
		{
			std::cout << "http_port = " << ConfManager::http_port << std::endl;
			std::cout << "streamer_executable = " << ConfManager::streamer_file << std::endl;
			std::cout << "default_udp_port = " << ConfManager::udp_port << std::endl;
		}

		static bool validate_conf()
		{
			bool success = true;
			if(!Utils::file_exist(streamer_file))
			{
				success = false;
				Utils::error("Streamer file \""+ streamer_file +"\" not found..");
			}
			if(http_port < 1025)
			{
				success = false;
				Utils::error("Invalid HTTP port.");
			}
			if(udp_port < 1025)
			{
				success = false;
				Utils::error("Invalid UDP port.");
			}
			if(udp_port == http_port)
			{
				success = false;
				Utils::error("Same udp and http port number. Maybe you should sleep a little..");
			}			
			
			return success;
		}
}

#endif
