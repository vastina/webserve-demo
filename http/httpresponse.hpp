#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

namespace vastina{

enum STATUS{
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

class cachetree{
private:
    ;
public:
    cachetree();
    ~cachetree();
};


class httpresponse
{
private:
    ;
public:
    httpresponse();
    ~httpresponse();
};

httpresponse::httpresponse()
{
}

httpresponse::~httpresponse()
{
}


}





#endif // _HTTP_RESPONSE_H_