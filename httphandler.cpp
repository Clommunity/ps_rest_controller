#include <signal.h>
#include <pthread.h>
#include <platform.h>
#include <microhttpd.h>
#include <iostream>
#include <map>
#include <string>

#include <api.hpp>
#include <conf_manager.hpp>
#include <utils.hpp>

using std::map;
using std::string;

#define PAGE "<html><head><title>Error</title></head><body>Bad data</body></html>"

static int shouldNotExit = 1;

static int send_bad_response( struct MHD_Connection *connection)
{
    static char *bad_response = (char *)PAGE;
    int bad_response_len = strlen(bad_response);
    int ret;
    struct MHD_Response *response;

    response = MHD_create_response_from_buffer ( bad_response_len,
                bad_response,MHD_RESPMEM_PERSISTENT);
    if (response == 0){
        return MHD_NO;
    }
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);
    return ret;
}


static int get_url_args(void *cls, MHD_ValueKind kind,
                    const char *key , const char* value)
{
    map<string, string> * url_args = static_cast<map<string, string> *>(cls);

    if (url_args->find(key) == url_args->end()) {
         if (!value)
             (*url_args)[key] = "";
         else 
            (*url_args)[key] = value;
    }
    return MHD_YES;

}
                
static int url_handler (void *cls,
    struct MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data, size_t *upload_data_size, void **ptr)
{
    static int aptr;
    const char *fmt = (const char *)cls;
    const char *val;
    char *me;
    const char *typexml = "xml";
    const char *typejson = "json";
    const char *type = typejson;

    struct MHD_Response *response;
    int ret;
    map<string, string> url_args;
    map<string, string>:: iterator  it;
    ourapi::api callapi;
    string respdata;

    if (0 != strcmp (method, "GET"))
        return MHD_NO; 


    if (&aptr != *ptr) {
        *ptr = &aptr;
        return MHD_YES;
    }


    if (MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, 
                           get_url_args, &url_args) < 0) {
        return send_bad_response(connection);

    }

    callapi.executeAPI(url, url_args, respdata);

    *ptr = 0;                  /* reset when done */
    val = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, "q");
    me = (char *)malloc (respdata.size() + 1);
    if (me == 0)
        return MHD_NO;
    strncpy(me, respdata.c_str(), respdata.size() + 1);
    response = MHD_create_response_from_buffer (strlen (me), me,
					      MHD_RESPMEM_MUST_FREE);
    if (response == 0){
        free (me);
        return MHD_NO;
    }

    it = url_args.find("type");
    if (it != url_args.end() && strcasecmp(it->second.c_str(), "xml") == 0)
        type = typexml;

    MHD_add_response_header(response, "Content-Type", "text");
    MHD_add_response_header(response, "OurHeader", type);

    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);
    return ret;
}

void handle_term(int signo)
{
    shouldNotExit = 0;
}

void* http(void *arg)
{
    int *port = (int *)arg;
    struct MHD_Daemon *d;

    d = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
                        *port,
                        0, 0, &url_handler, (void *)PAGE, MHD_OPTION_END);
    if (d == 0){
        return 0;
    }
    while(shouldNotExit) {
        sleep(1);
    }
    MHD_stop_daemon (d);
    return 0;
}

void usage()
{
	std::cout << "rest_streamer_controller usage:" << std::endl;
	std::cout << "\t./rest_streamer_controller [-h] [-p <http_port>] [ -s <streamer_executable> ] [ -u <default_udp_port> ] [-v] [-m] [-f <public_folder>]"  << std::endl << std::endl;
	std::cout << "\t-h\t\tshow this help and exit" << std::endl;
	std::cout << "\t-v\t\tverbosity" << std::endl;
	std::cout << "\t-m\t\tenable multiple streamer support" << std::endl;
	std::cout << "Defualt values are:" << std::endl;
	ConfManager::print_conf();
}

void parse_args(int argc, char *const *argv)
{
	int c;
	ConfManager* ist = ConfManager::instance();
	while ((c = getopt (argc, argv, "f:p:s:u:hmv")) != -1)
  	switch (c)
    {
			case 'p':	
				ist->set_http_port(atoi(optarg));
				break;
			case 'u':
				ist->set_udp_port(atoi(optarg));
				break;
			case 'f':
				ist->set_public_folder(optarg);
				break;
			case 's':
				ist->set_streamer_file(optarg);
				break;
			case 'v':
				ist->set_debug(true);
				break;
			case 'm':
				ist->set_single_streamer(false);
				break;	
			case 'h':
      default:
      	usage();
				exit(1);
		}
}

int main (int argc, char *const *argv)
{

//    daemon(0,0); // demonize and redirect all output to /dev/null
    signal(SIGTERM, handle_term);
		parse_args(argc,argv);
		ConfManager::print_conf();
		if(!Utils::validate_conf())
			exit(1);

		int port = ConfManager::http_port();
		http(&port);
    return 0;
}
