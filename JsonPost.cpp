/* 
 * File:   JsonPost.cpp
 * Author: mayur
 * 
 * Created on 14 July, 2016, 2:47 PM
 */

#include "JsonPost.h"


#include <iostream>
using Poco::NotificationQueue;
using Poco::URI;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPClientSession;
using Poco::StreamCopier;
using Poco::Notification;
using Poco::FastMutex;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Exception;

ResponseNotification::ResponseNotification(Response* res) {
    response = res;
}

void Response::setResponseString(std::string s){
    resStr = s;
}

std::string Response::getResponseString(){
    return resStr;
}

Response* ResponseNotification::getResponse(){
    return response;
}

void Response::setReqID(int id){
    reqID = id;
}

int Response::getReqID(){
    return reqID;
}

HTTPResponse::HTTPStatus Response::getHTTPStatus(){
    return status;
}

void Response::setHTTPStatus(Poco::Net::HTTPResponse::HTTPStatus st){
    status = st;
}

bool Observer::responseAvailable(){
    return _responseAvailable;
}

void Observer::setResponseAvailable(bool flag){
    _responseAvailable = flag;
}


reqType Request::getRequestType(){
    return _reqType;
}

int Request::getPostID() {
    return postID;
}

std::string Request::getPostTitle(){
    return title;
}

std::string Request::getPostBody(){
    return body;
}

int Request::getPostUserID(){
    return userID;
}


Response* Observer::getResponse() {
    Notification::Ptr pNf(responseQueue.waitDequeueNotification());
    if (pNf) {
        ResponseNotification::Ptr pResNf = pNf.cast<ResponseNotification>();
        if (pResNf) {
            Response* res = pResNf->getResponse();
            if(!responseQueue.size()){
                FastMutex::ScopedLock lock(observerMutex);
                _responseAvailable = false;
            }
            return res;
        } else
            return NULL;
    } else 
        return NULL;
}


/*

RequestNotification::RequestNotification(Request* req):
request(req)
{
}

Request* RequestNotification::getRequest() const
{
    return request;
}

 */

class ResponseRunnable : public Poco::Runnable{
public:
    ResponseRunnable(Request *req, int rID, Observer& o):
    request(req),
    reqID(rID),
    observer(o)
    {
    }
    virtual void run(){
        try{
            switch(request->getRequestType()){
                case (reqType::GETID):
                    runGetID();
                    break;
                case (reqType::GET):
                    runGet();
                    break;
                case (reqType::POST):
                    runPost();
                    break;
                case (reqType::UPDATE):
                    runUpdate();
                    break;
                case (reqType::DELETE):
                    runDelete();
                    break;
            }
        }
        catch(Exception& exc){
            std::cerr<< exc.displayText() << std::endl;
        }
    }
    
