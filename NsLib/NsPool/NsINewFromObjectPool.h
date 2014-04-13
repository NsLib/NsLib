#ifndef NS_I_NEW_FROM_OBJECT_POOL_H
#define	NS_I_NEW_FROM_OBJECT_POOL_H

#include "../NsInternalUse/NsCxx11Support.h"
#include "NsObjectPool.h"

namespace NsLib
{

/*!
 * \brief   侵入式Object Pool。
 * \ingroup NsPool
 *
 * \tparam  DataType - Object Pool中要存储对象的类型
 * \tparam  [可选]size_t capacity - 容量[默认 = 1000]
 * \tparam  [可选]Allocator - 内存分配器构造对象所需参数[默认 = UserDefaultAllocator]
 *
 * \details 本Object Pool默认容量为1000，超出1000在debug模式下会触发断言。\n
 *          默认使用的是UserDefaultAllocator内存分配器，如果对内存分配有特殊需求，
 *          请根据UserDefaultAllocator接口自定义内存分配器。\n
 *
 * \code
 * // 示例：
 * // 单继承情况下的使用(基类中没有继承自INewFromObjectPool类)：
 * class IntrusivePoolClass :
 *      public ::NsLib::INewFromObjectPool<IntrusivePoolClass, 3>
 * {
 * public:
 *      IntrusivePoolClass() { }
 *      IntrusivePoolClass(int data) { }
 *      ~IntrusivePoolClass() { }
 *
 *      int     data_;
 * };
 *
 * // 定义Object Pool名称，创建Object Pool和销毁Object Pool和非侵入式的方法基本一致，
 * // 区别仅在于使用的宏不同。
 *
 * // 首先要定义Object Pool名称，第一个参数后面的参数，
 * // 要和继承自INewFromObjectPool的模板参数相同。
 * NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(IntrusivePoolClassPool,
 *                                      IntrusivePoolClass, 3);
 *
 * // 创建Object Pool。
 * NS_CREATE_INTRUSIVE_OBJECT_POOL(IntrusivePoolClassPool);
 *
 * // 开始分配和删除操作。
 * IntrusivePoolClass *ptrs[3];
 *
 * ptr[0] = new IntrusivePoolClass();
 * ptr[1] = new IntrusivePoolClass(1);
 *
 * delete ptr[0];
 * delete ptr[1];
 *
 * // 销毁Object Pool。
 * NS_DESTROY_INTRUSIVE_OBJECT_POOL(IntrusivePoolClassPool);
 *
 * \endcode
 *
 * <hr>
 *
 * \code
 * // 示例：
 * // 多继承情况下的使用(基类中有继承自INewFromObjectPool的类)：
 * class BaseA :
 *      public ::NsLib::INewFromObjectPool<BaseA>
 * {
 * public:
 *      BaseA() { }
 *      virtual ~BaseA() { }
 *
 *      int dataA_;
 * };
 *
 * class BaseB :
 *      public ::NsLib::INewFromObjectPool<BaseB>
 * {
 * public:
 *      BaseB() { }
 *      virtual ~BaseB() { }
 *
 *      int dataB_;
 * };
 *
 * class Derived :
 *      public BaseA,
 *      public BaseB,
 *      public ::NsLib::INewFromObjectPool<Derived, 3>
 * {
 * public:
 *      // !!!对于多重继承的情况，在开头用public访问权限声明如下，参数应该和
 *      // ::NsLib::INewFromObjectPool<Derived, 3>相一致。
 *      NS_INTRUSIVE_OBJECT_POOL_MULTI_INHERIT(Derived, 3);
 *
 * public:
 *      Derived() : BaseA(), BaseB() { }
 *      virtual ~Derived() { }
 *
 *      int dataD_;
 * };
 *
 * // 首先要定义Object Pool名称，第一个参数后面的参数，
 * // 要和继承自INewFromObjectPool的模板参数相同。
 * NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(DerivedPool,
 *                                      Derived, 3);
 *
 * // 创建Object Pool。
 * NS_CREATE_INTRUSIVE_OBJECT_POOL(DerivedPool);
 *
 * // 开始分配和删除操作。
 * Derived *ptrs[3];
 *
 * ptr[0] = new Derived();
 * ptr[1] = new Derived();
 *
 * delete ptr[0];
 * delete ptr[1];
 *
 * // 销毁Object Pool。
 * NS_DESTROY_INTRUSIVE_OBJECT_POOL(DerivedPool);
 *
 * \endcode
 *
 * \note    默认内存配置器接口。
 *
 * \see     UserDefaultAllocator
 */
template <typename  DataType,
          size_t    capacity = 1000,
          template  <typename>
                    class Allocator = ::NsLib::UserDefaultAllocator>
class INewFromObjectPool
{
    friend class ::NsLib::ObjectPool<DataType, capacity, Allocator>;

public:
    void *operator new(size_t size)
    {
#ifdef _NS_DEBUG_TRACE_NEW_AND_DELETE_FROM_OBJECT_POOL_
        NS_TRACE_NEW_FROM_OBJECT_POOL(DataType);
#endif

        return ::NsLib::ObjectPool<DataType, capacity, Allocator>
               ::getObjectMemory();
    }

