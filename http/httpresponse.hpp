#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <filesystem>
#include <string>
#include <string_view>
#include <set>
#include <fstream>
#include <cstring>

#include "httpparse.hpp"


enum STATUS_CODE{
     CONTINUE = 100
    ,SWITCHING_PROTOCOLS                //101 switching_protocols
    ,PROCESSING
    ,OK = 200                           //200 ok
    ,CREATED
    ,ACCEPTED
    ,NON_AUTHORITATIVE_INFORMATION
    ,NO_CONTENT
    ,RESET_CONTENT
    ,PARTIAL_CONTENT
    ,MULTI_STATUS
    ,ALREADY_REPORTED
    ,IM_USED
    ,MULTIPLE_CHOICES = 300
    ,MOVED_PERMANENTLY
    ,FOUND
    ,SEE_OTHER
    ,NOT_MODIFIED
    ,USE_PROXY
    ,TEMPORARY_REDIRECT
    ,BAD_REQUEST = 400
    ,UNAUTHORIZED
    ,PAYMENT_REQUIRED
    ,FORBIDDEN
    ,NOT_FOUND                          //404 not_fonud
    ,METHOD_NOT_ALLOWED
    ,NOT_ACCEPTABLE
    ,PROXY_AUTHENTICATION_REQUIRED
    ,REQUEST_TIMEOUT
    ,CONFLICT
    ,GONE
    ,LENGTH_REQUIRED
    ,PRECONDITION_FAILED
    ,REQUEST_ENTITY_TOO_LARGE
    ,REQUEST_URI_TOO_LONG
    ,UNSUPPORTED_MEDIA_TYPE
    ,REQUESTED_RANGE_NOT_SATISFIABLE
    ,EXPECTATION_FAILED
    ,INTERNAL_SERVER_ERROR = 500
    ,NOT_IMPLEMENTED
    ,BAD_GATEWAY
    ,SERVICE_UNAVAILABLE 
    ,GATEWAY_TIMEOUT 
    ,HTTP_VERSION_NOT_SUPPORTED         //505 http_version_not_supported
};

const std::unordered_map<int, const std::string_view> STATUS_STR = {
     {100, "100 Continue\r\n"}
    ,{101, "101 Switching Protocols\r\n"}
    ,{102, "102 Processing\r\n"}
    ,{200, "200 OK\r\n"}
    ,{201, "201 Created\r\n"}
    ,{202, "202 Accepted\r\n"}
    ,{203, "203 Non-Authoritative Information\r\n"}
    ,{204, "204 No Content\r\n"}
    ,{205, "205 Reset Content\r\n"}
    ,{206, "206 Partial Content\r\n"}
    ,{207, "207 Multi-Status\r\n"}
    ,{208, "208 Already Reported\r\n"}
    ,{226, "226 IM Used\r\n"}
    ,{300, "300 Multiple Choices\r\n"}
    ,{301, "301 Moved Permanently\r\n"}
    ,{302, "302 Found\r\n"}
    ,{303, "303 See Other\r\n"}
    ,{304, "304 Not Modified\r\n"}
    ,{305, "305 Use Proxy\r\n"}
    ,{307, "307 Temporary Redirect\r\n"}
    ,{400, "400 Bad Request\r\n"}
    ,{401, "401 Unauthorized\r\n"}
    ,{402, "402 Payment Required\r\n"}
    ,{403, "403 Forbidden\r\n"}
    ,{404, "404 Not Found\r\n"}
    ,{405, "405 Method Not Allowed\r\n"}
    ,{406, "406 Not Acceptable\r\n"}
    ,{407, "407 Proxy Authentication Required\r\n"}
    ,{408, "408 Request Timeout\r\n"}
    ,{409, "409 Conflict\r\n"}
    ,{410, "410 Gone\r\n"}
    ,{411, "411 Length Required\r\n"}
    ,{412, "412 Precondition Failed\r\n"}
    ,{413, "413 Request Entity Too Large\r\n"}
    ,{414, "414 Request-URI Too Long\r\n"}
    ,{415, "415 Unsupported Media Type\r\n"}
    ,{416, "416 Requested Range Not Satisfiable\r\n"}
    ,{417, "417 Expectation Failed\r\n"}
    ,{500, "500 Internal Server Error\r\n"}
    ,{501, "501 Not Implemented\r\n"}
    ,{502, "502 Bad Gateway\r\n"}
    ,{503, "503 Service Unavailable\r\n"}
    ,{504, "504 Gateway Timeout\r\n"}
    ,{505, "505 HTTP Version Not Supported\r\n"}
};

namespace vastina{

namespace fs = std::filesystem;

class cachetree{//to do
private:
    cachetree(): static_files{ } {}

