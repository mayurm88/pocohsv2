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


/*
 * Use of enum class requires a compiler that support C++11 standard
 * this is the enum type of request type required while creating request objects.
 */

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
    int getPostID();
    int getPostUserID();
    std::string getPostTitle();
    std::string getPostBody();
private:
    int postID;
    int userID;
    std::string title;
    std::string body;
    reqType _reqType;
};



class Response {
public:
    void setResponseString(std::string);
    std::string getResponseString();
    void setReqID(int id);
    int getReqID();
    Poco::Net::HTTPResponse::HTTPStatus getHTTPStatus();
    void setHTTPStatus(Poco::Net::HTTPResponse::HTTPStatus);
private:
    int reqID;
    Poco::Net::HTTPResponse::HTTPStatus status;
    std::string resStr; // Response string if the user wants to convert to JSON.
};


/*
 * Observer class should ideally be one per thread. The thread can create an observer and wait for the responses in the responseQueue.
 * Thread has to pass the observer reference in each of the calls to the library.
 * 
 * If the response is available in the queue, responseAvailable will return true and the user can avail the pointer to the response object
 * via getResponse call.
 */
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

/*
 * The ResponseNotification which gets queued in the Observer's Notification Queue.
 */
class ResponseNotification: public Poco::Notification{
public:
    typedef Poco::AutoPtr<ResponseNotification> Ptr;
    ResponseNotification(Response* res);
    Response* getResponse();
private:
    Response* response;
};

static Poco::NotificationQueue reqeustQueue;


/*
 * Actual library interface for GET/POST/PUT/DELETE REST calls. In each of the method's Observer reference is required to queue the responses in the
 * observer's Notification Queue.
 */

class JsonPost {
public:
    JsonPost();
    JsonPost(const JsonPost& orig);
    int doGet(int id, Observer&);
    int doGet(Observer&);
    int doPost(std::string title, std::string body, int userID, Observer&);
    int doUpdate(int id, std::string title, std::string body, int userID, Observer&);
    int doDelete(int id, Observer&);
    virtual ~JsonPost();
private:
    static Poco::FastMutex reqIDMutex;
    int requestID;
};

#endif	/* JSONPOST_H */

