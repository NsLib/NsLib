#ifndef NS_UNIT_TEST_ASSERT_H
#define	NS_UNIT_TEST_ASSERT_H

#include "../NsInternalUse/NsCxx11Support.h"

#include <iostream>
#include <ostream>
#include <cmath>
#include <limits>

#include "NsUnitTestSourceTracer.h"
#include "NsUnitTestException.h"

namespace NsLib
{

namespace UnitTest
{

#define ASSERT_EQUAL_TRAITS_HACK(AssertType)                                   \
    template <>                                                                \
    class AssertEqualTraits<AssertType>                                        \
    {                                                                          \
    public:                                                                    \
        static bool equal(AssertType &x, AssertType &y)                        \
        {                                                                      \
            return std::abs(x - y)                                             \
                    <= std::numeric_limits<AssertType>::epsilon();             \
        }                                                                      \
                                                                               \
        static std::string toString(AssertType &x)                             \
        {                                                                      \
            std::stringstream str;                                             \
                                                                               \
            str << x;                                                          \
                                                                               \
            return str.str();                                                  \
        }                                                                      \
    }

template <typename AssertType>
class AssertEqualTraits
{
public:
    static bool equal(const AssertType &x, const AssertType &y)
    {
        return x == y;
    }

    static std::string toString(const AssertType &x)
    {
        std::stringstream str;

        str << x;

        return str.str();
    }
};

ASSERT_EQUAL_TRAITS_HACK(double);
ASSERT_EQUAL_TRAITS_HACK(const double);
ASSERT_EQUAL_TRAITS_HACK(volatile const double);
ASSERT_EQUAL_TRAITS_HACK(float);
ASSERT_EQUAL_TRAITS_HACK(const float);
ASSERT_EQUAL_TRAITS_HACK(volatile const float);

void failThrow(bool isFail,
               std::string assertType,
               std::string expression,
               std::string message,
               ::NsLib::UnitTest::SourceTracer
                    sourceTracer = NS_SOURCE_TRACER())
{
    if (isFail)
    {
        std::string msg;

        if ("" != message)
        {
            msg = "           " + assertType + " (" + expression + ")\n"
                  + "           Message: " + message + "\n"
                  + "           Line: " + sourceTracer.getLineNumber()
                  + "     "
                  + "FileName: " + sourceTracer.getFileName();
        }
        else
        {
            msg = "           " + assertType + " (" + expression + ")\n"
                  + "           Line: " + sourceTracer.getLineNumber()
                  + "     "
                  + "FileName: " + sourceTracer.getFileName();
        }

        throw ::NsLib::UnitTest::Exception(msg, sourceTracer);
    }
}

void failNotEqualThrow(bool isNotEqual,
                       std::string assertType,
                       std::string expected,
                       std::string actual,
                       std::string message = "",
                       ::NsLib::UnitTest::SourceTracer
                            sourceTracer = NS_SOURCE_TRACER())
{
    if (isNotEqual)
    {
        std::string msg;

        if ("" != message)
        {
            msg = "           " + assertType + " (" + expected +
                  ", " + actual + ", " + message + ")\n"
                  + "           Message: " + message + "\n"
                  + "           Line: " + sourceTracer.getLineNumber()
                  + "     "
                  + "FileName: " + sourceTracer.getFileName();
        }
        else
        {
            msg = "           " + assertType + " (" + expected +
                  ", " + actual +")\n"
                  + "           Line: " + sourceTracer.getLineNumber()
                  + "     "
                  + "FileName: " + sourceTracer.getFileName();
        }

        throw ::NsLib::UnitTest::NotEqualException(expected,
                                                   actual,
                                                   msg,
                                                   sourceTracer);
    }
}


template <typename AssertType>
void assertEqual(std::string assertType,
                 AssertType expected,
                 AssertType actual,
                 std::string message = "",
                 ::NsLib::UnitTest::SourceTracer
                    sourceTracer = NS_SOURCE_TRACER())
{
    if (!::NsLib::UnitTest::AssertEqualTraits<AssertType>::equal(expected,
                                                                 actual))
    {
        ::NsLib::UnitTest::failNotEqualThrow(true,
                            assertType,
                            AssertEqualTraits<AssertType>::toString(expected),
                            AssertEqualTraits<AssertType>::toString(actual),
                            message,
                            sourceTracer);
    }
}

}   // UnitTest

}   // NsLib

#endif