    std::set<std::string> static_files;
public:
    static cachetree& getInstance(){
        static cachetree cache;
        return cache;
    }
    void init_read(const fs::path& directory = fs::current_path()
        , const fs::path& relativePath = fs::path());

    bool static_file_exist(const std::string& str);
    //bool static_file_exist(std::string str);
};

bool cachetree::static_file_exist(const std::string& str) {
    return static_files.find(str) != static_files.end();
}

// bool cachetree::static_file_exist(std::string str) {
//     return static_files.find(str) != static_files.end();
// }

void cachetree::init_read(const fs::path& directory, const fs::path& relativePath) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            fs::path newRelativePath = relativePath / entry.path().filename();
            init_read(entry.path(), newRelativePath);  
        } else if (entry.is_regular_file()) {
            static_files.insert((relativePath / entry.path().filename()).string()); 
        }
    }
}

class httpresponse
{
private:
    ;
public:
    httpresponse(){};
    ~httpresponse(){};
    
    void reset();
    void makereponse_test(httpparser& parser, char *buf);//no buffer now, so send parser directly
};

void httpresponse::reset(){
    
}

void justfortest(std::string *response, int code, size_t length = 2000){
    response->append("HTTP/1.1 ").append(STATUS_STR.at(code));
    response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
    response->append("Server: localhost:7895\r\n");
    response->append("Content-Length: ").append(std::to_string(length)).append("\r\n");;
    response->append("Content-Type: text/html charset=utf-8\r\n");
    response->append("Connection: keep-alive\r\n");
    response->append("\r\n");
}

void httpresponse::makereponse_test(httpparser& parser, char *buf){
std::cout << parser.getMethod() <<'\n';
std::cout << parser.getPath() <<'\n';
std::cout << parser.getProtocol() <<'\n';

    std::string *response = new std::string("");
    response->reserve(200);
    bzero(buf, BUFSIZ);
    
    if(parser.getMethod()=="GET" && parser.getProtocol()=="HTTP/1.1"){
        
        if(parser.getPath()=="/"){
            if(cachetree::getInstance().static_file_exist(std::move("index.html"))){
                justfortest(response, STATUS_CODE::OK);
                strcpy(buf, response->c_str());
                int count = response->size();

                std::ifstream infile;
                infile.open("index.html", std::ifstream::in);
                infile.read(buf+count, BUFSIZ-count);
                infile.close();
            } 
        }

        else if(parser.getPath()=="/test"){
            std::string body{" 114514"};
            justfortest(response, STATUS_CODE::ACCEPTED, body.length());
            response->append(body);
            strcpy(buf, response->c_str());
        }

        else if(parser.getPath()=="/favicon.ico"){
            std::string body{
                "1145\r\n"
                "1419\r\n"
                "1981\r\n"
                "0258\r\n"
            };
            response->append("HTTP/1.1 ").append(STATUS_STR.at(STATUS_CODE::OK));
            response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
            response->append("Server: localhost:7895\r\n");
            response->append("Content-Length: ").append(std::to_string(body.length())).append("\r\n");;
            response->append("Content-Type: image/x-icon\r\n");
            response->append("Connection: keep-alive\r\n");
            response->append("\r\n");
            response->append(body);
            strcpy(buf, response->c_str());
        }

        else if(cachetree::getInstance().static_file_exist(parser.getPath().substr(1))){
            justfortest(response, STATUS_CODE::OK);
            strcpy(buf, response->c_str());
            int count = response->size();

            std::ifstream infile;
            infile.open(parser.getPath().substr(1), std::ifstream::in);
            infile.read(buf+count, BUFSIZ-count);
            infile.close();
        }
    
        else if(cachetree::getInstance().static_file_exist("404.html")){
            justfortest(response, STATUS_CODE::NOT_FOUND);
            strcpy(buf, response->c_str());
            int count = response->size();

            std::ifstream infile;
            infile.open("404.html", std::ifstream::in);
            infile.read(buf+count, BUFSIZ-count);
            infile.close();
        }

    }

    else if(parser.getProtocol()!="HTTP/1.1"){
        justfortest(response, STATUS_CODE::HTTP_VERSION_NOT_SUPPORTED);
        strcpy(buf, response->c_str());
        int count = response->size();

        std::ifstream infile;
        infile.open("505.html", std::ifstream::in);
        infile.read(buf+count, BUFSIZ-count);
        infile.close();
    }

    else{
        justfortest(response, STATUS_CODE::BAD_REQUEST, 0);
        strcpy(buf, response->c_str());
    }

    delete response;
}


}





#endif // _HTTP_RESPONSE_H_