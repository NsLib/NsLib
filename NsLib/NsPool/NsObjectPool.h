#ifndef NS_OBJECT_POOL_H
#define	NS_OBJECT_POOL_H

#include "../NsInternalUse/NsCxx11Support.h"

// for malloc() and free()
#include <cstdlib>
// for assert()
#include <cassert>
// for placement new and bad_alloc
#include <new>
#include <cstdint>

#include "../NsInternalUse/NsDebugInfo.h"
#include "../NsUtility/NsUncopyale.h"

namespace NsLib
{

// object pool调试用
//#define _NS_OBJECT_POOL_DEBUG_

/*!
 * \class   UserDefaultAllocator NsPool.h
 * \brief   Object Pool默认的内存分配器，简单封装了malloc()和free()。
 * \ingroup NsPool
 *
 * \details 为用户分配内存提供自由度，因为有些用户可能因为某种特殊需求，分配指定位置的内存，
 *          也可能使用第三方的memory pool进行内存分配。
 *
 * \note    仅仅分配内存，不进行构造，因为Object Pool会自动完成构造。\n
 *          用户可以根据此接口自己提供Object Pool使用的内存分配器。
 */
template <typename DataType>
class UserDefaultAllocator
{
    MAKE_CLASS_UNCOPYABLE(UserDefaultAllocator);

public:
    /*!
     * \brief   分配大小为size的内存块
     *
     * \tparam  DataType - 要分配内存的数据类型
     * \param   size - 要分配内存块的大小
     *
     * \return  分配内存块的首地址
     *
     * \throw   bad_alloc
     */
    static DataType *allocate(size_t size) throw (std::bad_alloc)
    {
        DataType *memPtr = static_cast<DataType *>(malloc(size));

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("UserDefaultAllocator<",
                        typeid(DataType).name(),
                        ">::allocate()",
                        memPtr);
#endif

        if (nullptr == memPtr)
        {
            throw std::bad_alloc();
        }
        else
        {
            return memPtr;
        }
    }

    /*!
     * \brief   释放分配的内存块
     *
     * \param   ptr - 要释放内存块的首地址
     *
     * \return  无
     *
     * \note    无需进行空指针检测, 因为Object Pool会完成此工作。
     */
    static void deallocate(DataType *ptr)
    {
#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("UserDefaultAllocator<",
                        typeid(DataType).name(),
                        ">::deallocate()",
                        ptr);
#endif

        free(static_cast<void *>(ptr));
    }
};

/*!
 * \internal
 * \brief   通用Object Pool。
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
 * \note    对于内置数据类型(char ,int, double...)请使用Memory Pool，使用Object Pool
 *          并不能提升效率。
 *
 * \note    默认内存配置器接口。
 *
 * \see     UserDefaultAllocator
 *
 * \note    不要直接使用ObjectPool类，请使用为ObjectPool提供的宏进行操作。
 *
 * \see     NS_DEFINE_OBJECT_POOL_NAME \n
 *          NS_CREATE_OBJECT_POOL      \n
 *          NS_NEW_FROM_OBJECT_POOL    \n
 *          NS_DELETE_IN_OBJECT_POOL   \n
 *          NS_DESTROY_OBJECT_POOL
 * \endinternal
 */
template <typename  DataType,
          size_t    capacity = 1000,
          template  <typename>
                    class Allocator = ::NsLib::UserDefaultAllocator>
class ObjectPool
{
    MAKE_CLASS_UNCOPYABLE(ObjectPool);

    typedef Allocator<DataType> PoolAllocator;
    typedef DataType            DataType_;

    // 维护内部的内存链表, 使可读性更好, 思想来自STL的allocator
    struct FreeBlockNode
    {
        FreeBlockNode   *pNext_;
    };

public:
    ~ObjectPool()
    {
        destroy();
    }

    /*!
     * \internal
     * \brief   创建Object Pool，分配所需内存。
     *
     * \param   无
     *
     * \return  无
     *
     * \throw   bad_alloc
     *
     * \details 使用Object Pool前一定要进行此操作，手动进行Pool的创建，
     *          这样就可以在运行期不检测Pool是否被创建，最大限度提升运行效率，
     *          同时给要求精确控制Pool创建时机的用户提供支持。\n
     *
     * \note    Debug模式下如果没有调用此函数而直接使用Pool会触发断言。
     *
     * \endinternal
     */
    static void create() throw (std::bad_alloc)
    {
        assert(!getInstance().isCreated()
               && "-- you have already create the object pool");

        getInstance().init();
    }

