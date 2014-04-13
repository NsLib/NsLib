/*
 * File:   TestObjectPool.h
 * Author: mdl
 *
 * Created on 2011-9-25, 20:15:29
 */

#ifndef TESTOBJECTPOOL_H
#define	TESTOBJECTPOOL_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#include "../../NsLib/NsPool.h"


class MyClassInObjectPool
{
    int     field_1_;
    double  field_2_;
    char    field_3_;
    double  field_4_;
};

class TestObjectPool : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestObjectPool);

    CPPUNIT_TEST(testCreate);
    CPPUNIT_TEST(testDestroy);
    CPPUNIT_TEST(testAllocate);
    CPPUNIT_TEST(testDeallocate);

    CPPUNIT_TEST_SUITE_END();

public:
    TestObjectPool();
    virtual ~TestObjectPool();
    void setUp();
    void tearDown();

private:
    void testCreate();
    void testDestroy();
    void testAllocate();
    void testDeallocate();

    MyClassInObjectPool *memPtr_;
};

#endif	/* TESTOBJECTPOOL_H */

