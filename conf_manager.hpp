#ifndef __CONF_MANAGER_HPP__
#define __CONF_MANAGER_HPP__ 1

#include<string>
#include<iostream>

class ConfManager {
	int _http_port;
	string _streamer_file;
	string _udp_addr;
	string _public_folder;
	int _udp_port;
	bool _single_streamer;
	bool _debug;


	ConfManager()
	{
			_http_port = 1234;
			_udp_addr = "127.0.0.1";
		_streamer_file = "./streamer-udp-grapes-static";
		_public_folder = "public_html";
		_udp_port = 7710;
		_single_streamer = true;
		_debug = false;
	}	
	ConfManager(ConfManager const&);
	void operator=(ConfManager const&);
	public:

	static ConfManager* instance() 
	{
		static ConfManager *inst; // instance
		if( inst == NULL)
			inst = new ConfManager();
		return inst;
	}
	static int http_port() { return instance()->_http_port;}
	static int udp_port() { return instance()->_udp_port;}
	static bool debug() { return instance()->_debug;}
	static bool single_streamer() { return instance()->_single_streamer;}
	static string streamer_file() { return instance()->_streamer_file;}
	static string udp_addr() { return instance()->_udp_addr;}
	static string public_folder() { return instance()->_public_folder;}

	void set_http_port(int arg) {_http_port = arg;}
	void set_udp_port(int arg) {_udp_port = arg;}
	void set_single_streamer(bool arg) {_single_streamer = arg;}
	void set_debug(bool arg) {_debug = arg;}
	void set_streamer_file(string arg) {_streamer_file = arg;}
	void set_udp_addr(string arg) {_udp_addr = arg;}
	void set_public_folder(string arg) {_public_folder = arg;}


	static void print_conf()
	{	
		std::cout << "http_port = " << http_port() << std::endl;
		std::cout << "streamer_executable = " << streamer_file() << std::endl;
		std::cout << "public_folder = " << public_folder() << std::endl;
		std::cout << "udp_addr = " << udp_addr() << std::endl;
		std::cout << "default_udp_port = " << udp_port() << std::endl;
		std::cout << "verbosity = " << (debug() ? "yes" : "no") << std::endl;
		std::cout << "multiple streamer support = " << (!(single_streamer()) ? "yes" : "no") << std::endl;
	}

};

#endif
