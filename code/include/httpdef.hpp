#ifndef _HTTP_DEF_H_
#define _HTTP_DEF_H_

#include <unordered_map>
#include <string_view>

enum STATUS_CODE
{
  CONTINUE = 100,
  SWITCHING_PROTOCOLS, // 101 switching_protocols
  PROCESSING,
  OK = 200, // 200 ok
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
  NOT_FOUND, // 404 not_fonud
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
  HTTP_VERSION_NOT_SUPPORTED // 505 http_version_not_supported
};

const static std::unordered_map<int, const std::string_view> STATUS_STR
  = { { 100, "100 Continue" },
      { 101, "101 Switching Protocols" },
      { 102, "102 Processing" },
      { 200, "200 OK" },
      { 201, "201 Created" },
      { 202, "202 Accepted" },
      { 203, "203 Non-Authoritative Information" },
      { 204, "204 No Content" },
      { 205, "205 Reset Content" },
      { 206, "206 Partial Content" },
      { 207, "207 Multi-Status" },
      { 208, "208 Already Reported" },
      { 226, "226 IM Used" },
      { 300, "300 Multiple Choices" },
      { 301, "301 Moved Permanently" },
      { 302, "302 Found" },
      { 303, "303 See Other" },
      { 304, "304 Not Modified" },
      { 305, "305 Use Proxy" },
      { 307, "307 Temporary Redirect" },
      { 400, "400 Bad Request" },
      { 401, "401 Unauthorized" },
      { 402, "402 Payment Required" },
      { 403, "403 Forbidden" },
      { 404, "404 Not Found" },
      { 405, "405 Method Not Allowed" },
      { 406, "406 Not Acceptable" },
      { 407, "407 Proxy Authentication Required" },
      { 408, "408 Request Timeout" },
      { 409, "409 Conflict" },
      { 410, "410 Gone" },
      { 411, "411 Length Required" },
      { 412, "412 Precondition Failed" },
      { 413, "413 Request Entity Too Large" },
      { 414, "414 Request-URI Too Long" },
      { 415, "415 Unsupported Media Type" },
      { 416, "416 Requested Range Not Satisfiable" },
      { 417, "417 Expectation Failed" },
      { 500, "500 Internal Server Error" },
      { 501, "501 Not Implemented" },
      { 502, "502 Bad Gateway" },
      { 503, "503 Service Unavailable" },
      { 504, "504 Gateway Timeout" },
      { 505, "505 HTTP Version Not Supported" } };

enum CONTENT_TYPE
{
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

const static std::unordered_map<CONTENT_TYPE, const std::string_view> CONTENNT_TYPE_STR
  = { { CONTENT_TYPE::PURETEST, "text/plain" },
      { CONTENT_TYPE::TEXT, "text/html" },
      { CONTENT_TYPE::ICON, "image/x-icon" },
      { CONTENT_TYPE::GIF, "image/gif" },
      { CONTENT_TYPE::PNG, "image/png" },
      { CONTENT_TYPE::JPEG, "image/jpeg" },
      { CONTENT_TYPE::AUDIO, "audio" },
      { CONTENT_TYPE::VIDEO, "video" },
      { CONTENT_TYPE::JSON, "application/json" },
      { CONTENT_TYPE::SCRIPT, "application/javascript" },
      { CONTENT_TYPE::OTHER, "other" } }; // 可能传送多种格式，所以前面不加”content-type“

enum VERSION
{
  NOT_SUPPORTED = 0,
  HTTP_09 = 9,
  HTTP_10,
  HTTP_11
};

enum METHOD
{
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

enum CONNECTION
{
  NO_CONNECTION,
  KEEP_ALIVE,
  CLOSE
};

const static std::unordered_map<CONNECTION, const std::string_view> CONNECTION_STR
  = { { CONNECTION::KEEP_ALIVE, "Connection: keep-alive\r\n" }, { CONNECTION::CLOSE, "Connection: close\r\n" } };

#endif // _HTTP_DEF_H_