/*
 * File:   newtestclass.cpp
 * Author: mdl
 *
 * Created on 2011-10-5, 13:55:10
 */

#include "newtestclass.h"


CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

newtestclass::newtestclass()
{
}

newtestclass::~newtestclass()
{
}

void newtestclass::setUp()
{
}

void newtestclass::tearDown()
{
}

void newtestclass::testMethod()
{
    CPPUNIT_ASSERT(true);
}

void newtestclass::testFailedMethod()
{
    CPPUNIT_ASSERT(false);
}

