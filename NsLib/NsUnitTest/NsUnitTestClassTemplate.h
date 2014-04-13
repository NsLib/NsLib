#ifndef NS_UNIT_TEST_CLASS_TEMPLATE_H
#define NS_UNIT_TEST_CLASS_TEMPLATE_H

#include "../NsInternalUse/NsCxx11Support.h"

#include <vector>
#include <iostream>
#include <iomanip>

#include "../NsUtility/NsUncopyale.h"
#include "NsUnitTestResult.h"
#include "NsUnitTestException.h"

namespace NsLib
{

/*!
 * \brief   用于测试类的配置。
 * \ingroup NsUnitTest
 *
 * \param   classname - 测试类的名称
 *
 * \details 设置测试名称。
 *
 * \post    需要与\link NS_TEST_END \endlink 配对使用。
 *
 * \see     TestRunner\n
 *          NS_TEST_END
 */
#define NS_TEST_BEGIN(classname)                                               \
public:                                                                        \
    typedef classname NsTestClassName;                                         \
    typedef void (NsTestClassName::*testFun)();                                \
    typedef std::vector<testFun>        TestFunctions;                         \
    typedef std::vector<std::string>    TestFunctionNames;                     \
                                                                               \
    std::string                     testName_;                                 \
    TestFunctions                   testFunctions_;                            \
    TestFunctionNames               testFunctionNames_;                        \
    ::NsLib::UnitTest::TestResult   testResult_;                               \
                                                                               \
    virtual void run()                                                         \
    {                                                                          \
        testName_ = #classname;                                                \
        testResult_.setTestName(testName_);                                    \
        testFunctions_.clear();                                                \
        testFunctionNames_.clear();                                            \
        setUp();

/*!
 * \brief   用于测试类的配置。
 * \ingroup NsUnitTest
 *
 * \param   functionName - 测试类中用于测试的函数名。
 *
 * \details 添加测试函数，函数原型必须为\code void funName(); \endcode
 *
 * \pre     需要先调用\link NS_TEST_BEGIN \endlink 。
 *
 * \post    添加完所有测试函数后需要调用\link NS_TEST_END \endlink。
 *
 * \see     TestRunner\n
 *          NS_TEST_BEGIN\n
 *          NS_TEST_END
 */
#define NS_TEST(functionName)                                                  \
        testFunctions_.push_back(&NsTestClassName::functionName);              \
        testFunctionNames_.push_back(#functionName);

/*!
 * \brief   用于测试类的配置。
 * \ingroup NsUnitTest
 *
 * \details 用于终止\link NS_TEST_BEGIN \endlink 。
 *
 * \pre     需要首先调用\link NS_TEST_BEGIN \endlink 。
 *
 * \see     TestRunner\n
 *          NS_TEST_BEGIN
 */
#define NS_TEST_END()                                                          \
        while (!testFunctions_.empty())                                        \
        {                                                                      \
            bool    isSuccess{true};                                           \
            TestFunctionNames::reverse_iterator                                \
                funNameIter = testFunctionNames_.rbegin();                     \
            TestFunctions::reverse_iterator                                    \
                funIter = testFunctions_.rbegin();                             \
            std::string funName{*funNameIter};                                 \
            testFun     fun{*funIter};                                         \
                                                                               \
            testFunctionNames_.pop_back();                                     \
            testFunctions_.pop_back();                                         \
                                                                               \
            try                                                                \
            {                                                                  \
                (this->*fun)();                                                \
            }                                                                  \
            catch (::NsLib::UnitTest::NotEqualException &e)                    \
            {                                                                  \
                isSuccess = false;                                             \
                testResult_.addFailure(funName, e.message());                  \
            }                                                                  \
            catch (::NsLib::UnitTest::Exception &e)                            \
            {                                                                  \
                isSuccess = false;                                             \
                testResult_.addFailure(funName, e.what());                     \
            }                                                                  \
            catch (std::exception &e)                                          \
            {                                                                  \
                isSuccess = false;                                             \
                testResult_.addFailure(funName, e.what());                     \
            }                                                                  \
            catch (...)                                                        \
            {                                                                  \
                isSuccess = false;                                             \
                testResult_.addFailure(funName, "Unknown Exception");          \
            }                                                                  \
                                                                               \
            if (isSuccess)                                                     \
            {                                                                  \
                testResult_.addSuccess(funName);                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        tearDown();                                                            \
    }                                                                          \
                                                                               \
    virtual ::NsLib::UnitTest::TestResult getResult()                          \
    {                                                                          \
        return testResult_;                                                    \
    }

/*!
 * \class   TestClassTemplate NsUnitTest.h
 * \brief   用于生成测试类的模板类。
 * \ingroup NsUnitTest
 *
 * \details NsLib提供的测试框架需要将所有测试封装在测试类中，
 *          而测试类则派生自\link TestClassTemplate \endlink \n
 *          测试类需要添加到\link TestRunner \endlink 中进行测试。
 *
 * \pre     需要在测试用例中使用\link NS_TEST_BEGIN \endlink ，
 *          \link NS_TEST \endlink，\link NS_TEST_END \endlink，
 *          来组织测试。\n
 *          测试函数原型必须为\code void funName(); \endcode
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
 * \endcode
 *
 * \see     TestRunner
 */
class TestClassTemplate
{
    MAKE_CLASS_UNCOPYABLE(TestClassTemplate);
    
public:
    TestClassTemplate() { }
    virtual ~TestClassTemplate() { }

    /*!
     * \brief   设置测试需要的前置条件。
     *
     * \param   无
     *
     * \return  无
     */
    virtual void setUp() { }

    /*!
     * \brief   清理测试资源。
     *
     * \param   无
     *
     * \return  无
     */
    virtual void tearDown() { }

    /*!
     * \brief   运行测试。
     *
     * \attention 不要改写此函数，此函数被测试框架内部使用！！！
     *
     * \param   无
     *
     * \return  无
     */
    virtual void run() = 0;

    /*!
     * \brief   运行测试。
     *
     * \attention 不要改写此函数，此函数被测试框架内部使用！！！
     *
     * \param   无
     *
     * \return  测试结果
     */
    virtual ::NsLib::UnitTest::TestResult getResult() = 0;
};

}

#endif