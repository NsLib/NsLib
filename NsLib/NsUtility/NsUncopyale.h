#ifndef NS_UNCOPYABLE_H
#define	NS_UNCOPYABLE_H

namespace NsLib
{

// 这是为了防止Argument Dependent Lookup出现问题
namespace Uncopyable_
{

/*!
 * \brief   防止对象被复制，特别适合对效率要求较高的代码使用。
 * \ingroup NsUtility
 *
 * \attention   一个类中只能声明一次。
 *
 * \code
 * // 示例：
 * class MyClass
 * {
 *      MAKE_CLASS_UNCOPYABLE(MyClass);
 *
 * public:
 *      // 具体代码...
 * };
 * \endcode
 */
#define MAKE_CLASS_UNCOPYABLE(classname)                                       \
private:                                                                       \
    classname(const classname &) = delete;                                     \
    classname &operator =(const classname &) = delete

class IUncopyable
{
    MAKE_CLASS_UNCOPYABLE(IUncopyable);

protected:
    IUncopyable() { }
    virtual ~IUncopyable() { }
};

}   // namespace Uncopyable_

/*!
 * \class   IUncopyable NsUtility.h
 * \brief   防止对象被复制，如果代码对性能要求较高，推荐使用MAKE_CLASS_UNCOPYABLE宏。
 * \ingroup NsUtility
 *
 * \details 用户自定义类型通过继承IUncopyable接口,，实现阻止复制的目的，
 *          详细资料请参考《Effective C++》。
 *          一般对于只有一个空基类的继承，编译器一般都可以使用EBO机制优化。
 *          但是对于多个基类的时候，一般的编译器就不能进行优化，造成效率损耗，
 *          所以对于性能要求较高的代码，推荐使用MAKE_CLASS_UNCOPYABLE宏。
 * \code
 * // 示例：
 * class MyClass : private ::NsLib::IUncopyable
 * {
 *      // 具体代码...
 * };
 * \endcode
 * \see MAKE_CLASS_UNCOPYABLE
 */
typedef Uncopyable_::IUncopyable IUncopyable;

}   // namespace NsLib

#endif
