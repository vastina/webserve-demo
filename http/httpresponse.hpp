#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <cstring>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>
#include <string_view>

#include "httpparse.hpp"

namespace vastina {

namespace fs = std::filesystem;

class cachetree { // to do
  private:
	cachetree() : static_files{} {}

	std::set<std::string> static_files;

  public:
	static cachetree& getInstance() {
		static cachetree cache;
		return cache;
	}
	void init_read(const fs::path &directory = fs::current_path(),
				   const fs::path &relativePath = fs::path());

	bool static_file_exist(const std::string &str);
};

constexpr size_t default_header_length = 200;
constexpr size_t default_body_length = 2000;
// 随便写的
class httpresponse {

  private:
	std::string filename;
	std::string body;
	STATUS_CODE state;
	CONNECTION connection;
	CONTENT_TYPE content_type;
	size_t length;

  public:
	httpresponse();
	~httpresponse();

	void reset();

	void autoresponse(httpparser &parser, char *buf);
	void solverequestline(httpparser &parser);
	void solvepath(const std::string &str);
	void addheader(std::string *response);
};

} // namespace vastina

#endif // _HTTP_RESPONSE_H_