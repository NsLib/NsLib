/*
 * File:   TestTypeTraits.cpp
 * Author: mdl
 *
 * Created on 2011-9-25, 12:59:56
 */

#include "TestTypeTraits.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestTypeTraits);

TestTypeTraits::TestTypeTraits()
{
}

TestTypeTraits::~TestTypeTraits()
{
}

void TestTypeTraits::setUp()
{
}

void TestTypeTraits::tearDown()
{
}

void TestTypeTraits::testTraits()
{
    bool res = typeTraitsResult(typename TypeTraits<char>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<signed char>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<unsigned char>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<wchar_t>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<unsigned wchar_t>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<short>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<unsigned short>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<int>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<unsigned int>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<long >::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<unsigned long>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<long long>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<unsigned long long>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<float>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<double>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<long double>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<int *>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
    res = typeTraitsResult(typename TypeTraits<TypeTraitsClass>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(false, res);
    res = typeTraitsResult(typename TypeTraits<TypeTraitsClass *>::IsPodType());
    CPPUNIT_ASSERT_EQUAL(true, res);
}
