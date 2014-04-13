#ifndef NS_SYNCHRONIZED_OBJECT_H
#define	NS_SYNCHRONIZED_OBJECT_H

#include "../NsInternalUse/NsCxx11Support.h"
#include "NsLock.h"

namespace NsLib
{

/*!
 * \class   SynchronizedObject NsSynchronization.h
 * \brief   同一时间要求同步的对象的创建以及成员函数的同步，或者二者选一。
 * \ingroup NsSynchronization
 *
 * \tparam  LcokProxy - 锁类型，默认为\link NsLcok \endlink，用户可以根据需要自己封装。
 *
 * \details 如果某些对象的构造需要同步，或者其成员函数调用需要同步，
 *          那么就可以使用此类作为基类。
 *
 * \note    锁定对象构造和成员函数的锁类型相同。\n
 *          锁定对象构造和成员函数的锁变量不可以为通一个，Debug模式下若相同会触发断言。
 *
 * \code
 * // 示例：
 * // 同时锁定对象的构造和成员函数。
 *
 * // 用于锁定对象构造。
 * static ::NsLib::NsLcok mutexForObject;
 * // 用于锁定对象的成员函数。
 * static ::NsLib::NsLcok mutexForMethod;
 *
 * class SynchronizedObjectClass :
 *      public ::NsLib::SynchronizedObject< ::NsLib::NsLcok>
 * {
 * public:
 *      SynchronizedObjectClass(LockType *lockObject,
 *                              LockType *lockMethod) :
 *                              SynchronizedObject(lockObject, lockMethod)
 *      {
 *      }
 *
 *      virtual ~SynchronizedObjectClass() { }
 *
 *      void lockedMethod()
 *      {
 *          // 要锁定一个函数，要在函数开始处加上一个锁，锁变量则固定为lockMethod_。
 *          NsLib::Lock<LockType> lockThisMethod(lockMethod_);
 *
 *          // 一些操作...
 *      }
 * };
 *
 * void threadFunA()
 * {
 *      // 锁定对象构造。
 *      SynchronizedObjectClass
 *          threadAObject{&mutexForObject, &mutexForMethod};
 *
 *      // 锁定成员函数。
 *      threadAObject.lockedMethod();
 * }
 *
 * void threadFunB()
 * {
 *      // 锁定对象构造。
 *      SynchronizedObjectClass
 *          threadBObject{&mutexForObject, &mutexForMethod};
 *
 *      // 锁定成员函数。
 *      threadBObject.lockedMethod();
 * }
 *
 * int main()
 * {
 *      // 创建线程A和B，分别使用threadFunA()和threadFunB()
 *      // 由于使用了SynchronizedObject，并且同时锁定对象的构造和成员函数，
 *      // 所以不会发生资源冲突。
 *
 *      // 等待线程执行完毕的代码...
 *
 *      return 0;
 * }
 *
 * \endcode
 * <hr>
 * \code
 * // 示例：
 * // 仅锁定成员函数。
 *
 * // 使用不同的锁变量，不锁定对象构造。
 * static ::NsLib::NsLcok mutexForObjectA;
 * static ::NsLib::NsLcok mutexForObjectB;
 * // 用于锁定对象的成员函数。
 * static ::NsLib::NsLcok mutexForMethod;
 *
 * class SynchronizedObjectClass :
 *      public ::NsLib::SynchronizedObject< ::NsLib::NsLcok>
 * {
 * public:
 *      SynchronizedObjectClass(LockType *lockObject,
 *                              LockType *lockMethod) :
 *                              SynchronizedObject(lockObject, lockMethod)
 *      {
 *      }
 *
 *      virtual ~SynchronizedObjectClass() { }
 *
 *      void lockedMethod()
 *      {
 *          // 要锁定一个函数，要在函数开始处加上一个锁，锁变量则固定为lockMethod_。
 *          NsLib::Lock<LockType> lockThisMethod(lockMethod_);
 *
 *          // 一些操作...
 *      }
 * };
 *
 * void threadFunA()
 * {
 *      // 不锁定对象构造。
 *      SynchronizedObjectClass
 *          threadAObject{&mutexForObjectA, &mutexForMethod};
 *
 *      // 锁定成员函数。
 *      threadAObject.lockedMethod();
 * }
 *
 * void threadFunB()
 * {
 *      // 不锁定对象构造。
 *      SynchronizedObjectClass
 *          threadBObject{&mutexForObjectB, &mutexForMethod};
 *
 *      // 锁定成员函数。
 *      threadBObject.lockedMethod();
 * }
 *
 * int main()
 * {
 *      // 创建线程A和B，分别使用threadFunA()和threadFunB()
 *      // 由于使用了SynchronizedObject，并且仅锁定对象的成员函数，
 *      // 所以对同一时间，只有一个线程能访问lockedMethod()。
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
template <typename LockProxy = ::NsLib::NsLcok>
class SynchronizedObject
{
protected:
    typedef LockProxy   LockType;

public:
    SynchronizedObject(LockType *lockObject, LockType *lockMethod) :
        lockObject_{lockObject}, lockMethod_{lockMethod}
    {
#ifndef NDEBUG
        assert(nullptr != lockObject_ && "-- lockObject is nullptr");
        assert(nullptr != lockMethod_ && "-- lockMethod is nullptr");
        assert(lockObject_ != lockMethod_
               && "-- lockObject == lockMethod that will case fatal error");
#endif

        lockObject_->lock();
    }

    virtual ~SynchronizedObject()
    {
        lockObject_->unlock();
    }

    /**
     * \code
     * void lockedMethod()
     * {
     *      ::NsLib::Lock<LockType> lockThisMethod(lockMethod_);
     *
     *      // do something
     * }
     * \endcode
     */

protected:
    LockType   *lockObject_;
    LockType   *lockMethod_;
};

}

#endif

