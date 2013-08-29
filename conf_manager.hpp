#ifndef __CONF_MANAGER_HPP__
#define __CONF_MANAGER_HPP__ 1

#include<string>
#include<iostream>

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
}

#endif
