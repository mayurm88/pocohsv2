/* 
 * File:   JsonPost.h
 * Author: mayur
 *
 * Created on 14 July, 2016, 2:47 PM
 */

#ifndef JSONPOST_H
#define	JSONPOST_H

#include "Poco/NotificationQueue.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/StreamCopier.h"
#include "Poco//Thread.h"


class Request {
public:
    int id;
    Poco::NotificationQueue* resQ;
    /*
    Request(int reqID, Poco::NotificationQueue* resQueue):
    id(reqID),
    resQ(resQueue)
    {
    }*/
};

class RequestNotification: public Poco::Notification{
public:
    typedef Poco::AutoPtr<RequestNotification> Ptr;
    RequestNotification(Request* req);
    Request* getRequest() const;
private:
    Request* request;
};


class Response {
public:
    void setResponseString(std::string);
    std::string getResponseString();
private:
    std::string resStr; // Response string if the user wants to convert to JSON.
};


class ResponseNotification: public Poco::Notification{
public:
    typedef Poco::AutoPtr<ResponseNotification> Ptr;
    ResponseNotification(Response* res);
    Response* getResponse();
private:
    Response* response;
};

static Poco::NotificationQueue reqeustQueue;


class JsonPost {
public:
    JsonPost();
    JsonPost(const JsonPost& orig);
    int doGet(int* id, Poco::NotificationQueue&);
    virtual ~JsonPost();
private:
};

#endif	/* JSONPOST_H */