    void operator delete(void *ptr)
    {
#ifdef _NS_DEBUG_TRACE_NEW_AND_DELETE_FROM_OBJECT_POOL_
        NS_TRACE_DELETE_FROM_OBJECT_POOL(DataType);
#endif

        ::NsLib::ObjectPool<DataType, capacity, Allocator>
               ::deallocateMemory(static_cast<DataType *>(ptr));
    }
};

/*!
 * \brief   定义Object Pool的名字。
 * \ingroup NsPool
 *
 * \param   PoolName - ObjectPool名称
 * \param   存储的数据类型
 * \param   [可选]容量[默认=1000]
 * \param   [可选]内存分配器[默认=UserDefaultAllocator]
 *
 * \details 定义Object Pool名称，用于区分不同Object Pool。
 *
 * \code
 * class MyClass ：
 *      public ::NsLib::INewFromObjectPool<MyClass, 100>
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 首先要定义Object Pool名称，第一个参数后面的参数，
 * // 要和继承自INewFromObjectPool的模板参数相同。
 * NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(MyClassPool, MyClass, 100);
 * \endcode
 *
 * \note    以下宏依赖此处定义的Object Pool名称：
 *
 * \see     NS_CREATE_INTRUSIVE_OBJECT_POOL      \n
 *          NS_DESTROY_INTRUSIVE_OBJECT_POOL
 */
#define NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(PoolName, ...) \
    typedef ::NsLib::ObjectPool<__VA_ARGS__> PoolName

/*!
 * \brief   创建Object Pool。
 * \ingroup NsPool
 *
 * \param   PoolName - ObjectPool名称
 *
 * \details 创建Object Pool并分配所需的内存，对于需要对Object Pool创建时机进行控制的用户，
 *          此宏必不可少。
 *
 * \note    请不要重复创建同一个Object Pool。\n
 *          Debug模式下重复创建同一个Object Pool会触发断言。
 *
 * \code
 * class MyClass ：
 *      public ::NsLib::INewFromObjectPool<MyClass, 100>
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 首先要定义Object Pool名称，第一个参数后面的参数，
 * // 要和继承自INewFromObjectPool的模板参数相同。
 * NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(MyClassPool, MyClass, 100);
 *
 * // 创建Object Pool。
 * NS_CREATE_INTRUSIVE_OBJECT_POOL(MyClassPool);
 * \endcode
 *
 * \see     NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME\n
 *          NS_DESTROY_INTRUSIVE_OBJECT_POOL
 */
#define NS_CREATE_INTRUSIVE_OBJECT_POOL(PoolName) \
    PoolName::create()

/*!
 * \brief   销毁Object Pool。
 * \ingroup NsPool
 *
 * \param   PoolName - ObjectPool名称
 *
 * \details 销毁Object Pool并释放分配的内存，对于需要对Object Pool销毁时机进行控制的用户，
 *          此宏必不可少。必须先调用NS_CREATE_INTRUSIVE_OBJECT_POOL先创建Object Pool，
 *          不可多次销毁。
 *
 * \note    Debug模式下会检测Object Pool是否被创建，未创建会触发断言。\n
 *          若多次调用，也会触发断言。
 *
 * \code
 * class MyClass ：
 *      public ::NsLib::INewFromObjectPool<MyClass, 100>
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 首先要定义Object Pool名称，第一个参数后面的参数，
 * // 要和继承自INewFromObjectPool的模板参数相同。
 * NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(MyClassPool, MyClass, 100);
 *
 * // 创建Object Pool。
 * NS_CREATE_INTRUSIVE_OBJECT_POOL(MyClassPool);
 *
 * // 销毁Object Pool。
 * NS_DESTROY_INTRUSIVE_OBJECT_POOL(MyClassPool);
 * \endcode
 *
 * \see     NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME\n
 *          NS_CREATE_INTRUSIVE_OBJECT_POOL
 */
#define NS_DESTROY_INTRUSIVE_OBJECT_POOL(PoolName) \
    PoolName::destroy()

/*!
 * \brief   侵入式Object Pool在多继承(基类中有继承自INewFromObjectPool的类)条件下，
 *          使用此宏进行声明。
 * \ingroup NsPool
 *
 * \param   具体的参数应该和继承自INewFromObjectPool的模板参数相同。
 * \tparam  DataType - Object Pool中要存储对象的类型
 * \tparam  [可选]size_t capacity - 容量[默认 = 1000]
 * \tparam  [可选]Allocator - 内存分配器构造对象所需参数[默认 = UserDefaultAllocator]
 *
 * \details 在类开头进行public声明，否则调用new和delete会发生错误。
 *
 * \code
 * // 示例：
 * // 多继承情况下的使用(基类中有继承自INewFromObjectPool的类)：
 * class BaseA :
 *      public ::NsLib::INewFromObjectPool<BaseA>
 * {
 * public:
 *      BaseA() { }
 *      virtual ~BaseA() { }
 *
 *      int dataA_;
 * }
 *
 * class BaseB :
 *      public ::NsLib::INewFromObjectPool<BaseB>
 * {
 * public:
 *      BaseB() { }
 *      virtual ~BaseB() { }
 *
 *      int dataB_;
 * }
 *
 * class Derived :
 *      public BaseA,
 *      public BaseB,
 *      public ::NsLib::INewFromObjectPool<Derived, 3>
 * {
 * public:
 *      // !!!对于多重继承的情况，在开头用public访问权限声明如下，参数应该和
 *      // ::NsLib::INewFromObjectPool<Derived, 3>相一致。
 *      NS_INTRUSIVE_OBJECT_POOL_MULTI_INHERIT(Derived, 3);
 *
 * public:
 *      Derived() : BaseA(), BaseB() { }
 *      virtual ~Derived() { }
 *
 *      int dataD_;
 * }
 * \endcode
 */
#define NS_INTRUSIVE_OBJECT_POOL_MULTI_INHERIT(...)                            \
    using ::NsLib::INewFromObjectPool<__VA_ARGS__>::operator new;              \
    using ::NsLib::INewFromObjectPool<__VA_ARGS__>::operator delete
} // NsLib

#endif


