#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>

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
{
}

bool Executor::about(const map<string,string>& args, outputType type,  string& response)
{
	string streamer_running = ((ChannelStreamer::getInstance())->valid_pid()) ? "yes" : "no" ;
	ptree info;
	info.put("application.name","ReST streamer controller");
	info.put("application.version","0.0.0");
	info.put("application.streamer_running",streamer_running);

  _generateOutput(&info, type, response);
  std::cout << response << std::endl;
	return true;
}

bool Executor::channelPort(const map<string,string>& args, outputType type,  string& response)
{
	PeerChannel* pc;
	try {
		pc = new PeerChannel(args);
	} catch (std::exception& e)
	{ return false; }

	pc->set_local_udp_port(7710);
	ptree channelInfo = pc->toParametersTree();
	
	(ChannelStreamer::getInstance())->startProcess(pc);

  _generateOutput(&channelInfo, type, response);
  std::cout << response << std::endl;

	delete(pc);
  return true;

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

