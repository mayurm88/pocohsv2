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