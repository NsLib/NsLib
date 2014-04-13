#ifndef NS_UNIT_TEST_H
#define	NS_UNIT_TEST_H

#include "NsInternalUse/NsCxx11Support.h"

#include "NsUnitTest/NsUnitTestAssert.h"
#include "NsUnitTest/NsUnitTestClassTemplate.h"
#include "NsUnitTest/NsUnitTestMessageLevel.h"
#include "NsUnitTest/NsUnitTestRunner.h"

namespace NsLib
{

/*!
 * \brief   测试断言。
 * \ingroup NsUnitTest
 *
 * \param   expression - 要断言真假的表达式
 *
 * \details 测试条件为真，则不触发断言。\n
 *          测试条件为假，则触发断言。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 * NS_ASSERT(true);
 * NS_ASSERT(false);
 * NS_ASSERT(1 == 2);
 * NS_ASSERT(1 != 2);
 * \endcode
 *
 * \see     NS_ASSERT_MESSAGE
 */
#define NS_ASSERT(expression)                                                  \
    ::NsLib::UnitTest::failThrow(!(expression),                                \
                                 "NS_ASSERT",                                  \
                                 (#expression),                                \
                                 "",                                           \
                                 NS_SOURCE_TRACER())

/*!
 * \brief   附加消息的测试断言。
 * \ingroup NsUnitTest
 *
 * \param   expression - 要断言真假的表达式
 * \param   message - 要附加的消息
 *
 * \details 测试条件为真，则不触发断言。\n
 *          测试条件为假，则触发断言，并将消息一起显示。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 * NS_ASSERT_MESSAGE(true, "This should be success");
 * NS_ASSERT_MESSAGE(false, "This should be fail");
 * NS_ASSERT_MESSAGE(1 == 2, "This should be fail");
 * NS_ASSERT_MESSAGE(1 != 2, "This should be success");
 * \endcode
 *
 * \see     NS_ASSERT
 */
#define NS_ASSERT_MESSAGE(expression, message)                                 \
    ::NsLib::UnitTest::failThrow(!(expression),                                \
                                 "NS_ASSERT_MESSAGE",                          \
                                 (#expression),                                \
                                 (message),                                    \
                                 NS_SOURCE_TRACER())

/*!
 * \brief   让测试失败，并附加一段消息。
 * \ingroup NsUnitTest
 *
 * \param   message - 要附加的消息
 *
 * \details 一般用于尚未完成的测试，用以指明进度。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 * NS_FAIL("Not finsh yet");
 * \endcode
 */
#define NS_FAIL(message)                                                       \
    ::NsLib::UnitTest::failThrow(true,                                         \
                                 "NS_FAIL",                                    \
                                 (#message),                                   \
                                 (message),                                    \
                                 NS_SOURCE_TRACER())

/*!
 * \brief   相等性测试断言。
 * \ingroup NsUnitTest
 *
 * \param   expected - 预期值
 * \param   actual - 实际值
 *
 * \details 如果预期值与实际值相等，则不触发断言。否则，触发断言。
 *
 * \pre     只能在测试用例中使用。
 *
 * \note    可以直接用于浮点数比较。
 *
 * \code
 * // 示例：
 * NS_ASSERT_EQUAL(1, 1);
 * NS_ASSERT_EQUAL(1, 2);
 * NS_ASSERT_EQUAL(1.0, 2.3);
 * NS_ASSERT_EQUAL(2, 4);
 * \endcode
 *
 * \see     NS_ASSERT_EQUAL_MESSAGE
 */
#define NS_ASSERT_EQUAL(expected, actual)                                      \
    ::NsLib::UnitTest::assertEqual("NS_ASSERT_EQUAL",                          \
                                   (expected),                                 \
                                   (actual),                                   \
                                   (""),                                       \
                                   NS_SOURCE_TRACER())

/*!
 * \brief   相等性测试断言并附加一段消息。
 * \ingroup NsUnitTest
 *
 * \param   expected - 预期值
 * \param   actual - 实际值
 * \param   message - 要附加的消息
 *
 * \details 如果预期值与实际值相等，则不触发断言。否则，触发断言，并将消息一起显示。
 *
 * \pre     只能在测试用例中使用。
 *
 * \note    可以直接用于浮点数比较。
 *
 * \code
 * // 示例：
 * NS_ASSERT_EQUAL(1, 1);
 * NS_ASSERT_EQUAL(1, 2);
 * NS_ASSERT_EQUAL(1.0, 2.3);
 * NS_ASSERT_EQUAL(2, 4);
 * \endcode
 *
 * \see     NS_ASSERT_EQUAL
 */
#define NS_ASSERT_EQUAL_MESSAGE(expected, actual, message)                     \
    ::NsLib::UnitTest::assertEqual("NS_ASSERT_EQUAL_MESSAGE",                  \
                                   (expected),                                 \
                                   (actual),                                   \
                                   (message),                                  \
                                   NS_SOURCE_TRACER())

/*!
 * \brief   是否抛出预期异常断言。
 * \ingroup NsUnitTest
 *
 * \param   expression - 表达式
 * \param   exceptionType - 待抛出异常类型
 *
 * \details 如果表达式抛出预期异常则不触发断言。否则，触发断言。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 * class MyException
 * {
 * public:
 *      MyException() { }
 *      ~MyException() { }
 * };
 *
 * void throwMyException()
 * {
 *      throw MyException();
 * }
 *
 * void throwStdException()
 * {
 *      throw std::exception();
 * }
 *
 *
 * NS_ASSERT_THROW(throwMyException(), MyException);
 * NS_ASSERT_THROW(throwStdException(), std::exception);
 * NS_ASSERT_THROW(throwMyException(), std::exception);
 *
 * \endcode
 *
 * \see     NS_ASSERT_THROW_MESSAGE
 */
#define NS_ASSERT_THROW(expression, exceptionType) \
    NS_ASSERT_THROW_MESSAGE(expression, exceptionType, "")

/*!
 * \brief   是否抛出预期异常断言，并附加一段消息。
 * \ingroup NsUnitTest
 *
 * \param   expression - 表达式
 * \param   exceptionType - 待抛出异常类型
 * \param   message - 要附加的消息
 *
 * \details 如果表达式抛出预期异常则不触发断言。否则，触发断言，并将消息一起显示。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 * class MyException
 * {
 * public:
 *      MyException() { }
 *      ~MyException() { }
 * };
 *
 * void throwMyException()
 * {
 *      throw MyException();
 * }
 *
 * void throwStdException()
 * {
 *      throw std::exception();
 * }
 *
 *
 * NS_ASSERT_THROW(throwMyException(), MyException, "This should be success");
 * NS_ASSERT_THROW(throwStdException(), std::exception, "This should be success");
 * NS_ASSERT_THROW(throwMyException(), std::exception, "This should be fail");
 *
 * \endcode
 *
 * \see     NS_ASSERT_THROW
 */
#define NS_ASSERT_THROW_MESSAGE(expression, exceptionType, message)            \
    do                                                                         \
    {                                                                          \
        bool            isExceptionTypeVaild{false};                           \
        std::string     msg;                                                   \
        std::string     expression_;                                           \
                                                                               \
        expression_ += #expression;                                            \
        expression_ += ", ";                                                   \
        expression_ += #exceptionType;                                         \
        expression_ += ", ";                                                   \
        expression_ += message;                                                \
        expression_ += ")\n";                                                  \
                                                                               \
        try                                                                    \
        {                                                                      \
            (expression);                                                      \
        }                                                                      \
        catch (exceptionType &)                                                \
        {                                                                      \
            isExceptionTypeVaild = true;                                       \
        }                                                                      \
        catch (const ::NsLib::UnitTest::NotEqualException &e)                  \
        {                                                                      \
            expression_ += "           Exception Type: ";                      \
            expression_ += e.ExceptionName();                                  \
            expression_ += "\n";                                               \
            expression_ += "           Exception Info: ";                      \
            expression_ += e.what();                                           \
        }                                                                      \
        catch (const ::NsLib::UnitTest::Exception &e)                          \
        {                                                                      \
            expression_ += "           Exception Type: ";                      \
            expression_ += e.ExceptionName();                                  \
            expression_ += "\n";                                               \
            expression_ += "           Exception Info: ";                      \
            expression_ += e.what();                                           \
        }                                                                      \
        catch (const std::exception &e)                                        \
        {                                                                      \
            expression_ += "           Exception Type: ";                      \
            expression_ += "std::exception or derived\n";                      \
            expression_ += "           Exception Info: ";                      \
            expression_ += e.what();                                           \
        }                                                                      \
        catch (...)                                                            \
        {                                                                      \
            expression_ += "           Exception Type: UnKnown Exception Type";\
        }                                                                      \
                                                                               \
        if (isExceptionTypeVaild)                                              \
        {                                                                      \
            break;                                                             \
        }                                                                      \
                                                                               \
        ::NsLib::UnitTest::failThrow(true,                                     \
                                     "NS_ASSERT_THROW_MESSAGE",                \
                                     (expression_),                            \
                                     message,                                  \
                                     NS_SOURCE_TRACER());                      \
    } while (false)


/*!
 * \brief   不抛出任何异常断言。
 * \ingroup NsUnitTest
 *
 * \param   expression - 表达式
 *
 * \details 如果表达式不抛出任何异常，则不触发断言。否则，触发断言。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 *
 * void throwMyException()
 * {
 *      throw MyException();
 * }
 *
 * void throwStdException()
 * {
 *      throw std::exception();
 * }
 *
 * void throwNothing()
 * {
 * }
 *
 * NS_ASSERT_NO_THROW(throwMyException());
 * NS_ASSERT_NO_THROW(throwStdException());
 * NS_ASSERT_NO_THROW(throwNothing());
 *
 * \endcode
 *
 * \see     NS_ASSERT_NO_THROW_MESSAGE
 */
#define NS_ASSERT_NO_THROW(expression) \
    NS_ASSERT_NO_THROW_MESSAGE(expression, "")

/*!
 * \brief   不抛出任何异常断言，并附加一段消息。
 * \ingroup NsUnitTest
 *
 * \param   expression - 表达式
 * \param   message - 要附加的消息
 *
 * \details 如果表达式不抛出任何异常，则不触发断言。否则，触发断言，并将消息一起显示。
 *
 * \pre     只能在测试用例中使用。
 *
 * \code
 * // 示例：
 *
 * void throwMyException()
 * {
 *      throw MyException();
 * }
 *
 * void throwStdException()
 * {
 *      throw std::exception();
 * }
 *
 * void throwNothing()
 * {
 * }
 *
 * NS_ASSERT_NO_THROW(throwMyException(), "This should be fail");
 * NS_ASSERT_NO_THROW(throwStdException(), "This should be fail");
 * NS_ASSERT_NO_THROW(throwNothing(), "This should be success");
 *
 * \endcode
 *
 * \see     NS_ASSERT_NO_THROW
 */
#define NS_ASSERT_NO_THROW_MESSAGE(expression, message)                        \
    do                                                                         \
    {                                                                          \
        bool            throwException{false};                                 \
        std::string     expression_;                                           \
                                                                               \
        expression_ += #expression;                                            \
        expression_ += ", ";                                                   \
        expression_ += message;                                                \
        expression_ += ")\n";                                                  \
                                                                               \
        try                                                                    \
        {                                                                      \
            (expression);                                                      \
        }                                                                      \
        catch (const std::exception &e)                                        \
        {                                                                      \
            expression_ += "           Exception Type: ";                      \
            expression_ += "std::exception or derived\n";                      \
            expression_ += "           Exception Info: ";                      \
            expression_ += e.what();                                           \
            throwException = true;                                             \
        }                                                                      \
        catch (...)                                                            \
        {                                                                      \
            expression_ += "           Exception Type: ";                      \
            expression_ += "UnKnown Exception Type\n";                         \
            expression_ += "           Exception Info: ";                      \
            expression_ += "UnKnown";                                          \
            throwException = true;                                             \
        }                                                                      \
                                                                               \
        if (!throwException)                                                   \
        {                                                                      \
            break;                                                             \
        }                                                                      \
                                                                               \
        ::NsLib::UnitTest::failThrow(true,                                     \
                                     "NS_ASSERT_NO_THROW_MESSAGE",             \
                                     (expression_),                            \
                                     message,                                  \
                                     NS_SOURCE_TRACER());                      \
    } while (false)

}

#endif

