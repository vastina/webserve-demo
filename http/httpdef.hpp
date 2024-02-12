#ifndef _HTTP_DEF_H_
#define _HTTP_DEF_H_

#include <unordered_map>
#include <string_view>

enum STATUS_CODE {
	CONTINUE = 100,
	SWITCHING_PROTOCOLS,            // 101 switching_protocols
	PROCESSING,
	OK = 200,                       // 200 ok
	CREATED,
	ACCEPTED,
	NON_AUTHORITATIVE_INFORMATION,
	NO_CONTENT,
	RESET_CONTENT,
	PARTIAL_CONTENT,
	MULTI_STATUS,
	ALREADY_REPORTED,
	IM_USED,
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY,
	FOUND,
	SEE_OTHER,
	NOT_MODIFIED,
	USE_PROXY,
	TEMPORARY_REDIRECT,
	BAD_REQUEST = 400,
	UNAUTHORIZED,
	PAYMENT_REQUIRED,
	FORBIDDEN,
	NOT_FOUND,                      // 404 not_fonud
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,
	PROXY_AUTHENTICATION_REQUIRED,
	REQUEST_TIMEOUT,
	CONFLICT,
	GONE,
	LENGTH_REQUIRED,
	PRECONDITION_FAILED,
	REQUEST_ENTITY_TOO_LARGE,
	REQUEST_URI_TOO_LONG,
	UNSUPPORTED_MEDIA_TYPE,
	REQUESTED_RANGE_NOT_SATISFIABLE,
	EXPECTATION_FAILED,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED,
	BAD_GATEWAY,
	SERVICE_UNAVAILABLE,
	GATEWAY_TIMEOUT,
	HTTP_VERSION_NOT_SUPPORTED      // 505 http_version_not_supported
};

const static std::unordered_map<int, const std::string_view> STATUS_STR = {
	{100, "100 Continue\r\n"},
	{101, "101 Switching Protocols\r\n"},
	{102, "102 Processing\r\n"},
	{200, "200 OK\r\n"},
	{201, "201 Created\r\n"},
	{202, "202 Accepted\r\n"},
	{203, "203 Non-Authoritative Information\r\n"},
	{204, "204 No Content\r\n"},
	{205, "205 Reset Content\r\n"},
	{206, "206 Partial Content\r\n"},
	{207, "207 Multi-Status\r\n"},
	{208, "208 Already Reported\r\n"},
	{226, "226 IM Used\r\n"},
	{300, "300 Multiple Choices\r\n"},
	{301, "301 Moved Permanently\r\n"},
	{302, "302 Found\r\n"},
	{303, "303 See Other\r\n"},
	{304, "304 Not Modified\r\n"},
	{305, "305 Use Proxy\r\n"},
	{307, "307 Temporary Redirect\r\n"},
	{400, "400 Bad Request\r\n"},
	{401, "401 Unauthorized\r\n"},
	{402, "402 Payment Required\r\n"},
	{403, "403 Forbidden\r\n"},
	{404, "404 Not Found\r\n"},
	{405, "405 Method Not Allowed\r\n"},
	{406, "406 Not Acceptable\r\n"},
	{407, "407 Proxy Authentication Required\r\n"},
	{408, "408 Request Timeout\r\n"},
	{409, "409 Conflict\r\n"},
	{410, "410 Gone\r\n"},
	{411, "411 Length Required\r\n"},
	{412, "412 Precondition Failed\r\n"},
	{413, "413 Request Entity Too Large\r\n"},
	{414, "414 Request-URI Too Long\r\n"},
	{415, "415 Unsupported Media Type\r\n"},
	{416, "416 Requested Range Not Satisfiable\r\n"},
	{417, "417 Expectation Failed\r\n"},
	{500, "500 Internal Server Error\r\n"},
	{501, "501 Not Implemented\r\n"},
	{502, "502 Bad Gateway\r\n"},
	{503, "503 Service Unavailable\r\n"},
	{504, "504 Gateway Timeout\r\n"},
	{505, "505 HTTP Version Not Supported\r\n"}};

enum CONTENT_TYPE {
	PURETEST,
	TEXT,
	ICON,
	GIF,
	PNG,
	JPEG,
	AUDIO,
	VIDEO,
	JSON,
	SCRIPT,
	OTHER
};

const static std::unordered_map<int, const std::string_view> CONTENNT_TYPE_STR = {
	{PURETEST, "text/plain"},
	{TEXT, "text/html"},
	{ICON, "image/x-icon"},
	{GIF, "image/gif"},
	{PNG, "image/png"},
	{JPEG, "image/jpeg"},
	{AUDIO, "audio"},
	{VIDEO, "video"},
	{JSON, "application/json"},
	{SCRIPT, "application/javascript"},
	{OTHER, "other"}
};//可能传送多种格式，所以前面不加”content-type“

enum VERSION { NOT_SUPPORTED = 0, HTTP_09 = 9, HTTP_10, HTTP_11 };

enum METHOD {
	INVALID = 0,
	GET = 1 << 0,
	POST = 1 << 1,
	DELETE = 1 << 2,
	PUT = 1 << 3,
	HEAD = 1 << 4,
	TRACE = 1 << 5,
	CONNECT = 1 << 6,
	OPTIONS = 1 << 7,
	PATCH = 1 << 8
};

enum CONNECTION { NO_CONNECTION , KEEP_ALIVE , CLOSE };

const static std::unordered_map<int, const std::string_view> CONNECTION_STR = {
    {KEEP_ALIVE, "Connection: keep-alive\r\n"},
    {CLOSE, "Connection: close\r\n"}
};

#endif // _HTTP_DEF_H_