    /*!
     * \internal
     * \brief   销毁Object Pool，并释放所有分配的内存。
     *
     * \param   无
     *
     * \return  无
     *
     * \details 对于需要精确控制Pool生命周期的用户，这个函数必不可少。
     *
     * \note    Debug模式下如果没有创建Pool而直接调用此函数会触发断言。
     * \endinternal
     */
    static void destroy()
    {
        assert(getInstance().isCreated()
               && "-- you have not create a object pool");

        if (!getInstance().isDestroyed())
        {
#ifdef _NS_DEBUG_TRACE_MEMORRY_
            NS_TRACE_MEMORY("ObjectPool<",
                            typeid(DataType).name(),
                            ">::destroy()",
                            getInstance().poolMemPtr_);
#endif

            PoolAllocator::deallocate(getInstance().poolMemPtr_);

            getInstance().poolMemPtr_ = nullptr;
        }
    }

    static DataType *getObjectMemory()
    {
        assert(getInstance().isCreated()
               && "-- you have not create a object pool");
        assert(0 < getInstance().currentCapacity_
               && "-- the object pool has not enough object");

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        DataType *ptr = getInstance().getMemoryBlock();

        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::getObject()",
                        ptr);
        return ptr;
#else
        return getInstance().getMemoryBlock();
#endif
    }

    static void deleteObject(DataType *objectPtr)
    {
        assert(getInstance().isPointerValid(objectPtr)
               && "-- you have not create a object pool");

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::deleteObject()",
                        objectPtr);
#endif

        objectPtr->~DataType();

        getInstance().returnMemoryBlock((FreeBlockNode *)(objectPtr));
    }

    static void deallocateMemory(DataType *objectPtr)
    {
        assert(getInstance().isPointerValid(objectPtr)
               && "-- you have not create a object pool");

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::deallocateMemory()",
                        objectPtr);
#endif

        getInstance().returnMemoryBlock((FreeBlockNode *)(objectPtr));
    }


// 如果需要调试信息, 则需要获取内部状态, 这里要使用public
#ifndef  _NS_OBJECT_POOL_DEBUG_
private:
#else
public:
#endif

    ObjectPool() :
        capacity_{capacity},
        currentCapacity_{capacity},
        poolMemPtr_{nullptr},
        alignedMemPtr_{nullptr}
    {
            if (sizeof(DataType) <= sizeof(FreeBlockNode *))
            {
                dataSize_ = sizeof(FreeBlockNode *);
            }
            else
            {
                dataSize_ = sizeof(DataType);
            }
    }

    static ObjectPool<DataType, capacity, Allocator> &getInstance()
    {
        static ObjectPool<DataType, capacity, Allocator> poolInstance;

        return poolInstance;
    }

    // 为了内存访问效率, 内存分配要满足数据类型内存对齐的需要,
    // 这里经过一些trick来实现可移植的分配手段
    void init()
    {
#ifdef _NS_DEBUG_
        NS_DEBUG_MESSAGE_4("ObjectPool<",
                           typeid(DataType).name(),
                           ">::init() sizeof(DataType): ",
                           sizeof(DataType));
        NS_DEBUG_MESSAGE_4("ObjectPool<",
                           typeid(DataType).name(),
                           ">::init() dataSize_: ",
                           dataSize_);
        NS_DEBUG_MESSAGE_4("ObjectPool<",
                           typeid(DataType).name(),
                           ">::init() alignof(DataType): ",
                           alignof(DataType));
#endif

        // 多分配一个dataSize_大小, 因为后面的算法需要在分配了capacity_个对象后,
        // 有一个dataSize_大小保存firstFreeBlock_指针
        size_t size = dataSize_ * (capacity_ + 1) + alignof(DataType);

#ifdef _NS_DEBUG_
        NS_DEBUG_MESSAGE_4("ObjectPool<",
                           typeid(DataType).name(),
                           ">::init() size: ",
                           size);
#endif

        poolMemPtr_ = PoolAllocator::allocate(size);

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::init() poolMemPtr",
                        poolMemPtr_);
#endif

        alignedMemPtr_ = (DataType *)(
            ((uintptr_t)(poolMemPtr_) + alignof(DataType) - 1)
            & (uintptr_t)(~(alignof(DataType) - 1)));

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::init() alignedMemPtr_",
                        alignedMemPtr_);
#endif

        firstFreeBlock_ = (FreeBlockNode *)(alignedMemPtr_);

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::init() firstFreeBlock_",
                        firstFreeBlock_);
#endif

        firstFreeBlock_->pNext_ = nullptr;
    }

    bool isCreated()
    {
        return nullptr == alignedMemPtr_ ? false : true;
    }

    bool isDestroyed()
    {
        return nullptr == poolMemPtr_ ? true: false;
    }

    bool isPointerValid(DataType *ptr)
    {
#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::isPointerValid() object pool end Address",
                        (alignedMemPtr_ + capacity_ - 1));
