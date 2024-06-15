#ifndef _HTTP_PARSE_H_
#define _HTTP_PARSE_H_

#include <map>
#include <string>

#include "httpdef.hpp"

namespace vastina {

class httpparser
{

private:
  METHOD method;
  VERSION version;
  std::string path;
  // CONTENT_TYPE content_type;
  std::string body;

  std::map<std::string, std::string> headers;

  std::string format_key( std::string& str );

  void parsemethod( const std::string& buf );
  void parseversion( const std::string& buf );

public:
  httpparser();
  ~httpparser();

  void autoparse( const char* buf );
  void autoparse( const std::string& buf );
  enum parts
  {
    START_LINE,
    HEADERS,
    BODY
  };

  void reset();

  std::string operator[]( const std::string& str ) const;

  METHOD getMethod() const;
  const std::string& getPath() const;
  VERSION getProtocol() const;
  const std::string& getBody() const;
};

} // namespace vastina

#endif