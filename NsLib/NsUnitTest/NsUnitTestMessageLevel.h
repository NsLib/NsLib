#ifndef NS_UNIT_TEST_MESSAGE_LEVEL_H
#define NS_UNIT_TEST_MESSAGE_LEVEL_H

namespace NsLib
{

namespace UnitTestMessageLevel
{


/*！
 * \enum    MessageLevel
 * \brief   测试结果输出的级别
 * \ingroup NsUnitTest
 *
 * \details Lite - 仅显示失败的测试函数以及总测试结果汇总。
 *          Normal - 显式失败的测试函数及每个TestCase的测试汇总，以及总测试结果汇总。
 *          More - 显式所有测试函数的测试结果及每个TestCase的测试汇总，以及总测试结果汇总。
 */
enum MessageLevel
{
    Lite    = 0,
    Normal  = 5,
    More    = 10
};

}   // UnitTestMessageLevel

}   // NsLib

#endif