#endif

        return (nullptr != ptr
                && alignedMemPtr_ <= ptr
                && ptr <= (alignedMemPtr_ + capacity_ - 1));
    }

    DataType *getMemoryBlock()
    {
        FreeBlockNode *object = firstFreeBlock_;

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::getMemoryBlock() object",
                        object);
#endif

        // 这里维护的是空闲节点的数目, 即Pool的剩余容量
        --currentCapacity_;

        if (0 == firstFreeBlock_->pNext_)
        {
            firstFreeBlock_ = (FreeBlockNode *)(
                (char *)(firstFreeBlock_) + dataSize_);

#ifdef _NS_DEBUG_TRACE_MEMORRY_
            NS_TRACE_MEMORY("ObjectPool<",
                            typeid(DataType).name(),
                            ">::getMemoryBlock() firstFreeBlock_",
                            firstFreeBlock_);
#endif

            firstFreeBlock_->pNext_ = nullptr;
        }
        else
        {
            firstFreeBlock_ = firstFreeBlock_->pNext_;
        }

        return static_cast<DataType *>(reinterpret_cast<void *>(object));
    }

    void returnMemoryBlock(FreeBlockNode *objectBlock)
    {
#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::returnMemoryBlock() objectBlock",
                        objectBlock);
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::returnMemoryBlock() firstFreeBlock_",
                        firstFreeBlock_);
#endif

        objectBlock->pNext_ = firstFreeBlock_;
        firstFreeBlock_ = objectBlock;

#ifdef _NS_DEBUG_TRACE_MEMORRY_
        NS_TRACE_MEMORY("ObjectPool<",
                        typeid(DataType).name(),
                        ">::returnMemoryBlock() firstFreeBlock_ after adjust: ",
                        firstFreeBlock_);
#endif

        ++currentCapacity_;
    }

// 如果需要调试信息, 则需要获取内部状态, 这里要使用public
#ifndef  _NS_OBJECT_POOL_DEBUG_
private:
#else
public:
#endif

    size_t               capacity_;
    size_t               dataSize_;
    size_t               currentCapacity_;
    DataType             *poolMemPtr_;      // 实际分配内存首地址, 用于释放
    DataType             *alignedMemPtr_;   // 满足内存对齐要求的首地址
    FreeBlockNode        *firstFreeBlock_;  // 第一个未分配结点
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
 * class MyClass
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 定义默认容量，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass);
 *
 * // 定义容量为2000，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass, 2000);
 *
 * // 定义容量为2000，内存分配器为MyAllocator，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass, 2000, MyAllocator);
 * \endcode
 *
 * \note    以下宏依赖此处定义的Object Pool名称：
 *
 * \see     NS_CREATE_OBJECT_POOL      \n
 *          NS_NEW_FROM_OBJECT_POOL    \n
 *          NS_DELETE_IN_OBJECT_POOL   \n
 *          NS_DESTROY_OBJECT_POOL
 */
#define NS_DEFINE_OBJECT_POOL_NAME(PoolName, ...) \
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
 * // 示例：
 * class MyClass
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 定义默认容量，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass);
 *
 * // 创建名称为MyClassPool的Object Pool。
 * NS_CREATE_OBJECT_POOL(MyClassPool);
 *
 * \endcode
 *
 * \see     NS_DEFINE_OBJECT_POOL_NAME
 */
#define NS_CREATE_OBJECT_POOL(PoolName) \
    PoolName::create()

/*!
 * \brief   从指定的Object Pool中分配对象。
 * \ingroup NsPool
 *
 * \param   PoolName - ObjectPool名称
 * \param   [可选]构造对象所需参数[参数数量不受限制]
 *
 * \details 在指定的ObjectPool中分配对象，支持带参数的构造。\
 *          删除对象需要调用NS_DELETE_IN_OBJECT_POOL。
 *
 * \note    在Debug模式下，若分配对象个数超出Object Pool容量，会触发断言。\n
 *          若调用的Object Pool未调用NS_CREATE_OBJECT_POOL也会触发断言。
 *
 * \code
 * // 示例：
 * class MyClass
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 定义默认容量，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass);
 *
 * // 创建名称为MyClassPool的Object Pool。
 * NS_CREATE_OBJECT_POOL(MyClassPool);
 *
 * MyClass *myClasses[5];
 *
 * // 从MyClassPool中分配对象，使用默认构造函数进行构造。
 * myClasses[0] = NS_NEW_FROM_OBJECT_POOL(MyClassPool);
 * // 从MyClassPool中分配对象，使用参数构造。
 * myClasses[1] = NS_NEW_FROM_OBJECT_POOL(MyClassPool, 1, 2);
 *
 * \endcode
 *
 * \see     NS_DEFINE_OBJECT_POOL_NAME\n
 *          NS_CREATE_OBJECT_POOL\n
 *          NS_DELETE_IN_OBJECT_POOL
 */
