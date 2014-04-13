/*
 * File:   dummy.cpp
 * Author: mdl
 *
 * Created on 2011-10-5, 13:57:19
 */

#include "dummy.h"


CPPUNIT_TEST_SUITE_REGISTRATION(dummy);

dummy::dummy()
{
}

dummy::~dummy()
{
}

void dummy::setUp()
{
}

void dummy::tearDown()
{
}

void dummy::testMethod()
{
    CPPUNIT_ASSERT(false);
    CPPUNIT_ASSERT(true);
    CPPUNIT_ASSERT(false);

}

void dummy::testFailedMethod()
{
    CPPUNIT_ASSERT(true);
    CPPUNIT_ASSERT(false);
}

