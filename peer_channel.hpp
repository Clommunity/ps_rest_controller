#ifndef __PEER_CHANNEL__
#define __PEER_CHANNEL__ 1

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <map>
#include <exception>
#include <conf_manager.hpp>
#include <sstream>
#include <signal.h>
#include <sys/wait.h>
#include <utils.hpp>


using boost::property_tree::ptree;

class Missing_parameter_exception: public std::exception
{
	virtual const char * what() const throw()
	{
		return "A parameter is missing.";
	}
} missing_parameter_exception;

class PeerChannel {
	private:
		string name;
		string source_ip;
		string source_port;
		int local_udp_port;

	public:
		string get_name() { return name; }
		string get_source_ip() { return source_ip; }
		string get_source_port() { return source_port; }
		int get_local_udp_port() { return local_udp_port; }

		PeerChannel(string& n,string& ip, string& port)
		{
			name = n;
			source_ip = ip;
			source_port = port;
		}

		PeerChannel(const map<string,string>& dict)
		{
			map<string,string>::const_iterator it;
			if ((it = dict.find("name")) != dict.end()) 
				name = it->second; 
			else throw missing_parameter_exception;

			if ((it = dict.find("source_ip")) != dict.end()) 
				source_ip = it->second;  
			else throw missing_parameter_exception;

			if ((it = dict.find("source_port")) != dict.end()) 
				source_port = it->second; 
			else throw missing_parameter_exception;

			if ((it = dict.find("local_udp_port")) != dict.end()) 
				local_udp_port = atoi((it->second).c_str()); 
		}

		ptree toParametersTree()
		{
			ptree info;
			info.put("channel.name",name);	
			info.put("channel.source_ip",source_ip);	
			info.put("channel.source_port",source_port);
			info.put("channel.local_udp_port",local_udp_port);
			return info;
		}

		void set_local_udp_port(int port)
		{
			local_udp_port = port;
		}
};


class ChannelStreamer {
	private:
		int process_id;
		static ChannelStreamer* instance;
		
		ChannelStreamer() { process_id = 0; }
	public:
		static ChannelStreamer* getInstance()
		{
			if (instance == NULL)
				instance = new ChannelStreamer();
			return instance;
		}
		bool startProcess(PeerChannel* pc)
		{
			if(streamer_running())
				killCurrent();
			if(Utils::file_exist(ConfManager::streamer_file()))
			{
				Utils::debug("starting streamer");
				process_id = fork();
				if (process_id == 0)
					while (true)
						execl(ConfManager::streamer_file().c_str(),ConfManager::streamer_file().c_str(),
							"-P",get_random_port().c_str(),
							 "-i",pc->get_source_ip().c_str(),"-p",pc->get_source_port().c_str(),
								"-F",get_outmodule_flags(pc).c_str(),(char*)0);

				Utils::debug("process id:" + Utils::int2str(process_id));

				return true;
			} else
			{
				Utils::debug(std::string("File ") + ConfManager::streamer_file() + std::string(" not found."));
				return false;
			}
		}
		
		string get_random_port()
		{
			int streamer_port = Utils::random_int(6000,7000); 
			return Utils::int2str(streamer_port);
		}

		string get_outmodule_flags(PeerChannel* pc)
		{
			std::ostringstream oss;
			oss << "/dev/stdout,dechunkiser=udp,port0=" << pc->get_local_udp_port() << ",addr=" << ConfManager::udp_addr();
	return oss.str();
		}

		bool killCurrent()
		{
			if( streamer_running() )
			{	
				Utils::debug("killing streamer");
				kill(process_id,SIGINT);
				waitpid(process_id,0,0);
				process_id = 0;
				return true;
			} else
				return false;
		}

		bool streamer_running()
		{
			if (!valid_pid()) return false;
			int status;
			pid_t result = waitpid(process_id, &status, WNOHANG);	
			return result == 0 ? true : false;
		}
		bool valid_pid()
		{ return process_id > 0 ? true : false; }

};

ChannelStreamer* ChannelStreamer::instance = NULL;

#endif
