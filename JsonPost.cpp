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
    ResponseRunnable(int* id, NotificationQueue& q):
    reqID(id),
    resQueue(q)
    {
    }
    virtual void run(){
        try{
            std::string result;
            URI uri("http://jsonplaceholder.typicode.com/posts/" + std::to_string(*reqID));
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
                    resQueue.enqueueNotification(new ResponseNotification(res));
                }
        }
        catch(Exception& exc){
            std::cerr<< exc.displayText() << std::endl;
        }
    }
private:
    int* reqID;
    NotificationQueue& resQueue;
};


int JsonPost::doGet(int* id, NotificationQueue& queue){
    ResponseRunnable* runnable = new ResponseRunnable(id, queue);
    Thread* t = new Thread;
    t->start(*runnable);
    return 0;
}

JsonPost::JsonPost() {
}

JsonPost::JsonPost(const JsonPost& orig) {
}

JsonPost::~JsonPost() {
}

