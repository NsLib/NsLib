#ifndef NS_UNIT_TEST_RESULT_H
#define NS_UNIT_TEST_RESULT_H

#include "../NsInternalUse/NsCxx11Support.h"

#include <iostream>
#include <vector>
#include <utility>

namespace NsLib
{

namespace UnitTest
{

// 每个testclass使用一个
class TestResult
{
public:
    typedef std::vector<std::pair<std::string, std::string> > FailTestCases;
    typedef std::vector<std::string>                          SuccessTestCases;

public:
    TestResult() :
        fialedCasesCount_{0},
        successedCasesCount_{0}
    {
        failedCases_.clear();
        successedCases_.clear();
    }

    ~TestResult()
    {
    }

    TestResult(const TestResult &rhs) :
        testName_{rhs.testName_},
        fialedCasesCount_{rhs.fialedCasesCount_},
        successedCasesCount_{rhs.successedCasesCount_},
        failedCases_{rhs.failedCases_},
        successedCases_{rhs.successedCases_}
    {
    }

    const TestResult &operator =(const TestResult &rhs)
    {
        if (this != &rhs)
        {
            testName_ = rhs.testName_;
            fialedCasesCount_ = rhs.fialedCasesCount_;
            successedCasesCount_ = rhs.successedCasesCount_;
            failedCases_ = rhs.failedCases_;
            successedCases_ = rhs.successedCases_;
        }

        return *this;
    }

    void setTestName(std::string testName)
    {
        testName_ = testName;
    }

    void addFailure(std::string funName, std::string failedMessage)
    {
        ++fialedCasesCount_;

        failedCases_.push_back(std::make_pair(funName, failedMessage));
    }

    void addSuccess(std::string funName)
    {
        ++successedCasesCount_;

        successedCases_.push_back(funName);
    }

    std::string getTestName()
    {
        return testName_;
    }

    int getTestCount()
    {
        return (fialedCasesCount_ + successedCasesCount_);
    }

    int getFailureCount()
    {
        return fialedCasesCount_;
    }

    int getSuccessCount()
    {
        return successedCasesCount_;
    }

    FailTestCases getFailures()
    {
        return failedCases_;
    }
    
    SuccessTestCases getSuccesses()
    {
        return successedCases_;
    }

private:
    std::string         testName_;
    int                 fialedCasesCount_;
    int                 successedCasesCount_;
    FailTestCases       failedCases_;
    SuccessTestCases    successedCases_;
};

class TestResultCollector
{
    MAKE_CLASS_UNCOPYABLE(TestResultCollector);

public:
    typedef std::vector< ::NsLib::UnitTest::TestResult>  TestResults;

public:
    TestResultCollector()
    {
        testResults_.clear();
    }

    ~TestResultCollector()
    {

    }

    void addTestResult(::NsLib::UnitTest::TestResult result)
    {
        testResults_.push_back(result);
    }

    TestResults getTestResult()
    {
        return testResults_;
    }

private:
    TestResults     testResults_;
};

}   // UnitTest

}   // NsLib

#endif