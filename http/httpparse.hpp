#ifndef _HTTP_PARSE_H_
#define _HTTP_PARSE_H_

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <chrono>

constexpr enum VERSION{
	 HTTP_09 = 9
	,HTTP_10
	,HTTP_11	
};

constexpr enum METHOD{
     GET = 1<<0
    ,POST = 1<<1
    ,DELETE = 1<<2
    ,PUT = 1<<3
    ,HEAD = 1<<4
    ,TRACE = 1<<5
    ,CONNECT = 1<<6
    ,OPTIONS = 1<<7
    ,PATCH = 1<<8
};

constexpr enum CONNECTION{
	 KEEP_ALIVE = 0
	,CLOSE
};

constexpr enum TRANSFER_ENCODING{
	 CHUNKED
	,COMPRESS
	,DEFLATE
	,GZIP
	,IDENTITY
};

constexpr enum CONTENT_TYPE{
	 PURETEST
	,TEXT
	,GIF
	,PNG
	,JPEG
	,AUDIO
	,VIDEO
	,JSON
	,SCRIPT
	,OTHER
};

const std::unordered_map<int, const std::string_view> CONTENNT_TYPE_STR = {
	 {PURETEST, "text/plain"}
	,{TEXT, "text/html"}
	,{GIF, "image/gif"}
	,{PNG, "image/png"}
	,{JPEG, "image/jpeg"}
	,{AUDIO, "audio"}
	,{VIDEO, "video"}
	,{JSON, "application/json"}
	,{SCRIPT, "application/javascript"}
	,{OTHER, "other"}
};

namespace vastina {

class httpparser {

  private:
  	METHOD method;
	VERSION version;
	CONNECTION connection;
	std::string path;
	std::string body;

	std::map<std::string, std::string> headers;

	std::string format_key(std::string &str);
  public:
	httpparser() : headers{} {};
	~httpparser(){};
	void autoparse(const char *buf);
	void autoparse(std::string &buf);
	void parsemethod(const std::string &buf);
	void parseversion(const std::string &buf);

	void reset();

	std::string operator[](std::string str);
	std::string operator[](std::string &str);

	METHOD getMethod();
	const std::string& getPath();
	VERSION getProtocol();
	std::string getBody();
};

void httpparser::reset() { headers.clear(); }

METHOD httpparser::getMethod() { return method; }
const std::string& httpparser::getPath() { return path; }
VERSION httpparser::getProtocol() { return version; }
std::string httpparser::getBody() { return body; }

void httpparser::parsemethod(const std::string &tmp) {
	switch (tmp[0])
	{
	case 'G':
		if(tmp[1] == 'E' && tmp[2] == 'T')
			method = GET;
		break;
	case 'P':
		switch (tmp[1])
		{
		case 'O':
			if(tmp[2] == 'S' && tmp[3] == 'T')
				method = POST;
			break;
		case 'U':
			if(tmp[2] == 'T')
				method = PUT;
			break;
		case 'A':
			if(tmp[2] == 'T' && tmp[3] == 'C' && tmp[4] == 'H')
				method = PATCH;
			break;
		default:
			break;
		}
		break;
	case 'D':
		if(tmp[1] == 'E' && tmp[2] == 'L' && tmp[3] == 'E' && tmp[4] == 'T' && tmp[5] == 'E')
			method = DELETE;
		break;
	case 'H':
		if(tmp[1] == 'E' && tmp[2] == 'A' && tmp[3] == 'D')
			method = HEAD;
		break;
	case 'T':
		if(tmp[1] == 'R' && tmp[2] == 'A' && tmp[3] == 'C' && tmp[4] == 'E')
			method = TRACE;
		break;
	default:
		break;
	}
}

void httpparser::parseversion(const std::string &tmp) {
	if(tmp == "HTTP/1.1")
		version = HTTP_11;
	else if(tmp == "HTTP/1.0")
		version = HTTP_10;
	else if(tmp == "HTTP/0.9")
		version = HTTP_09;
}

void httpparser::autoparse(const char *buf) {
auto t1 = std::chrono::high_resolution_clock::now();

	std::istringstream buf_stream(buf);
	int part = 0;
	std::string line;
	while (getline(buf_stream, line)) {
		switch (part) { // 0: request line, 1: header, 2: body
		case 0: {
			std::istringstream line_stream(line);
			std::string tmp;
			line_stream >> tmp;
			if (tmp.find("HTTP") == std::string::npos) {
				parsemethod(tmp);
				line_stream >> path;
				line_stream >> tmp;
				parseversion(tmp);
			} else {
				headers.insert(std::make_pair("version", tmp));
				line_stream >> tmp;
				headers.insert(std::make_pair("status", tmp));
				line_stream >> tmp;
				headers.insert(std::make_pair("status_text", tmp));
			}
			part = 1;
			break;
		}
		case 1: {
			if (line.size() == 1) {
				part = 2;
				break;
			}
			auto pos = line.find(":");
			if (pos == std::string::npos)
				continue;
			std::string tmp1(line, 0, pos);
			std::string tmp2(line, pos + 2);
			headers.insert(std::make_pair(format_key(tmp1), tmp2));
			break;
		}
		case 2: {
			body.append(line);
			body.push_back('\n');
			break;
		}
		default:
			break;
		}
	}

auto t2 = std::chrono::high_resolution_clock::now();
std::cout << "parse time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << '\n';
}

void httpparser::autoparse(std::string &buf) { autoparse(buf.c_str()); }

std::string httpparser::operator[](std::string str) {
	auto it = headers.find(str);
	return it != headers.end() ? it->second : "";
}

std::string httpparser::operator[](std::string &str) {
	auto it = headers.find(str);
	return it != headers.end() ? it->second : "";
}

std::string httpparser::format_key(std::string &str) {
	if (str[0] >= 'A' && str[0] <= 'Z') {
		str[0] = str[0] - 'A' + 'a';
	}
	unsigned long position = 0;
	while ((position = str.find("-", position)) != std::string::npos) {
		if (str[position + 1] >= 'A' && str[position + 1] <= 'Z') {
			str[position + 1] = str[position + 1] - 'A' + 'a';
		}
		position++;
	}
	return str;
}

} // namespace vastina

#endif