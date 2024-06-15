#include "httpresponse.hpp"
#include "config.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <format>
#include <string>
#include <unistd.h>
#include <utility>
#include <sys/uio.h>
#include <vector>

namespace vastina {

bool cachetree::static_file_exist( const std::string& str )
{
  return file_content.contains( str );
}

void cachetree::init_read( const fs::path& directory )
{
  for ( const auto& entry : fs::recursive_directory_iterator( directory ) ) {
    fs::path relativePath = fs::relative( entry.path(), directory );
    file_content.insert( std::make_pair<std::string, fileview>( relativePath.string(), { nullptr, 0 } ) );
  }
}

httpresponse::httpresponse()
  : filename { "index.html" }, state { OK }, connection { KEEP_ALIVE }, content_type { TEXT } {};

httpresponse::~httpresponse()
{
  filename.clear();
  body.clear();
};

void httpresponse::solveRequest( const httpparser& parser )
{
  if ( parser["connection"] == "close\r" ) {
    connection = CLOSE;
  } else {
    connection = KEEP_ALIVE;
  }
  switch ( parser.getProtocol() ) {
    case HTTP_11: {
      switch ( parser.getMethod() ) {
        case GET:
          solvepath( parser.getPath() );
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

void httpresponse::solvepath( const std::string& str )
{
  if ( str == "/" ) {
    filename = "index.html";
    state = OK;
    content_type = TEXT;
  } else if ( str == "/test" ) {
    filename = "204.html";
    state = ACCEPTED;
    content_type = TEXT;
  } else if ( str == "/login?text=test" ) {
    // todo: parse action?body
    filename = "page2/gettest.html";
    state = OK;
    content_type = TEXT;
  } else {
    filename = str.substr( 1 );
    if ( !cachetree::getInstance().static_file_exist( filename ) ) {
      filename = "404.html";
      state = NOT_FOUND;
      content_type = TEXT;
    } else
      state = OK;
    if ( state == OK ) {
      if ( filename.find( ".html" ) != std::string::npos )
        content_type = TEXT;
      else if ( filename.find( ".ico" ) != std::string::npos )
        content_type = ICON;
      else if ( filename.find( ".gif" ) != std::string::npos )
        content_type = GIF;
      else if ( filename.find( ".png" ) != std::string::npos )
        content_type = PNG;
      else if ( filename.find( ".jpeg" ) != std::string::npos )
        content_type = JPEG;
      else if ( filename.find( ".audio" ) != std::string::npos )
        content_type = AUDIO;
      else if ( filename.find( ".video" ) != std::string::npos )
        content_type = VIDEO;
      else if ( filename.find( ".json" ) != std::string::npos )
        content_type = JSON;
      else if ( filename.find( ".js" ) != std::string::npos )
        content_type = SCRIPT;
      else
        content_type = OTHER;
    }
  }
}

static std::string formatRFC7231()
{
  auto time { std::time( nullptr ) };
  auto tm { *std::gmtime( &time ) }; // 获取UTC时间
  tm.tm_hour += 8;                   // 东八区
  std::ostringstream oss {};
  oss << std::put_time( &tm, "%a, %d %b %Y %H:%M:%S GMT" );
  return oss.str();
}

void httpresponse::addheader( std::vector<char>& resp )
{
  const auto response = std::format( ""
                                     "HTTP/1.1 {}\r\n"
                                     "Date: {}\r\n"
                                     "Server: 127.0.0.1\r\n"
                                     "Content-Length: {}\r\n"
                                     "Content-Type: {} charset=utf-8\r\n"
                                     "{}"
                                     "\r\n\r\n",
                                     STATUS_STR.at( state ),
                                     // config::port,
                                     formatRFC7231(),
                                     fs::file_size( filename ),
                                     CONTENNT_TYPE_STR.at( content_type ),
                                     CONNECTION_STR.at( connection ) );
  for ( const auto ch : response )
    resp.push_back( ch );
}

void httpresponse::makeresponse( const httpparser& parser, int fd )
{
  solveRequest( parser );
  std::vector<char> response {};
  addheader( response );
  ::write( fd, (void*)response.data(), response.size() );
  auto& view { cachetree::getInstance().getfilecontent( filename ) };
  ::write( fd, view.text, view.length );
  // writev not work, why
  //::writev( fd, iv, 2 );
}

void httpresponse::reset()
{
  filename.clear();
  state = NOT_FOUND;
  connection = NO_CONNECTION;
  content_type = TEXT;
}

} // namespace vastina