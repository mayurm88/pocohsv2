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



enum class reqType {GET, GETID, POST, UPDATE, DELETE};


class Request {
public:
    Request(int id, std::string postTitle, std::string postBody, int uID, reqType rType) :
    postID(id),
    title(postTitle),
    body(postBody),
    userID(uID),
    _reqType(rType)
    {
    }
    Request(std::string postTitle, std::string postBody, int uID, reqType rType) :
    title(postTitle),
    body(postBody),
    userID(uID),
    _reqType(rType)
    {
        postID = -1;
    }
    Request(int id, reqType rType) :
    postID(id),
    _reqType(rType)
    {
        title = "";
        body = "";
        userID = -1;
    }
    Request(reqType rType):
    _reqType(rType)
    {
        postID = -1;
        userID = -1;
        title = "";
        body = "";
    }
    reqType getRequestType(void);
private:
    int postID;
    int userID;
    std::string title;
    std::string body;
    reqType _reqType;
};

/*
class RequestNotification: public Poco::Notification{
public:
    typedef Poco::AutoPtr<RequestNotification> Ptr;
    RequestNotification(Request* req);
    Request* getRequest() const;
private:
    Request* request;
};
*/


class Response {
public:
    void setResponseString(std::string);
    std::string getResponseString();
    void setReqID(int id);
    int getReqID();
private:
    int reqID;
    std::string resStr; // Response string if the user wants to convert to JSON.
};


class Observer{
public:
    bool responseAvailable();
    Response* getResponse();
    void setResponse(Response* r);
    void setResponseAvailable(bool);
    Poco::NotificationQueue responseQueue;
    static Poco::FastMutex observerMutex;
private:
    bool _responseAvailable;
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
    int doGet(int id, Observer&);
    virtual ~JsonPost();
private:
    static Poco::FastMutex reqIDMutex;
    int requestID;
};

#endif	/* JSONPOST_H */

