#ifndef NS_MY_EXCEPTION_FUNCTION_H
#define	NS_MY_EXCEPTION_FUNCTION_H

#include <exception>

#include "NsMyException.h"

namespace NsLibTest
{

void throwMyException()
{
    throw ::NsLibTest::MyException();
}

void throwStdException()
{
    throw std::exception();
}

void throwNotEqualException()
{
    throw ::NsLib::UnitTest::NotEqualException("a", "b");
}

void nothrowException()
{

}

}

#endif