    void runGetID(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/" + std::to_string(request->getPostID()));
            std::string path(uri.getPathAndQuery());
            HTTPClientSession session(uri.getHost(), uri.getPort());
            HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
            HTTPResponse response;
            session.sendRequest(request);
            std::istream& rs = session.receiveResponse(response);
            if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
                StreamCopier::copyToString(rs, result);
                Response *res = new Response;
                res->setResponseString(result);
                res->setReqID(reqID);
                res->setHTTPStatus(HTTPResponse::HTTP_OK);
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            } else {
                Response *res = new Response;
                res->setHTTPStatus(response.getStatus());
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
        }
        catch(Exception& exc){
            std::cerr<< exc.displayText() << std::endl;
        }
    }
    
    void runGet(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/");
            std::string path(uri.getPathAndQuery());
            HTTPClientSession session(uri.getHost(), uri.getPort());
            HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
            HTTPResponse response;
            session.sendRequest(request);
            std::istream& rs = session.receiveResponse(response);
            if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
                StreamCopier::copyToString(rs, result);
                Response *res = new Response;
                res->setResponseString(result);
                res->setReqID(reqID);
                res->setHTTPStatus(HTTPResponse::HTTP_OK);
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            } else {
                Response *res = new Response;
                res->setHTTPStatus(response.getStatus());
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
        }        
        catch (Exception& exc) {
            std::cerr<< exc.displayText() << std::endl;
        }
    }
    
    void runPost(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/");
            std::string path(uri.getPathAndQuery());
            HTTPClientSession session(uri.getHost(), uri.getPort());
            session.setKeepAlive(true);
            HTTPRequest hrequest(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
            hrequest.setKeepAlive(true);
            hrequest.setContentType("application/x-www-form-urlencoded");
            std::string requestBody("title="+request->getPostTitle()+"&body="+request->getPostBody()+"&userID="+std::to_string(request->getPostUserID()));
            hrequest.setContentLength(requestBody.length());
            HTTPResponse response;
            std::ostream& ostreamSession = session.sendRequest(hrequest);
            ostreamSession << requestBody;
            std::istream& rs = session.receiveResponse(response);
            if(response.getStatus() == Poco::Net::HTTPResponse::HTTP_CREATED){
                StreamCopier::copyToString(rs, result);
                Response *res = new Response;
                res->setResponseString(result);
                res->setReqID(reqID);
                res->setHTTPStatus(HTTPResponse::HTTP_CREATED);
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
            else{
                Response *res = new Response;
                res->setHTTPStatus(response.getStatus());
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
        }
        catch(Exception& exc){
            std::cerr<< exc.displayText() << std::endl;
        }
    }
    
    void runUpdate(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/" + std::to_string(request->getPostID()));
            std::string path(uri.getPathAndQuery());
            HTTPClientSession session(uri.getHost(), uri.getPort());
            session.setKeepAlive(true);
            HTTPRequest hrequest(HTTPRequest::HTTP_PUT, path, HTTPMessage::HTTP_1_1);
            hrequest.setKeepAlive(true);
            hrequest.setContentType("application/x-www-form-urlencoded");
            std::string requestBody("id="+std::to_string(request->getPostID())+"&title="+request->getPostTitle()+"&body="+request->getPostBody()+"&userID="+std::to_string(request->getPostUserID()));
            hrequest.setContentLength(requestBody.length());
            HTTPResponse response;
            std::ostream& ostreamSession = session.sendRequest(hrequest);
            ostreamSession << requestBody;
            std::istream& rs = session.receiveResponse(response);
            if(response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK){
                StreamCopier::copyToString(rs, result);
                Response *res = new Response;
                res->setResponseString(result);
                res->setReqID(reqID);
                res->setHTTPStatus(HTTPResponse::HTTP_OK);
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            } 
            else {
                Response *res = new Response;
                res->setHTTPStatus(response.getStatus());
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
            
        }
        catch(Exception& exc){
            std::cerr<< exc.displayText() << std::endl;
        }
    }
    
    void runDelete(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/" + std::to_string(request->getPostID()) );
            std::string path(uri.getPathAndQuery());
            HTTPClientSession session(uri.getHost(), uri.getPort());
            HTTPRequest hrequest(HTTPRequest::HTTP_DELETE, path, HTTPMessage::HTTP_1_1);
            HTTPResponse response;
            session.sendRequest(hrequest);
            std::istream& rs = session.receiveResponse(response);
            if(response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK){
                Response *res = new Response;
                res->setReqID(reqID);
                res->setHTTPStatus(HTTPResponse::HTTP_OK);
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
            else {
                Response *res = new Response;
                res->setHTTPStatus(response.getStatus());
                observer.responseQueue.enqueueNotification(new ResponseNotification(res));
                {
                    FastMutex::ScopedLock lock(Observer::observerMutex);
                    observer.setResponseAvailable(true);
                }
            }
        }
        catch(Exception& exc){
            std::cerr<< exc.displayText() << std::endl;
        }
    }
private:
    int reqID;
    Request *request;
    Observer& observer;
};


FastMutex JsonPost::reqIDMutex;
FastMutex Observer::observerMutex;

int JsonPost::doGet(int id, Observer& o){
    int reqID;
    {
        FastMutex::ScopedLock lock(reqIDMutex);
        reqID = ++requestID;
    }
    Request *req = new Request(id, reqType::GETID);
    ResponseRunnable* runnable = new ResponseRunnable(req, reqID, o);
    Thread* t = new Thread;
    t->start(*runnable);
    return reqID;
}

int JsonPost::doGet(Observer& o){
    int reqID;
    {
        FastMutex::ScopedLock lock(reqIDMutex);
        reqID = ++requestID;
    }
    Request *req = new Request(reqType::GET);
    ResponseRunnable* runnable = new ResponseRunnable(req, reqID, o);
    Thread* t = new Thread;
    t->start(*runnable);
    return reqID;
}


int JsonPost::doPost(std::string title, std::string body, int userID, Observer& o){
    int reqID;
    {
        FastMutex::ScopedLock lock(reqIDMutex);
        reqID = ++requestID;
    }
    Request *req = new Request(title, body, userID, reqType::POST);
    ResponseRunnable* runnable = new ResponseRunnable(req, reqID, o);
    Thread* t = new Thread;
    t->start(*runnable);
    return reqID;
}


int JsonPost::doUpdate(int id, std::string title, std::string body, int userID, Observer& o){
    int reqID;
    {
        FastMutex::ScopedLock lock(reqIDMutex);
        reqID = ++requestID;
    }
    Request *req = new Request(id, title, body, userID, reqType::UPDATE);
    ResponseRunnable* runnable = new ResponseRunnable(req, reqID, o);
    Thread* t = new Thread;
    t->start(*runnable);
    return reqID;
}

int JsonPost::doDelete(int id, Observer& o){
    int reqID;
    {
        FastMutex::ScopedLock lock(reqIDMutex);
        reqID = ++requestID;
    }
    Request *req = new Request(id, reqType::DELETE);
    ResponseRunnable* runnable = new ResponseRunnable(req, reqID, o);
    Thread* t = new Thread;
    t->start(*runnable);
    return reqID;
}

JsonPost::JsonPost() {
    requestID = 0;
}

JsonPost::JsonPost(const JsonPost& orig) {
}

JsonPost::~JsonPost() {
}

