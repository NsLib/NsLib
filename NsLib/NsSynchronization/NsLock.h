#ifndef NS_LOCK_H
#define	NS_LOCK_H

#include "../NsInternalUse/NsCxx11Support.h"

#include <mutex>

#include "../NsUtility/NsUncopyale.h"

namespace NsLib
{

class UserDefaultLockProxy
{
    MAKE_CLASS_UNCOPYABLE(UserDefaultLockProxy);

public:
    UserDefaultLockProxy() : mutex_{new std::mutex}
    {
    }

    ~UserDefaultLockProxy()
    {
        delete mutex_;
    }

    void lock() const
    {
        mutex_->lock();
    }

    void unlock() const
    {
        mutex_->unlock();
    }

private:
    std::mutex *mutex_;
};

/*!
 * \class   NsLcok NsSynchronization.h
 * \brief   提供给用户的默认锁变量类型。
 * \ingroup NsSynchronization
 *
 * \note    可以用于NsLib所有的线程相关类及函数。\n
 *          用户可以根据NsLcok的接口，自行封装需要的锁类型，用与NsLib线程相关类及函数。
 */
typedef NsLib::UserDefaultLockProxy NsLcok;

/*!
 * \class   Lock NsSynchronization.h
 * \brief   多线程条件下，锁定一块代码块。
 * \ingroup NsSynchronization
 *
 * \tparam  LcokProxy - 锁类型，默认为\link NsLcok \endlink，用户可以根据需要自己封装。
 *
 * \details 对于多个线程共享的资源，多个线程同时进行操作可能会引发问题，使用Lcok类就可以
 *          将代码块锁定，同一时间只允许一个线程访问。
 *
 * \note    锁定周期为Lock对象的生命周期，用户可以自己指定锁的类型。
 *
 * \code
 * // 示例：
 *
 * // 线程间共享的数据。
 * static char threadSharedResource = 'X';
 * // 用于锁定资源的锁，默认使用的是NsLcok，用户可以自己封装锁类型。
 * static ::NsLib::NsLcok threadMutex;
 *
 * void threadFunA()
 * {
 *      // 锁定代码块。
 *      ::NsLib::Lock< ::NsLib::NsLcok> threadALcok{&threadMutex};
 *
 *      // 访问共享资源。
 *      threadSharedResource = 'A';
 * }    // 锁定的周期到这里
 *
 * void threadFunB()
 * {
 *      // 锁定代码块。
 *      ::NsLib::Lock< ::NsLib::NsLcok> threadALcok{&threadMutex};
 *
 *      // 访问共享资源。
 *      threadSharedResource = 'B';
 * }    // 锁定的周期到这里
 *
 * int main()
 * {
 *      // 创建线程A和B，分别使用threadFunA()和threadFunB()
 *      // 由于使用了Lock，所以同一时间只有一个线程能访问到共享资源
 *
 *      // 等待线程执行完毕的代码...
 *
 *      return 0;
 * }
 *
 * \endcode
 *
 * \see     NsLcok
 */
template <typename LcokProxy = ::NsLib::NsLcok>
class Lock
{
    MAKE_CLASS_UNCOPYABLE(Lock);

public:

    explicit Lock(LcokProxy *lockObject) : lockObject_{lockObject}
    {
#ifndef NDEBUG
        assert(nullptr != lockObject_ && "-- lockObject is nullptr");
#endif

        lockObject_->lock();
    }

    ~Lock()
    {
        lockObject_->unlock();
    }

private:
    LcokProxy   *lockObject_;
};

}

#endif