#define NS_NEW_FROM_OBJECT_POOL(PoolName, ...) \
    ::new(PoolName::getObjectMemory()) \
        typename PoolName::DataType_(__VA_ARGS__)

/*!
 * \brief   删除指定Object Pool中分配的对象。
 * \ingroup NsPool
 *
 * \param   PoolName - ObjectPool名称
 * \param   objectPtr - 待删除的对象指针
 *
 * \details 删除使用NS_NEW_FROM_OBJECT_POOL分配的对象，ObjectPool名称
 *          要和NS_NEW_FROM_OBJECT_POOL的名称相同。会自动调用析构函数。
 *
 * \note    在Debug模式下会检测待删除的对象指针是否合法。\n
 *          非法指针：nullptr和不是从当前ObjectPool中分配的对象指针。
 *
 * \code
 * // 示例：
 * class MyClass
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 定义默认容量，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass);
 *
 * // 创建名称为MyClassPool的Object Pool。
 * NS_CREATE_OBJECT_POOL(MyClassPool);
 *
 * MyClass *myClasses[5];
 *
 * // 从MyClassPool中分配对象，使用默认构造函数进行构造。
 * myClasses[0] = NS_NEW_FROM_OBJECT_POOL(MyClassPool);
 * // 从MyClassPool中分配对象，使用参数构造。
 * myClasses[1] = NS_NEW_FROM_OBJECT_POOL(MyClassPool, 1, 2);
 *
 * // 删除从MyClassPool中分配的对象。
 * NS_DELETE_IN_OBJECT_POOL(MyClassPool, myClasses[0]);
 * NS_DELETE_IN_OBJECT_POOL(MyClassPool, myClasses[1]);
 *
 * \endcode
 *
 * \see     NS_DEFINE_OBJECT_POOL_NAME\n
 *          NS_CREATE_OBJECT_POOL\n
 *          NS_NEW_FROM_OBJECT_POOL
 */
#define NS_DELETE_IN_OBJECT_POOL(PoolName, objectPtr) \
    PoolName::deleteObject(objectPtr)

/*!
 * \brief   销毁指定的Object Pool。
 * \ingroup NsPool
 *
 * \param   PoolName - ObjectPool名称
 *
 * \details 销毁Object Pool并释放分配的内存，对于需要对Object Pool销毁时机进行控制的用户，
 *          此宏必不可少。必须先调用NS_CREATE_OBJECT_POOL先创建Object Pool，不可多次销毁。
 *
 * \note    Debug模式下会检测Object Pool是否被创建，未创建会触发断言。\n
 *          若多次调用，也会触发断言。
 *
 * \code
 * // 示例：
 * class MyClass
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 定义默认容量，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass);
 *
 * // 创建名称为MyClassPool的Object Pool。
 * NS_CREATE_OBJECT_POOL(MyClassPool);
 *
 * // ...
 *
 * // 销毁MyClassPool。
 * NS_DESTROY_OBJECT_POOL(MyClassPool);
 *
 * \endcode
 *
 * \see     NS_DEFINE_OBJECT_POOL_NAME\n
 *          NS_CREATE_OBJECT_POOL\n
 */
#define NS_DESTROY_OBJECT_POOL(PoolName) \
    PoolName::destroy()


/*!
 * \addtogroup  NsPool
 *              一个完整的非侵入式Object Pool使用实例：
 * \code
 * class MyClass
 * {
 *      MyClass();
 *      MyClass(int a, int b);
 * };
 *
 * // 定义默认容量，默认内存分配器，存储MyClass类型的Object Pool。
 * NS_DEFINE_OBJECT_POOL_NAME(MyClassPool, MyClass);
 *
 * // 创建名称为MyClassPool的Object Pool。
 * NS_CREATE_OBJECT_POOL(MyClassPool);
 *
 * MyClass *myClasses[5];
 *
 * // 从MyClassPool中分配对象，使用默认构造函数进行构造。
 * myClasses[0] = NS_NEW_FROM_OBJECT_POOL(MyClassPool);
 * // 从MyClassPool中分配对象，使用参数构造。
 * myClasses[1] = NS_NEW_FROM_OBJECT_POOL(MyClassPool, 1, 2);
 *
 * // 销毁MyClassPool。
 * NS_DESTROY_OBJECT_POOL(MyClassPool);
 *
 * \endcode
 *
 * <hr>
 *
 * 一个完整的侵入式Object Pool使用实例：
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
 */
}   // NsLib

#endif
