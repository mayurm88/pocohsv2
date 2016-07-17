/*
 * File:   JsonPlaceHolderTests.h
 * Author: mayur
 *
 * Created on 16 Jul, 2016, 11:58:56 AM
 */

#ifndef JSONPLACEHOLDERTESTS_H
#define	JSONPLACEHOLDERTESTS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../JsonPost.h"

class JsonPlaceHolderTests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(JsonPlaceHolderTests);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);
    CPPUNIT_TEST(testGetID);
    CPPUNIT_TEST(testPost);

    CPPUNIT_TEST_SUITE_END();

public:
    JsonPlaceHolderTests();
    virtual ~JsonPlaceHolderTests();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
    void testGetID();
    void testPost();
};

#endif	/* JSONPLACEHOLDERTESTS_H */

