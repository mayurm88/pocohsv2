/*
 * File:   JsonPlaceHolderTests.cpp
 * Author: mayur
 *
 * Created on 16 Jul, 2016, 11:58:57 AM
 */

#include <Poco/Random.h>

#include "JsonPlaceHolderTests.h"
#include "Poco/ThreadLocal.h"

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
using Poco::ThreadLocal;
using Poco::Exception;


class TestRunnable : public Poco::Runnable{
public:
    TestRunnable(Observer& ob, JsonPost& _jp, int _customSeed, std::string name, bool *res):
    o(ob),
    jp(_jp),
    customSeed(_customSeed),
    _name(name),
    result(res)
    {
        random.seed(customSeed);
    }
    virtual void run(){
        requests = getRandom100();
        {
            FastMutex::ScopedLock lock(printMutex);
            std::cout<<_name<<" will make "<<requests<<" requests."<<std::endl;
        }
        for(int i = 0; i < requests; i++){
            method = getMethod();
            int reqID;
            std::string methodString;
            switch(method){
                case 1:
                    reqID = jp.doGet(getRandom100(), o);
                    methodString = "GETID";
                    break;
                case 2:
                    reqID = jp.doGet(o);
                    methodString = "GETALL";
                    break;
                case 3:
                    reqID = jp.doPost("mytitle", "mybody", getRandom100(), o);
                    methodString = "POST";
                    break;
                case 4:
                    reqID = jp.doUpdate(getRandom100(), "mytitle", "mybody", getRandom100(), o);
                    methodString = "UPDATE";
                    break;
                case 5:
                    reqID = jp.doDelete(getRandom100(), o);
                    methodString = "DELETE";
                    break;
            }
            {
                FastMutex::ScopedLock lock(printMutex);
                std::cout<<_name<<" sent "<<methodString<<" request and got request id : "<<reqID<<std::endl;
            }
        }
        while(requests > 0){
            if(o.responseAvailable()){
                Response *res = o.getResponse();
                {
                    FastMutex::ScopedLock lock(printMutex);
                    std::cout<<_name<<" got response for request ID : "<<res->getReqID()<<" and the response status is : "<<res->getHTTPStatus()<<std::endl;
                }
                (requests)--;
            }
            else{
                Thread::sleep(20);
            }
        }
        Thread::sleep(500);
        if(o.responseAvailable())
            *result = false;
        else
            *result = true;
    }
    int getRandom100(){
        return (random.next(100) + 1);
    }
    int getMethod(){
        return (random.next(5) + 1);
    }
private:
    Observer& o;
    JsonPost& jp;
    int method;
    int requests;
    int customSeed;
    Poco::Random random;
    std::string _name;
    bool* result;
    static FastMutex printMutex;
};


FastMutex TestRunnable::printMutex;

CPPUNIT_TEST_SUITE_REGISTRATION(JsonPlaceHolderTests);

JsonPlaceHolderTests::JsonPlaceHolderTests() {
}

JsonPlaceHolderTests::~JsonPlaceHolderTests() {
}

void JsonPlaceHolderTests::setUp() {
}

void JsonPlaceHolderTests::tearDown() {
}

void JsonPlaceHolderTests::testMethod() {
    CPPUNIT_ASSERT(true);
}

void JsonPlaceHolderTests::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

