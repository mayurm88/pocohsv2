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

bool Observer::responseAvailable(){
    return _responseAvailable;
}

void Observer::setResponseAvailable(bool flag){
    _responseAvailable = flag;
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
    ResponseRunnable(int pID, Observer& o, int rID):
    postID(pID),
    observer(o),
    reqID(rID)
    {
    }
    virtual void run(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/" + std::to_string(postID));
            std::string path(uri.getPathAndQuery());
            HTTPClientSession session(uri.getHost(), uri.getPort());
            HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
            HTTPResponse response;
                session.sendRequest(request);
                std::istream& rs = session.receiveResponse(response);
                if(response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK){
                    StreamCopier::copyToString(rs, result);
                    Response *res = new Response;
                    res->setResponseString(result);
                    res->setReqID(reqID);
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
    int postID;
    int reqID;
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
    ResponseRunnable* runnable = new ResponseRunnable(id, o, reqID);
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

