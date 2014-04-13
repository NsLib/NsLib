#ifndef NS_UNIT_TEST_RUNNER_H
#define	NS_UNIT_TEST_RUNNER_H

#include "../NsInternalUse/NsCxx11Support.h"

#include "../NsUtility/NsUncopyale.h"
#include "../NsSynchronization/NsLock.h"
#include "NsUnitTestMessageLevel.h"

namespace NsLib
{

static ::NsLib::NsLcok nsThreadMutexForTestRunnerObject_;

/*!
 * \class   TestRunner  NsUnitTest.h
 * \brief   测试类收集器，并进行统一测试。
 * \ingroup NsUnitTest
 *
 * \details 所有的测试类实例都要加入到TestRunner中进行统一管理及测试。
 *
 * \note    线程安全。
 *
 * \pre     需要首先添加测试用例\link TestClassTemplate \endlink
 *
 * \code
 * // 示例：
 * class TestClass : public ::NsLib::TestClassTemplate
 * {
 *      // !!!参数需要指定为测试类的名称
 *      NS_TEST_BEGIN(TestClass)
 *          // 添加测试函数的名称
 *          NS_TEST(testMethodA)
 *          NS_TEST(testMethodB)
 *          NS_TEST(testMethodC)
 *          NS_TEST(testMethodD)
 *      // 不要忘记调用
 *      NS_TEST_END()
 *
 * public:
 *      void testMethodA() { //... }
 *      void testMethodB() { //... }
 *      void testMethodC() { //... }
 *      void testMethodD() { //... }
 *
 *      void setUp()
 *      {
 *          // 此函数会在测试开始前被自动调用，如果需要前置条件，请在此设置。
 *      }
 *
 *      void tearDown()
 *      {
 *          // 测试完成后自动调用，如果需要清理资源等，请在此进行。
 *      }
 * };
 *
 * int main()
 * {
 *      // 测试类实例。
 *      TestClass  a;
 *      TestClass  b;
 *      TestClass  c;
 *
 *      // 使用默认消息级别及默认输出的TestRunner。
 *      ::NsLib::TestRunner testRunner;
 *
 *      // 添加测试用例。
 *      testRunner.addTests(&a);
 *      testRunner.addTests(&b);
 *      testRunner.addTests(&c);
 *
 *      // 运行测试。
 *      testRunner.run();
 *
 *      // 显式测试结果。
 *      testRunner.output();
 *
 *      return 0;
 * }
 *
 * \endcode
 *
 * \see     TestClassTemplate
 */
class TestRunner
{
    MAKE_CLASS_UNCOPYABLE(TestRunner);

public:
    /// \attention   TestRunner析构是会自动释放所有测试用例，不需要手工释放。
    ~TestRunner()
    {
        for (auto testIter = testCases_.begin();
             testCases_.end() != testIter;
             ++testIter)
        {
            delete (*testIter);
        }
    }

    /*!
     * \brief   添加测试用例。
     *
     * \param   test - 待添加测试用例的指针
     *
     * \return  无
     *
     * \note    请不要添加非法指针。
     */
    void addTests(::NsLib::TestClassTemplate *test)
    {
        testCases_.push_back(test);
    }

    /*!
     * \brief   运行测试。
     *
     * \param   无
     *
     * \return  无
     */
    void run()
    {
        for (auto testIter = testCases_.begin();
             testCases_.end() != testIter;
             ++testIter)
        {
            (*testIter)->run();

            results_.addTestResult((*testIter)->getResult());
        }
    }

    /*!
     * \fn void output()
     * \brief   输出测试结果。
     *
     * \param   无
     *
     * \return  无
     */
    void output()
    {
        ::NsLib::Lock< ::NsLib::NsLcok>
            threadALcok{&nsThreadMutexForTestRunnerObject_};

        ::NsLib::UnitTest::TestResultCollector::TestResults
            results = results_.getTestResult();

        for (auto resultIter = results.begin();
             results.end() != resultIter;
             ++resultIter)
        {
            ::NsLib::UnitTest::TestResult
                result = *resultIter;
            ::NsLib::UnitTest::TestResult::SuccessTestCases
                successes = result.getSuccesses();
            ::NsLib::UnitTest::TestResult::FailTestCases
                failures = result.getFailures();
            int successCount = result.getSuccessCount();
            int failureCount = result.getFailureCount();
            int testCount = result.getTestCount();

            successCount_ += successCount;
            failureCount_ += failureCount;
            testCount_ += testCount;

            ::NsLib::UnitTest::TestResult::FailTestCases
                failureCases = result.getFailures();

            for (auto iter = failureCases.begin();
                 failureCases.end() != iter;
                 ++iter)
            {
                nsout_ << "<Failed>:  "
                       << (*iter).first << "\n"
                       << (*iter).second << "\n";
            }

            if (messageLevel_ > ::NsLib::UnitTestMessageLevel::Normal)
            {
                ::NsLib::UnitTest::TestResult::SuccessTestCases
                    successCases = result.getSuccesses();

                for (auto iter = successCases.begin();
                     successCases.end() != iter;
                     ++iter)
                {
                    nsout_ << "<Succeed>: " << (*iter) << "\n";
                }
            }

            if (messageLevel_ > ::NsLib::UnitTestMessageLevel::Lite)
            {
                nsout_ << "[ -- Test Case: " << result.getTestName()
                       << " -- ]\n["
                       << "Success: " << successCount << "     "
                       << "Failure: " << failureCount << "     "
                       << "All: " << testCount << "]\n";
            }
        }

        nsout_ << "[ -- Test Result Summarizing -- ]" << "\n";
        nsout_ << "[Success: " << successCount_ << "     "
               << "Failure: " << failureCount_ << "     "
               << "All: " << testCount_ << "]\n";
    }

    // Doxygen有Bug，构造函数放在前面会导致其他函数无法创建文档。
    /*!
     * \brief   默认输出为std::cerr，
     *          默认测试结果消息级别为::NsLib::UnitTestMessageLevel::Normal。
     *
     * \param   outputStream - 测试结果输出流
     * \param   messageLevel - 测试结果消息级别
     */
    TestRunner(std::ostream &outputStream = std::cerr,
               ::NsLib::UnitTestMessageLevel::MessageLevel
                    messageLevel = ::NsLib::UnitTestMessageLevel::Normal) :
        nsout_(outputStream),   // 好吧, 这里用到了HACK, 使用{}不支持
        messageLevel_{messageLevel},
        successCount_{0},
        failureCount_{0},
        testCount_{0}
    {
        testCases_.clear();
    }

private:
    ::NsLib::UnitTest::TestResultCollector          results_;
    std::vector< ::NsLib::TestClassTemplate *>      testCases_;
    std::ostream                                    &nsout_;
    ::NsLib::UnitTestMessageLevel::MessageLevel     messageLevel_;
    int                                             successCount_;
    int                                             failureCount_;
    int                                             testCount_;
};

}

#endif