void JsonPlaceHolderTests::testGetID(){
    JsonPost jp;
    Observer ob;
    std::string testString, resultString="";
    int rID;
    int id;
    Response* res;
    testString = "{\n  \"userId\": 1,\n  \"id\": 10,\n  \"title\": \"optio molestias id quia eum\",\n  \"body\": \"quo et expedita modi cum officia vel magni\\ndoloribus qui repudiandae\\nvero nisi sit\\nquos veniam quod sed accusamus veritatis error\"\n}";
    try{    
        id = 10;
        rID = jp.doGet(id, ob);
        std::cout<<std::endl<<"Request id for GETID request is "<<rID<<std::endl;
        while(1){
            if(ob.responseAvailable()){
                res = ob.getResponse();
                if(res && res->getHTTPStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
                    resultString = res->getResponseString();
                else
                    resultString = "";
                break;
            }
            else{
                //std::cout<<"I am doing some other work"<<std::endl;
                Thread::sleep(10);
            }
        }
    }
    catch(Exception& e){
            std::cerr<<e.displayText()<<std::endl;
    }
    std::cout<<"Received response for request ID : "<<res->getReqID()<<std::endl;
    if(!testString.compare(resultString))
        CPPUNIT_ASSERT(true);
    else
        CPPUNIT_ASSERT(false);
}


void JsonPlaceHolderTests::testPost(){
    JsonPost jp;
    Observer ob;
    std::string testString, resultString="";
    int rID;
    Response* res;
    std::string title = "mytitle";
    std::string body = "mybody";
    int userID = 5;
    testString = "{\n  \"title\": \""+title+"\",\n  \"body\": \""+body+"\",\n  \"userID\": "+std::to_string(userID)+",\n  \"id\": 101\n}";
    try{    
        rID = jp.doPost(title, body, userID, ob);
        std::cout<<std::endl<<"Request id for POST request is "<<rID<<std::endl;
        while(1){
            if(ob.responseAvailable()){
                res = ob.getResponse();
                if(res && res->getHTTPStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_CREATED)
                    resultString = res->getResponseString();
                else
                    resultString = "";
                break;
            }
            else{
                //std::cout<<"I am doing some other work"<<std::endl;
                Thread::sleep(10);
            }
        }
    }
    catch(Exception& e){
            std::cerr<<e.displayText()<<std::endl;
    }
    std::cout<<"Received response for request ID : "<<res->getReqID()<<std::endl;
    //std::cout<<"Response is : "<<resultString<<std::endl;
    //std::cout<<"testString is : "<<testString<<std::endl;
    if(!testString.compare(resultString))
        CPPUNIT_ASSERT(true);
    else
        CPPUNIT_ASSERT(false);
}



void JsonPlaceHolderTests::testUpdate(){
    JsonPost jp;
    Observer ob;
    std::string testString, resultString="";
    int rID;
    int postID = 89;
    Response* res;
    std::string title = "mytitle";
    std::string body = "mybody";
    int userID = 69;
    testString = "{\n  \"id\": "+std::to_string(postID)+",\n  \"title\": \""+title+"\",\n  \"body\": \""+body+"\",\n  \"userID\": "+std::to_string(userID)+"\n}";
    try{    
        rID = jp.doUpdate(postID, title, body, userID, ob);
        std::cout<<std::endl<<"Request id for UPDATE request is "<<rID<<std::endl;
        while(1){
            if(ob.responseAvailable()){
                res = ob.getResponse();
                if(res && res->getHTTPStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
                    resultString = res->getResponseString();
                else
                    resultString = "";
                break;
            }
            else{
                //std::cout<<"I am doing some other work"<<std::endl;
                Thread::sleep(10);
            }
        }
    }
    catch(Exception& e){
            std::cerr<<e.displayText()<<std::endl;
    }
    std::cout<<"Received response for request ID : "<<res->getReqID()<<std::endl;
    //std::cout<<"Response is : "<<resultString<<std::endl;
    //std::cout<<"testString is : "<<testString<<std::endl;
    if(!testString.compare(resultString))
        CPPUNIT_ASSERT(true);
    else
        CPPUNIT_ASSERT(false);
}


void JsonPlaceHolderTests::testDelete(){
    JsonPost jp;
    Observer ob;
    int rID;
    int id;
    bool deleteSuccessful = false;
    Response* res;
    try{    
        id = 88;
        rID = jp.doDelete(id, ob);
        std::cout<<std::endl<<"Request id for DELETE request is "<<rID<<std::endl;
        while(1){
            if(ob.responseAvailable()){
                res = ob.getResponse();
                if(res->getHTTPStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
                    deleteSuccessful = true;
                else
                    deleteSuccessful = false;
                break;
            }
            else{
                //std::cout<<"I am doing some other work"<<std::endl;
                Thread::sleep(10);
            }
        }
    }
    catch(Exception& e){
            std::cerr<<e.displayText()<<std::endl;
    }
    std::cout<<"Received response for request ID : "<<res->getReqID()<<std::endl;
    CPPUNIT_ASSERT(deleteSuccessful);
}


void JsonPlaceHolderTests::testGet(){
    JsonPost jp;
    Observer ob;
    int rID;
    bool getSuccessful = false;
    Response* res;
    try{    
        rID = jp.doGet(ob);
        std::cout<<std::endl<<"Request id for GET request is "<<rID<<std::endl;
        while(1){
            if(ob.responseAvailable()){
                res = ob.getResponse();
                if(res->getHTTPStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
                    getSuccessful = true;
                else
                    getSuccessful = false;
                break;
            }
            else{
                //std::cout<<"I am doing some other work"<<std::endl;
                Thread::sleep(10);
            }
        }
    }
    catch(Exception& e){
            std::cerr<<e.displayText()<<std::endl;
    }
    std::cout<<"Received response for request ID : "<<res->getReqID()<<std::endl;
    //std::cout<<"Response string is : "<<std::endl;
    //std::cout<<res->getResponseString()<<std::endl;
    CPPUNIT_ASSERT(getSuccessful);
}


void JsonPlaceHolderTests::testThreads(){
    Observer o1, o2, o3;
    JsonPost jp;
    bool *t1result, *t2result, *t3result;
    t1result = new bool;
    t2result = new bool;
    t3result = new bool;
    *t1result = false;
    *t2result = false;
    *t3result = false;
    TestRunnable tr1(o1, jp, 998754, "Thread1", t1result);
    TestRunnable tr2(o2, jp, 554678, "Thread2", t2result);
    TestRunnable tr3(o3, jp, 789546, "Thread3", t3result);
    Thread t1, t2, t3;
    t1.start(tr1);
    t2.start(tr2);
    t3.start(tr3);
    t1.join();
    t2.join();
    t3.join();
    CPPUNIT_ASSERT(*t1result && *t2result && *t3result);
}