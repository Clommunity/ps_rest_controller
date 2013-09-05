#include <string.h>

#include <boost/foreach.hpp>

#include <api.hpp>
#include <conf_manager.hpp>
#include <utils.hpp>
#include <strutil.hpp>

using namespace ourapi;

bool in_public_folder(string filename)
{
	Utils::debug("checking existence of: "+ConfManager::public_folder() +filename);
	return Utils::file_exist(ConfManager::public_folder() + filename);
}

struct validate_data
{
    string api;
    set <string>* params; 
};

api::api()
{
		string channelPortParams[] = {"channel"};
// TODO parameters are not taken in account..
    _apiparams["/channelPort"] = set<string>(channelPortParams, channelPortParams + 1);
    _apiparams["/about"] = set<string>(channelPortParams, channelPortParams + 1);
}

bool api::executeAPI(const string& url, const map<string, string>& argvals, string& response)
{
    // Ignore all the args except the "fields" param 
    validate_data vdata ;
    vdata.api = url;
    Executor::outputType type = Executor::TYPE_JSON;

	vdata.params = new set<string>(); // TODO validation of the url and the params 
    if ( !_validate(&vdata)) {
        _getInvalidResponse(response);
        return false;
    }

    map<string,string>::const_iterator it1 = argvals.find("type");
	  if (it1 != argvals.end()){
        const string outputtype = it1->second;
        if (strcasecmp(outputtype.c_str(), "xml") == 0 ) {
            type = Executor::TYPE_XML;
        }
    }

    return _executeAPI(url, argvals, type, response);
}

string find_index_file()
{
	// TODO think a better implementation
	Utils::debug("finding index");
	if( Utils::file_exist(ConfManager::public_folder() + "/index.htm"))
		return ConfManager::public_folder() + "/index.htm";
	if( Utils::file_exist(ConfManager::public_folder() + "/index.html"))
		return ConfManager::public_folder() + "/index.html";
	return "/";
}

bool api::_executeAPI(const string& url, const map<string,string>& argvals, 
        Executor::outputType type, string& response)
{
    bool ret = false;
    if (url == "/channelPort")
        ret = _executor.channelPort(argvals, type, response);
    if (url == "/about")
        ret = _executor.about(argvals, type, response);
    if (in_public_folder(url)) {
				if ( url == "/" )
				{
					response = find_index_file();
					Utils::debug(response);
				}
				else
					response = ConfManager::public_folder() + url;
        ret = _executor.public_file(argvals, type, response);
		}

    return ret;
}

bool api::_validate(const void *data)
{
    const validate_data *vdata = static_cast<const validate_data *>(data );
    map<string, set<string> > ::iterator it =  _apiparams.find(vdata->api);

    it = _apiparams.find(vdata->api);

    if ( it == _apiparams.end()){
        return in_public_folder(vdata->api); // if not an api command check if is a public file
    }
    set<string>::iterator it2 = vdata->params->begin();
    while (it2 != vdata->params->end()) { //TODO this check doesn't work
        if (it->second.find(*it2) == it->second.end()) 
            return false;
        ++it2;
    }

    return true;
}

void api::_getInvalidResponse(string& response)
{
    response = "Some error in your data ";
}

