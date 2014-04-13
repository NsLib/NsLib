/*
 * File:   TestObjectPool.cpp
 * Author: mdl
 *
 * Created on 2011-9-25, 20:15:29
 */

#include "TestObjectPool.h"
#include "NsLib/NsPool/NsObjectPool.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestObjectPool);

TestObjectPool::TestObjectPool()
{
}

TestObjectPool::~TestObjectPool()
{
}

void TestObjectPool::setUp()
{
    memPtr_ = 0;
}

void TestObjectPool::tearDown()
{
}

void TestObjectPool::testCreate()
{
    CPPUNIT_ASSERT(
        !NsLib::ObjectPool<double>::getInstance().isCreated());
    NsLib::ObjectPool<double>::create();
    
    
    CPPUNIT_ASSERT(
        NsLib::ObjectPool<double>::getInstance().isCreated());
    
    CPPUNIT_ASSERT();
    
    // 以下的测试一定要失败, 否则程序逻辑就有问题
    //NsLib::ObjectPool<double>::create();
}

void TestObjectPool::testDestroy()
{
    NsLib::ObjectPool<double>::destroy();
}

void TestObjectPool::testAllocate()
{
    // 只需要测试能否编译及是否类型安全即可
    size_t size = sizeof(MyClassInObjectPool) * 10;
    memPtr_ = NsLib::UserDefaultAllocator<MyClassInObjectPool>::allocate(size);
    
    (void)(memPtr_);
}

void TestObjectPool::testDeallocate()
{
    NsLib::UserDefaultAllocator<MyClassInObjectPool>::deallocate(memPtr_);
}

