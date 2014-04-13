#ifndef NS_UNIT_TEST_SOURCE_TRACER_H
#define NS_UNIT_TEST_SOURCE_TRACER_H

#include <iostream>
#include <sstream>

namespace NsLib
{

namespace UnitTest
{

#define NS_SOURCE_TRACER() ::NsLib::UnitTest::SourceTracer(__FILE__, __LINE__)

class SourceTracer
{
public:
    SourceTracer() :
        fileName_{""},
        lineNumber_{-1}
    {
    }

    SourceTracer(const std::string fileName, int lineNumber) :
        fileName_{fileName},
        lineNumber_{lineNumber}
    {
    }

    ~SourceTracer() {}

    SourceTracer(const SourceTracer &rhs) :
        fileName_{rhs.fileName_},
        lineNumber_{rhs.lineNumber_}
    {
    }

    const SourceTracer &operator =(const SourceTracer &rhs)
    {
        if (*this != rhs)
        {
            fileName_ = rhs.fileName_;
            lineNumber_ = rhs.lineNumber_;
        }

        return *this;
    }

    bool isValid() const
    {
        return (fileName_ != "") && (lineNumber_ >= 0);
    }

    std::string getFileName() const
    {
        return fileName_;
    }

    std::string getLineNumber() const
    {
        std::stringstream str;

        str << lineNumber_;

        return str.str();
    }

    bool operator ==(const SourceTracer &rhs) const
    {
        return fileName_ == rhs.fileName_ && lineNumber_ == rhs.lineNumber_;
    }

    bool operator !=(const SourceTracer &rhs) const
    {
        return !(*this == rhs);
    }

private:
    std::string     fileName_;
    int             lineNumber_;
};

}   // UnitTest

}   // NsLib

#endif