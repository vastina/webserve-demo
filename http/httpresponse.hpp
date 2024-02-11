#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <cstring>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>
#include <string_view>

#include "httpparse.hpp"
#include "httpdef.hpp"

namespace vastina {

namespace fs = std::filesystem;

class cachetree { // to do
  private:
	cachetree() : static_files{} {}

	std::set<std::string> static_files;

  public:
	static cachetree &getInstance() {
		static cachetree cache;
		return cache;
	}
	void init_read(const fs::path &directory = fs::current_path(),
				   const fs::path &relativePath = fs::path());

	bool static_file_exist(const std::string &str);
	// bool static_file_exist(std::string str);
};

bool cachetree::static_file_exist(const std::string &str) {
	return static_files.find(str) != static_files.end();
}

// bool cachetree::static_file_exist(std::string str) {
//     return static_files.find(str) != static_files.end();
// }

void cachetree::init_read(const fs::path &directory,
						  const fs::path &relativePath) {
	const std::string directoryPath = "/home/net/webserve-demo/f";
	fs::path absDirectoryPath = fs::absolute(directoryPath);

	for (const auto &entry :
		 fs::recursive_directory_iterator(absDirectoryPath)) {
		fs::path relativePath = fs::relative(entry.path(), absDirectoryPath);
		static_files.insert(relativePath.string());
	}
}

constexpr size_t default_header_length = 200;
constexpr size_t default_body_length = 2000;
// 随便写的
class httpresponse {
  private:
	std::string filename;
	CONNECTION connection_type;
	CONTENT_TYPE content_type;

  public:
	httpresponse(){};
	~httpresponse(){};

	void reset();
	void makereponse_test(httpparser &parser,
						  char *buf); // no buffer now, so send parser directly

	void autoresponse(httpparser &parser, char *buf);
	void solverequestline(httpparser &parser);
	void solvepath(const std::string &str);
	void addheader(std::string *response, int code, size_t length = default_body_length, CONNECTION connection_type);
};

void writefile(char *buf, int count, std::string filename) {
	std::ifstream infile;
	infile.open(filename, std::ifstream::in);
	infile.read(buf + count, BUFSIZ - count);
	infile.close();
}

void httpresponse::solverequestline(httpparser &parser) {
	switch (parser.getProtocol()) {
	case HTTP_11:
		switch (parser.getMethod()) {
		case GET:
			solvepath(parser.getPath());

			break;
		case POST:
			/* code */
			break;
		default:
			break;
		}
		break;
	default:
		//addheader()
		break;
	}
}

void httpresponse::solvepath(const std::string &str) {
	if (str == "/"){
		filename = "index.html";
	}
	else if (str == "/test"){
		filename.clear();
	}
	else if (str == "/login?text=test") {
		// todo: parse action?body
		filename = "page2/gettest.html";
	} else {
		filename = str.substr(1);
		if (!cachetree::getInstance().static_file_exist(filename)) {
			filename = "404.html";
		}
	}
}

void httpresponse::addheader(std::string *response, int state, size_t length, CONNECTION connection_type) {
	{//no if part
		response->append("HTTP/1.1 ").append(STATUS_STR.at(state));
		response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
		response->append("Server: localhost:6780\r\n");
		response->append("Content-Length: ")
		.append(std::to_string(length))
		.append("\r\n");
	}
	{//???
		response->append("Content-Type: ")
			.append(CONTENNT_TYPE_STR.at(content_type))
			.append("\r\n");
		response->append(CONNECTION_STR.at(connection_type));
	}
	response->append("\r\n");
}

void httpresponse::autoresponse(httpparser &parser, char *buf) {
	std::string *response = new std::string("");
	response->reserve(default_header_length);
	memset(buf, 0, BUFSIZ);

	switch (parser.getProtocol()) {
	case HTTP_11:
		switch (parser.getMethod()) {
		case GET:
			solvepath(parser.getPath());

			break;
		case POST:
			/* code */
			break;
		default:
			break;
		}
		break;
	default:
		addheader(response, STATUS_CODE::HTTP_VERSION_NOT_SUPPORTED);
		break;
	}
}

void httpresponse::reset() {}

void justfortest(std::string *response, int code, bool connection = false,
				 size_t length = 2000) {
	response->append("HTTP/1.1 ").append(STATUS_STR.at(code));
	response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
	response->append("Server: localhost:7895\r\n");
	response->append("Content-Length: ")
		.append(std::to_string(length))
		.append("\r\n");
	;
	response->append("Content-Type: text/html charset=utf-8\r\n");
	if (connection)
		response->append("Connection: keep-alive\r\n");
	else
		response->append("Connection: close\r\n");
	response->append("\r\n");
}

// response of existed requests should be writen into cache to speed up?

void httpresponse::makereponse_test(httpparser &parser, char *buf) {

	// var 1, 2, 3...
	// makeresponse(&1, &2, &3...)
	// update_state(1, 2, 3)
	// or they should be the private member of this class

	std::string *response = new std::string("");
	response->reserve(200);
	bzero(buf, BUFSIZ);

	if (parser.getMethod() == GET && parser.getProtocol() == HTTP_11) {

		if (parser.getPath() == "/") {
			if (cachetree::getInstance().static_file_exist(
					std::move("index.html"))) {
				justfortest(response, STATUS_CODE::OK);
				strcpy(buf, response->c_str());

				writefile(buf, response->size(), "index.html");
			}
		}

		else if (parser.getPath() == "/test") {
			std::string body{" 114514"};
			justfortest(response, STATUS_CODE::ACCEPTED, body.length());
			response->append(body);
			strcpy(buf, response->c_str());
		}

		else if (parser.getPath() == "/login?text=vastina") {
			justfortest(response, STATUS_CODE::OK);
			strcpy(buf, response->c_str());

			if (cachetree::getInstance().static_file_exist(
					std::move("page2/gettest.html")))
				writefile(buf, response->size(), "page2/gettest.html");
		}

		else if (parser.getPath() == "/favicon.ico") {
			response->append("HTTP/1.1 ")
				.append(STATUS_STR.at(STATUS_CODE::OK));
			response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
			response->append("Server: localhost:7895\r\n");
			response->append("Content-Length: ")
				.append(std::to_string(default_body_length))
				.append("\r\n");
			;
			response->append("Content-Type: image/x-icon\r\n");
			response->append("Connection: keep-alive\r\n");
			response->append("\r\n");
			strcpy(buf, response->c_str());

			writefile(buf, response->size(), "favicon.ico");
		}

		else if (cachetree::getInstance().static_file_exist(
					 parser.getPath().substr(1))) {
			justfortest(response, STATUS_CODE::OK);
			strcpy(buf, response->c_str());

			writefile(buf, response->size(),
					  parser.getPath().substr(1).c_str());
		}

		else if (cachetree::getInstance().static_file_exist("404.html")) {
			justfortest(response, STATUS_CODE::NOT_FOUND);
			strcpy(buf, response->c_str());

			writefile(buf, response->size(), "404.html");
		}

	}

	else if (parser.getMethod() == POST) {
	}

	else if (parser.getProtocol() != HTTP_11) {
		justfortest(response, STATUS_CODE::HTTP_VERSION_NOT_SUPPORTED);
		strcpy(buf, response->c_str());

		writefile(buf, response->size(), "505.html");
	}

	else {
		justfortest(response, STATUS_CODE::BAD_REQUEST, false, 0);
		strcpy(buf, response->c_str());

		writefile(buf, response->size(), "502.html");
	}

	delete response;
}

} // namespace vastina

#endif // _HTTP_RESPONSE_H_