/*
 * File:   TestTypeTraits.h
 * Author: mdl
 *
 * Created on 2011-9-25, 12:59:56
 */

#ifndef TESTTYPETRAITS_H
#define	TESTTYPETRAITS_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#include "../../NsLib/NsInternalUse/NsTypeTraits.h"

using namespace NsLib;

class TypeTraitsClass
{
};

class TestTypeTraits : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTypeTraits);

    CPPUNIT_TEST(testTraits);

    CPPUNIT_TEST_SUITE_END();

public:
    TestTypeTraits();
    virtual ~TestTypeTraits();
    void setUp();
    void tearDown();

private:
    void testTraits();
    
    bool typeTraitsResult(True_ type)
    {
        return true;
    }
    
    bool typeTraitsResult(False_ type)
    {
        return false;
    }
};

#endif	/* TESTTYPETRAITS_H */

