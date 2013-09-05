#ifndef __EXECUTOR_FOR_API__
#define __EXECUTOR_FOR_API__

#include <string>
#include <set>

using std::string;
using std::map;

namespace ourapi
{

class Executor
{   
public:
    enum outputType {
        TYPE_JSON, TYPE_XML   
    };
    Executor();
    bool channelPort(const map<string,string>& args, outputType type, string& response);
    bool public_file(const map<string,string>& args, outputType type, string& response);
    bool about(const map<string,string>& args, outputType type, string& response);
    bool data_msg(const map<string,string>& args, outputType type, string& response);
private:
    void _generateOutput(void *data, outputType type, string& output);

};



}  // namespace ourapi

#endif
