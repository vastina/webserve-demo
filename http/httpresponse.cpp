#include "httpresponse.hpp"

namespace vastina {

bool cachetree::static_file_exist(const std::string &str) {
	return static_files.find(str) != static_files.end();
}

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


void writefile(char *buf, int count, std::string filename) {
	std::ifstream infile;
	infile.open(filename, std::ifstream::in);
	infile.read(buf + count, BUFSIZ - count);
	infile.close();
}

httpresponse::httpresponse(): 
	filename{"index.html"}, state{OK}, connection{KEEP_ALIVE}, 
	content_type{TEXT}, length{default_body_length}{
};

httpresponse::~httpresponse(){
	filename.clear();
	body.clear();
};

void httpresponse::solverequestline(httpparser &parser) {
	switch (parser.getProtocol()) {
	case HTTP_11:{
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
	}
	default: {
		filename = "505.html";
		state = HTTP_VERSION_NOT_SUPPORTED;
		content_type = TEXT;
		connection = CLOSE;
		break;
	}
	}
}

void httpresponse::solvepath(const std::string &str) {
	if (str == "/"){
		filename = "index.html";
		state = OK;
		content_type = TEXT;

		length = default_body_length;
	}
	else if (str == "/test"){
		filename = "204.html";
		state = ACCEPTED;
		content_type = TEXT;

		length = default_body_length;
	}
	else if (str == "/login?text=test") {
		// todo: parse action?body
		filename = "page2/gettest.html";
		state = OK;
		content_type = TEXT;

		length = default_body_length;
	} else {
		filename = str.substr(1);
		if (!cachetree::getInstance().static_file_exist(filename)) {
			filename = "404.html";
			state = NOT_FOUND;
			content_type = TEXT;

			length = default_body_length;
		}
		else state = OK;
		if(state == OK){
			if(filename.find(".html") != std::string::npos)
				content_type = TEXT;
			else if(filename.find(".ico") != std::string::npos)
				content_type = ICON;
			else if(filename.find(".gif") != std::string::npos)
				content_type = GIF;
			else if(filename.find(".png") != std::string::npos)
				content_type = PNG;
			else if(filename.find(".jpeg") != std::string::npos)
				content_type = JPEG;
			else if(filename.find(".audio") != std::string::npos)
				content_type = AUDIO;
			else if(filename.find(".video") != std::string::npos)
				content_type = VIDEO;
			else if(filename.find(".json") != std::string::npos)
				content_type = JSON;
			else if(filename.find(".js") != std::string::npos)
				content_type = SCRIPT;
			else
				content_type = OTHER;
		}
	}
}

void httpresponse::addheader(std::string *response) {
	response->append("HTTP/1.1 ").append(STATUS_STR.at(state));
	response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
	response->append("Server: localhost:6780\r\n");
	response->append("Content-Length: ")
		.append(std::to_string(length))
		.append("\r\n")
	;
	response->append("Content-Type: ")
		.append(CONTENNT_TYPE_STR.at(content_type))
		.append(" charset=utf-8\r\n")
	;
	response->append(CONNECTION_STR.at(connection));
	response->append("\r\n");
}

void httpresponse::autoresponse(httpparser &parser, char *buf) {
	std::string *response = new std::string();
	response->reserve(default_header_length);
	memset(buf, 0, BUFSIZ);

	if(parser["connection"] == "keep-alive\r"){
		connection = KEEP_ALIVE;
	}
	else{
		connection = CLOSE;
	}

	solverequestline(parser);

	addheader(response);	
	strcpy(buf, response->c_str());	

	writefile(buf, response->size(), filename);
	
	delete response;
}

void httpresponse::reset() {
	filename.clear();
	state = NOT_FOUND;
	connection = NO_CONNECTION;
	content_type = TEXT;
	length = default_body_length;
}

} // namespace vastina