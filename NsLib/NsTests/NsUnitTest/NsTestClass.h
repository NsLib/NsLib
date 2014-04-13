#ifndef NS_TEST_CLASS_H
#define	NS_TEST_CLASS_H

#include "../../NsUnitTest.h"
#include "NsMyExceptionFunction.h"

namespace NsLibTest
{

class TestClass : public ::NsLib::TestClassTemplate
{
    NS_TEST_BEGIN(TestClass)
        NS_TEST(testNS_ASSERT)
        NS_TEST(testNS_ASSERT_MESSAGE)
        NS_TEST(testNS_FAIL)
        NS_TEST(testNS_ASSERT_EQUAL)
        NS_TEST(testNS_ASSERT_EQUAL_MESSAGE)
        NS_TEST(testNS_ASSERT_THROW)
        NS_TEST(testNS_ASSERT_THROW_MESSAGE)
        NS_TEST(testNS_ASSERT_NO_THROW)
        NS_TEST(testNS_ASSERT_NO_THROW_MESSAGE)
    NS_TEST_END()

public:
    virtual void setUp()
    {
        std::cout << "setUp()" << std::endl;
    }

    virtual void tearDown()
    {
        std::cout << "tearDown()" << std::endl;
    }

    void testNS_ASSERT()
    {
        NS_ASSERT(true);
        NS_ASSERT(1 != 2);
        NS_ASSERT(1 == 2);
        NS_ASSERT(false);
    }

    void testNS_ASSERT_MESSAGE()
    {
        NS_ASSERT_MESSAGE(1 != 2, "This should be true");
        NS_ASSERT_MESSAGE(1 == 2, "This should be false");
    }

    void testNS_FAIL()
    {
        NS_FAIL("This should fail");
    }

    void testNS_ASSERT_EQUAL()
    {
        NS_ASSERT_EQUAL(1, 1);
        NS_ASSERT_EQUAL(1.0, 1.0);
        NS_ASSERT_EQUAL(1.1, 2.3);
    }

    void testNS_ASSERT_EQUAL_MESSAGE()
    {
        NS_ASSERT_EQUAL_MESSAGE(1, 1, "This should be true");
        NS_ASSERT_EQUAL_MESSAGE(1.0, 1.0, "This should be true");
        NS_ASSERT_EQUAL_MESSAGE(1.1, 2.3, "This should fail");
    }

    void testNS_ASSERT_THROW()
    {
        NS_ASSERT_THROW(::NsLibTest::throwMyException(),
                        ::NsLibTest::MyException);
        NS_ASSERT_THROW(::NsLibTest::throwStdException(),
                        std::exception);
        NS_ASSERT_THROW(::NsLibTest::throwNotEqualException(),
                        ::NsLibTest::MyException);
    }

    void testNS_ASSERT_THROW_MESSAGE()
    {
        NS_ASSERT_THROW_MESSAGE(::NsLibTest::throwMyException(),
                                ::NsLibTest::MyException,
                                "This exception should be caught");
        NS_ASSERT_THROW_MESSAGE(::NsLibTest::throwStdException(),
                                std::exception,
                                "This exception should be caught");
        NS_ASSERT_THROW_MESSAGE(::NsLibTest::throwNotEqualException(),
                                ::NsLibTest::MyException,
                                "This exception is wrong");
    }

    void testNS_ASSERT_NO_THROW()
    {
        NS_ASSERT_NO_THROW(::NsLibTest::nothrowException());
        NS_ASSERT_NO_THROW(::NsLibTest::throwNotEqualException());
    }

    void testNS_ASSERT_NO_THROW_MESSAGE()
    {
        NS_ASSERT_NO_THROW_MESSAGE(::NsLibTest::nothrowException(),
                                   "This should not throw any exception");
        NS_ASSERT_NO_THROW_MESSAGE(::NsLibTest::throwNotEqualException(),
                                   "This throw an exception");
    }
};

}

#endif

