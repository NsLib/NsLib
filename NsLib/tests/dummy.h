/*
 * File:   dummy.h
 * Author: mdl
 *
 * Created on 2011-10-5, 13:57:18
 */

#ifndef DUMMY_H
#define	DUMMY_H

#include <cppunit/extensions/HelperMacros.h>

class dummy : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(dummy);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    dummy();
    virtual ~dummy();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* DUMMY_H */

