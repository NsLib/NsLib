#ifndef NS_UNIT_TEST_EXCEPTION_H
#define	NS_UNIT_TEST_EXCEPTION_H

#include "../NsInternalUse/NsCxx11Support.h"

#include <exception>

#include "NsUnitTestSourceTracer.h"

namespace NsLib
{

namespace UnitTest
{

class Exception : public std::exception
{
public:
    Exception(std::string  message = "",
	       ::NsLib::UnitTest::SourceTracer sourceTracer =  NS_SOURCE_TRACER()) :
           message_{message},
           sourceTracer_{sourceTracer}
    {
    }

    virtual ~Exception () throw() { }

    Exception(const Exception &obj) :
        message_{obj.message_},
        sourceTracer_{obj.sourceTracer_}
    {
    }

    const Exception &operator =(const Exception &rhs)
    {
        if (this != &rhs)
        {
            message_ = rhs.message_;
            sourceTracer_ = rhs.sourceTracer_;
        }

        return *this;
    }

    virtual const char* what() const throw()
    {
        return message_.c_str();
    }

    ::NsLib::UnitTest::SourceTracer sourceTracer() const
    {
        return sourceTracer_;
    }

    virtual std::string ExceptionName() const throw()
    {
        return "::NsLib::UnitTest::Exception";
    }

protected:
    std::string                         message_;
    ::NsLib::UnitTest::SourceTracer     sourceTracer_;
};


class NotEqualException : public ::NsLib::UnitTest::Exception
{
public:
    NotEqualException(std::string expected,
                      std::string actual,
                      std::string message = "",
                      ::NsLib::UnitTest::SourceTracer
                          sourceTracer = NS_SOURCE_TRACER()) :
                      Exception(message, sourceTracer),
                      expected_{expected},
                      actual_{actual}
    {

    }

    virtual ~NotEqualException() throw() { }

    NotEqualException(const NotEqualException &obj) :
        Exception(obj.message_, obj.sourceTracer_),
        expected_{obj.expected_},
        actual_{obj.actual_}
    {

    }

    NotEqualException &operator =(const NotEqualException &rhs)
    {
        if (this != &rhs)
        {
            expected_ = rhs.expected_;
            actual_ = rhs.actual_;
            message_ = rhs.message_;
            sourceTracer_ = rhs.sourceTracer_;
        }

        return *this;
    }

    std::string expectedValue() const
    {
        return expected_;
    }

    std::string actualValue() const
    {
        return actual_;
    }

    virtual std::string message() const
    {
        return message_;
    }

    virtual std::string ExceptionName() const throw()
    {
        return "::NsLib::UnitTest::NotEqualException";
    }

private:
    std::string       expected_;
    std::string       actual_;
};

}   // UnitTest

}   // NsLib

#endif

