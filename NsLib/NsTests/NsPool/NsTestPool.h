#ifndef NS_TEST_POOL_H
#define	NS_TEST_POOL_H

#include "../NsTestInfo.h"
#include "NsSimpleClass.h"
#include "NsIntrusivePoolClass.h"
#include "NsIntrusivePoolClassMultiInherit.h"

namespace NsLibTest
{

void testObjectPoolUsePodType()
{
    NS_TEST_MESSAGE("-----testObjectPoolUsePodType() entry-----");

    NS_DEFINE_OBJECT_POOL_NAME(DoublePool,
                               double, 3);
    NS_CREATE_OBJECT_POOL(DoublePool);

    double *ptrs[10] = {0};

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = NS_NEW_FROM_OBJECT_POOL(DoublePool, i);
        std::cout << *ptrs[i] << std::endl;
    }

    for (int i = 0; i < 3; ++i)
    {
        NS_DELETE_IN_OBJECT_POOL(DoublePool, ptrs[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = NS_NEW_FROM_OBJECT_POOL(DoublePool, i);
        std::cout << *ptrs[i] << std::endl;
    }

    for (int i = 0; i < 3; ++i)
    {
        NS_DELETE_IN_OBJECT_POOL(DoublePool, ptrs[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = NS_NEW_FROM_OBJECT_POOL(DoublePool, i);
        std::cout << *ptrs[i] << std::endl;
    }

    for (int i = 0; i < 3; ++i)
    {
        NS_DELETE_IN_OBJECT_POOL(DoublePool, ptrs[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = NS_NEW_FROM_OBJECT_POOL(DoublePool, i);
        std::cout << *ptrs[i] << std::endl;
    }

    for (int i = 0; i < 3; ++i)
    {
        NS_DELETE_IN_OBJECT_POOL(DoublePool, ptrs[i]);
    }

    NS_DESTROY_OBJECT_POOL(DoublePool);

    NS_TEST_MESSAGE("-----testObjectPoolUsePodType() leave-----");
}

void testObjectPoolUseMySimpleClass()
{
    NS_TEST_MESSAGE("-----testObjectPoolUseMySimpleClass() entry-----");

    NS_DEFINE_OBJECT_POOL_NAME(MySimpleClassPool,
                               ::NsLibTest::MySimpleClass, 3);
    NS_CREATE_OBJECT_POOL(MySimpleClassPool);

    ::NsLibTest::MySimpleClass *ptrs[10] = {0};

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = NS_NEW_FROM_OBJECT_POOL(MySimpleClassPool, i);
    }

    for (int i = 0; i < 3; ++i)
    {
        NS_DELETE_IN_OBJECT_POOL(MySimpleClassPool, ptrs[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = NS_NEW_FROM_OBJECT_POOL(MySimpleClassPool, i);
    }

    for (int i = 0; i < 3; ++i)
    {
        NS_DELETE_IN_OBJECT_POOL(MySimpleClassPool, ptrs[i]);
    }

    NS_DESTROY_OBJECT_POOL(MySimpleClassPool);

    NS_TEST_MESSAGE("-----testObjectPoolUseMySimpleClass() leave-----");
}

void testIntrusiveObjectPoolUseIntrusivePoolClass()
{
    NS_TEST_MESSAGE(
        "-----testIntrusiveObjectPoolUseIntrusivePoolClass() entry-----");

    NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(IntrusivePoolClassPool,
                                         ::NsLibTest::IntrusivePoolClass, 3);
    NS_CREATE_INTRUSIVE_OBJECT_POOL(IntrusivePoolClassPool);

    ::NsLibTest::IntrusivePoolClass *ptrs[10] = {0};

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = new ::NsLibTest::IntrusivePoolClass(i, i + 1);
    }

    for (int i = 0; i < 3; ++i)
    {
        delete ptrs[i];
    }

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = new ::NsLibTest::IntrusivePoolClass(i + 2, i + 3);
    }

    for (int i = 0; i < 3; ++i)
    {
        delete ptrs[i];
    }

    NS_DESTROY_INTRUSIVE_OBJECT_POOL(IntrusivePoolClassPool);

    NS_TEST_MESSAGE(
        "-----testIntrusiveObjectPoolUseIntrusivePoolClass() leave-----");
}

void testIntrusiveObjectPoolUseMultiInheritIntrusivePoolClass()
{
    NS_TEST_MESSAGE("-----testIntrusiveObjectPoolUseMultiInherit"
        "IntrusivePoolClass() entry-----");

    NS_DEFINE_INTRUSIVE_OBJECT_POOL_NAME(MultiInheritIntrusivePool,
                           ::NsLibTest::DerivedInstrusivePoolClass,
                           3);
    NS_CREATE_INTRUSIVE_OBJECT_POOL(MultiInheritIntrusivePool);

    ::NsLibTest::DerivedInstrusivePoolClass *ptrs[10] = {0};

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = new ::NsLibTest::DerivedInstrusivePoolClass;
    }

    for (int i = 0; i < 3; ++i)
    {
        delete ptrs[i];
    }

    for (int i = 0; i < 3; ++i)
    {
        ptrs[i] = new ::NsLibTest::DerivedInstrusivePoolClass;
    }

    for (int i = 0; i < 3; ++i)
    {
        delete ptrs[i];
    }

    NS_DESTROY_INTRUSIVE_OBJECT_POOL(MultiInheritIntrusivePool);

    NS_TEST_MESSAGE("-----testIntrusiveObjectPoolUseMultiInherit"
        "IntrusivePoolClass() leave-----");
}

}

#endif

