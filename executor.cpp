#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <stdint.h>
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <executor.hpp>
#include <strutil.hpp>
#include <peer_channel.hpp>
#include <conf_manager.hpp>

using namespace ourapi;
using std::vector;
using boost::property_tree::ptree;
using std::make_pair;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::format;
using boost::regex_search;
using boost::match_default;
using boost::match_results;
using boost::regex;


Executor::Executor()
{}

bool Executor::public_file(const map<string,string>& args, outputType type,  string& response)
{
	Utils::debug("I'm trying to serve a public file");
	std::ifstream file (response.c_str());
	std::ostringstream out_stream;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file,response);
			out_stream << response << std::endl;
		}
		response = out_stream.str();
		
		file.close();
	}
	else
		return false;
}

bool Executor::about(const map<string,string>& args, outputType type,  string& response)
{
	string streamer_running = ((ChannelStreamer::getInstance())->streamer_running()) ? "yes" : "no" ;
	map<string,string> msgs;
	msgs["application.name"] = "ReST streamer controller";
	msgs["application.version"] = "0.0.1";
	msgs["application.streamer_running"] = streamer_running; 

	return data_msg(msgs,type, response);
}

bool Executor::data_msg(const map<string,string>& args, outputType type,  string& response)
/* Simply display the args data. Useful for giving alerts and errors report */
{
	ptree errorInfo;
	map<string,string>::const_iterator mit;
	for(mit = args.begin(); mit != args.end(); mit++)
		errorInfo.put(mit->first, mit->second);
	

	_generateOutput(&errorInfo, type, response);
	return true;
}

bool Executor::channelPort(const map<string,string>& args, outputType type,  string& response)
{
	PeerChannel* pc;
	bool success = true;
	map<string,string> msgs;

	try {
		pc = new PeerChannel(args);
	} catch (std::exception& e)	{ 
		msgs["error"] = "parameters required.";
		data_msg(msgs,type,response);
		success = false; 
	}

	if(success)
	{	
		if(ConfManager::single_streamer()	==true)
			pc->set_local_udp_port(ConfManager::udp_port()); // FIXED udp port
		else
			pc->set_local_udp_port(Utils::random_int(7000,8000)); // FIXED udp port
	
		if((ChannelStreamer::getInstance())->startProcess(pc) == true)
		{
			ptree channelInfo = pc->toParametersTree();
		  _generateOutput(&channelInfo, type, response);
  		//std::cout << response << std::endl; // DEBUG purpose
			success = true;
		}
		else
		{
			msgs["error"] = "unable to start the streamer process.";
			data_msg(msgs,type,response);
			success = false;
		}

		delete(pc);
	}
  return success;

}

void Executor::_generateOutput(void *data, outputType type, string& output)
{
    std::ostringstream ostr;
    ptree *pt = (ptree *) data;
    if (TYPE_JSON == type)
        write_json(ostr, *pt);
    else if (TYPE_XML == type)
        write_xml(ostr, *pt);

    output = ostr.str();
}

