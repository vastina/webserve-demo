#include "httpresponse.hpp"

#include <cerrno>
#include <charconv>
#include <csignal>
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
  : filename { "index.html" }, state { NOT_FOUND }, connection {NO_CONNECTION }, content_type { TEXT } {};

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

void httpresponse::addheader( int fd )
{
  // const auto response = std::format( ""
  //                                    "HTTP/1.1 {}\r\n"
  //                                    "Date: {}\r\n"
  //                                    "Server: 127.0.0.1\r\n"
  //                                    "Content-Length: {}\r\n"
  //                                    "Content-Type: {} charset=utf-8\r\n"
  //                                    "{}"
  //                                    "\r\n\r\n",
  //                                    STATUS_STR.at( state ),
  //                                    // config::port,
  //                                    formatRFC7231(),
  //                                    fs::file_size( filename ),
  //                                    CONTENNT_TYPE_STR.at( content_type ),
  //                                    CONNECTION_STR.at( connection ) );
  // for ( const auto ch : response )
  //   resp.push_back( ch );
  ::write( fd, "HTTP/1.1 ", 9 );
  const auto& status_str { STATUS_STR.at( state ) };
  ::write( fd, status_str.data(), status_str.size() );
  ::write( fd, "\r\nDate: ", 8 );
  const auto& date_str { formatRFC7231() };
  ::write( fd, date_str.data(), date_str.size() );
  ::write(fd, "\r\nServer:127.0.0.1\r\nContent-Length: ", 36);
  char buf[6];
  auto [p, ec] = std::to_chars( buf, buf + 6, fs::file_size(filename));
  ::write(fd, buf, p - buf);
  ::write(fd, "\r\nContent-Type: ", 16);
  const auto& content_type_str { CONTENNT_TYPE_STR.at( content_type ) };
  ::write(fd, content_type_str.data(), content_type_str.size());
  ::write(fd, " charset=utf-8\r\n", 16);
  const auto& connection_str { CONNECTION_STR.at( connection ) };
  ::write(fd, connection_str.data(), connection_str.size());
  ::write(fd, "\r\n\r\n", 4);
}

std::string httpresponse::addheader() const
{
  return std::format( ""
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
}

void httpresponse::makeresponse( const httpparser& parser, int fd )
{
  solveRequest( parser );

  // 1
  // addheader( fd );
  // 2
  // std::vector<char> response {};  addheader( response );
  // 3
  std::string response { addheader() };
  while (true) {
    int res = ::write( fd, (void*)response.data(), response.size() );
    if ( res == -1 ) {
      return;
    } else break;
  }
  // 1和3差不多，2差不多得了

  auto& view { cachetree::getInstance().getfilecontent( filename ) };
  while (true) {
    int res = ::write( fd, view.text, view.length );
    if ( res == -1 ) {
      // if( SIGPIPE == errno )
      //   return;
      return;
    } else break;
  }
}

void httpresponse::reset()
{
  filename.clear();
  state = NOT_FOUND;
  connection = NO_CONNECTION;
  content_type = TEXT;
}

} // namespace vastina