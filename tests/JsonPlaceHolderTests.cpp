/*
 * File:   JsonPlaceHolderTests.cpp
 * Author: mayur
 *
 * Created on 16 Jul, 2016, 11:58:57 AM
 */

#include "JsonPlaceHolderTests.h"

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
        std::cout<<std::endl<<"Request id for GET request is "<<rID<<std::endl;
        while(1){
            if(ob.responseAvailable()){
                res = ob.getResponse();
                if(res)
                    resultString = res->getResponseString();
                break;
            }
            else{
                std::cout<<"I am doing some other work"<<std::endl;
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